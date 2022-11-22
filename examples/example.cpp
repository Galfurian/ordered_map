#include "ordered_map/ordered_map.hpp"

#include <iostream>
#include <sstream>

int main(int, char *[])
{
    typedef ordered_map::ordered_map_t<std::string, int> Table;
    Table table;

    table.push("a", 5);
    table.push("b", 8);
    table.push("c", 12);

    Table::const_iterator it;
    if ((it = table.find("a")) != table.end())
        std::cout << it->second << "\n";
    if ((it = table.find("b")) != table.end())
        std::cout << it->second << "\n";
    if ((it = table.find("c")) != table.end())
        std::cout << it->second << "\n";
    if ((it = table.at(0)) != table.end())
        std::cout << it->second << "\n";
    if ((it = table.at(1)) != table.end())
        std::cout << it->second << "\n";
    if ((it = table.at(2)) != table.end())
        std::cout << it->second << "\n";
    std::cout << "\n";
    return 0;
}