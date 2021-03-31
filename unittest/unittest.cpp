#include "unittest.h"

#include <compare>

namespace {
	std::filesystem::path g_ExePath;
	std::filesystem::path g_ExeFolder;
	std::filesystem::path g_ProjectRoot;
	std::filesystem::path g_AssetFolder;
}

std::filesystem::path getExePath() {
	return g_ExePath;
}

std::filesystem::path getExeFolder() {
	return g_ExeFolder;
}

std::filesystem::path getProjectRoot() {
	return g_ProjectRoot;
}

std::filesystem::path getAssetFolder() {
	return g_AssetFolder;
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);


	g_ExePath     = argv[0];
	g_ExeFolder   = g_ExePath.parent_path();
	g_ProjectRoot = g_ExeFolder.parent_path().parent_path();
	g_AssetFolder  = g_ProjectRoot / "assets";

	return RUN_ALL_TESTS();
}