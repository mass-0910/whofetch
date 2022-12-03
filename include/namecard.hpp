#pragma once

#include <cstddef>
#include <map>
#include <string>

class namecard {
public:
    std::string get_icon_AA();
    std::string get_username();
    std::string get_machinename();
    std::string get_properties(std::string);

private:
    std::byte icon_ascii_art[40 * 20];
    std::string username;
    std::string machinename;
    std::map<std::string, std::string> properties;
};