#include "../include/BplusTree.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>

int main() {

  BplusTree<int, uint64_t> tree(3);

  // test1:测试插入
  tree.insert(1, 10);
  tree.insert(2, 20);
  tree.insert(3, 30);
  tree.insert(4, 40);
  tree.insert(5, 50);
  tree.insert(6, 60);
  tree.insert(7, 70);
  tree.insert(8, 80);
  tree.insert(9, 90);
  tree.insert(10, 100);
  tree.insert(11, 110);
  tree.insert(12, 120);
  tree.insert(13, 130);
  tree.insert(14, 140);
  tree.insert(15, 150);
  tree.printBplusTree(tree.getRoot(), 0);
  tree.inorderTraversal();

  // test2:测试查询（单一）
  std::cout << "测试存在的key" << std::endl;
  std::cout << "key 1 是 10.  " << tree.search(1) << std::endl;
  std::cout << "key 5 是 50.  " << tree.search(5) << std::endl;

  std::cout << "测试不存在的key" << std::endl;
  if (tree.search(100) == 0) {
    std::cout << "key 100 不存在" << std::endl;
  }
  if (tree.search(19) == 0) {
    std::cout << "key 19 不存在" << std::endl;
  }

  // test3:测试查询（范围）
  std::cout << "测试范围查询 5-15" << std::endl;
  std::vector<std::pair<int, uint64_t>> rangeResult = tree.rangeSearch(5, 15);
  std::cout << "范围查询 5-15 结果: ";
  for (const auto &pair : rangeResult) {
    std::cout << pair.first << " ";
  }
  std::cout << std::endl;

  // test4:测试修改
  std::cout << "测试修改 key 10 的值为 999" << std::endl;
  if (tree.modify(10, 999)) {
    std::cout << "修改成功. 新值为: " << tree.search(10) << std::endl;
  } else {
    std::cout << "修改失败." << std::endl;
  }
  std::cout << "测试修改 key 55 的值为 666" << std::endl;
  if (tree.modify(55, 666)) {
    std::cout << "修改成功. 新值为: " << tree.search(55) << std::endl;
  } else {
    std::cout << "修改失败." << std::endl;
  }

  // test5:测试删除
  std::cout << "测试删除 key 3" << std::endl;
  if (tree.remove(3)) {
    std::cout << "删除成功. 再次查询 key 3: " << tree.search(3) << std::endl;
  } else {
    std::cout << "删除失败." << std::endl;
  }
  tree.remove(1);
  std::cout << "测试删除 key 1" << std::endl;
  tree.remove(2);
  std::cout << "测试删除 key 2" << std::endl;
  tree.remove(4);
  std::cout << "测试删除 key 4" << std::endl;
  tree.remove(6);
  std::cout << "测试删除 key 6" << std::endl;
  tree.printBplusTree(tree.getRoot(), 0);
  tree.inorderTraversal();

  // test6:测试持久化
  std::cout << "测试持久化" << std::endl;
  std::string filename = "bplustree.dat";
  tree.serialize(filename);
  BplusTree<int, uint64_t> newTree(3);
  newTree.deserialize(filename);
  std::cout << "反序列化后的树结构:" << std::endl;
  newTree.printBplusTree(newTree.getRoot(), 0);
  newTree.inorderTraversal();

  return 0;
}