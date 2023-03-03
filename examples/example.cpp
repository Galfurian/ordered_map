#include <iostream>
#include <sstream>

#include "ordered_map/ordered_map.hpp"

typedef ordered_map::ordered_map_t<std::string, int> Table;

inline void print(const Table &table)
{
    std::cout << "{ ";
    for (Table::const_iterator it = table.begin(); it != table.end(); ++it)
        std::cout << "[" << it->first << "](" << it->second << ") ";
    std::cout << "}\n";
}

bool compare_gt(const Table::list_entry_t &lhs, const Table::list_entry_t &rhs)
{
    return lhs.first > rhs.first;
}

bool compare_lt(const Table::list_entry_t &lhs, const Table::list_entry_t &rhs)
{
    return lhs.first < rhs.first;
}

int main(int, char *[])
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
    if ((it_erase = table.find("d")) != table.end())
        table.erase(it_erase);
    print(table);

    // Sort the value but preserve the internal mechanisms, i.e., the key value
    // association.
    table.sort(compare_gt);
    print(table);
    table.sort(compare_lt);
    print(table);

    return 0;
}
