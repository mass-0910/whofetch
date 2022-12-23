#pragma once

#include <string>
#include <vector>

/// @brief Error code of functions in resource class
enum resource_error_t {
    RESOURCE_OK = 0,
    RESOURCE_NOT_FOUND,
    RESOURCE_INVALID_PATH,
    RESOURCE_UNAVAILABLE_DATA_DIR,
};

class resource {
public:
    /**
     * @brief Construct a new resource object
     *
     * @param resource_name resource identifier
     */
    resource(std::string resource_name);

    /**
     * @brief Construct a new resource object
     *
     * @param resource_name resource identifier
     * @param group_name resource group identifier
     */
    resource(std::string resource_name, std::string group_name);

    /**
     * @brief Destroy the resource object
     */
    ~resource();

    /**
     * @brief Create text resource data from string
     *
     * @param data The string stored to resource buffer
     * @return Error code
     */
    resource_error_t make_resource(std::string data);

    /**
     * @brief Create binary resource data from byte array
     *
     * @param buffer The byte array buffer stored to resource buffer
     * @param buf_size The length of buffer
     * @return Error code
     */
    resource_error_t make_resource(std::byte *buffer, size_t buf_size);

    /**
     * @brief Destroy resource from system
     *
     * @return Error code
     */
    resource_error_t delete_resource();

    /**
     * @brief Copy resource data to specified buffer
     *
     * @param buffer The buffer stored from resource buffer
     * @return Error code
     */
    resource_error_t copy_resource_to_buffer(std::vector<std::byte> &buffer);

    /**
     * @brief Copy resource data to specified buffer
     *
     * @param buffer The buffer stored from resource buffer
     * @return Error code
     */
    resource_error_t copy_resource_to_buffer(std::string &buffer);

    /**
     * @brief Get resource value as a string
     *
     * @return The value of resource
     */
    std::string string();

    /**
     * @brief Return true if resource is available
     *
     * @retval true available
     * @retval false unavaliable
     */
    explicit operator bool() const;

private:
    resource_error_t load_resource();
    resource_error_t save_resource();
    resource_error_t get_resource_file_path(std::string &file_path);
    std::string resource_name;
    std::string group_name;
    std::string file_fullpath;
    bool is_alive;
    std::vector<std::byte> resource_buffer;
};