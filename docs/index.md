# Ordered Map

[![Ubuntu](https://github.com/Galfurian/ordered_map/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/ordered_map/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/ordered_map/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/ordered_map/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/documentation.yml)

A C++ container that behaves like a map but **preserves the order of insertion** and offers **efficient key lookup**.

## Overview

The **`ordered_map`** library provides an `ordered_map_t<Key, Value>` class template that combines:

- A `std::list` to preserve insertion order.
- A `std::map<Key, iterator>` for efficient key lookup.

It supports **unique keys**, **stable iterators**, and **ordered traversal**.

## Features

- **Insertion-order iteration**
- **Efficient key lookup** via internal `std::map`
- **Stable iterators** — safe during most operations
- **Custom sorting** with user-defined comparators
- **Rich API** including:
  - `set`, `emplace`, `erase`, `find`, `count`, `has`
  - `equal_range`, `merge`, `index_of`
  - `front`, `back`, `keys`, `values`, `to_vector`
- **Full iterator support**: `begin`, `end`, `rbegin`, `rend`

## Summary of Trade-Offs

| Feature                   | std::map | std::unordered_map  | `ordered_map_t`                 |
|---------------------------|----------|---------------------|---------------------------------|
| Maintains key order       | V        | X                   | Optional via `sort()`           |
| Maintains insertion order | X        | X                   | V                               |
| Allows duplicate keys     | X        | X                   | X (use `ordered_multimap_t`)    |
| Lookup time               | O(log N) | O(1) avg            | O(log N) via internal map       |
| Insert/remove performance | O(log N) | O(1) avg            | O(1) for list, O(log N) for map |
| Iterator stability        | X        | X                   | V                               |
| Custom sorting            | X        | X                   | V                               |

## Requirements

- **C++11** or later.
- Standard C++ libraries (`<list>`, `<map>`, `<utility>`, etc.)

## Installation

To use the `ordered_map_t` class in your project, include the header file:

```c++
#include "ordered_map.hpp"
```

Ensure the `ordered_map.hpp` file is in your project's include path.

## Usage

Below is an example showcasing basic usage of `ordered_map_t`:

```c++
#include "ordered_map.hpp"
#include <iostream>

int main()
{
    ordered_map::ordered_map_t<int, std::string> omap;

    omap.set(1, "one");
    omap.set(2, "two");
    omap.set(3, "three");

    // Iteration preserves insertion order.
    for (const auto &entry : omap)
    {
        std::cout << entry.first << ": " << entry.second << '\n';
    }

    // Overwrite a value.
    omap.set(2, "dos");

    // Lookup.
    auto it = omap.find(2);
    if (it != omap.end())
    {
        std::cout << "Found: " << it->first << " -> " << it->second << '\n';
    }

    // Merge another map.
    ordered_map::ordered_map_t<int, std::string> other;
    other.set(4, "four");
    omap.merge(std::move(other));

    // Convert to vector.
    auto vec = omap.to_vector();
    for (const auto &[key, val] : vec)
    {
        std::cout << key << " = " << val << '\n';
    }
}
```

## License

This project is licensed under the **MIT License**.

Copyright (c) 2024 Enrico Fraccaroli <enry.frak@gmail.com>.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.