
#ifndef BNODE_H
#define BNODE_H

#include <memory>
#include <vector>

// 定义模板点类
template <typename keyType, typename valueType>
class Node
{
public:
  // 指向父节点
  std::shared_ptr<Node> parent;

  virtual ~Node() = default;

  // 是否为叶子节点
  virtual bool isLeafNode() const = 0;
};

// 定义内部节点类
template <typename keyType, typename valueType>
class InterNode : public Node<keyType, valueType>
{

public:
  // 存储关键字
  std::vector<keyType> keys;

  // 存储指向子节点的指针
  std::vector<std::shared_ptr<Node<keyType, valueType>>> children;

  bool isLeafNode() const override { return false; }
};

// 定义叶子结点类
template <typename keyType, typename valueType>
class LeafNode : public Node<keyType, valueType>
{

public:
  std::vector<keyType> keys;
  std::vector<valueType> values;

  // 指向下一个叶子结点
  std::shared_ptr<LeafNode> next;

  bool isLeafNode() const override { return true; }
};

#endif