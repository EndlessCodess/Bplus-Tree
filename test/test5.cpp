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

  tree.remove(2);
  tree.remove(3);
  tree.remove(4);
  tree.remove(5);
  tree.printBplusTree(tree.getRoot(), 0);

  // 案例 : 内部节点借key (删除 6 后从右邻居借)
  std::cout << "\nCase : (remove 6)\n";
  if (tree.remove(6)) {
    std::cout << "Deletion of 6 successful.\n";
  } else {
    std::cout << "Deletion of 6 failed.\n";
  }
  tree.printBplusTree(tree.getRoot(), 0);
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  return 0;
}