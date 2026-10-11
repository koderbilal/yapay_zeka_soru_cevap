#include "core/File.h"

#include <SDL.h>

#include <fstream>

namespace voxel {

namespace {

std::vector<unsigned char> ReadWhole(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        return {};
    }

    const std::streamsize size = file.tellg();
    if (size <= 0) {
        return {};
    }

    std::vector<unsigned char> bytes(static_cast<std::size_t>(size));
    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(bytes.data()), size)) {
        return {};
    }
    return bytes;
}

}

std::vector<unsigned char> ReadFile(const std::string& path) {
    std::vector<unsigned char> bytes = ReadWhole(path);
    if (!bytes.empty()) {
        return bytes;
    }

    char* basePath = SDL_GetBasePath();
    if (basePath != nullptr) {
        bytes = ReadWhole(std::string(basePath) + path);
        SDL_free(basePath);
    }
    return bytes;
}

}
