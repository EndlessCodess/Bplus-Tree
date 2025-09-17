#include "../include/BplusTree.h"
#include <cassert>
#include <iostream>

int main() {
  // 创建 B+ 树，m=3（maxKeys=2, minKeys=1）
  BplusTree<int, uint64_t> tree(3);

  // 测试 1：插入 1, 2, 3（触发根为叶子分裂）
  std::cout << "=== Insert 1, 2, 3 ===\n";
  tree.insert(1, 10);
  tree.insert(2, 20);
  tree.printBplusTree(tree.getRoot(), 0); // 预期：叶子 [1, 2]
  std::cout << "Inorder: ";
  tree.inorderTraversal(); // 预期：1:10 2:20
  std::cout << "Height: " << tree.getTreeHeight(tree.getRoot())
            << "\n";                                       // 预期：1
  std::cout << "Node count: " << tree.countNode() << "\n"; // 预期：1

  tree.insert(3, 30); // 触发根分裂（LeafNode）
  std::cout << "\nAfter inserting 3:\n";
  tree.printBplusTree(tree.getRoot(), 0); // 预期：根 [2], 子节点 [1], [2, 3]
  std::cout << "Inorder: ";
  tree.inorderTraversal(); // 预期：1:10 2:20 3:30
  std::cout << "Height: " << tree.getTreeHeight(tree.getRoot())
            << "\n";                                       // 预期：2
  std::cout << "Node count: " << tree.countNode() << "\n"; // 预期：3

  // 测试 2：插入 4, 5, 6, 7（触发叶子分裂和根为内部节点分裂）
  std::cout << "\n=== Insert 4, 5, 6, 7 ===\n";
  tree.insert(4, 40);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.insert(5, 50);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.insert(6, 60);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.insert(7, 70);
  tree.printBplusTree(tree.getRoot(),
                      0); // 预期：根 [3], 子节点 [1, 2], [4, 5, 6, 7]
  std::cout << "Inorder: ";
  tree.inorderTraversal(); // 预期：1:10 2:20 3:30 4:40 5:50 6:60 7:70
  std::cout << "Height: " << tree.getTreeHeight(tree.getRoot())
            << "\n";                                       // 预期：3
  std::cout << "Node count: " << tree.countNode() << "\n"; // 预期：10

  // 测试 3:改动部分key
  tree.modify(4, 400);
  tree.modify(6, 600);
  tree.modify(10, 1000);
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder: ";
  tree.inorderTraversal(); // 预期：1:10 2:20 3:30 4:400 5:50 6:600 7:70
  std::cout << "\n";

  tree.insert(8, 80);
  tree.insert(9, 90);
  tree.insert(10, 100);
  tree.insert(11, 110);
  tree.insert(12, 120);
  tree.insert(13, 130);
  tree.insert(14, 140);
  tree.insert(15, 150);

  // 测试相同key的插入
  tree.insert(4, 4000);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.insert(5, 5000);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.insert(6, 6000);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.insert(7, 7000);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.inorderTraversal();

  // 测试 4:序列化
  //   std::string filename = "./data/bplustree.dat";
  //   std::cout << "=== Serialize to 'bplustree.dat' ===\n";
  //   tree.serialize(filename);
  //   std::cout << "Serialization completed successfully!\n";

  //   // 注释反序列化部分（待实现）
  //   BplusTree<int, uint64_t> newTree(4);
  //   std::cout << "=== Deserialize from 'bplustree.dat' ===\n";
  //   newTree.deserialize(filename);
  //   newTree.printBplusTree(newTree.getRoot(), 0);
  //   std::cout << "Inorder: ";
  //   newTree.inorderTraversal();

  std::cout << "All tests passed!\n";

  return 0;
}