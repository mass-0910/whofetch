#pragma once

#include <string>
#include <vector>

/**
 * @brief Print error message
 *
 * @param err_msg Error message
 */
void print_error(std::string err_msg);

/**
 * @brief Returns a pointer to the element with the specified name.
 *
 * @tparam T The type of the structure.
 * @param vec The vector to search for the name.
 * @param name The name to search for.
 * @return A pointer to the element with the specified name. nullptr if not found.
 */
template <typename T>
T* find_by_name(std::vector<T>& vec, std::string& name) {
    for (auto& elem : vec) {
        if (elem.name == name) {
            return &elem;
        }
    }
    return nullptr;
}