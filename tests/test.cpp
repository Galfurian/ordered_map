/// @file test_ordered_map.cpp
/// @brief Initial unit tests for ordered_map_t.
///
/// @details This test suite covers the basic functionality of `ordered_map_t`,
/// including insertion, update, erase, lookup, iteration, sort, and clear.
/// It is designed to be used with CTest and does not depend on any testing framework.
///

#include <cassert>
#include <iostream>
#include <string>

#include "ordered_map/ordered_map.hpp"

using Table = ordered_map::ordered_map_t<std::string, int>;

struct Thing {
    int x, y;
    Thing(int a, int b)
        : x(a)
        , y(b)
    {
    }
    bool operator==(const Thing &other) const { return x == other.x && y == other.y; }
};

void test_set_and_overwrite()
{
    std::cout << ">>> test_set_and_overwrite\n";

    Table table;
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);

    // Overwrite "c"
    table.set("c", 4);

    std::vector<std::string> keys;
    std::vector<int> values;
    for (const auto &entry : table) {
        keys.push_back(entry.first);
        values.push_back(entry.second);
    }

    assert((keys == std::vector<std::string>{"a", "b", "c"}));
    assert((values == std::vector<int>{1, 2, 4}));
}

void test_find_and_erase()
{
    std::cout << ">>> test_find_and_erase\n";

    Table table;
    table.set("x", 10);
    table.set("y", 20);
    table.set("z", 30);

    auto it = table.find("y");
    assert(it != table.end());
    assert(it->second == 20);

    table.erase("y");
    assert(table.find("y") == table.end());
    assert(table.size() == 2);
}

void test_erase_by_iterator()
{
    std::cout << ">>> test_erase_by_iterator\n";

    Table table;
    table.set("x", 100);
    table.set("y", 200);
    table.set("z", 300);

    auto it = table.find("y");
    assert(it != table.end());

    table.erase(it);

    std::vector<std::string> keys;
    for (const auto &entry : table) {
        keys.push_back(entry.first);
    }

    assert((keys == std::vector<std::string>{"x", "z"}));
}

void test_clear_and_size()
{
    std::cout << ">>> test_clear_and_size\n";

    Table table;
    table.set("k1", 1);
    table.set("k2", 2);
    table.set("k3", 3);

    assert(table.size() == 3);
    table.clear();
    assert(table.size() == 0);
    assert(table.begin() == table.end());
}

void test_sort()
{
    std::cout << ">>> test_sort\n";

    Table table;
    table.set("gamma", 3);
    table.set("alpha", 1);
    table.set("beta", 2);

    table.sort([](const Table::list_entry_t &a, const Table::list_entry_t &b) { return a.first < b.first; });

    std::vector<std::string> keys;
    for (const auto &entry : table) {
        keys.push_back(entry.first);
    }

    assert((keys == std::vector<std::string>{"alpha", "beta", "gamma"}));
}

void test_at()
{
    std::cout << ">>> test_at\n";

    Table table;
    table.set("a", 10);
    table.set("b", 20);
    table.set("c", 30);

    auto it0 = table.at(0);
    auto it1 = table.at(1);
    auto it2 = table.at(2);

    assert(it0 != table.end() && it0->first == "a");
    assert(it1 != table.end() && it1->first == "b");
    assert(it2 != table.end() && it2->first == "c");

    // Out of bounds
    auto it_out = table.at(99);
    assert(it_out == table.end());
}

void test_copy_and_move()
{
    std::cout << ">>> test_copy_and_move\n";

    Table original;
    original.set("a", 1);
    original.set("b", 2);

    Table copy = original;
    assert(copy.size() == 2);
    assert(copy.find("a")->second == 1);

    Table moved = std::move(copy);
    assert(moved.size() == 2);
    assert(moved.find("b")->second == 2);
}

void test_iterator_consistency()
{
    std::cout << ">>> test_iterator_consistency\n";

    Table table;
    table.set("x", 10);
    auto it = table.set("y", 20);

    assert(it->first == "y");
    assert(it->second == 20);

    table.sort([](const Table::list_entry_t &a, const Table::list_entry_t &b) { return a.first > b.first; });

    assert(it->first == "y");
    assert(it->second == 20);
}

void test_order_after_set()
{
    std::cout << ">>> test_order_after_set\n";

    Table table;
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);

    // Update middle key
    table.set("b", 99);

    std::vector<std::string> keys;
    for (const auto &entry : table)
        keys.push_back(entry.first);

    assert((keys == std::vector<std::string>{"a", "b", "c"}));
    assert(table.find("b")->second == 99);
}

void test_set_vs_insert_logic()
{
    std::cout << ">>> test_set_vs_insert_logic\n";

    Table table;
    table.set("x", 100);
    table.set("x", 200); // Should overwrite, not add

    assert(table.size() == 1);
    assert(table.find("x")->second == 200);
}

void test_has()
{
    std::cout << ">>> test_has\n";

    Table table;
    table.set("a", 1);
    table.set("b", 2);

    assert(table.has("a"));
    assert(table.has("b"));
    assert(!table.has("c"));

    table.erase("a");
    assert(!table.has("a"));
}

void test_count()
{
    std::cout << ">>> test_count\n";

    Table table;
    table.set("x", 10);
    table.set("y", 20);

    assert(table.count("x") == 1);
    assert(table.count("y") == 1);
    assert(table.count("z") == 0);

    table.erase("x");
    assert(table.count("x") == 0);
}

void test_emplace()
{
    std::cout << ">>> test_emplace\n";

    ordered_map::ordered_map_t<std::string, Thing> table;

    table.emplace("obj1", 1, 2);
    table.emplace("obj2", 3, 4);

    auto it = table.find("obj1");
    assert(it != table.end());
    assert(it->second == Thing(1, 2));

    // Overwrite with new constructed object
    table.emplace("obj1", 5, 6);
    assert(table.find("obj1")->second == Thing(5, 6));
}

void test_reverse_iteration()
{
    std::cout << ">>> test_reverse_iteration\n";

    Table table;
    table.set("first", 1);
    table.set("second", 2);
    table.set("third", 3);

    std::vector<std::string> keys;
    for (auto it = table.rbegin(); it != table.rend(); ++it) {
        keys.push_back(it->first);
    }

    assert((keys == std::vector<std::string>{"third", "second", "first"}));
}

void test_front_and_back()
{
    std::cout << ">>> test_front_and_back\n";

    Table table;
    table.set("first", 100);
    table.set("middle", 200);
    table.set("last", 300);

    auto front = table.front();
    auto back  = table.back();

    assert(front != table.end());
    assert(back != table.end());

    assert(front->first == "first");
    assert(front->second == 100);

    assert(back->first == "last");
    assert(back->second == 300);
}

void test_index_of()
{
    std::cout << ">>> test_index_of\n";

    Table table;
    auto it0 = table.set("a", 10);
    auto it1 = table.set("b", 20);
    auto it2 = table.set("c", 30);

    assert(table.index_of(it0) == 0);
    assert(table.index_of(it1) == 1);
    assert(table.index_of(it2) == 2);
}

void test_keys_and_values()
{
    std::cout << ">>> test_keys_and_values\n";

    Table table;
    table.set("x", 10);
    table.set("y", 20);
    table.set("z", 30);

    auto keys = table.keys();
    auto vals = table.values();

    assert((keys == std::vector<std::string>{"x", "y", "z"}));
    assert((vals == std::vector<int>{10, 20, 30}));
}

void test_to_vector()
{
    std::cout << ">>> test_to_vector\n";

    Table table;
    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);

    auto vec = table.to_vector();

    assert(vec.size() == 3);
    assert(vec[0] == std::make_pair(std::string("a"), 1));
    assert(vec[1] == std::make_pair(std::string("b"), 2));
    assert(vec[2] == std::make_pair(std::string("c"), 3));
}

void test_empty()
{
    std::cout << ">>> test_empty\n";

    Table table;
    assert(table.empty()); // Should be true initially

    table.set("x", 1);
    assert(!table.empty()); // Should now be false

    table.clear();
    assert(table.empty()); // Back to true
}

void test_merge()
{
    std::cout << ">>> test_merge\n";

    Table table1;
    table1.set("a", 1);
    table1.set("b", 2);

    Table table2;
    table2.set("c", 3);
    table2.set("b", 99); // Will overwrite "b" in table1

    table1.merge(std::move(table2));

    assert(table1.size() == 3);
    assert(table1.find("a")->second == 1);
    assert(table1.find("b")->second == 99); // Overwritten
    assert(table1.find("c")->second == 3);
    assert(table2.empty()); // Source should be cleared
}

int main()
{
    std::cout << "Running ordered_map_t tests...\n";

    test_set_and_overwrite();
    test_find_and_erase();
    test_erase_by_iterator();
    test_clear_and_size();
    test_sort();
    test_at();
    test_copy_and_move();
    test_iterator_consistency();
    test_order_after_set();
    test_set_vs_insert_logic();
    test_has();
    test_count();
    test_emplace();
    test_reverse_iteration();
    test_front_and_back();
    test_index_of();
    test_keys_and_values();
    test_to_vector();
    test_empty();
    test_merge();

    std::cout << "All tests passed!\n";
    return 0;
}
