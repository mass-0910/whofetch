#include <string>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "resource.hpp"

const std::string appname = "whofetch";

resource::resource(std::string resource_name) : resource_name(resource_name) {
    is_alive = false;
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
    std::filesystem::path _file_path(resource_dir.append(resource_name));
    file_path = _file_path.string();
    return RESOURCE_OK;
}

resource_error_t resource::make_resource(std::string &data) {
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

resource::operator bool() const {
    return is_alive;
}