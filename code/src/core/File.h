#pragma once

#include <string>
#include <vector>

namespace voxel {

// Reads a whole file. The path is tried relative to the working directory first,
// then relative to the executable. Returns an empty vector if missing or empty.
std::vector<unsigned char> ReadFile(const std::string& path);

}
