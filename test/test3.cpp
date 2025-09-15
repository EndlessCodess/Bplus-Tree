#include "../include/BplusTree.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

int main() {

  // 1.测试插入

  // 设置 B+ 树，m=3 maxKeys = 2
  BplusTree<int, uint64_t> tree(3);

  // 获取当前时间作为默认种子
  unsigned int seed = static_cast<unsigned int>(time(nullptr));
  std::cout << "Using seed: " << seed << "\n";
  std::cout << "Enter a different seed (or press Enter for default): ";
  std::string seedInput;
  std::getline(std::cin, seedInput);
  if (!seedInput.empty()) {
    try {
      seed = std::stoi(seedInput);
      std::cout << "Using custom seed: " << seed << "\n";
    } catch (const std::exception &e) {
      std::cout << "Invalid seed, using default: " << seed << "\n";
    }
  }

  // 初始化随机数生成器
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> dis(1, 1000); // 随机键范围 1-100

  // 插入 15 个随机键

  std::vector<int> insertedKeys; // 记录已插入的键

  const int numKeys = 10;
  std::cout << "Inserting " << numKeys << " random keys:\n";
  for (int i = 0; i < numKeys; ++i) {
    int key = dis(gen);
    uint64_t value = static_cast<uint64_t>(key * 10); // 值 = 键 * 10
    std::cout << "Inserting key: " << key << ", value: " << value << "\n";
    tree.insert(key, value);
    insertedKeys.push_back(key);
  }

  // 输出树结构
  std::cout << "\nTree structure:\n";
  tree.printBplusTree(tree.getRoot(), 0);

  // 输出中序遍历
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 输出树高度和节点数
  std::cout << "Height: " << tree.getTreeHeight(tree.getRoot()) << "\n";
  std::cout << "Node count: " << tree.countNode() << "\n";

  // 2.测试删除
  std::cout << "\nTesting deletion:\n";

  // 随机选择 10 个键进行删除（从 insertedKeys 中）
  std::shuffle(insertedKeys.begin(), insertedKeys.end(), gen);
  const int numDeletions = std::min(8, static_cast<int>(insertedKeys.size()));
  for (int i = 0; i < numDeletions; ++i) {
    int keyToDelete = insertedKeys[i];
    std::cout << "Deleting key: " << keyToDelete << "\n";
    bool success = tree.remove(keyToDelete);
    if (success) {
      std::cout << "Deletion successful.\n";
    } else {
      std::cout << "Deletion failed (key not found).\n";
    }
    tree.printBplusTree(tree.getRoot(), 0);
    std::cin.get(); // 等待用户按下回车继续
  }

  // 输出删除后的树结构
  std::cout << "\nTree structure after deletion:\n";
  tree.printBplusTree(tree.getRoot(), 0);

  // 输出删除后的中序遍历
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 输出删除后的树高度和节点数
  std::cout << "Height: " << tree.getTreeHeight(tree.getRoot()) << "\n";
  std::cout << "Node count: " << tree.countNode() << "\n";

  return 0;
}