#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "resource.hpp"

const std::string appname = "whofetch";

resource::resource(std::string resource_name) : resource_name(resource_name), group_name(""), is_alive(false) {
    if (load_resource() == RESOURCE_OK) {
        is_alive = true;
    }
}

resource::resource(std::string resource_name, std::string group_name) : resource_name(resource_name), group_name(group_name), is_alive(false) {
    if (load_resource() == RESOURCE_OK) {
        is_alive = true;
    }
}

resource::~resource() {
    if (is_alive) {
        save_resource();
    }
}

resource_error_t resource::get_resource_file_path(std::string &file_path) {
#ifdef _WIN32
    char *value_p;
    size_t size;
    errno_t err = _dupenv_s(&value_p, &size, "AppData");
    if (err) {
        return RESOURCE_UNAVAILABLE_DATA_DIR;
    }
    std::filesystem::path resource_dir(value_p);
    resource_dir.append(appname);
#else
    std::filesystem::path resource_dir(std::getenv("USER"));
    resource_dir.append(std::string(".") + appname);
#endif
    if (!group_name.empty()) {
        resource_dir.append(group_name);
    }
    std::filesystem::path _file_path(resource_dir.append(resource_name));
    file_path = _file_path.string();
    return RESOURCE_OK;
}

resource_error_t resource::make_resource(std::string data) {
    resource_buffer.resize(data.length() + 1);
    std::memcpy(resource_buffer.data(), (void *)data.c_str(), data.length() + 1);
    is_alive = true;
    return RESOURCE_OK;
}

resource_error_t resource::make_resource(std::byte *buffer, size_t buf_size) {
    resource_buffer.clear();
    resource_buffer.resize(buf_size);
    std::memcpy(resource_buffer.data(), (void *)buffer, buf_size);
    is_alive = true;
    return RESOURCE_OK;
}

resource_error_t resource::delete_resource() {
    is_alive = false;
    std::string file_path;
    if (get_resource_file_path(file_path) != RESOURCE_OK) {
        return RESOURCE_UNAVAILABLE_DATA_DIR;
    }
    std::error_code err;
    std::filesystem::remove(file_path, err);
    if (err) {
        return RESOURCE_INVALID_PATH;
    }
    // Delete group directory if it does not contain any file
    if (!group_name.empty()) {
        auto group_dir_path(std::filesystem::path(file_path).parent_path());
        if (!std::filesystem::is_directory(group_dir_path)) {
            return RESOURCE_INVALID_PATH;
        }
        if (std::filesystem::is_empty(group_dir_path)) {
            std::filesystem::remove(group_dir_path, err);
            if (err) {
                return RESOURCE_INVALID_PATH;
            }
        }
    }
    return RESOURCE_OK;
}

resource_error_t resource::copy_resource_to_buffer(std::vector<std::byte> &buffer) {
    buffer = resource_buffer;
    return RESOURCE_OK;
}

resource_error_t resource::copy_resource_to_buffer(std::string &buffer) {
    std::string converted_str(reinterpret_cast<const char *>(resource_buffer.data()), resource_buffer.size());
    buffer = converted_str;
    return RESOURCE_OK;
}

resource_error_t resource::load_resource() {
    std::error_code err;
    std::string file_path;
    if (get_resource_file_path(file_path) != RESOURCE_OK) {
        return RESOURCE_UNAVAILABLE_DATA_DIR;
    }
    size_t file_size = std::filesystem::file_size(file_path, err);
    if (err) {
        return RESOURCE_INVALID_PATH;
    }
    resource_buffer.resize(file_size);
    std::ifstream ifs(file_path, std::ios::binary);
    if (!ifs) {
        return RESOURCE_INVALID_PATH;
    }
    ifs.read(reinterpret_cast<char *>(resource_buffer.data()), file_size);
    is_alive = true;
    return RESOURCE_OK;
}

resource_error_t resource::save_resource() {
    std::string file_path;
    if (get_resource_file_path(file_path) != RESOURCE_OK) {
        return RESOURCE_UNAVAILABLE_DATA_DIR;
    }

    // Create direcotry if resource direcotry is not found
    std::filesystem::path _file_path(file_path);
    if (!std::filesystem::is_directory(_file_path.parent_path())) {
        std::filesystem::create_directories(_file_path.parent_path());
    }

    std::ofstream ofs(file_path, std::ios::binary);
    if (!ofs) {
        return RESOURCE_INVALID_PATH;
    }
    ofs.write(reinterpret_cast<const char *>(resource_buffer.data()), resource_buffer.size());
    return RESOURCE_OK;
}

std::string resource::string() {
    size_t size = resource_buffer.size();
    if (*(resource_buffer.end() - 1) == (std::byte)'\0') {
        size--;
    }
    std::string as_string(reinterpret_cast<const char *>(resource_buffer.data()), size);
    return as_string;
}

resource::operator bool() const {
    return is_alive;
}