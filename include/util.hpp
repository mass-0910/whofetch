#pragma once

#include <map>
#include <string>
#include <vector>

/**
 * @brief Print error message
 *
 * @param err_msg Error message
 */
void print_error(std::string err_msg);

/**
 * @brief Print usage message
 *
 * @param argv_0 argv[0]
 */
void print_usage(char *argv_0);

/**
 * @brief Print version message
 *
 * @param argv_0 argv[0]
 */
void print_version(char *argv_0);

/**
 * @brief Parse arguments to options
 *
 * @param argc The argc passed on main
 * @param argv The argv passed on main
 * @param option_arg The list of options which has itself argument
 * @return std::map<std::string, std::string> parsed arguments
 */
std::map<std::string, std::string> parse_args(int argc, char *argv[], std::vector<std::string> &option_arg, int *arg_num_buf = nullptr);

/**
 * @brief Return true if an option which specified exists in arguments
 *
 * @param parsed_args The arguments parsed to map object
 * @param option_name Specified option name
 * @retval true Specified option is in arguments
 * @retval false Specified option is not in arguments
 */
bool contains_option(std::map<std::string, std::string> &parsed_args, std::string option_name);