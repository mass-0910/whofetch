#include <iostream>
#include <vector>
#include <regex>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "whofetch.hpp"
#include "util.hpp"
#include "cmake_config.h"
#include "jp2a.h"

whofetch::whofetch() {}

whofetch_error_t whofetch::show_my_whofetch() {
    resource name("name", "myself"), company("company", "myself"), parameters("parameter", "myself"), icon_aa("icon", "myself");
    if (!(name && company && parameters && icon_aa)) {
        print_error("User data not registered");
        std::cout << "First, register your data with the following command." << std::endl << std::endl;
        std::cout << "whofetch --make" << std::endl;
        return WHOFETCH_NOT_FOUND;
    }
    print_icon_aa(icon_aa);
    std::cout << ansi_move_cursor(43, -21);
    int r, g, b;
    most_distinctive_color(icon_aa, r, g, b);
    std::cout << ansi_color(r, g, b, true) << name.string() << ansi_reset() << "@" << ansi_color(r, g, b, true) << company.string();
    size_t size = name.string().length() + company.string().length() + 1;
    std::cout << ansi_reset() << ansi_move_cursor(-(int)size, 1);
    for (int i = 0; i < size; i++) {
        std::cout << "-";
    }
    std::cout << ansi_move_cursor(-(int)size, 1);
    int params;
    print_parameters(parameters, r, g, b, params);
    std::cout << ansi_move_cursor(0, 1);
    for (int i = 0; i < 16; i++) {
        std::cout << ansi_backcolor(i) << "   ";
        if (i == 7) {
            std::cout << ansi_move_cursor(-24, 1);
        }
    }
    std::cout << ansi_move_cursor(0, 40 - (2 + params + 3)) << ansi_abs_from_left(0) << ansi_reset();
    return WHOFETCH_OK;
}

whofetch_error_t whofetch::show_whofetch(std::string name) {
    std::vector<std::byte> whofetch_buf;
    resource whofetch_profile_resource(name + "#whofetch");
    whofetch_profile_resource.copy_resource_to_buffer(whofetch_buf);
    std::cout << whofetch_buf.data() << std::endl;
    return WHOFETCH_OK;
}

whofetch_error_t whofetch::make_whofetch() {
    std::string input;
    const int PARAMETER_NUM_MAX = 15;
    bool input_is_valid;
    std::cout << "whofetch interactive setting console v" << PRODUCT_VERSION << std::endl;

    {
        resource user_name("name", "myself");
        if (!user_name) {
            // Get user name
            input_is_valid = false;
            while (!input_is_valid) {
                std::cout << "What is your name?:";
                std::getline(std::cin, input);
                if (trim(input).empty()) {
                    std::cout << "Warning! name cannot be empty." << std::endl;
                    continue;
                }
                std::string invalid_char = "/;:";
                input_is_valid = true;
                for (char c : invalid_char) {
                    if (input.find(c) != std::string::npos) {
                        std::cout << "Warning! You cannot include '" << c << "' into name." << std::endl;
                        input_is_valid = false;
                    }
                }
            }
            user_name.make_resource(input);
        }
    }

    {
        resource company_name("company", "myself");
        if (!company_name) {
            // Get user company
            input_is_valid = false;
            while (!input_is_valid) {
                std::cout << "What company do you belong to?:";
                std::getline(std::cin, input);
                if (trim(input).empty()) {
                    std::cout << "Warning! company cannot be empty." << std::endl;
                    continue;
                }
                input_is_valid = true;
            }
            company_name.make_resource(input);
        }
    }

    {
        resource user_parameter("parameter", "myself");
        bool recieve_end = false;
        if (!user_parameter) {
            // Get user profile parameters
            std::string parameter_name[PARAMETER_NUM_MAX];
            std::string parameter_value[PARAMETER_NUM_MAX];
            int parameter_count = 0;
            for (int i = 0; i < PARAMETER_NUM_MAX; i++) {
                input_is_valid = false;
                std::regex re(R"((\w+):(.+))");
                std::smatch m;
                while (!input_is_valid) {
                    std::cout << "Set your params in the formatted string [Param:Value] (enpty=end):";
                    std::getline(std::cin, input);
                    if (trim(input).empty()) {
                        recieve_end = true;
                        break;
                    }
                    // Parse input with regex
                    if (!std::regex_match(input, m, re)) {
                        std::cout << "Warning! Invalid input." << std::endl;
                    } else {
                        input_is_valid = true;
                        for (int j = 0; j < i; j++) {
                            if (parameter_name[j] == m[1]) {
                                std::cout << "Warning! " << m[1] << " is already specified." << std::endl;
                                input_is_valid = false;
                                break;
                            }
                        }
                    }
                }
                if (recieve_end) {
                    break;
                }
                parameter_name[i] = trim(m[1]);
                parameter_value[i] = trim(m[2]);
                parameter_count++;
            }
            std::string parameter_buf;
            for (int i = 0; i < parameter_count; i++) {
                parameter_buf += parameter_name[i] + "\n";
                parameter_buf += parameter_value[i] + "\n";
            }
            user_parameter.make_resource(parameter_buf);
        }
    }

    {
        resource icon_aa("icon", "myself");
        if (!icon_aa) {
            input_is_valid = false;
            while (!input_is_valid) {
                std::cout << "Set your icon source path (JPEG or PNG):";
                std::getline(std::cin, input);
                if (std::filesystem::is_regular_file(input)) {
#ifdef _WIN32
                    char *value_p;
                    size_t size;
                    errno_t err = _dupenv_s(&value_p, &size, "TEMP");
                    if (err) {
                        print_error("Cannot find environment varilable 'TEMP'");
                        return WHOFETCH_FAILURE_MAKE_PROFILE;
                    }
                    std::filesystem::path temp_dir(value_p);
#else
                    std::filesystem::path temp_dir(std::getenv("TEMP"));
#endif
                    auto temp_file = temp_dir.append("whofetch_temp_AA.txt");
                    std::string output_option = std::string("--output=") + temp_file.string();
                    std::vector<const char *> jp2a_args = {"jp2a", "--size=40x20", "--colors", output_option.c_str(), input.c_str()};
                    jp2a_main(static_cast<int>(jp2a_args.size()), (char **)jp2a_args.data());
                    size_t file_size = std::filesystem::file_size(temp_file);
                    {
                        std::ifstream ifs(temp_file.string(), std::ios::binary);
                        if (!ifs) {
                            print_error("Cannot open temporary file.");
                            return WHOFETCH_FAILURE_MAKE_PROFILE;
                        }
                        std::vector<std::byte> icon_aa_buf(file_size);
                        ifs.read(reinterpret_cast<char *>(icon_aa_buf.data()), file_size);
                        icon_aa.make_resource(icon_aa_buf.data(), icon_aa_buf.size());
                        std::cout << "========== Your icon preview ===========" << std::endl;
                        print_icon_aa(icon_aa);
                    }
                    std::filesystem::remove(temp_file);
                    std::cout << "This image will be registered as your icon. Do you want to change?[y/N]:";
                    std::cin.ignore();
                    std::getline(std::cin, input);
                    if (input == "" || input == "N" || input == "n") {
                        input_is_valid = true;
                    }
                } else {
                    std::cout << "Warning! " << input << " does not exist." << std::endl;
                }
            }
        }
    }

    return WHOFETCH_OK;
}

whofetch_error_t whofetch::remove_my_whofetch() {
    resource name("name", "myself"), company("company", "myself"), parameters("parameter", "myself"), icon_aa("icon", "myself");
    name.delete_resource();
    company.delete_resource();
    parameters.delete_resource();
    icon_aa.delete_resource();
    return WHOFETCH_OK;
}

void whofetch::most_distinctive_color(resource &icon, int &r, int &g, int &b) {
    std::vector<std::byte> buf;
    icon.copy_resource_to_buffer(buf);
    std::regex re(R"(38;2;([0-9]+);([0-9]+);([0-9]+)m(.))");
    std::smatch sm;
    std::string icon_aa_str(reinterpret_cast<const char *>(buf.data()), buf.size());
    std::string::const_iterator search_start(icon_aa_str.cbegin());
    int count = 0;
    r = g = b = 0;
    while (std::regex_search(search_start, icon_aa_str.cend(), sm, re)) {
        if (sm[4] != " ") {
            r += std::stoi(sm[1]);
            g += std::stoi(sm[2]);
            b += std::stoi(sm[3]);
            count++;
        }
        search_start = sm.suffix().first;
        count++;
    }
    if (count != 0) {
        r /= count;
        g /= count;
        b /= count;
    }
}

void whofetch::print_icon_aa(resource &icon) {
    std::vector<std::byte> output_buf;
    icon.copy_resource_to_buffer(output_buf);
    output_buf.resize(output_buf.size() + 1);
    output_buf[output_buf.size() - 1] = (std::byte)'\0';
    std::cout << (char *)output_buf.data() << std::endl;
}

void whofetch::print_parameters(resource &parameters, int r, int g, int b, int &param_num) {
    std::istringstream iss(parameters.string());
    std::string line;
    bool is_parameter_name = true;
    param_num = 0;
    while (std::getline(iss, line)) {
        if (is_parameter_name) {
            std::cout << ansi_color(r, g, b, true) << line << ansi_reset() << ":";
            param_num++;
        } else {
            std::cout << " " << line << ansi_move_cursor(0, 1) << ansi_abs_from_left(44);
        }
        is_parameter_name = !is_parameter_name;
    }
}

void make_combined_whofetch() {}