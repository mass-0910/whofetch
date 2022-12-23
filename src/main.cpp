#include <iostream>
#include <string>

#include "cmake_config.h"
#include "util.hpp"
#include "whofetch.hpp"
#include "parsearg.hpp"

int main(int argc, char *argv[]) {
    parsearg argument_parser;
    argument_parser.argument("name", "The name of the person whose profile you want to see", true);
    argument_parser.option("make", "Make whofetch profile", false);
    argument_parser.option("remake", "Remake whofetch profile", false);
    argument_parser.option("delete-my-profile", "delete my whofetch profile", false);
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
        std::cout << argument_parser.get_program_name() << " version " << PRODUCT_VERSION << std::endl;
        return 0;
    }

    whofetch whofetch;

    if (argument_parser.contains_option("make")) {
        whofetch.make_whofetch();
    } else if (argument_parser.contains_option("remake")) {
        whofetch.remove_my_whofetch();
        whofetch.make_whofetch();
    } else if (argument_parser.contains_option("delete-my-profile")) {
        whofetch.remove_my_whofetch();
    } else {  // Print user profile if this program receive no argument
        whofetch.show_my_whofetch();
    }
}
