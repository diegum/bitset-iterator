/*
 * Copyright (c) 2019-present, DIEGO DAGUM.
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file

#include "catch.hpp"

#include "bitset/bitset_iter.h"

#include <bitset>
#include <set>

SCENARIO("Index bitset", "[index]")
{
  GIVEN("An empty bitset")
  {
    std::bitset<8> my_bitset{ 0b0 };

    WHEN("Iterated thru \"on\" indices")
    {
      THEN("No iteration occurs")
      {
        for (const auto& index : bitset::indices_on(my_bitset)) {
          (void)index;
          FAIL("It wasn't suppossed to iterate any time thru an empty set.");
        }
      }

      AND_WHEN("Negated, iterated thru \"off\" indices")
      {
        THEN("No iteration occurs")
        {
          for (const auto& index : bitset::indices_off(~my_bitset)) {
            (void)index;
            FAIL("It wasn't suppossed to iterate any time thru an empty set.");
          }
        }
      }
    }

    AND_GIVEN("Some expected indices")
    {
      auto expected_indices = std::set<size_t>{ 0, 1, 2, 3, 4, 5, 6, 7 };

      WHEN("Iterated thru \"off\" indices")
      {
        THEN("iterator hits these indices")
        {
          for (const auto& index : bitset::indices_off(my_bitset)) {
            {
              REQUIRE(1 == expected_indices.erase(index));
            }
          }

          AND_THEN("iterations were eight")
          {
            REQUIRE(expected_indices.empty());
          }
        }

        AND_WHEN("Negated, iterated thru \"on\" indices")
        {
          THEN("indices are expected")
          {
            for (const auto& index : bitset::indices_on(~my_bitset)) {
              {
                REQUIRE(1 == expected_indices.erase(index));
              }
            }

            AND_THEN("iterations were eight")
            {
              REQUIRE(expected_indices.empty());
            }
          }
        }
      }
    }
  }

  GIVEN("A single bit bitset")
  {
    std::bitset<8> my_bitset{ 0b1 };

    WHEN("Iterated thru \"on\" indices")
    {
      auto no_iterations_yet{ true };

      THEN("Iteration happens once")
      {
        for (const auto& index : bitset::indices_on(my_bitset)) {
          REQUIRE(no_iterations_yet);
          no_iterations_yet = false;

          AND_THEN("single bit index is the one set") { REQUIRE(0 == index); }
        }
      }
    }

    AND_WHEN("Negated, then iterated thru \"off\" indices")
    {
      auto no_iterations_yet{ true };

      THEN("Iteration happens once")
      {
        for (const auto& index : bitset::indices_off(~my_bitset)) {
          REQUIRE(no_iterations_yet);
          no_iterations_yet = false;

          AND_THEN("single bit index is the one set") { REQUIRE(0 == index); }
        }
      }
    }

    AND_GIVEN("Some expected indices")
    {
      auto expected_indices = std::set<size_t>{ 1, 2, 3, 4, 5, 6, 7 };

      WHEN("Iterated thru \"off\" indices")
      {
        THEN("iterator hits these indices")
        {
          for (const auto& index : bitset::indices_off(my_bitset)) {
            {
              REQUIRE(1 == expected_indices.erase(index));
            }
          }

          AND_THEN("iterations were seven")
          {
            REQUIRE(expected_indices.empty());
          }
        }

        AND_WHEN("Negated, iterated thru \"on\" indices")
        {
          THEN("indices are expected")
          {
            for (const auto& index : bitset::indices_on(~my_bitset)) {
              {
                REQUIRE(1 == expected_indices.erase(index));
              }
            }

            AND_THEN("iterations were seven")
            {
              REQUIRE(expected_indices.empty());
            }
          }
        }
      }
    }
  }

  GIVEN("An odd bitset")
  {
    std::bitset<8> my_bitset{ 0b10101010 };

    WHEN("Iterated thru \"on\" indices")
    {
      THEN("indices are expected")
      {
        auto expected_indices = std::set<size_t>{ 1, 3, 5, 7 };

        for (const auto& index : bitset::indices_on(my_bitset)) {
          {
            REQUIRE(1 == expected_indices.erase(index));
          }
        }

        AND_THEN("iterations were four") { REQUIRE(expected_indices.empty()); }
      }
    }

    AND_WHEN("Iterated thru \"off\" indices")
    {
      THEN("indices are expected")
      {
        auto expected_indices = std::set<size_t>{ 0, 2, 4, 6 };

        for (const auto& index : bitset::indices_off(my_bitset)) {
          {
            REQUIRE(1 == expected_indices.erase(index));
          }
        }

        AND_THEN("iterations were four") { REQUIRE(expected_indices.empty()); }
      }
    }
  }

  GIVEN("An upper bitset")
  {
    std::bitset<8> my_bitset{ 0b11110000 };

    WHEN("Iterated thru \"on\" indices")
    {
      auto expected_indices = std::set<size_t>{ 4, 5, 6, 7 };

      THEN("indices are expected")
      {
        for (const auto& index : bitset::indices_on(my_bitset)) {
          {
            REQUIRE(1 == expected_indices.erase(index));
          }
        }

        AND_THEN("iterations were four") { REQUIRE(expected_indices.empty()); }
      }
    }
  }

  GIVEN("A full bitset")
  {
    std::bitset<8> my_bitset{ 0b11111111 };

    WHEN("Iterated thru \"on\" indices")
    {
      auto expected_indices = std::set<size_t>{ 0, 1, 2, 3, 4, 5, 6, 7 };

      THEN("indices are expected")
      {
        for (const auto& index : bitset::indices_on(my_bitset)) {
          {
            REQUIRE(1 == expected_indices.erase(index));
          }
        }

        AND_THEN("iterations were eight") { REQUIRE(expected_indices.empty()); }
      }
    }
  }
}
