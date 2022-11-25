#include "ordered_map/ordered_map.hpp"

#include <iostream>
#include <sstream>

typedef ordered_map::ordered_map_t<std::string, int> Table;

inline void print_at(const Table &table, std::size_t i)
{
    Table::const_iterator it;
    if ((it = table.at(i)) != table.end())
        std::cout << it->second << ", ";
}

inline void print_with_key(const Table &table, const std::string &key)
{
    Table::const_iterator it;
    if ((it = table.find(key)) != table.end())
        std::cout << it->second << ", ";
}

class Foo {
public:
    Table table;
};

int main(int, char *[])
{
    Table table;

    table.set("a", 1);
    table.set("b", 2);
    table.set("c", 3);

    std::cout << "\n";
    print_with_key(table, "a");
    print_with_key(table, "b");
    print_with_key(table, "c");
    std::cout << "\n";
    for (std::size_t i = 0; i < 10; ++i)
        print_at(table, i);
    std::cout << "\n";

    Table table2 = table;

    table2.set("c", 4);

    std::cout << "\n";
    print_with_key(table2, "a");
    print_with_key(table2, "b");
    print_with_key(table2, "c");
    std::cout << "\n";
    for (std::size_t i = 0; i < 10; ++i)
        print_at(table2, i);
    std::cout << "\n";

    table2.set("d", 5);
    table2.erase("c");

    std::cout << "\n";
    print_with_key(table2, "a");
    print_with_key(table2, "b");
    print_with_key(table2, "c");
    print_with_key(table2, "d");
    std::cout << "\n";
    for (std::size_t i = 0; i < 10; ++i)
        print_at(table2, i);
    std::cout << "\n";

    Table::iterator it_erase;
    if ((it_erase = table2.find("d")) != table2.end())
        table2.erase(it_erase);

    std::cout << "\n";
    print_with_key(table2, "a");
    print_with_key(table2, "b");
    print_with_key(table2, "c");
    print_with_key(table2, "d");
    std::cout << "\n";
    for (std::size_t i = 0; i < 10; ++i)
        print_at(table2, i);
    std::cout << "\n";

    return 0;
}