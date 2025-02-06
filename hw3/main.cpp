#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "allocator.h"
#include "custom_list.h"

int Factorial(int n) {
  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  constexpr size_t kContainerMaxSize = 10;

  // Standart Map
  std::map<int, int> standart_map;
  for (size_t i = 0; i < kContainerMaxSize; ++i) {
    standart_map[i] = Factorial(i);
  }
  std::cout << "Standart map" << std::endl;
  for (const auto &[key, value] : standart_map)
    std::cout << '[' << key << "] = " << value << "; ";
  std::cout << std::endl;

  // Map with custom allocator
  using CustomAllocatorMap =
      CustomAllocator<std::pair<const int, int>, kContainerMaxSize>;
  std::map<int, int, std::less<>, CustomAllocatorMap> custom_alloc_map;
  for (size_t i = 0; i < kContainerMaxSize; ++i) {
    custom_alloc_map[i] = Factorial(i);
  }
  std::cout << "Map with custom allocator" << std::endl;
  for (const auto &[key, value] : standart_map)
    std::cout << '[' << key << "] = " << value << "; ";
  std::cout << std::endl;

  CustomList<int> list;
  for (size_t i = 0; i < kContainerMaxSize; ++i) {
    list.push_back(i);
  }
  std::cout << "List with standart allocator" << std::endl;
  auto *node = list.head();
  while (node) {
    std::cout << node->data_ << " ";
    node = node->next_;
  }
  std::cout << std::endl;

  using CustomAllocatorInt = CustomAllocator<int, kContainerMaxSize>;
  CustomList<int, CustomAllocatorInt> list_with_custom_allocator;
  for (size_t i = 0; i < kContainerMaxSize; ++i) {
    list_with_custom_allocator.push_back(i);
  }
  std::cout << "List with custom allocator" << std::endl;
  node = list.head();
  while (node) {
    std::cout << node->data_ << " ";
    node = node->next_;
  }
  std::cout << std::endl;
}