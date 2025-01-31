#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace ipv4 {
struct ipv4_addr {
public:
  ipv4_addr(const std::string &addr_str) {
    int start = 0;
    for (int i = 0; i < 4; ++i) {
      size_t pos = addr_str.find('.', start);
      if (pos != std::string::npos) {
        addr[i] = std::stoi(addr_str.substr(start, pos - start));
        start = pos + 1;
      } else {
        addr[i] = std::stoi(addr_str.substr(start));
      }
    }
  }

  const uint8_t &operator[](size_t i) const { return addr[i]; }

  void print() const {
    printf("%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
    std::cout << std::endl;
  }

  uint32_t to_uint32() const {
    return static_cast<uint32_t>(addr[3] | addr[2] << 8 | addr[1] << 16 |
                                 addr[0] << 24);
  }

private:
  std::array<uint8_t, 4> addr;
};

class ip_addr_pool {
public:
  void push_back(ipv4_addr &addr) { pool.push_back(addr); }

  void sort() {
    std::sort(pool.begin(), pool.end(), [](ipv4_addr &a, ipv4_addr &b) {
      return a.to_uint32() > b.to_uint32();
    });
  }

  template <typename FilterFunc> void print_filtered(FilterFunc filter_func) {
    for (const auto &addr : pool) {
      if (filter_func(addr)) {
        addr.print();
      }
    }
  }

  void print() {
    for (const auto &addr : pool) {
      addr.print();
    }
  }

private:
  std::vector<ipv4_addr> pool;
};

} // namespace ipv4