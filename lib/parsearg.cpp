#include <filesystem>
#include <iostream>
#include <iomanip>

#include "config.h"
#include "util.hpp"
#include "parsearg.hpp"

struct argument_record_t {
    std::string name;
    std::string description;
    bool is_optional;
};

struct option_record_t {
    std::string name;
    std::string description;
    char short_name;
    bool has_argument;
};

using parsed_arg_t = std::map<std::string, std::string>;

static parsed_arg_t parsed_args;
static std::map<char, option_record_t> short_option;
static char* argv_0;
static std::vector<argument_record_t> argument_list;
static std::vector<option_record_t> option_list;
static unsigned int argument_len_max = 0;
static unsigned int option_len_max = 0;
static int required_argument_num = 0;
static bool optional_argument_has_passed = false;

using namespace parsearg;

static std::string get_program_name() {
    std::string program_name;
    if (std::filesystem::exists(argv_0)) {
        program_name = std::filesystem::path(argv_0).filename().string();
    } else {
        program_name = argv_0;
    }
    return program_name;
}

void parsearg::argument(std::string argument_name, std::string description, bool is_optional) {
    argument_list.push_back(argument_record_t{argument_name, description});
    if (argument_name.length() > argument_len_max) {
        argument_len_max = argument_name.length();
    }
    if (is_optional) {
        if (!optional_argument_has_passed) {
            optional_argument_has_passed = true;
        }
    } else {
        required_argument_num++;
        if (optional_argument_has_passed) {
            print_error("You can't add required arguments after optional arguments.");
            exit(1);
        }
    }
}

void parsearg::option(std::string option_name, std::string description, bool has_argument, char short_option_name) {
    option_list.push_back(option_record_t{option_name, description, short_option_name, has_argument});
    if (short_option_name) {
        short_option[short_option_name] = option_list.back();
    }
    if (option_name.length() > option_len_max) {
        option_len_max = option_name.length();
    }
}

void parsearg::print_usage(std::string argument_descriptions) {
    std::cout << "usage: " << get_program_name() << " " << argument_descriptions << std::endl << std::endl;
    // Print argument usage
    if (argument_list.size() > 0) {
        std::cout << "Arguments:" << std::endl;
        for (auto arg : argument_list) {
            std::cout << "    " << std::setw(argument_len_max + 4) << std::left << arg.name;
            std::cout << arg.description << std::endl;
        }
        std::cout << std::endl;
    }
    // Print option usage
    if (option_list.size() > 0) {
        std::cout << "Options:" << std::endl;
        std::string opt_print;
        for (auto opt : option_list) {
            if (opt.short_name) {
                std::cout << "    " << std::setw(option_len_max + 2) << std::left << std::string("--") + opt.name;
                std::cout << std::setw(8) << std::left << std::string(" | -") + opt.short_name;
            } else {
                std::cout << "    " << std::setw(option_len_max + 10) << std::left << std::string("--") + opt.name;
            }
            std::cout << opt.description << std::endl;
        }
    }
}

void parsearg::print_version() {
    std::cout << get_program_name() << " version " << PRODUCT_VERSION << std::endl;
}

parsearg_error_t parsearg::parse_args(int argc, char* argv[]) {
    int arg_num = 0;
    std::string arg;
    std::string option_name;
    for (int i = 1; i < argc; i++) {
        arg = argv[i];
        if (arg[0] == '-') {  // if the argument is option
            if (arg[1] == '-') {
                option_name = arg.substr(2);
                auto option_record_p = find_by_name(option_list, option_name);
                if (option_record_p) {
                    if (option_record_p->has_argument) {
                        if (i + 1 < argc) {
                            parsed_args[option_name + "@opt"] = argv[i++ + 1];
                        } else {
                            // Error if there are no arguments after the option which requires an argument of its own
                            print_error(arg + " option requires an argument");
                            return PARSE_ERROR_LACK_OF_OPTION_ARGUMENTS;
                        }
                    } else {
                        parsed_args[option_name + "@opt"] = "";
                    }
                } else {
                    // Error if the option is not in the option_list
                    print_error("option " + arg + " is not a valid option.");
                    return PARSE_ERROR_INVALID_OPTIONS;
                }
            } else {
                for (int j = 1; j < arg.length(); j++) {
                    if (short_option.find(arg[j]) != short_option.end()) {
                        if (short_option[arg[j]].has_argument) {
                            if (j + 1 < arg.length()) {
                                parsed_args[short_option[arg[j]].name + "@opt"] = arg.substr(j + 1);
                            } else if (i + 1 < argc) {
                                parsed_args[short_option[arg[j]].name + "@opt"] = argv[i++ + 1];
                            } else {
                                // Error if there are no arguments after the option which requires an argument of its own
                                print_error(std::string("-") + arg[j] + " option requires an argument");
                                return PARSE_ERROR_LACK_OF_OPTION_ARGUMENTS;
                            }
                        } else {
                            parsed_args[short_option[arg[j]].name + "@opt"] = "";
                        }
                    } else {
                        // Error if the option is not in the option_list
                        print_error(std::string("option -") + arg[j] + " is not a valid option.");
                        return PARSE_ERROR_INVALID_OPTIONS;
                    }
                }
            }
        } else {
            parsed_args[argument_list[arg_num++].name] = arg;
        }
    }
    // Error if it lacks of required arguments
    if (arg_num < required_argument_num) {
        print_error(std::to_string(required_argument_num) + " arguments required");
        return PARSE_ERROR_LACK_OF_ESSENTIAL_ARGUMENTS;
    }
    argv_0 = argv[0];
    return PARSE_OK;
}

bool parsearg::contains_argument(std::string argument_name) {
    return parsed_args.find(argument_name) != parsed_args.end();
}

bool parsearg::contains_option(std::string option_name) {
    return parsed_args.find(option_name + "@opt") != parsed_args.end();
}

std::string parsearg::parsed_value(std::string name, bool is_option) {
    if (is_option) {
        return parsed_args.at(name + "@opt");
    } else {
        return parsed_args.at(name);
    }
}