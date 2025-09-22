#include "../include/BplusTree.h"

int main() {

  BplusTree<int, int> tree(3);
  tree.insert(1, 10);
  tree.insert(2, 20);
  tree.insert(3, 30);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.inorderTraversal();

  return 0;
}