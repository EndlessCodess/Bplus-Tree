
#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

//定义模板点类
template <typename keyType, typename valueType> class Node {
public:
  //指向父节点
  std::shared_ptr<Node> parent;

  virtual ~Node() = default;

  //是否为叶子节点
  virtual bool isLeafNode() const = 0;
};

//定义内部节点类
template <typename keyType, typename valueType>
class InterNode : public Node<keyType, valueType> {

public:
  //存储关键字
  std::vector<keyType> keys;

  //存储指向子节点的指针
  std::vector<std::shared_ptr<Node<keyType, valueType>>> children;

  bool isLeafNode() const override { return false; }
};

//定义叶子结点类
template <typename keyType, typename valueType>
class LeafNode : public Node<keyType, valueType> {

public:
  std::vector<keyType> keys;
  std::vector<valueType> values;

  //指向下一个叶子结点
  std::shared_ptr<LeafNode> next;

  bool isLeafNode() const override { return true; }
};

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