#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "util.hpp"
#include "parsearg.hpp"

int main(int argc, char *argv[]) {
    parsearg::argument("name", "The name of the person whose profile you want to see", true);
    parsearg::option("help", "Print this message", false, 'h');
    parsearg::option("version", "Print version info", false);

    if (parsearg::parse_args(argc, argv)) {
        return 1;
    }

    // Print usage if help option exists
    if (parsearg::contains_option("help") && argc >= 1) {
        parsearg::print_usage("[options]");
        return 0;
    }

    // Print version info if version option exists
    if (parsearg::contains_option("version")) {
        parsearg::print_version();
    }

    // Print user information if this program receive no argument
    if (argc == 1) {
    }
}
