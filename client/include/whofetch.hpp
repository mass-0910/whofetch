#pragma once

#include <string>

#include "resource.hpp"

enum whofetch_error_t {
    WHOFETCH_OK,
    WHOFETCH_NOT_FOUND,
    WHOFETCH_FAILURE_MAKE_PROFILE,
};

class whofetch {
public:
    whofetch::whofetch();

    /**
     * @brief Print to stdout my whofetch profile
     *
     * @return Error code
     */
    whofetch_error_t show_my_whofetch();

    /**
     * @brief Print to stdout some name profile
     *
     * @return Error code
     */
    whofetch_error_t show_whofetch(std::string name);

    /**
     * @brief Make profile
     *
     * @return Error code
     */
    whofetch_error_t make_whofetch();

    /**
     * @brief Remove my whofetch profile
     */
    whofetch_error_t remove_my_whofetch();

private:
    void most_distinctive_color(resource &icon, int &r, int &g, int &b);
    void print_icon_aa(resource &icon);
    void print_parameters(resource &parameters, int r, int g, int b, int &param_num);
    void make_combined_whofetch();
};