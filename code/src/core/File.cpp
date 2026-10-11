#include "core/File.h"

#include <fstream>
#include <iterator>

namespace voxel {

std::vector<unsigned char> ReadFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return {};
    }

    return {
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };
}

}
