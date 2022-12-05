#include <string>
#include <vector>

enum resource_error_t {
    RESOURCE_OK = 0,
    RESOURCE_NOT_FOUND,
    RESOURCE_INVALID_PATH,
    RESOURCE_UNAVAILABLE_DATA_DIR,
};

class resource {
public:
    resource(std::string resource_name);
    ~resource();
    resource_error_t make_resource();
    resource_error_t make_resource(std::string &data);
    resource_error_t delete_resource();
    resource_error_t load_resource(std::string &file_path);

private:
    resource_error_t save_resource();
    resource_error_t get_resource_file_path(std::string &file_path);
    std::string resource_name;
    std::string file_fullpath;
    bool is_alive;
    std::vector<std::byte> resource_buffer;
};