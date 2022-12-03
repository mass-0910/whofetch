#include <iostream>
#include <string>
#include <vector>

#include "util.hpp"

int main(int argc, char *argv[]) {
    std::vector<std::string> optopt = {};
    int arg_num;
    auto parsed_args = parse_args(argc, argv, optopt, &arg_num);

    // Print usage if help option exists
    if ((contains_option(parsed_args, "h") || contains_option(parsed_args, "help")) && argc >= 1) {
        print_usage(argv[0]);
        return 0;
    }

    // Print version info if version option exists
    if (contains_option(parsed_args, "v") || contains_option(parsed_args, "version")) {
        print_version(argv[0]);
    }

    // Print user information if this program receive no argument
    if (argc == 1) {
    }
}
