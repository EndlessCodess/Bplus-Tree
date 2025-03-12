
#include "../include/BNode.h"
#include "../include/BplusTree.h"
#include <algorithm>
#include <memory.h>

//寻找叶子结点
template <typename keyType, typename valueType>
inline std::shared_ptr<LeafNode<keyType, valueType>>
BplusTree<keyType, valueType>::findLeaf(
    std::shared_ptr<Node<keyType, valueType>> currentNode, const keyType &key) {

  //判断是否为叶子结点
  if (currentNode->isLeafNode()) {
    return currentNode;
  }

  //内部节点需要遍历
  else {
    std::shared_ptr<InterNode<keyType, valueType>> interNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(currentNode);

    // key有序排列
    for (size_t i = 0; i < interNode->keys.size(); ++i) {
      if (key < interNode->keys[i]) {
        return findLeaf(interNode->children[i], key);
      }
    }

    //大于所有key
    return findLeaf(interNode->children[interNode->children.size() - 1], key);
  }

  //异常情况
  return nullptr;
}

//插入叶子结点
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::insertInLeaf(
    std::shared_ptr<LeafNode<keyType, valueType>> targetLeaf,
    const keyType &key, const valueType &value) {

  //查找插入位置
  auto it =
      std::lower_bound(targetLeaf->keys.begin(), targetLeaf->keys.end(), key);

  //插入新的键值对
  targetLeaf->keys.insert(it, key);
  targetLeaf->values.insert(targetLeaf.values.begin() +
                                std::distance(targetLeaf->keys.begin(), it),
                            value);
}

//分裂叶子
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::splitLeaf(
    std::shared_ptr<LeafNode<keyType, valueType>> leafNode) {

  size_t midIndex = leafNode->keys.size() / 2;

  //将后半部分移入新的叶子结点,前半部分保留
  auto newLeaf = std::make_shared<LeafNode<keyType, valueType>>();
  newLeaf->keys = std::vector<keyType>(leafNode->keys.begin() + midIndex,
                                       leafNode->keys.end());
  newLeaf->values = std::vector<valueType>(leafNode->values.begin() + midIndex,
                                           leafNode->values.end());
  leafNode->keys.resize(midIndex);
  leafNode->values.resize(midIndex);

  //更新相应的指针结构
  newLeaf->parent = leafNode->parent;
  newLeaf->next = leafNode->next;
  leafNode->next = newLeaf;

  //将新节点插入父节点
  auto parent = leafNode->parent;
  updateParentPointers(parent, leafNode, newLeaf, newLeaf->keys.front());
}

//分裂内部
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::splitInter(
    std::shared_ptr<InterNode<keyType, valueType>> interNode) {

  size_t midIndex = interNode->keys.size() / 2;

  //分开存储
  auto newInter = std::make_shared<InterNode<keyType, valueType>>();
  newInter->keys = std::vector<keyType>(interNode->keys.begin() + midIndex + 1,
                                        interNode->keys.end());
  newInter->children = std::vector<std::shared_ptr<Node<keyType, valueType>>>(
      interNode->children.begin() + midIndex + 1, interNode->children.end());
  interNode->keys.resize(midIndex);
  interNode->children.resize(midIndex + 1);
  newInter->parent = interNode->parent;

  //更新子节点的父指针
  for (auto &child : newInter->children) {
    child->parent = newInter;
  }

  //更新父指针结构
  auto parent = interNode->parent;
  updateParentPointers(parent, interNode, newInter, newInter->keys.front());
}

//分裂根结点
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::splitRoot(
    std::shared_ptr<Node<keyType, valueType>> root) {

  //创建一个新的内部节点
  auto newInter = std::make_shared<InterNode<keyType, valueType>>();

  //分开存储
  size_t midIndex = root->keys.size() / 2;

  newInter->keys =
      std::vector<keyType>(root->keys.begin() + midIndex + 1, root->keys.end());
  newInter->children = std::vector<std::shared_ptr<Node<keyType, valueType>>>(
      root->children.begin() + midIndex + 1, root->children.end());

  //更新新内部节点父指针
  for (auto &child : newInter->children) {
    child->parent = newInter;
  }

  //创建新根结点
  auto newRoot = std::make_shared<InterNode<keyType, valueType>>();

  //提升原节点最后一个key作为新跟节点的key
  newRoot->keys.push_back(root->keys[midIndex]);
  root->keys.resize(midIndex);
  root->children.resize(midIndex + 1);

  newRoot->children.push_back(root);
  newRoot->children.push_back(newInter);

  //更新子节点父指针
  root->parent = newRoot;
  newInter->parent = newRoot;

  //更新树的根结点
  this->root = newRoot;
}

//分裂后更新父亲指针
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::updateParentPointers(
    std::shared_ptr<Node<keyType, valueType>> parent,
    std::shared_ptr<Node<keyType, valueType>> oldNode,
    std::shared_ptr<Node<keyType, valueType>> newNode, const keyType &key) {

  //查找插入位置
  auto it = std::lower_bound(parent->keys.begin(), parent->keys.end(), key);
  parent->keys.insert(it, key);

  //更改孩子指针
  auto index = std::distance(parent->keys.begin(), it);
  parent->children.insert(parent->children.begin() + index + 1, newNode);
}

//删除后调整操作(改为通用)
template <typename keyType, typename valueType>
bool BplusTree<keyType, valueType>::adjust(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  auto leftSibling = getLeftSibling(node);
  auto rightSibling = getRightSibling(node);

  //右兄弟借出
  if (rightSibling && rightSibling->keys.size() > minKeys) {
    borrowFromR(node, rightSibling, parent);
    return true;
  }

  //左兄弟借出
  if (leftSibling && leftSibling->keys.size() > minKeys) {
    borrowFromL(node, leftSibling, parent);
    return true;
  }

  //右兄弟合并
  if (rightSibling) {
    mergeWithR(node, rightSibling, parent);
    return true;
  }

  //左兄弟合并
  if (leftSibling) {
    mergeWithL(node, leftSibling, parent);
    return true;
  }

  //一般不会执行
  return false;
}

//找左兄弟
template <typename keyType, typename valueType>
std::shared_ptr<Node<keyType, valueType>>
BplusTree<keyType, valueType>::getLeftSibling(
    std::shared_ptr<Node<keyType, valueType>> node) {

  if (node->parent) {
    auto parent = node->parent;

    //找当前节点的位置
    auto it = std::find(parent->children.begin(), parent->children.end(), node);

    if (it != parent->children.begin()) {
      return *(it - 1);
    }
  }
  return std::shared_ptr<Node<keyType, valueType>>();
}

//找右兄弟
template <typename keyType, typename valueType>
std::shared_ptr<Node<keyType, valueType>>
BplusTree<keyType, valueType>::getRightSibling(
    std::shared_ptr<Node<keyType, valueType>> node) {
  if (node->parent) {
    auto parent = node->parent;

    //找当前节点的位置
    auto it = std::find(parent->children.begin(), parent->children.end(), node);

    if (it != parent->children.end() - 1) {
      return *(it + 1);
    }
  }
  return std::shared_ptr<Node<keyType, valueType>>();
}

//从右兄弟借
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::borrowFromR(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> rightSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  //判断node类型
  if (node->isLeafNode()) { //叶子结点
    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    auto currentRight =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(rightSibling);

    //移入当前节点
    currentNode->keys.push_back(currentRight->keys.front());
    currentNode->values.push_back(currentRight->values.front());

    //右兄弟删除该key
    currentRight->keys.erase(currentRight->keys.begin());
    currentRight->values.erase(currentRight->values.begin());

    //父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys[i] = currentRight->keys.front();
    }

  } else { //内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentRight =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(rightSibling);

    //移入当前节点
    currentNode->keys.push_back(currentRight->keys.front());
    currentNode->children.push_back(currentRight->children.front());

    //右节点删除信息
    currentRight->keys.erase(currentRight->keys.begin());
    currentRight->children.erase(currentRight->children.begin());

    //父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);

      //遍历获得右边最小值
      while (!currentRight->isLeafNode()) {
        currentRight = currentRight->children.begin();
      }

      //已经为叶子结点
      parent->keys[i] = currentRight->keys.front();
    }
  }
}

//找右兄弟合并(合并到右)
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::mergeWithR(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> rightSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  // 判断node类型
  if (node->isLeafNode()) { //叶子结点
    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    auto currentRight =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(rightSibling);

    currentNode->keys.insert(currentNode->keys.end(),
                             currentRight->keys.begin(),
                             currentRight->keys.end());

    currentNode->values.insert(currentNode->values.end(),
                               currentRight->values.begin(),
                               currentRight->values.end());

    //更新next指针，维持链表结构(叶子结点)
    currentNode->next = currentRight->next;

    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys.erase(parent->keys.begin() + i);
      parent->children.erase(childIt + 1);
    }

  } else { //内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentRight =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(rightSibling);

    //将右边插入到当前节点
    currentNode->keys.insert(currentNode->keys.end(),
                             currentRight->keys.begin(),
                             currentRight->keys.end());
    currentNode->children.insert(currentNode->children.end(),
                                 currentRight->children.begin(),
                                 currentRight->children.end());

    //更新父节点结构
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent.begin(), childIt);
      parent->keys.erase(parent->keys.begin() + i);
      parent->children.erase(childIt + 1);
    }
  }

  //递归调整父节点
  if (parent->keys.size() < minKeys) {
    adjustFather(parent);
  }
}

//从左兄弟借
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::borrowFromL(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> leftSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  //判断node类型
  if (node->isLeafNode()) { //叶子结点
    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(leftSibling);

    //移入当前节点
    currentNode->keys.insert(currentNode->keys.begin(),
                             currentLeft->keys.back());
    currentNode->values.insert(currentNode->values.begin(),
                               currentLeft->values.back());

    //左兄弟删除该key
    currentLeft->keys.pop_back();
    currentLeft->values.pop_back();

    //父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys[i - 1] = currentNode->keys.front();
    }

  } else { //内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(leftSibling);

    //移入当前节点
    currentNode->keys.insert(currentNode->keys.begin(),
                             currentLeft->keys.back());
    currentNode->children.insert(currentNode->children.begin(),
                                 currentLeft->children.back());

    //左兄弟删除该key
    currentLeft->keys.pop_back();
    currentLeft->children.pop_back();

    //父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);

      //遍历获得右边最小值
      while (!currentNode->isLeafNode()) {
        currentNode = currentNode->children.begin();
      }

      //已经为叶子结点
      parent->keys[i - 1] = currentNode->keys.front();
    }
  }
}

//找左兄弟合并(合并到左)
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::mergeWithL(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> leftSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  //判断node类型
  if (node->isLeafNode()) { //叶子结点
    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(leftSibling);

    currentLeft->keys.insert(currentLeft->keys.end(), currentNode->keys.begin(),
                             currentNode->keys.end());
    currentLeft->values.insert(currentLeft->values.end(),
                               currentNode->values.begin(),
                               currentNode->values.end());

    //更新链表结构(叶子结点)
    currentLeft->next = currentNode->next;

    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys.erase(parent->keys.begin() + i - 1);
      parent->children.erase(childIt);
    }

  } else { //内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(leftSibling);

    //将当前节点合并到左节点
    currentLeft->keys.insert(currentLeft->keys.end(), currentNode->keys.begin(),
                             currentNode->keys.end());
    currentLeft->children.insert(currentLeft->children.end(),
                                 currentNode->children.begin(),
                                 currentNode->children.end());

    //更新父节点
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys.erase(parent->keys.begin() + i - 1);
      parent->children.erase(childIt);
    }
  }

  //递归调整父节点
  if (parent->keys.size() < minKeys) {
    adjustFather(parent);
  }
}

//合并后调整父节点
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::adjustFather(
    std::shared_ptr<InterNode<keyType, valueType>> currentNode) {

  //判断是否高度减少
  if (currentNode == root) {
    if (currentNode->keys.empty() && currentNode->children.size() == 1) {
      root = currentNode->children[0];
      root->parent = nullptr;
    }
    return;
  }

  auto parent = currentNode->parent;
  if (parent) { //不为根结点
    bool flag = adjust(currentNode, parent);
  }
}

//删除操作(test)
template <typename keyType, typename valueType>
bool BplusTree<keyType, valueType>::remove(const keyType &key) {
  // 1.寻找目标叶子结点
  auto targetLeaf = findLeaf(root, key);
  if (!targetLeaf) {
    return false; //未找到叶子结点
  }

  // 2.在叶子结点中找到对应key，并删除
  auto it = std::find(targetLeaf->keys.begin(), targetLeaf->keys.end(), key);
  if (it != targetLeaf->keys.end()) {
    size_t index = std::distance(targetLeaf->keys.begin(), it);
    targetLeaf->keys.erase(it);
    targetLeaf->values.erase(targetLeaf->values.begin() + index);
  } else {
    return false; //未在叶子结点中找到key
  }

  // 3.不满足要求，进入调整过程
  if (targetLeaf->keys.size() < minKeys) {
    auto parent = targetLeaf->parent;
    if (parent) { //非根节点
      return adjust(targetLeaf, parent);
    } else { //根结点
      if (targetLeaf->keys.empty()) {
        root = nullptr;
      }
      return true;
    }
  }

  return false;
}

//插入操作(test)
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::insert(const keyType &key,
                                           const valueType &value) {

  // 1.判断是否为空
  if (!root) {
    root = std::make_shared<LeafNode<keyType, valueType>>();
    // head = root;
  }

  // 2.循环遍历找到插入位置
  std::shared_ptr<LeafNode<keyType, valueType>> targetLeaf =
      findLeaf(root, key);

  // 3.进行插入操作
  insertInLeaf(targetLeaf, key, value);

  // 4.检查是否需要分裂
  if (targetLeaf->keys.size() > maxKeys) {
    std::shared_ptr<Node<keyType, valueType>> currentNode = targetLeaf;
    while (currentNode) {

      //根结点跳出
      if (currentNode == root) {
        break;
      }
      //叶子节点分裂
      if (currentNode->keys.size() > maxKeys && currentNode->isLeafNode()) {

        splitLeaf(currentNode);
      }
      //内部节点分裂
      else if (currentNode->keys.size() > maxKeys &&
               !currentNode->isLeafNode()) {

        splitInter(currentNode);
      } else {
        break;
      }

      //循环处理父节点
      currentNode = currentNode->parent;
    }

    //判断根结点是否分裂
    if (root->keys.size() > maxKeys) {
      splitRoot(root);
    }
  }
}

//单一查询(test)
template <typename keyType, typename valueType>
valueType BplusTree<keyType, valueType>::search(const keyType &key) const {

  //获取叶子结点
  auto targetLeaf = findLeaf(root, key);

  //未找到直接返回
  if (!targetLeaf) {
    return valueType{}; //返回默认构造值
  }

  //查找候选目标key
  auto it =
      std::lower_bound(targetLeaf->keys.begin(), targetLeaf->keys.end(), key);

  //进一步判断
  if (it != targetLeaf->keys.end() && *it == key) {
    size_t i = std::distance(targetLeaf->keys.begin(), it);
    return targetLeaf->values[i];
  }

  return valueType{};
}

//范围查询(test)
template <typename keyType, typename valueType>
std::vector<std::pair<keyType, valueType>>
BplusTree<keyType, valueType>::rangeSearch(const keyType &startKey,
                                           const keyType &endKey) const {

  std::vector<std::pair<keyType, valueType>> result;

  //寻找起始叶子结点
  auto startLeaf = findLeaf(root, startKey);

  //未找到返回
  if (!startLeaf) {
    return result;
  }

  //寻找第一个满足的key
  auto it = std::lower_bound(startLeaf->keys.begin(), startLeaf->keys.end(),
                             startKey);

  //遍历当前叶子节点
  while (it != startLeaf->keys.end()) {
    //边界判断
    if (*it > endKey) {
      break;
    }

    //加入查询结果
    size_t i = std::distance(startLeaf->keys.begin(), it);
    result.push_back({*it, startLeaf->values[i]});

    ++it;
  }

  //继续寻找后面叶子结点
  auto nextLeaf = startLeaf->next;
  while (nextLeaf) {
    it = nextLeaf->keys.begin();
    while (it != nextLeaf->keys.end()) {
      if (*it > endKey) {
        break;
      }
      size_t i = std::distance(nextLeaf->keys.begin(), it);
      result.push_back({*it, nextLeaf->values[i]});
      ++it;
    }
    nextLeaf = nextLeaf->next;
  }

  return result;
}

//中序遍历
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::inorderTraversal() const {
  //获取头节点
  std::shared_ptr<Node<keyType, valueType>> currentNode = root;
  while (currentNode && !currentNode->isLeafNode()) {
    currentNode = currentNode->children[0];
  }

  //遍历叶子结点
  auto leaf =
      std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(currentNode);
  while (leaf) {
    for (size_t i = 0; i < leaf->keys.size(); ++i) {
      std::cout << leaf->keys[i] << ":" << leaf->values[i] << " ";
    }
    leaf = leaf->next;
  }
  std::cout << std::endl;
}

//打印B+树
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::printBplusTree(
    std::shared_ptr<Node<keyType, valueType>> node, const int level) const {

  //判断树是否为空
  if (!root)
    return;

  for (int i = 0; i < level; ++i) {
    std::cout << " ";
  }

  //先打印当前节点的key
  std::cout << "Node[keys:";
  for (size_t i = 0; i < node->keys.size(); ++i) {
    std::cout << node->keys[i] << " ";
  }
  std::cout << "]\n";

  //再根据当前节点类型输出
  if (!node->isLeafNode()) { //内部节点，循环调用
    auto interNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    for (auto &child : interNode->children) {
      printBplusTree(child, level + 1);
    }
  } else { //叶子结点
    auto leafNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    for (size_t i = 0; i < leafNode->keys.size(); ++i) {
      std::cout << " " << leafNode->keys[i] << ":" << leafNode->values[i]
                << "\n";
    }
  }
}

//获取树高
template <typename keyType, typename valueType>
int BplusTree<keyType, valueType>::getTreeHeight(
    std::shared_ptr<Node<keyType, valueType>> node) const {
  if (!node)
    return 0;
  if (node->isLeafNode())
    return 1;

  auto interNode =
      std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
  int maxHeight = 0;
  for (auto &child : interNode->children) {
    maxHeight = std::max(maxHeight, getTreeHeight(child));
  }
  return maxHeight + 1;
}
