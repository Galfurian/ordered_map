/// @file test.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Simple test set for the ordered map.
/// 
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
/// 

#include <iostream>
#include <sstream>
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

inline bool check(const Table &table, const std::string &key, int value)
{
    Table::const_iterator it = table.find(key);
    if (it == table.end()) {
        return false;
    }
    return it->second == value;
}

inline bool check(const Table &table, std::size_t position, int value)
{
    Table::const_iterator it = table.at(position);
    if (it == table.end()) {
        return false;
    }
    return it->second == value;
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
    if (!check(table, "a", 1) || !check(table, "b", 2) || !check(table, "c", 3)) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check(table, 0, 1) || !check(table, 1, 2) || !check(table, 2, 3)) {
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
    if (!check(table, "a", 1) || !check(table, "b", 2) || !check(table, "c", 5)) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check(table, 0, 1) || !check(table, 1, 2) || !check(table, 2, 5)) {
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
    if (!check(table, "a", 1) || !check(table, "b", 2) || check(table, "c", 3)) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check(table, 0, 1) || !check(table, 1, 2) || check(table, 2, 3)) {
        std::cerr << "The position->value association is wrong.\n";
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
    if (!check(table, "a", 1) || !check(table, "b", 2) || check(table, "c", 3) || !check(table, "d", 7)) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    if (!check(table, 0, 1) || !check(table, 1, 2) || check(table, 2, 3) || !check(table, 2, 7)) {
        std::cerr << "The position->value association is wrong.\n";
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
    if (!check(table, "a", 3) || !check(table, "b", 2) || !check(table, "c", 1)) {
        std::cerr << "The key->value association is wrong.\n";
        return 1;
    }
    // Run the index-based test.
    if (!check(table, 0, 1) || !check(table, 1, 2) || !check(table, 2, 3)) {
        std::cerr << "The position->value association is wrong.\n";
        return 1;
    }
    // Sort the table.
    table.sort(compare);
    // Run the basic checks again, the key-value association should stay the same.
    if (!check(table, "a", 3) || !check(table, "b", 2) || !check(table, "c", 1)) {
        std::cerr << "The key->value association is wrong (after sort).\n";
        return 1;
    }
    // Run again the index-based test, which should find the values swapped.
    if (!check(table, 0, 3) || !check(table, 1, 2) || !check(table, 2, 1)) {
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