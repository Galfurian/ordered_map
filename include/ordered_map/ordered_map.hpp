/// @file ordered_map.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief The ordered map class.
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include <list>
#include <map>
#include <vector>

enum : unsigned char {
    ORDERED_MAP_MAJOR_VERSION = 1, ///< Major version of the library.
    ORDERED_MAP_MINOR_VERSION = 0, ///< Minor version of the library.
    ORDERED_MAP_MICRO_VERSION = 5, ///< Micro version of the library.
};

/// @brief This namespace contains the main table class.
namespace ordered_map
{

/// @brief A wrapper for a `std::list` container, which uses a `std::map` for accessing the data.
/// @tparam Key the type of the key for building the `std::map`.
/// @tparam Value the value stored inside the `std::list`.
template <typename Key, typename Value> class ordered_map_t
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
    auto size() const -> std::size_t { return list.size(); }

    /// @brief Checks whether the map is empty.
    /// @return True if empty, false otherwise.
    auto empty() const -> bool { return list.empty(); }

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

    /// @brief Constructs a value in-place at the end of the map with the given key.
    /// @details If the key already exists, its value is updated. If not,
    /// a new entry is emplaced at the end of the list.
    /// @tparam Args Types of arguments to construct a `Value`.
    /// @param key The key associated with the new value.
    /// @param args Arguments forwarded to construct the `Value`.
    /// @return An iterator to the inserted or updated element.
    template <typename... Args> auto emplace(const Key &key, Args &&...args) -> iterator
    {
        table_iterator it_table = table.find(key);
        iterator it_list;
        if (it_table == table.end()) {
            it_list = list.emplace(
                list.end(), std::piecewise_construct, std::forward_as_tuple(key),
                std::forward_as_tuple(std::forward<Args>(args)...));
            table.insert(std::make_pair(key, it_list));
        } else {
            it_list         = it_table->second;
            it_list->second = Value(std::forward<Args>(args)...);
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

    /// @brief Returns a reverse iterator to the last element in the list.
    /// @return A reverse iterator to the last element.
    auto rbegin() -> typename list_t::reverse_iterator { return list.rbegin(); }

    /// @brief Returns a const reverse iterator to the last element in the list.
    /// @return A const reverse iterator to the last element.
    auto rbegin() const -> typename list_t::const_reverse_iterator { return list.rbegin(); }

    /// @brief Returns a reverse iterator to the position before the first element.
    /// @return A reverse iterator to the position before the first.
    auto rend() -> typename list_t::reverse_iterator { return list.rend(); }

    /// @brief Returns a const reverse iterator to the position before the first element.
    /// @return A const reverse iterator to the position before the first.
    auto rend() const -> typename list_t::const_reverse_iterator { return list.rend(); }

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

    /// @brief Returns a vector of all keys in insertion order.
    /// @return A vector containing the keys.
    auto keys() const -> std::vector<Key>
    {
        std::vector<Key> result;
        result.reserve(list.size());
        for (const auto &entry : list) {
            result.push_back(entry.first);
        }
        return result;
    }

    /// @brief Returns a vector of all values in insertion order.
    /// @return A vector containing the values.
    auto values() const -> std::vector<Value>
    {
        std::vector<Value> result;
        result.reserve(list.size());
        for (const auto &entry : list) {
            result.push_back(entry.second);
        }
        return result;
    }

    /// @brief Returns an iterator to the first element in the list.
    /// @return An iterator to the first element.
    auto front() -> iterator { return list.begin(); }

    /// @brief Returns a const iterator to the first element in the list.
    /// @return A const iterator to the first element.
    auto front() const -> const_iterator { return list.begin(); }

    /// @brief Returns an iterator to the last element in the list.
    /// @return An iterator to the last element.
    auto back() -> iterator { return std::prev(list.end()); }

    /// @brief Returns a const iterator to the last element in the list.
    /// @return A const iterator to the last element.
    auto back() const -> const_iterator { return std::prev(list.end()); }

    /// @brief Returns the index of the given iterator in the internal list.
    /// @details This computes the distance between `begin()` and the provided iterator.
    /// The behavior is undefined if the iterator is not from this container.
    /// @param it The iterator whose position to determine.
    /// @return The zero-based index of the iterator.
    auto index_of(const const_iterator &it) const -> std::size_t { return std::distance(list.begin(), it); }

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

    /// @brief Checks whether an element with the given key exists.
    /// @details This function is a shorthand for `find(key) != end()`.
    /// It improves readability when checking for key existence.
    /// @param key The key to check.
    /// @return True if the key exists, false otherwise.
    auto has(const Key &key) const -> bool { return table.find(key) != table.end(); }

    /// @brief Sorts the internal list.
    /// @param fun the sorting function.
    void sort(const sort_function_t &fun) { list.sort(fun); }

    /// @brief Counts how many elements exist with the given key.
    /// @details Since this is a map (not a multimap), this will return either 0 or 1.
    /// @param key The key to check.
    /// @return 1 if the key exists, 0 otherwise.
    auto count(const Key &key) const -> std::size_t { return table.count(key); }

    /// @brief Returns all key-value pairs as a vector in insertion order.
    /// @return A vector of key-value pairs.
    auto to_vector() const -> std::vector<list_entry_t> { return std::vector<list_entry_t>(list.begin(), list.end()); }

    /// @brief Merges contents from another map, overwriting any existing keys.
    /// @param other The other map to merge from.
    void merge(ordered_map_t &&other)
    {
        for (auto &entry : other.list) {
            set(std::move(entry.first), std::move(entry.second));
        }
        other.clear();
    }

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
