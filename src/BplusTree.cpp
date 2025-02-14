
#include "BplusTree.h"
#include "BNode.h"
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

// 打印B+树
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
