
#ifndef BPLUSTREE_H
#define BPLUSTREE_H


#include "BNode.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

//定义b+树类
template <typename keyType, typename valueType> class BplusTree {
private:
  //根节点
  std::shared_ptr<Node<keyType, valueType>> root;

  //叶链表头节点
  std::shared_ptr<LeafNode<keyType, valueType>> head;

  //每个节点的最大和最小键数(关键字)
  size_t maxKeys, minKeys;

  void split(std::shared_ptr<Node<keyType, valueType>> node, const keyType &key,
             const valueType &value);

  void merge(std::shared_ptr<Node<keyType, valueType>> node);

  void printNode(const std::shared_ptr<Node<keyType, valueType>> &node,
                 int depth) const;

public:
  explicit BplusTree(size_t m)
      : root(nullptr), head(nullptr), maxKeys(m - 1), minKeys((m + 1) / 2 - 1) {
  }

  //插入操作
  void insert(const keyType &key, const valueType &value);

  //删除操作
  void remove(const keyType &key);

  //搜索单个建
  valueType search(const keyType &key) const;

  //范围查找
  std::vector<valueType> rangeSearch(const keyType &start,
                                     const keyType &end) const;

  //打印b+树
  void printBplusTree() const;

  //统计节点数量
  size_t countNode() const;
};

#endif