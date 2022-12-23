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
 * @brief Get ANSI color code as a string
 *
 * @param r red
 * @param g green
 * @param b blue
 * @param bold bold
 * @return ANSI color code
 */
std::string ansi_color(int r, int g, int b, bool bold = false);

/**
 * @brief Get ANSI back color code as a string
 *
 * @param colorcode ANSI color code
 * @return ANSI color code
 */
std::string ansi_backcolor(int colorcode);

/**
 * @brief Get ANSI back color code as a string
 *
 * @param r red
 * @param g green
 * @param b blue
 * @return ANSI color code
 */
std::string ansi_backcolor(int r, int g, int b);

/**
 * @brief Get ANSI reset code as a string
 *
 * @return ANSI reset code
 */
std::string ansi_reset();

/**
 * @brief Get ANSI move cursor code as a string
 *
 * @param x move value x
 * @param y move value y
 * @return ANSI move code
 */
std::string ansi_move_cursor(int x, int y);

/**
 * @brief Get ANSI cursor code as a string
 *
 * @param x absolute value x
 * @return ANSI code
 */
std::string ansi_abs_from_left(int x);

/**
 * @brief Returns a pointer to the element with the specified name.
 *
 * @tparam T The type of the structure.
 * @param vec The vector to search for the name.
 * @param name The name to search for.
 * @return A pointer to the element with the specified name. nullptr if not found.
 */
template <typename T>
T* find_by_name(std::vector<T>& vec, std::string name) {
    for (auto& elem : vec) {
        if (elem.name == name) {
            return &elem;
        }
    }
    return nullptr;
}

/**
 * @brief Trim brank character from string
 *
 * @param s string
 * @return trimmed string
 */
std::string trim(const std::string& s);