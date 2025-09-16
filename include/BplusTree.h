
#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "BNode.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string.h>
#include <unordered_map>
#include <vector>

// 定义b+树类(key支持int/string，value为uint64_t)
template <typename keyType = int, typename valueType = uint64_t>
class BplusTree {
private:
  // 根节点
  std::shared_ptr<Node<keyType, valueType>> root;

  // 每个节点的最大和最小键数(关键字)
  size_t maxKeys, minKeys;

  // 元数据结构
  struct MetaData {
    size_t maxKeys;      // 每个节点的最大键数
    size_t minKeys;      // 每个节点的最小键数
    uint64_t rootOffset; // 根节点在文件中的偏移
    int treeHeight;      // 树的高度
  };

  // 序列化节点
  struct SerializedNode {
    bool isLeaf;
    size_t keyCount;
    std::vector<keyType> keys;      // key数组
    std::vector<valueType> values;  // value数组(仅叶子节点)
    std::vector<uint64_t> children; // 子节点偏移(仅内部节点)
    uint64_t nextOffset;            // 下一个叶子节点偏移(仅叶子节点)
  };

  // 叶链表头节点
  //  std::shared_ptr<LeafNode<keyType, valueType>> head;

  // 寻找叶子结点
  std::shared_ptr<LeafNode<keyType, valueType>>
  findLeaf(std::shared_ptr<Node<keyType, valueType>> currentNode,
           const keyType &key) const;

  // 插入叶子结点
  void insertInLeaf(std::shared_ptr<LeafNode<keyType, valueType>> targetLeaf,
                    const keyType &key, const valueType &value);

  // 分裂叶子
  void splitLeaf(std::shared_ptr<LeafNode<keyType, valueType>> leafNode);

  // 分裂内部
  void splitInter(std::shared_ptr<InterNode<keyType, valueType>> interNode);

  // 分裂根结点
  void splitRoot(std::shared_ptr<Node<keyType, valueType>> root);

  // 分裂后更新父亲指针
  void
  updateParentPointers(std::shared_ptr<InterNode<keyType, valueType>> parent,
                       std::shared_ptr<Node<keyType, valueType>> newNode,
                       const keyType &key);

  // 删除后调整操作
  bool adjust(std::shared_ptr<Node<keyType, valueType>> node,
              std::shared_ptr<InterNode<keyType, valueType>> parent);

  // 得到左兄弟
  std::shared_ptr<Node<keyType, valueType>>
  getLeftSibling(std::shared_ptr<Node<keyType, valueType>> node);

  // 得到右兄弟
  std::shared_ptr<Node<keyType, valueType>>
  getRightSibling(std::shared_ptr<Node<keyType, valueType>> node);

  // 从左兄弟借
  void borrowFromL(std::shared_ptr<Node<keyType, valueType>> node,
                   std::shared_ptr<Node<keyType, valueType>> leftSibling,
                   std::shared_ptr<InterNode<keyType, valueType>> parent);

  // 从右兄弟借
  void borrowFromR(std::shared_ptr<Node<keyType, valueType>> node,
                   std::shared_ptr<Node<keyType, valueType>> rightSibling,
                   std::shared_ptr<InterNode<keyType, valueType>> parent);

  // 与左兄弟合并
  void mergeWithL(std::shared_ptr<Node<keyType, valueType>> node,
                  std::shared_ptr<Node<keyType, valueType>> leftSibling,
                  std::shared_ptr<InterNode<keyType, valueType>> parent);

  // 与右兄弟合并
  void mergeWithR(std::shared_ptr<Node<keyType, valueType>> node,
                  std::shared_ptr<Node<keyType, valueType>> rightSibling,
                  std::shared_ptr<InterNode<keyType, valueType>> parent);

  // 合并后递归调整父节点
  void adjustFather(std::shared_ptr<InterNode<keyType, valueType>> currentNode);

  // 分裂函数
  /*void split(std::shared_ptr<Node<keyType, valueType>> node, const keyType
     &key, const valueType &value);*/

  // 合并函数
  //  void merge(std::shared_ptr<Node<keyType, valueType>> node);

  // 打印单点
  void printNode(const std::shared_ptr<Node<keyType, valueType>> &node,
                 int depth) const;

  // 递归辅助函数
  size_t
  countNodeHelper(const std::shared_ptr<Node<keyType, valueType>> &node) const;

  // 持久化辅助函数
  void saveNodeToFile(
      const std::shared_ptr<Node<keyType, valueType>> &node,
      std::ofstream &outFile,
      std::unordered_map<std::shared_ptr<Node<keyType, valueType>>, uint64_t>
          &nodeOffsetMap,
      uint64_t &currentOffset) const;

  void loadNodeFromFile(
      std::ifstream &inFile, uint64_t offset,
      std::unordered_map<uint64_t, std::shared_ptr<Node<keyType, valueType>>>
          &offsetNodeMap) const;

public:
  explicit BplusTree(size_t m)
      : root(nullptr), maxKeys(m - 1), minKeys((m + 1) / 2 - 1) {}

  // 插入操作
  void insert(const keyType &key, const valueType &value);

  // 删除操作
  bool remove(const keyType &key);

  // 搜索单个键
  valueType search(const keyType &key) const;

  // 更改单个键
  bool modify(const keyType &key, const valueType &newValue);

  // 范围查找
  std::vector<std::pair<keyType, valueType>>
  rangeSearch(const keyType &startKey, const keyType &endKey) const;

  // 中序遍历
  void inorderTraversal() const;

  // 打印b+树
  void printBplusTree(std::shared_ptr<Node<keyType, valueType>> node,
                      const int level) const;

  // 获取树的高度
  int getTreeHeight(std::shared_ptr<Node<keyType, valueType>> node) const;

  // 统计节点数量
  size_t countNode() const;

  // 持久化接口
  // 序列化
  void serialize(const std::string &filename) const;

  // 反序列化
  void deserialize(const std::string &filename);

  // 获取root
  inline std::shared_ptr<Node<keyType, valueType>> getRoot() const {
    return root;
  }
};

// 寻找叶子结点
template <typename keyType, typename valueType>
inline std::shared_ptr<LeafNode<keyType, valueType>>
BplusTree<keyType, valueType>::findLeaf(
    std::shared_ptr<Node<keyType, valueType>> currentNode,
    const keyType &key) const {

  // 判断是否为叶子结点
  if (currentNode->isLeafNode()) {
    return std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(currentNode);
  }

  // 内部节点需要遍历
  else {
    std::shared_ptr<InterNode<keyType, valueType>> interNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(currentNode);

    // key有序排列
    for (size_t i = 0; i < interNode->keys.size(); ++i) {
      if (key < interNode->keys[i]) {
        return findLeaf(interNode->children[i], key);
      }
    }

    // 大于所有key
    return findLeaf(interNode->children[interNode->children.size() - 1], key);
  }

  // 异常情况
  return nullptr;
}

// 插入叶子结点
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::insertInLeaf(
    std::shared_ptr<LeafNode<keyType, valueType>> targetLeaf,
    const keyType &key, const valueType &value) {

  // 查找插入位置
  // std::cout << "keys.size(): " << targetLeaf->keys.size()
  //          << ", values.size(): " << targetLeaf->values.size() << "\n";
  auto it =
      std::lower_bound(targetLeaf->keys.begin(), targetLeaf->keys.end(), key);
  size_t pos = std::distance(targetLeaf->keys.begin(), it);
  // std::cout << "Insert position: " << pos << "\n";

  // 插入新的键值对
  targetLeaf->keys.insert(it, key);
  targetLeaf->values.insert(targetLeaf->values.begin() + pos, value);
}

// 分裂叶子
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::splitLeaf(
    std::shared_ptr<LeafNode<keyType, valueType>> leafNode) {

  size_t midIndex = leafNode->keys.size() / 2;

  // 将后半部分移入新的叶子结点,前半部分保留
  auto newLeaf = std::make_shared<LeafNode<keyType, valueType>>();
  newLeaf->keys = std::vector<keyType>(leafNode->keys.begin() + midIndex,
                                       leafNode->keys.end());
  newLeaf->values = std::vector<valueType>(leafNode->values.begin() + midIndex,
                                           leafNode->values.end());
  leafNode->keys.resize(midIndex);
  leafNode->values.resize(midIndex);

  // 更新相应的指针结构
  newLeaf->parent = leafNode->parent;
  newLeaf->next = leafNode->next;
  leafNode->next = newLeaf;

  // 将新节点插入父节点
  std::shared_ptr<InterNode<keyType, valueType>> parent =
      std::dynamic_pointer_cast<InterNode<keyType, valueType>>(
          leafNode->parent);
  updateParentPointers(parent, newLeaf, newLeaf->keys.front());
}

// 分裂内部
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::splitInter(
    std::shared_ptr<InterNode<keyType, valueType>> interNode) {

  size_t midIndex = interNode->keys.size() / 2;
  keyType midKey = interNode->keys[midIndex];

  // 分开存储
  auto newInter = std::make_shared<InterNode<keyType, valueType>>();
  newInter->keys = std::vector<keyType>(interNode->keys.begin() + midIndex + 1,
                                        interNode->keys.end());
  newInter->children = std::vector<std::shared_ptr<Node<keyType, valueType>>>(
      interNode->children.begin() + midIndex + 1, interNode->children.end());
  interNode->keys.resize(midIndex);
  interNode->children.resize(midIndex + 1);
  newInter->parent = interNode->parent;

  // 更新子节点的父指针
  for (auto &child : newInter->children) {
    child->parent = newInter;
  }

  // 更新父指针结构
  std::shared_ptr<InterNode<keyType, valueType>> parent =
      std::dynamic_pointer_cast<InterNode<keyType, valueType>>(
          interNode->parent);
  updateParentPointers(parent, newInter, midKey);
}

// 分裂根结点
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::splitRoot(
    std::shared_ptr<Node<keyType, valueType>> root) {

  // 根节点为叶子结点
  if (root->isLeafNode()) {

    auto leafRoot =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(root);
    size_t midIndex = leafRoot->keys.size() / 2;

    // 创建一个新的叶子节点
    auto newLeaf = std::make_shared<LeafNode<keyType, valueType>>();
    newLeaf->keys = std::vector<keyType>(leafRoot->keys.begin() + midIndex,
                                         leafRoot->keys.end());
    newLeaf->values = std::vector<valueType>(
        leafRoot->values.begin() + midIndex, leafRoot->values.end());
    leafRoot->keys.resize(midIndex);
    leafRoot->values.resize(midIndex);

    // 更新叶子节点的指针
    newLeaf->next = leafRoot->next;
    leafRoot->next = newLeaf;
    newLeaf->parent = nullptr;
    leafRoot->parent = nullptr;

    // 创建新的根节点
    auto newRoot = std::make_shared<InterNode<keyType, valueType>>();
    newRoot->keys.push_back(newLeaf->keys.front());
    newRoot->children.push_back(leafRoot);
    newRoot->children.push_back(newLeaf);

    // 更新子节点的父指针
    leafRoot->parent = newRoot;
    newLeaf->parent = newRoot;

    // 更新树的根节点
    this->root = newRoot;
  }
  // 根节点为内部节点
  else {

    auto interRoot =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(root);
    size_t midIndex = interRoot->keys.size() / 2;

    // 创建一个新的内部节点
    auto newInter = std::make_shared<InterNode<keyType, valueType>>();

    // 分开存储
    newInter->keys = std::vector<keyType>(
        interRoot->keys.begin() + midIndex + 1, interRoot->keys.end());
    newInter->children = std::vector<std::shared_ptr<Node<keyType, valueType>>>(
        interRoot->children.begin() + midIndex + 1, interRoot->children.end());

    // 更新新内部节点父指针
    for (auto &child : newInter->children) {
      child->parent = newInter;
    }

    // 创建新根结点
    auto newRoot = std::make_shared<InterNode<keyType, valueType>>();

    // 提升原节点最后一个key作为新跟节点的key
    newRoot->keys.push_back(interRoot->keys[midIndex]);
    interRoot->keys.resize(midIndex);
    interRoot->children.resize(midIndex + 1);

    newRoot->children.push_back(interRoot);
    newRoot->children.push_back(newInter);

    // 更新子节点父指针
    interRoot->parent = newRoot;
    newInter->parent = newRoot;

    // 更新树的根结点
    this->root = newRoot;
  }
}
// 分裂后更新父亲指针
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::updateParentPointers(
    std::shared_ptr<InterNode<keyType, valueType>> parent,
    std::shared_ptr<Node<keyType, valueType>> newNode, const keyType &key) {

  // 查找插入位置
  auto it = std::lower_bound(parent->keys.begin(), parent->keys.end(), key);
  auto index = std::distance(parent->keys.begin(), it);
  parent->keys.insert(it, key);

  // 更改孩子指针
  parent->children.insert(parent->children.begin() + index + 1, newNode);
}

// 删除后调整操作(改为通用)
template <typename keyType, typename valueType>
inline bool BplusTree<keyType, valueType>::adjust(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  auto leftSibling = getLeftSibling(node);
  auto rightSibling = getRightSibling(node);

  // 左兄弟借出
  if (leftSibling && leftSibling->keys.size() > minKeys) {
    borrowFromL(node, leftSibling, parent);
    std::cout << "Borrowed from left sibling.\n" << std::endl;
    return true;
  }

  // 右兄弟借出
  if (rightSibling && rightSibling->keys.size() > minKeys) {
    borrowFromR(node, rightSibling, parent);
    std::cout << "Borrowed from right sibling.\n" << std::endl;
    return true;
  }

  // 左兄弟合并
  if (leftSibling) {
    mergeWithL(node, leftSibling, parent);
    std::cout << "Merged with left sibling.\n" << std::endl;
    return true;
  }

  // 右兄弟合并
  if (rightSibling) {
    mergeWithR(node, rightSibling, parent);
    std::cout << "Merged with right sibling.\n" << std::endl;
    return true;
  }

  // 一般不会执行
  return false;
}

// 找左兄弟
template <typename keyType, typename valueType>
inline std::shared_ptr<Node<keyType, valueType>>
BplusTree<keyType, valueType>::getLeftSibling(
    std::shared_ptr<Node<keyType, valueType>> node) {

  if (node->parent) {
    auto parent =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node->parent);

    // 找当前节点的位置
    auto it = std::find(parent->children.begin(), parent->children.end(), node);

    if (it != parent->children.begin()) {
      return *(it - 1);
    }
  }
  return std::shared_ptr<Node<keyType, valueType>>();
}

// 找右兄弟
template <typename keyType, typename valueType>
inline std::shared_ptr<Node<keyType, valueType>>
BplusTree<keyType, valueType>::getRightSibling(
    std::shared_ptr<Node<keyType, valueType>> node) {
  if (node->parent) {
    auto parent =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node->parent);

    // 找当前节点的位置
    auto it = std::find(parent->children.begin(), parent->children.end(), node);

    if (it != parent->children.end() - 1) {
      return *(it + 1);
    }
  }
  return std::shared_ptr<Node<keyType, valueType>>();
}

// 从左兄弟借(已修改子指针)
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::borrowFromL(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> leftSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  // 判断node类型
  if (node->isLeafNode()) { // 叶子结点
    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(leftSibling);

    // 移入当前节点
    currentNode->keys.insert(currentNode->keys.begin(),
                             currentLeft->keys.back());
    currentNode->values.insert(currentNode->values.begin(),
                               currentLeft->values.back());

    // 左兄弟删除该key
    currentLeft->keys.pop_back();
    currentLeft->values.pop_back();

    // 父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys[i - 1] = currentNode->keys.front();
    }
  } else { // 内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(leftSibling);

    // 移入当前节点

    // 更新子节点父指针
    auto newChild = currentLeft->children.back();
    newChild->parent = currentNode;

    // currentNode->keys.insert(currentNode->keys.begin(),
    //                        currentLeft->keys.back());

    // 父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);

      parent->keys[i - 1] = currentLeft->keys.back();
    }

    // 更新当前节点
    // 遍历获得右边最小值
    auto current = currentNode; // InterNode
    auto tempNode =
        std::dynamic_pointer_cast<Node<keyType, valueType>>(current); // Node
    while (!tempNode->isLeafNode()) {
      current =
          std::dynamic_pointer_cast<InterNode<keyType, valueType>>(tempNode);
      tempNode = std::dynamic_pointer_cast<Node<keyType, valueType>>(
          current->children.front());
    }
    currentNode->keys.insert(currentNode->keys.begin(), tempNode->keys.front());

    currentNode->children.insert(currentNode->children.begin(),
                                 currentLeft->children.back());

    // 左兄弟删除该key
    currentLeft->keys.pop_back();
    currentLeft->children.pop_back();
  }
}

// 从右兄弟借(已修改子指针)
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::borrowFromR(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> rightSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  // 判断node类型
  if (node->isLeafNode()) { // 叶子结点
    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    auto currentRight =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(rightSibling);

    // 移入当前节点
    currentNode->keys.push_back(currentRight->keys.front());
    currentNode->values.push_back(currentRight->values.front());

    // 右兄弟删除该key
    currentRight->keys.erase(currentRight->keys.begin());
    currentRight->values.erase(currentRight->values.begin());

    // 父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys[i] = currentRight->keys.front();
    }
  } else { // 内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentRight =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(rightSibling);

    // 更新子节点父指针
    auto newChild = currentRight->children.front();
    newChild->parent = currentNode;

    // 父节点指针更新
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys[i] = currentRight->keys.front();
    }

    // 更新当前节点
    auto current = currentRight; // InterNode
    auto tempNode =
        std::dynamic_pointer_cast<Node<keyType, valueType>>(current); // Node
    while (!tempNode->isLeafNode()) {
      current =
          std::dynamic_pointer_cast<InterNode<keyType, valueType>>(tempNode);
      tempNode = std::dynamic_pointer_cast<Node<keyType, valueType>>(
          current->children.front());
    }
    currentNode->keys.push_back(tempNode->keys.front());

    currentNode->children.push_back(currentRight->children.front());

    // 右节点删除信息
    currentRight->keys.erase(currentRight->keys.begin());
    currentRight->children.erase(currentRight->children.begin());
  }
}

// 找左兄弟合并(合并到左)(已修改)
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::mergeWithL(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> leftSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  // 判断node类型
  if (node->isLeafNode()) { // 叶子结点
    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(leftSibling);

    currentLeft->keys.insert(currentLeft->keys.end(), currentNode->keys.begin(),
                             currentNode->keys.end());
    currentLeft->values.insert(currentLeft->values.end(),
                               currentNode->values.begin(),
                               currentNode->values.end());

    // 更新链表结构(叶子结点)
    currentLeft->next = currentNode->next;

    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys.erase(parent->keys.begin() + i - 1);
      parent->children.erase(childIt);
    }
  } else { // 内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentLeft =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(leftSibling);

    // 将子节点的父亲改为currentLeft
    for (auto &child : currentNode->children) {
      child->parent = currentLeft;
    }

    // 更新父节点
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      // 添加key到左节点
      currentLeft->keys.push_back(parent->keys[i - 1]);
      // 删除父节点key和children
      parent->keys.erase(parent->keys.begin() + i - 1);
      parent->children.erase(childIt);
    }

    // 将当前节点合并到左节点
    currentLeft->keys.insert(currentLeft->keys.end(), currentNode->keys.begin(),
                             currentNode->keys.end());
    currentLeft->children.insert(currentLeft->children.end(),
                                 currentNode->children.begin(),
                                 currentNode->children.end());
  }

  // 递归调整父节点
  if (parent->keys.size() < minKeys) {
    adjustFather(parent);
  }
}

// 找右兄弟合并(右合并到当前)
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::mergeWithR(
    std::shared_ptr<Node<keyType, valueType>> node,
    std::shared_ptr<Node<keyType, valueType>> rightSibling,
    std::shared_ptr<InterNode<keyType, valueType>> parent) {

  // 判断node类型
  if (node->isLeafNode()) { // 叶子结点
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

    // 更新next指针，维持链表结构(叶子结点)
    currentNode->next = currentRight->next;

    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      parent->keys.erase(parent->keys.begin() + i);
      parent->children.erase(childIt + 1);
    }
  } else { // 内部节点
    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    auto currentRight =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(rightSibling);

    // 将子节点的父亲改为currentNode
    for (auto &child : currentRight->children) {
      child->parent = currentNode;
    }

    // 更新父节点
    auto childIt = std::find(parent->children.begin(), parent->children.end(),
                             currentNode);
    if (childIt != parent->children.end()) {
      size_t i = std::distance(parent->children.begin(), childIt);
      // 添加key到当前节点
      currentNode->keys.push_back(parent->keys[i]);
      // 删除父节点key和children
      parent->keys.erase(parent->keys.begin() + i);
      parent->children.erase(childIt + 1);
    }

    // 将右节点合并到当前节点
    currentNode->keys.insert(currentNode->keys.end(),
                             currentRight->keys.begin(),
                             currentRight->keys.end());
    currentNode->children.insert(currentNode->children.end(),
                                 currentRight->children.begin(),
                                 currentRight->children.end());
  }

  // 递归调整父节点
  if (parent->keys.size() < minKeys) {
    adjustFather(parent);
  }
}

// 合并后调整父节点
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::adjustFather(
    std::shared_ptr<InterNode<keyType, valueType>> currentNode) {

  // 判断是否高度减少
  if (currentNode == root) {
    if (currentNode->keys.empty() && currentNode->children.size() == 1) {
      root = currentNode->children[0];
      root->parent = nullptr;
    }
    return;
  }

  auto parent = std::dynamic_pointer_cast<InterNode<keyType, valueType>>(
      currentNode->parent);
  if (parent) { // 不为根结点
    adjust(std::dynamic_pointer_cast<Node<keyType, valueType>>(currentNode),
           parent);
  }
}

// 打印单一节点
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::printNode(
    const std::shared_ptr<Node<keyType, valueType>> &node, int depth) const {
  if (!node)
    return;

  std::string str = std::string(depth * 2, ' ');

  // 打印缩进深度
  std::cout << str << "Depth " << depth << ": ";

  if (node->isLeafNode()) {

    auto currentNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);

    // 打印节点类型
    std::cout << "Leaf Node:[Size:" << currentNode->keys.size() << "]"
              << std::endl;

    // 打印节点
    std::cout << str << "Keys:[";
    for (size_t i = 0; i < currentNode->keys.size(); ++i) {
      std::cout << currentNode->keys[i];
      if (i < currentNode->keys.size() - 1)
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    // 打印值(叶子节点)
    std::cout << str << "Values:[";
    for (size_t i = 0; i < currentNode->values.size(); ++i) {
      std::cout << currentNode->values[i];
      if (i < currentNode->values.size() - 1)
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    // 打印下一个叶子节点
    if (currentNode->next) {
      std::cout << str << "Next Leaf:Exists" << std::endl;
    }

    // 打印父节点
    if (currentNode->parent) {
      std::cout << str << "Parent: Exists" << std::endl;
    }

  } else {

    auto currentNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);

    // 打印节点类型
    std::cout << "Inter Node:[Size:" << currentNode->keys.size() << "]"
              << std::endl;

    // 打印节点
    std::cout << str << "Keys:[";
    for (size_t i = 0; i < currentNode->keys.size(); ++i) {
      std::cout << currentNode->keys[i];
      if (i < currentNode->keys.size() - 1)
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    // 打印子节点(内部节点)
    std::cout << str << "Children pointers:[";
    for (size_t i = 0; i < currentNode->children.size(); ++i) {
      std::cout << (currentNode->children[i] ? "Node" : "Null");
      if (i < currentNode->children.size() - 1)
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    // 打印父节点
    if (currentNode->parent) {
      std::cout << str << "Parent:Exists" << std::endl;
    }
  }
}

// 将节点存入文件
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::saveNodeToFile(
    const std::shared_ptr<Node<keyType, valueType>> &node,
    std::ofstream &outFile,
    std::unordered_map<std::shared_ptr<Node<keyType, valueType>>, uint64_t>
        &nodeOffsetMap,
    uint64_t &currentOffset) const {
  if (!node) {
    std::cout << "Skipping null node at offset " << currentOffset << std::endl;
    return;
  }

  // 分配当前偏移量给节点
  uint64_t nodeOffset = currentOffset;
  nodeOffsetMap[node] = nodeOffset;
  std::cout << "Saving node at offset " << nodeOffset
            << ", isLeaf: " << node->isLeafNode() << ", keys: ";
  for (const auto &key : node->keys)
    std::cout << key << " ";
  std::cout << std::endl;

  SerializedNode snode; // 确保模板化
  snode.isLeaf = node->isLeafNode();
  snode.keyCount = node->keys.size();
  snode.keys = node->keys;

  // 预计算节点大小
  uint64_t nodeSize =
      sizeof(bool) + sizeof(size_t) + snode.keys.size() * sizeof(keyType) +
      (snode.isLeaf ? snode.keyCount * sizeof(valueType) + sizeof(uint64_t)
                    : (snode.keyCount + 1) * sizeof(uint64_t));

  // 递归保存子节点，递增 currentOffset
  if (node->isLeafNode()) {
    auto leafNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    snode.values = leafNode->values;

    snode.nextOffset = 0; // 初始为0，稍后更新
    std::cout << "Leaf node nextOffset: " << snode.nextOffset << std::endl;
  } else {
    auto interNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    snode.children.resize(interNode->children.size());
    for (size_t i = 0; i < interNode->children.size(); ++i) {

      if (interNode->children[i]) {
        if (i == 0) {
          std::cout << "CurrentOffset is:" << currentOffset << std::endl;
          currentOffset += nodeSize; // 预留空间
          std::cout << "CurrentOffset is:" << currentOffset << std::endl;
        }
        saveNodeToFile(interNode->children[i], outFile, nodeOffsetMap,
                       currentOffset);
        std::cout << "Insert currentOffset is:" << currentOffset << std::endl;
      }
    }
    for (size_t i = 0; i < interNode->children.size(); ++i) {
      snode.children[i] =
          interNode->children[i] ? nodeOffsetMap[interNode->children[i]] : 0;
      std::cout << "Child " << i << " offset: " << snode.children[i]
                << std::endl;
    }
  }

  // 写入数据
  std::cout << "CurrentOffset is:" << nodeOffset << std::endl;
  outFile.seekp(static_cast<std::streamoff>(nodeOffset));
  outFile.write(reinterpret_cast<const char *>(&snode.isLeaf), sizeof(bool));
  outFile.write(reinterpret_cast<const char *>(&snode.keyCount),
                sizeof(size_t));
  outFile.write(reinterpret_cast<const char *>(snode.keys.data()),
                snode.keys.size() * sizeof(keyType));
  if (snode.isLeaf) {
    outFile.write(reinterpret_cast<const char *>(snode.values.data()),
                  snode.values.size() * sizeof(valueType));
    // 写入初始 nextOffset
    outFile.write(reinterpret_cast<const char *>(&snode.nextOffset),
                  sizeof(uint64_t));
  } else {
    outFile.write(reinterpret_cast<const char *>(snode.children.data()),
                  snode.children.size() * sizeof(uint64_t));
  }

  // 动态更新叶子节点的 nextOffset
  // if (snode.isLeaf) {
  //   auto leafNode =
  //       std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
  //   if (leafNode->next &&
  //       nodeOffsetMap.find(leafNode->next) != nodeOffsetMap.end()) {
  //     snode.nextOffset = nodeOffsetMap[leafNode->next];
  //     // 回写 nextOffset
  //     outFile.seekp(static_cast<std::streamoff>(
  //         nodeOffset + sizeof(bool) + sizeof(size_t) +
  //         snode.keys.size() * sizeof(keyType) +
  //         snode.keyCount * sizeof(valueType)));
  //     outFile.write(reinterpret_cast<const char *>(&snode.nextOffset),
  //                   sizeof(uint64_t));
  //     std::cout << "Updated Leaf node nextOffset: " << snode.nextOffset
  //               << std::endl;
  //   }
  // }

  // 更新 currentOffset
  if (node->isLeafNode()) {
    currentOffset = nodeOffset + nodeSize;
  }

  std::cout << "Updated offset after writing: " << currentOffset << std::endl;

  if (!outFile.good()) {
    throw std::runtime_error("Failed to write node to file");
  }
}

// 从文件加载节点
template <typename keyType, typename valueType>
void BplusTree<keyType, valueType>::loadNodeFromFile(
    std::ifstream &inFile, uint64_t offset,
    std::unordered_map<uint64_t, std::shared_ptr<Node<keyType, valueType>>>
        &offsetNodeMap) const {
  if (offsetNodeMap.find(offset) != offsetNodeMap.end()) {
    std::cout << "Node at offset " << offset << " already loaded, skipping"
              << std::endl;
    return;
  }

  std::cout << "Loading node at offset " << offset << std::endl;
  inFile.seekg(static_cast<std::streamoff>(offset));
  if (!inFile.good()) {
    throw std::runtime_error("Failed to seek to offset " +
                             std::to_string(offset) + " in file");
  }

  SerializedNode snode;
  inFile.read(reinterpret_cast<char *>(&snode.isLeaf), sizeof(bool));
  inFile.read(reinterpret_cast<char *>(&snode.keyCount), sizeof(size_t));
  snode.keys.resize(snode.keyCount);
  inFile.read(reinterpret_cast<char *>(snode.keys.data()),
              snode.keys.size() * sizeof(keyType));
  std::cout << "Read node: isLeaf=" << snode.isLeaf
            << ", keyCount=" << snode.keyCount << ", keys: ";
  for (const auto &key : snode.keys)
    std::cout << key << " ";
  std::cout << std::endl;
  if (!inFile.good()) {
    throw std::runtime_error("Failed to read keys at offset " +
                             std::to_string(offset));
  }

  if (snode.isLeaf) {
    snode.values.resize(snode.keyCount);
    inFile.read(reinterpret_cast<char *>(snode.values.data()),
                snode.values.size() * sizeof(valueType));
    inFile.read(reinterpret_cast<char *>(&snode.nextOffset), sizeof(uint64_t));
    std::cout << "Leaf node values: ";
    for (const auto &val : snode.values)
      std::cout << val << " ";
    std::cout << ", nextOffset=" << snode.nextOffset << std::endl;
    if (!inFile.good()) {
      throw std::runtime_error(
          "Failed to read values or nextOffset at offset " +
          std::to_string(offset));
    }
  } else {
    snode.children.resize(snode.keyCount + 1);
    inFile.read(reinterpret_cast<char *>(snode.children.data()),
                snode.children.size() * sizeof(uint64_t));
    std::cout << "Internal node children offsets: ";
    for (const auto &child : snode.children)
      std::cout << child << " ";
    std::cout << std::endl;
    if (!inFile.good()) {
      throw std::runtime_error("Failed to read children at offset " +
                               std::to_string(offset));
    }
  }

  std::shared_ptr<Node<keyType, valueType>> newNode;
  if (snode.isLeaf) {
    auto leaf = std::make_shared<LeafNode<keyType, valueType>>();
    leaf->keys = snode.keys;
    leaf->values = snode.values;
    newNode = leaf;
  } else {
    auto inter = std::make_shared<InterNode<keyType, valueType>>();
    inter->keys = snode.keys;
    inter->children.resize(snode.children.size());
    newNode = inter;
  }

  offsetNodeMap[offset] = newNode;
  std::cout << "Node cached at offset " << offset << std::endl;

  if (snode.isLeaf && snode.nextOffset != 0) {
    std::cout << "Loading next leaf at offset " << snode.nextOffset
              << std::endl;
    loadNodeFromFile(inFile, snode.nextOffset, offsetNodeMap);
    std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(newNode)->next =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(
            offsetNodeMap[snode.nextOffset]);
  } else if (!snode.isLeaf) {
    auto inter =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(newNode);
    for (size_t i = 0; i < snode.children.size(); ++i) {
      if (snode.children[i] != 0) {
        std::cout << "Loading child " << i << " at offset " << snode.children[i]
                  << std::endl;
        loadNodeFromFile(inFile, snode.children[i], offsetNodeMap);
        inter->children[i] = offsetNodeMap[snode.children[i]];
      }
    }
  }
}

// 外部接口
// 插入操作(test)
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::insert(const keyType &key,
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
    // 可能需要分裂
    while (currentNode && currentNode->keys.size() > maxKeys) {

      // 根节点
      if (currentNode == root) {
        splitRoot(currentNode);
      } else {
        // 叶子节点
        if (currentNode->isLeafNode()) {
          splitLeaf(std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(
              currentNode));
        } else {
          // 内部节点
          splitInter(std::dynamic_pointer_cast<InterNode<keyType, valueType>>(
              currentNode));
        }
        currentNode = currentNode->parent;
      }
    }
  }

  // // 叶子节点
  // if (currentNode->isLeafNode()) {
  //   splitLeaf(std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(
  //       currentNode));
  // } else {
  //   // 根节点
  //   if (currentNode == root) {
  //     splitRoot(currentNode);
  //   }
  //   // 内部节点
  //   else {
  //     splitInter(std::dynamic_pointer_cast<InterNode<keyType,
  //     valueType>>(
  //         currentNode));
  //   }
  //   currentNode = currentNode->parent;
  // }

  // （原有逻辑） 4.检查是否需要分裂
  //  if (targetLeaf->keys.size() > maxKeys) {
  //    std::shared_ptr<Node<keyType, valueType>> currentNode = targetLeaf;
  //    while (currentNode && currentNode->keys.size() > maxKeys) {

  //     // 根结点跳出
  //     if (currentNode == root) {
  //       break;
  //     }
  //     // 叶子节点分裂
  //     if (currentNode->keys.size() > maxKeys &&
  //     currentNode->isLeafNode())
  //     {

  //       splitLeaf(std::dynamic_pointer_cast<LeafNode<keyType,
  //       valueType>>(
  //           currentNode));
  //     }
  //     // 内部节点分裂
  //     else if (currentNode->keys.size() > maxKeys &&
  //              !currentNode->isLeafNode()) {

  //       splitInter(std::dynamic_pointer_cast<InterNode<keyType,
  //       valueType>>(
  //           currentNode));
  //     } else {
  //       break;
  //     }

  //     // 循环处理父节点
  //     currentNode = currentNode->parent;
  //   }

  //   // 判断根结点是否分裂
  //   if (root->keys.size() > maxKeys) {
  //     splitRoot(std::dynamic_pointer_cast<InterNode<keyType,
  //     valueType>>(root));
  //   }
  // }
}

// 删除操作(test)
template <typename keyType, typename valueType>
inline bool BplusTree<keyType, valueType>::remove(const keyType &key) {
  // 1.寻找目标叶子结点
  auto targetLeaf = findLeaf(root, key);
  if (!targetLeaf) {
    std::cout << "Key not found in the tree.\n" << std::endl;
    return false; // 未找到叶子结点
  }

  // 2.在叶子结点中找到对应key，并删除
  auto it =
      std::lower_bound(targetLeaf->keys.begin(), targetLeaf->keys.end(), key);
  if (it != targetLeaf->keys.end()) {
    size_t index = std::distance(targetLeaf->keys.begin(), it);
    targetLeaf->keys.erase(it);
    targetLeaf->values.erase(targetLeaf->values.begin() + index);
    std::cout << "Key deleted successfully.\n" << std::endl;
  } else {
    std::cout << "Key not found in the leaf node.\n" << std::endl;
    return false; // 未在叶子结点中找到key
  }

  // 3.不满足要求，进入调整过程
  if (targetLeaf->keys.size() < minKeys) {
    auto parent = std::dynamic_pointer_cast<InterNode<keyType, valueType>>(
        targetLeaf->parent);
    if (parent) { // 非根节点
      return adjust(
          std::dynamic_pointer_cast<Node<keyType, valueType>>(targetLeaf),
          parent);
    } else { // 根结点
      if (targetLeaf->keys.empty()) {
        root = nullptr;
      }
      return true;
    }
  }

  std::cout << "No adjustment needed after deletion.\n" << std::endl;
  return true;
}

// 单一查询(test)
template <typename keyType, typename valueType>
inline valueType
BplusTree<keyType, valueType>::search(const keyType &key) const {

  // 获取叶子结点
  auto targetLeaf = findLeaf(root, key);

  // 未找到直接返回
  if (!targetLeaf) {
    return valueType{}; // 返回默认构造值
  }

  // 查找候选目标key
  auto it =
      std::lower_bound(targetLeaf->keys.begin(), targetLeaf->keys.end(), key);

  // 进一步判断
  if (it != targetLeaf->keys.end() && *it == key) {
    size_t i = std::distance(targetLeaf->keys.begin(), it);
    return targetLeaf->values[i];
  }

  return valueType{};
}

// 改动单键
template <typename keyType, typename valueType>
inline bool BplusTree<keyType, valueType>::modify(const keyType &key,
                                                  const valueType &newValue) {

  // 查找搜索key
  auto targetLeaf = findLeaf(root, key);

  // 未找到节点
  if (!targetLeaf) {
    return valueType{}; // 返回默认构造值
  }

  // 查找候选目标key
  auto it =
      std::lower_bound(targetLeaf->keys.begin(), targetLeaf->keys.end(), key);

  // 进一步判断
  if (it != targetLeaf->keys.end() && *it == key) {
    size_t i = std::distance(targetLeaf->keys.begin(), it);
    targetLeaf->values[i] = newValue;
    return true;
  }

  return false;
}

// 范围查询(test)
template <typename keyType, typename valueType>
inline std::vector<std::pair<keyType, valueType>>
BplusTree<keyType, valueType>::rangeSearch(const keyType &startKey,
                                           const keyType &endKey) const {

  std::vector<std::pair<keyType, valueType>> result;

  // 寻找起始叶子结点
  auto startLeaf = findLeaf(root, startKey);

  // 未找到返回
  if (!startLeaf) {
    return result;
  }

  // 寻找第一个满足的key
  auto it = std::lower_bound(startLeaf->keys.begin(), startLeaf->keys.end(),
                             startKey);

  // 遍历当前叶子节点
  while (it != startLeaf->keys.end()) {
    // 边界判断
    if (*it > endKey) {
      break;
    }

    // 加入查询结果
    size_t i = std::distance(startLeaf->keys.begin(), it);
    result.push_back({*it, startLeaf->values[i]});

    ++it;
  }

  // 继续寻找后面叶子结点
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

// 中序遍历
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::inorderTraversal() const {
  // 获取头节点
  std::shared_ptr<Node<keyType, valueType>> currentNode = root;
  while (currentNode && !currentNode->isLeafNode()) {
    currentNode =
        (std::dynamic_pointer_cast<InterNode<keyType, valueType>>(currentNode)
             ->children)
            .front();
  }
  // 遍历叶子结点
  auto leaf =
      std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(currentNode);
  size_t count = 0;
  while (leaf) {
    for (size_t i = 0; i < leaf->keys.size(); ++i) {
      std::cout << leaf->keys[i] << ":" << leaf->values[i] << " ";
      if (count % 10 == 9) {
        std::cout << "\n" << "                   ";
      }
      ++count;
    }
    leaf = leaf->next;
  }

  std::cout << std::endl;
}

// 打印B+树
template <typename keyType, typename valueType>
inline void BplusTree<keyType, valueType>::printBplusTree(
    std::shared_ptr<Node<keyType, valueType>> node, const int level) const {

  // 判断树是否为空
  if (!root) {
    return;
  }

  for (int i = 0; i < level; ++i) {
    std::cout << "-";
  }

  // 先打印当前节点的key
  std::cout << "Node[keys:";
  for (size_t i = 0; i < node->keys.size(); ++i) {
    std::cout << node->keys[i] << " ";
  }
  std::cout << "]\n";

  // 再根据当前节点类型输出
  if (!node->isLeafNode()) { // 内部节点，循环调用
    auto interNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    for (auto &child : interNode->children) {
      printBplusTree(child, level + 1);
    }
  } else { // 叶子结点
    auto leafNode =
        std::dynamic_pointer_cast<LeafNode<keyType, valueType>>(node);
    for (size_t i = 0; i < leafNode->keys.size(); ++i) {
      std::cout << " " << leafNode->keys[i] << ":" << leafNode->values[i]
                << "\n";
    }
  }
}

// 获取树高
template <typename keyType, typename valueType>
inline int BplusTree<keyType, valueType>::getTreeHeight(
    std::shared_ptr<Node<keyType, valueType>> node) const {
  if (!node) {
    return 0;
  }
  if (node->isLeafNode()) {
    return 1;
  }
  auto interNode =
      std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
  int maxHeight = 0;
  for (auto &child : interNode->children) {
    maxHeight = std::max(maxHeight, getTreeHeight(child));
  }
  return maxHeight + 1;
}

// 统计节点数量
template <typename keyType, typename valueType>
inline size_t BplusTree<keyType, valueType>::countNode() const {

  // 如果树为空，返回0
  if (!root) {
    return 0;
  }
  // 调用递归辅助函数
  return countNodeHelper(root);
}

// 统计辅助函数
template <typename keyType, typename valueType>
inline size_t BplusTree<keyType, valueType>::countNodeHelper(
    const std::shared_ptr<Node<keyType, valueType>> &node) const {

  if (!node) {
    return 0;
  }

  // 当前节点计数为1
  size_t count = 1;

  // 如果是内部节点，递归统计子节点
  if (!node->isLeafNode()) {

    auto interNode =
        std::dynamic_pointer_cast<InterNode<keyType, valueType>>(node);
    for (const auto &child : interNode->children) {
      count += countNodeHelper(child);
    }
  }

  return count;
}

template <typename keyType, typename valueType>
inline void
BplusTree<keyType, valueType>::serialize(const std::string &filename) const {
  std::cout << "Starting serialization to: " << filename << std::endl;
  std::ofstream outFile(filename, std::ios::binary);
  if (!outFile.is_open()) {
    throw std::runtime_error("Failed to open file for writing: " + filename);
  }

  MetaData metaData = {maxKeys, minKeys, 0, getTreeHeight(root)};
  std::cout << "Writing metadata: maxKeys=" << metaData.maxKeys
            << ", minKeys=" << metaData.minKeys
            << ", initial rootOffset=0, height=" << metaData.treeHeight
            << std::endl;
  outFile.write(reinterpret_cast<const char *>(&metaData), sizeof(MetaData));

  std::unordered_map<std::shared_ptr<Node<keyType, valueType>>, uint64_t>
      nodeOffsetMap;
  uint64_t currentOffset = sizeof(MetaData);
  std::cout << "Initial offset after metadata: " << currentOffset << std::endl;

  saveNodeToFile(root, outFile, nodeOffsetMap, currentOffset);

  outFile.seekp(0);
  metaData.rootOffset = root ? nodeOffsetMap[root] : 0;
  std::cout << "Updating metadata with rootOffset: " << metaData.rootOffset
            << std::endl;
  outFile.write(reinterpret_cast<const char *>(&metaData), sizeof(MetaData));

  outFile.close();
  std::cout << "Serialization completed, final offset: " << currentOffset
            << std::endl;

  if (!outFile.good()) {
    throw std::runtime_error("Failed to write to file: " + filename);
  }
}

// 反序列化主函数
template <typename keyType, typename valueType>
inline void
BplusTree<keyType, valueType>::deserialize(const std::string &filename) {
  std::cout << "Starting deserialization from: " << filename << std::endl;
  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile.is_open()) {
    throw std::runtime_error("Failed to open file for reading: " + filename);
  }

  MetaData metaData;
  inFile.read(reinterpret_cast<char *>(&metaData), sizeof(MetaData));
  std::cout << "Read metadata: maxKeys=" << metaData.maxKeys
            << ", minKeys=" << metaData.minKeys
            << ", rootOffset=" << metaData.rootOffset
            << ", height=" << metaData.treeHeight << std::endl;
  if (!inFile.good()) {
    inFile.close();
    throw std::runtime_error("Failed to read metadata from file: " + filename);
  }

  if (metaData.maxKeys != maxKeys || metaData.minKeys != minKeys) {
    inFile.close();
    throw std::runtime_error(
        "Incompatible B+ tree parameters: file (maxKeys=" +
        std::to_string(metaData.maxKeys) +
        ", minKeys=" + std::to_string(metaData.minKeys) +
        ") vs constructor (maxKeys=" + std::to_string(maxKeys) +
        ", minKeys=" + std::to_string(minKeys) + ")");
  }

  root = nullptr;

  std::unordered_map<uint64_t, std::shared_ptr<Node<keyType, valueType>>>
      offsetNodeMap;

  if (metaData.rootOffset != 0) {
    std::cout << "Loading root node from offset: " << metaData.rootOffset
              << std::endl;
    loadNodeFromFile(inFile, metaData.rootOffset, offsetNodeMap);
    if (offsetNodeMap.find(metaData.rootOffset) == offsetNodeMap.end()) {
      throw std::runtime_error("Root node not found at offset " +
                               std::to_string(metaData.rootOffset));
    }
    root = offsetNodeMap[metaData.rootOffset];
    std::cout << "Root node loaded, keys: ";
    for (const auto &key : root->keys)
      std::cout << key << " ";
    std::cout << std::endl;
  }

  inFile.close();
  std::cout << "Deserialization completed, total nodes loaded: "
            << offsetNodeMap.size() << std::endl;

  if (!inFile.good()) {
    throw std::runtime_error("Failed to read file: " + filename);
  }
}

#endif