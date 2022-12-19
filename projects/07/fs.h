#pragma once

#include <filesystem>
#include <string>

void writeFile(const std::filesystem::path& path, const std::string& content);
