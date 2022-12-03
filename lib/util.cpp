#include "util.hpp"
#include "config.h"

#include <filesystem>
#include <iostream>

void print_error(std::string err_msg) {
    std::cerr << "error: " << err_msg << std::endl;
}

void print_usage(char *argv_0) {
    std::string program_name;
    if (std::filesystem::exists(argv_0)) {
        program_name = std::filesystem::path(argv_0).filename().string();
    } else {
        program_name = argv_0;
    }

    std::cout << "usage: " << program_name << " [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "    --help | -h                 Print this message" << std::endl;
}

void print_version(char *argv_0) {
    std::string program_name;
    if (std::filesystem::exists(argv_0)) {
        program_name = std::filesystem::path(argv_0).filename().string();
    } else {
        program_name = argv_0;
    }

    std::cout << program_name << " version " << PRODUCT_VERSION << std::endl;
}

std::map<std::string, std::string> parse_args(int argc, char *argv[], std::vector<std::string> &option_arg, int *arg_num_buf) {
    int arg_num = 0;
    std::string arg;
    std::string option_name;
    std::map<std::string, std::string> parsed_arg_map;
    for (int i = 0; i < argc; i++) {
        arg = argv[i];
        if (arg[0] == '-') {  // if the argument is option
            if (arg[1] == '-') {
                option_name = arg.substr(2);
            } else {
                option_name = arg.substr(1);
            }
            for (auto a : option_arg) {
                if (a == option_name) {
                    if (i + 1 < argc) {
                        parsed_arg_map[option_name] = argv[i + 1];
                        i++;
                        break;
                    } else {
                        print_error("option " + arg + " requires a option");
                        exit(1);
                    }
                }
            }
            // Store empty string if option do not have any itself options
            if (parsed_arg_map.find(option_name) == parsed_arg_map.end()) {
                parsed_arg_map[option_name] = "";
            }
        } else {
            parsed_arg_map["@" + std::to_string(arg_num++)] = arg;
        }
    }
    if (arg_num_buf) *arg_num_buf = arg_num;
    return parsed_arg_map;
}

bool contains_option(std::map<std::string, std::string> &parsed_args, std::string option_name) {
    return parsed_args.find(option_name) != parsed_args.end();
}