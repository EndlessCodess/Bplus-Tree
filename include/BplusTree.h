
#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "BNode.h"
#include <algorithm>
#include <iostream>

// 定义b+树类(key支持int/string，value为uint64_t)
template <typename keyType = int, typename valueType = uint64_t>
class BplusTree
{
private:
  // 根节点
  std::shared_ptr<Node<keyType, valueType>> root;

  // 叶链表头节点
  //  std::shared_ptr<LeafNode<keyType, valueType>> head;

  // 每个节点的最大和最小键数(关键字)
  const size_t maxKeys, minKeys;

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
  void splitRoot(std::shared_ptr<InterNode<keyType, valueType>> root);

  // 分裂后更新父亲指针
  void updateParentPointers(std::shared_ptr<InterNode<keyType, valueType>> parent,
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

public:
  explicit BplusTree(size_t m)
      : root(nullptr), maxKeys(m - 1), minKeys((m + 1) / 2 - 1) {}

  // 插入操作
  void insert(const keyType &key, const valueType &value);

  // 删除操作
  bool remove(const keyType &key);

  // 搜索单个建
  valueType search(const keyType &key) const;

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
};

#endif