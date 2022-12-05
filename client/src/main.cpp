#include <iostream>
#include <string>

#include "util.hpp"
#include "parsearg.hpp"

int main(int argc, char *argv[]) {
    parsearg argument_parser;
    argument_parser.argument("name", "The name of the person whose profile you want to see", true);
    argument_parser.option("help", "Print this message", false, 'h');
    argument_parser.option("version", "Print version info", false);

    if (argument_parser.parse_args(argc, argv)) {
        return 1;
    }

    // Print usage if help option exists
    if (argument_parser.contains_option("help") && argc >= 1) {
        argument_parser.print_usage("[options] [name]");
        return 0;
    }

    // Print version info if version option exists
    if (argument_parser.contains_option("version")) {
        argument_parser.print_version();
    }

    // Print user information if this program receive no argument
    if (argc == 1) {
    }
}
