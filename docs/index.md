# Ordered Map

[![Ubuntu](https://github.com/Galfurian/ordered_map/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/ordered_map/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/ordered_map/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/ordered_map/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/ordered_map/actions/workflows/documentation.yml)

A c++ map that can preserves the order of insertion.

## Overview

The **`ordered_map`** library provides an `ordered_map_t` class template that
wraps a `std::list` container, while leveraging a `std::map` for efficient
access to its elements by key. This data structure maintains the order of
insertion and provides constant-time key-based access.

## Features

- **Ordered Storage**: Maintains the order in which elements are inserted.
- **Fast Key-Based Lookup**: Uses `std::map` to enable efficient element access.
- **Customizable Sorting**: Supports sorting the internal list with user-defined
  comparison functions.
- **Flexible API**: Provides iterators for traversal and modification of the
  stored elements.

## Requirements

- **C++11** or later.
- Standard C++ libraries: `<list>` and `<map>`.

## Installation

To use the `ordered_map_t` class in your project, include the header file:

```c++
#include "ordered_map.hpp"
```

Ensure the `ordered_map.hpp` file is in your project's include path.

## Usage

Below is an example showcasing basic usage of `ordered_map_t`:

```c++
#include "ordered_map.hpp" #include <iostream>

int main() {
    ordered_map::ordered_map_t<int, std::string> my_map;
    // Add elements
    my_map.set(1, "One");
    my_map.set(2, "Two");
    my_map.set(3, "Three");

    // Iterate and print
    for (auto it = my_map.begin(); it != my_map.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << std::endl;

    // Find an element
    auto it = my_map.find(2);
    if (it != my_map.end()) {
        std::cout << "Found: " << it->first << " -> " << it->second << std::endl;
    }

    // Erase an element
    my_map.erase(2);

    return 0;
}
```

Output:

```bash
1: One
2: Two
3: Three

Found: 2 -> Two
```

## License

This project is licensed under the **MIT License**.

See the [LICENSE.md](LICENSE.md) file for details.
