/// @file test.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Simple test set for the ordered map.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "ordered_map/ordered_map.hpp"

typedef ordered_map::ordered_map_t<std::string, int> Table;

inline int get_choice(char *argument)
{
    std::stringstream ss;
    ss << argument;
    int choice;
    ss >> choice;
    return choice;
}

inline bool exists(const Table &table, const std::string &key)
{
    return table.find(key) != table.end();
}

inline bool exists(const Table &table, std::size_t position)
{
    return table.at(position) != table.end();
}

inline bool check_key(const Table &table, const std::string &key, int value)
{
    Table::const_iterator it = table.find(key);
    if (it == table.end()) {
        return false;
    }
    return it->second == value;
}

inline bool check_position(const Table &table, std::size_t position, int value)
{
    Table::const_iterator it = table.at(position);
    if (it == table.end()) {
        return false;
    }
    return it->second == value;
}

inline bool check_keys(const Table &table,
                       const std::vector<std::string> &keys,
                       const std::vector<int> &values)
{
    if (keys.size() != values.size()) {
        throw std::runtime_error("Keys and values have different size.");
    }
    for (std::size_t i = 0; i < keys.size(); ++i) {
        if (!check_key(table, keys[i], values[i])) {
            return false;
        }
    }
    return true;
}

inline bool check_positions(const Table &table,
                            const std::vector<std::size_t> &positions,
                            const std::vector<int> &values)
{
    if (positions.size() != values.size()) {
        throw std::runtime_error("Positions and values have different size.");
    }
    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (!check_position(table, positions[i], values[i])) {
            return false;
        }
    }
    return true;
}

bool compare(const Table::list_entry_t &lhs, const Table::list_entry_t &rhs)
{
    return lhs.first < rhs.first;
}

int run_test_0()
{
    // Create the table.
    Table table;
    // Set the values.
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);
    // Run the checks.
    if (table.size() != 3) {
        std::cerr << "Table size is wrong.\n";
        return 1;
    }
    if (!check_keys(table, { "a", "b", "c" }, { 1, 2, 3 })) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check_positions(table, { 0, 1, 2 }, { 1, 2, 3 })) {
        std::cerr << "The position->value association is wrong (after sort).\n";
        return 1;
    }
    return 0;
}

int run_test_1()
{
    // Create the table.
    Table table;
    // Set the values.
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);
    // Change a value.
    table.set("c", 5);
    // Run the checks.
    if (table.size() != 3) {
        std::cerr << "Table size is wrong.\n";
        return 1;
    }
    if (!check_keys(table, { "a", "b", "c" }, { 1, 2, 5 })) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check_positions(table, { 0, 1, 2 }, { 1, 2, 5 })) {
        std::cerr << "The position->value association is wrong (after sort).\n";
        return 1;
    }
    return 0;
}

int run_test_2()
{
    // Create the table.
    Table table;
    // Set the values.
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);
    // Remove a key.
    table.erase("c");
    // Run the checks.
    if (table.size() != 2) {
        std::cerr << "Table size is wrong.\n";
        return 1;
    }
    if (!check_keys(table, { "a", "b" }, { 1, 2 }) || exists(table, "c")) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check_positions(table, { 0, 1 }, { 1, 2 }) || exists(table, 2)) {
        std::cerr << "The position->value association is wrong (after sort).\n";
        return 1;
    }
    return 0;
}

int run_test_3()
{
    // Create the table.
    Table table;
    // Set the values.
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);
    // Remove a key and add another.
    table.erase("c");
    table.set("d", 7);
    // Run the checks.
    if (table.size() != 3) {
        std::cerr << "Table size is wrong.\n";
        return 1;
    }
    if (!check_keys(table, { "a", "b", "d" }, { 1, 2, 7 }) || exists(table, "c")) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check_positions(table, { 0, 1, 2 }, { 1, 2, 7 }) || check_position(table, 2, 3)) {
        std::cerr << "The position->value association is wrong (after sort).\n";
        return 1;
    }
    return 0;
}

int run_test_4()
{
    // Create the table.
    Table table;
    // Set the values.
    table.set("c", 1);
    table.set("b", 2);
    table.set("a", 3);
    // Check the number of elements.
    if (table.size() != 3) {
        std::cerr << "Table size is wrong.\n";
        return 1;
    }
    // Run the basic checks.
    if (!check_keys(table, { "a", "b", "c" }, { 3, 2, 1 })) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check_positions(table, { 0, 1, 2 }, { 1, 2, 3 })) {
        std::cerr << "The position->value association is wrong (after sort).\n";
        return 1;
    }
    // Sort the table.
    table.sort(compare);
    // Run the basic checks again, the key-value association should stay the same.
    if (!check_keys(table, { "a", "b", "c" }, { 3, 2, 1 })) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check_positions(table, { 0, 1, 2 }, { 3, 2, 1 })) {
        std::cerr << "The position->value association is wrong (after sort).\n";
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 2) {
        int choice = get_choice(argv[1]);
        if (choice == 0) {
            return run_test_0();
        }
        if (choice == 1) {
            return run_test_1();
        }
        if (choice == 2) {
            return run_test_2();
        }
        if (choice == 3) {
            return run_test_3();
        }
        if (choice == 4) {
            return run_test_4();
        }
    }
    return 1;
}
