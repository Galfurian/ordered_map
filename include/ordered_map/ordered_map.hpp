/// @file ordered_map.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief The ordered map class.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#pragma once

#include <cstdint>
#include <list>
#include <map>

enum : std::uint8_t {
    ORDERED_MAP_MAJOR_VERSION = 1, ///< Major version of the library.
    ORDERED_MAP_MINOR_VERSION = 0, ///< Minor version of the library.
    ORDERED_MAP_MICRO_VERSION = 2  ///< Micro version of the library.
};

/// @brief This namespace contains the main table class.
namespace ordered_map
{

/// @brief A wrapper for a `std::list` container, which uses a `std::map` for accessing the data.
/// @tparam Key the type of the key for building the `std::map`.
/// @tparam Value the value stored inside the `std::list`.
template <typename Key, typename Value>
class ordered_map_t
{
public:
    /// @brief This stores the key->value association.
    using list_entry_t    = std::pair<Key, Value>;
    /// @brief The actual storage.
    using list_t          = std::list<list_entry_t>;
    /// @brief Iterator for the list, for the user.
    using iterator        = typename list_t::iterator;
    /// @brief Constant iterator for the list, for the user.
    using const_iterator  = typename list_t::const_iterator;
    /// @brief The type of a compatible sort function.
    using sort_function_t = bool (*)(const list_entry_t &, const list_entry_t &);

    /// @brief Construct a new ordered map.
    ordered_map_t()
        : list()
        , table()
    {
        // Nothing to do.
    }

    /// @brief Copy constructor.
    /// @param other a reference to the map to copy.
    /// @details I had to define one, otherwise copying this map will screw up
    /// the copy of the iterators contained inside the `std::map`. That is why,
    /// here I copy each individual element of the list and store the iterator,
    /// again.
    ordered_map_t(const ordered_map_t &other)
        : list()
        , table()
    {
        for (const auto &entry : other.list) {
            table.insert({entry.first, list.insert(list.end(), entry)});
        }
    }

    /// @brief Move constructor.
    /// @param other a reference to the map to move.
    ordered_map_t(ordered_map_t &&other) noexcept
        : list(std::move(other.list))
        , table(std::move(other.table))
    {
        // Nothing to do.
    }

    /// @brief Move assignment operator.
    /// @param other a reference to the map to move.
    /// @return a reference to the current map.
    auto operator=(ordered_map_t &&other) noexcept -> ordered_map_t &
    {
        if (this != &other) {
            this->clear();
            list  = std::move(other.list);
            table = std::move(other.table);
        }
        return *this;
    }

    /// @brief Destructor.
    ~ordered_map_t() = default;

    /// @brief Clears the content of the map.
    void clear()
    {
        list.clear();
        table.clear();
    }

    /// @brief Returns the number of element in the map.
    /// @return the number of elements.
    [[nodiscard]]
    auto size() const -> std::size_t
    {
        return list.size();
    }

    /// @brief Sets/updates the `<key,value>` pair inside the map.
    /// @param key the value identifier.
    /// @param value the actual value.
    /// @return the iterator to the newly inserted/updated element in the map.
    auto set(const Key &key, const Value &value) -> iterator
    {
        // First, we search for the element inside the table.
        table_iterator it_table = table.find(key);
        // Create the return iterator.
        iterator it_list;
        // Now, if the element is not in the table, we need to add itr.
        if (it_table == table.end()) {
            // First, we add the element to the list and get the pointer to the
            // newly inserted element.
            it_list = list.insert(list.end(), std::make_pair(key, value));
            // Finally, we store inside the map the `<key, iteartor>` pair to
            // easily access the element inside the list.
            table.insert(std::make_pair(key, it_list));
        } else {
            // First, we get the list iterator.
            it_list         = it_table->second;
            // Then, we update the associated value.
            it_list->second = value;
        }
        return it_list;
    }

    /// @brief Returns an iterator the beginning of the list.
    /// @return an iterator to the beginning of the list.
    auto begin() -> iterator { return list.begin(); }

    /// @brief Returns a const iterator the beginning of the list.
    /// @return an iterator to the beginning of the list.
    auto begin() const -> const_iterator { return list.begin(); }

    /// @brief Returns an iterator the end of the list.
    /// @return an iterator to the end of the list.
    auto end() -> iterator { return list.end(); }

    /// @brief Returns a const iterator the end of the list.
    /// @return an iterator to the end of the list.
    auto end() const -> const_iterator { return list.end(); }

    /// @brief Erases the elment from the list, and returns an iteator to the
    /// same position in the list (i.e., the elment after the one removed).
    /// @param key the key of the element to remove.
    /// @return an iterator to the same position in the list.
    auto erase(const Key &key) -> iterator
    {
        table_iterator it_table = table.find(key);
        if (it_table == table.end()) {
            return list.end();
        }
        iterator it_list = it_table->second;
        ++it_list;
        list.erase(it_table->second);
        table.erase(it_table);
        return it_list;
    }

    /// @brief Erases the elment from the list, and returns an iteator to the
    /// same position in the list (i.e., the elment after the one removed).
    /// @param it_list the iterator of the element to remove.
    /// @return an iterator to the same position in the list.
    auto erase(iterator it_list) -> iterator
    {
        table_iterator it_table = table.find(it_list->first);
        if (it_table == table.end()) {
            return list.end();
        }
        ++it_list;
        list.erase(it_table->second);
        table.erase(it_table);
        return it_list;
    }

    /// @brief Returns an iterator to the element in the given position.
    /// @param position the position of the element to retrieve.
    /// @return an iterator to the element, or the end of the list if not found.
    auto at(std::size_t position) -> iterator
    {
        iterator itr = list.begin();
        std::advance(itr, std::min(position, list.size()));
        return itr;
    }

    /// @brief Returns an iterator to the element in the given position.
    /// @param position the position of the element to retrieve.
    /// @return an iterator to the element, or the end of the list if not found.
    auto at(std::size_t position) const -> const_iterator
    {
        const_iterator itr = list.begin();
        std::advance(itr, std::min(position, list.size()));
        return itr;
    }

    /// @brief Returns an iterator to the element associated with the given key.
    /// @param key the key of the element to search for.
    /// @return an iterator to the element, or the end of the list if not found.
    auto find(const Key &key) -> iterator
    {
        table_iterator itr = table.find(key);
        if (itr == table.end()) {
            return list.end();
        }
        return itr->second;
    }

    /// @brief Returns an iterator to the element associated with the given key.
    /// @param key the key of the element to search for.
    /// @return an iterator to the element, or the end of the list if not found.
    auto find(const Key &key) const -> const_iterator
    {
        table_const_iterator itr = table.find(key);
        if (itr == table.end()) {
            return list.end();
        }
        return itr->second;
    }

    /// @brief Sorts the internal list.
    /// @param fun the sorting function.
    void sort(const sort_function_t &fun) { list.sort(fun); }

    /// @brief Assign operator.
    /// @param other a reference to the map to copy.
    /// @details I had to define one, otherwise copying this map will screw up
    /// the copy of the iterators contained inside the `std::map`. That is why,
    /// here I copy each individual element of the list and store the iterator,
    /// again.
    /// @return a reference to the current map.
    auto operator=(const ordered_map_t &other) -> ordered_map_t &
    {
        if (this != &other) {
            this->clear();
            for (const auto &entry : other.list) {
                table.insert({entry.first, list.insert(list.end(), entry)});
            }
        }
        return *this;
    }

private:
    /// @brief Type of the map.
    using table_t              = std::map<Key, iterator>;
    using table_iterator       = typename table_t::iterator;
    using table_const_iterator = typename table_t::const_iterator;
    /// @brief The list containing the actual data.
    list_t list;
    /// @brief A table for easy access to the data by using a key.
    table_t table;
};

} // namespace ordered_map
