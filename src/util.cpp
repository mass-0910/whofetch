#include "util.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

void print_error(std::string err_msg) {
    std::cerr << "error: " << err_msg << std::endl;
}

std::string ansi_color(int r, int g, int b, bool bold) {
    std::ostringstream oss;
    if (bold) {
        oss << "\x1b[1m";
    }
    oss << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
    return oss.str();
}

std::string ansi_backcolor(int colorcode) {
    std::ostringstream oss;
    oss << "\x1b[48;5;" << colorcode << "m";
    return oss.str();
}

std::string ansi_backcolor(int r, int g, int b) {
    std::ostringstream oss;
    oss << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
    return oss.str();
}

std::string ansi_reset() {
    return "\x1b[0m";
}

std::string ansi_move_cursor(int x, int y) {
    std::ostringstream oss;
    if (x > 0) {
        oss << "\x1b[" << x << "C";
    } else if (x < 0) {
        oss << "\x1b[" << -x << "D";
    }
    if (y > 0) {
        oss << "\x1b[" << y << "B";
    } else if (y < 0) {
        oss << "\x1b[" << -y << "A";
    }
    return oss.str();
}

std::string ansi_abs_from_left(int x) {
    std::ostringstream oss;
    oss << "\x1b[" << x << "G";
    return oss.str();
}

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}