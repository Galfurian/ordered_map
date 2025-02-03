/// @file example.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief A couple of examples for the ordered map.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#include <iostream>
#include <sstream>

#include "ordered_map/ordered_map.hpp"

using Table = ordered_map::ordered_map_t<std::string, int>;

inline void print(const Table &table)
{
    std::cout << "{ ";
    for (const auto &it : table) {
        std::cout << "[" << it.first << "](" << it.second << ") ";
    }
    std::cout << "}\n";
}

auto compare_gt(const Table::list_entry_t &lhs, const Table::list_entry_t &rhs) -> bool
{
    return lhs.first > rhs.first;
}

auto compare_lt(const Table::list_entry_t &lhs, const Table::list_entry_t &rhs) -> bool
{
    return lhs.first < rhs.first;
}

auto main(int /*unused*/, char * /*unused*/[]) -> int
{
    Table table;

    // Setting values.
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);
    print(table);

    // Change one value.
    table.set("c", 4);
    print(table);

    // Change one value and remove a key.
    table.set("d", 5);
    table.erase("c");
    print(table);

    // Erase using iterator.
    Table::iterator it_erase;
    if ((it_erase = table.find("d")) != table.end()) {
        table.erase(it_erase);
    }
    print(table);

    // Sort the value but preserve the internal mechanisms, i.e., the key value
    // association.
    table.sort(compare_gt);
    print(table);
    table.sort(compare_lt);
    print(table);

    return 0;
}
