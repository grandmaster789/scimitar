#pragma once

#include <gtest/gtest.h> 
// https://github.com/google/googletest/blob/master/googletest/docs/primer.md

#include <filesystem>

#include <scimitar.h>

std::filesystem::path getExePath();
std::filesystem::path getExeFolder();
std::filesystem::path getProjectRoot();
std::filesystem::path getAssetFolder();