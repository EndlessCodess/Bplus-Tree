#include "../include/BplusTree.h"
#include <iostream>
#include <vector>

int main() {
  // 设置 B+ 树，maxKeys = 4 (m=3 通常指最小键数，maxKeys = m 或 2m)
  BplusTree<int, uint64_t> tree(4);

  // 1. 手动插入构造特定树结构
  std::cout << "Constructing initial tree...\n";
  int keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // 初始键
  uint64_t values[] = {100, 200,  300,  400,  500,  600,  700, 800,
                       900, 1000, 1100, 1200, 1300, 1400, 1500};
  for (size_t i = 0; i < 15; ++i) {
    tree.insert(keys[i], values[i]);
  }

  // 输出初始树结构
  std::cout << "\nInitial tree structure:\n";
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\nHeight: " << tree.getTreeHeight(tree.getRoot()) << "\n";
  std::cout << "Node count: " << tree.countNode() << "\n";

  // 2. 测试删除所有情况
  std::cout << "\nTesting deletion cases:\n";

  // 案例 1: 简单删除 (删除叶子节点中间键10)
  std::cout << "\nCase 1: Simple deletion (remove 10)\n";
  tree.remove(10);
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 案例 2: 节点借用 (删除 9 后从左邻居借)
  std::cout << "\nCase 2: Borrow after deletion (remove 9)\n";
  if (tree.remove(9)) {
    std::cout << "Deletion of 9 successful.\n";
  } else {
    std::cout << "Deletion of 9 failed.\n";
  }
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 案例 3: 节点借用 (删除 8 后从右邻居借)
  std::cout << "\nCase 3: Borrow after deletion (remove 8)\n";
  if (tree.remove(8)) {
    std::cout << "Deletion of 8 successful.\n";
  } else {
    std::cout << "Deletion of 8 failed.\n";
  }
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 案例 4: 节点不足，触发合并 (删除 11 后左合并)
  std::cout << "\nCase 4: Merge after deletion (remove 11)\n";
  if (tree.remove(11)) {
    std::cout << "Deletion of 11 successful.\n";
  } else {
    std::cout << "Deletion of 11 failed.\n";
  }
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 案例 5: 节点不足，触发合并 (删除 7 后右合并)
  std::cout << "\nCase 5: Merge after deletion (remove 7)\n";
  if (tree.remove(7)) {
    std::cout << "Deletion of 7 successful.\n";
  } else {
    std::cout << "Deletion of 7 failed.\n";
  }
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 案例 6: 删除不存在的键
  std::cout << "\nCase 6: Delete non-existent key (remove 90)\n";
  if (tree.remove(90)) {
    std::cout << "Deletion of 90 successful.\n";
  } else {
    std::cout << "Deletion of 90 failed.\n";
  }
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  tree.remove(14);
  tree.remove(15);

  // 案例 7: 内部节点借key (删除 12 后从左邻居借)
  std::cout << "\nCase 7: (remove 12)\n";
  if (tree.remove(12)) {
    std::cout << "Deletion of 12 successful.\n";
  } else {
    std::cout << "Deletion of 12 failed.\n";
  }
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 案例 7: 根节点变化 (删除后根从内部变为叶子)
  //   std::cout << "\nCase 7: Root change after deletion (remove 40)\n";
  //   if (tree.remove(40)) {
  //     std::cout << "Deletion of 40 successful.\n";
  //   } else {
  //     std::cout << "Deletion of 40 failed.\n";
  //   }
  //   tree.printBplusTree(tree.getRoot(), 0);
  //   std::cout << "Inorder traversal: ";
  //   tree.inorderTraversal();
  //   std::cout << "\n";

  return 0;
}