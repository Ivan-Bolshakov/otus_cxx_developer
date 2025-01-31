#include "ipv4_addr.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  ipv4::ip_addr_pool ipv4_addr_pool;
  for (std::string str; std::getline(std::cin, str);) {
    if (str.empty()) {
      break;
    }
    auto ip_addr_end = str.find_first_of('\t');
    if (ip_addr_end != std::string::npos) {
      ipv4::ipv4_addr ip_addr(str.substr(0, ip_addr_end));
      ipv4_addr_pool.push_back(ip_addr);
    } else {
      break;
    }
  }

  // Print sorted  ipv4 pool
  ipv4_addr_pool.sort();
  ipv4_addr_pool.print();

  // Print sorted ipv4 addrs which started with 1.x.x.x

  ipv4_addr_pool.print_filtered(
      [](const ipv4::ipv4_addr &addr) { return addr[0] == 1; });

  // Print sorted ipv4 addrs which started with 46.70.x.x
  ipv4_addr_pool.print_filtered([](const ipv4::ipv4_addr &addr) {
    return addr[0] == 46 && addr[1] == 70;
  });

  // Print sorted ipv4 addrs which contain 46
  ipv4_addr_pool.print_filtered([](const ipv4::ipv4_addr &addr) {
    return (addr[0] == 46) || (addr[1] == 46) || (addr[2] == 46) ||
           (addr[3] == 46);
  });

  return 0;
}