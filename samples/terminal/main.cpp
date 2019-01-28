/*
 * Copyright (c) 2019-present, DIEGO DAGUM.
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "bitset/bitset_iter.h"

#include <bitset>
#include <iostream>

int
main()
{
  std::bitset<10> my_bitset{ 0b10110010 };

  std::cout << "For bitset " << my_bitset << ":" << std::endl;

  std::cout << "* Indices \"ON\" are";
  for (const auto& index : bitset::indices_on(my_bitset)) {
    std::cout << ' ' << index;
  }
  std::cout << std::endl;

  std::cout << "* Indices \"OFF\" are";
  for (const auto& index : bitset::indices_off(my_bitset)) {
    std::cout << ' ' << index;
  }
  std::cout << std::endl;

  std::cout << "* Negated indices \"ON\" are";
  for (const auto& index : bitset::indices_on(~my_bitset)) {
    std::cout << ' ' << index;
  }
  std::cout << std::endl;

  std::cout << "* Negated indices \"OFF\" are";
  for (const auto& index : bitset::indices_off(~my_bitset)) {
    std::cout << ' ' << index;
  }
  std::cout << std::endl;

  return (0);
}
