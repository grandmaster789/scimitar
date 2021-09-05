#include "../unittest.h"

#include <filesystem>
#include <cstdio>
#include <fstream>

namespace test {
	using scimitar::util::UniqueResource;

	TEST(util, unique_resource) {
		namespace fs = std::filesystem;

		auto test_filename = "test.xyz";
		auto data_buffer   = "abcdef";

		{
			auto fileCloser = [](FILE* handle) {
				fclose(handle);
			};

			UniqueResource<FILE*, void(*)(FILE*)>       ur_type1;
			UniqueResource<FILE*, decltype(fileCloser)> ur_type2;

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
			// MSVC will issue warnings about non-secure C functions - we suppress it here because the suggestion
			// to use the windows-specific fopen_s variant is a no-go
			// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4996?view=msvc-160

			#pragma warning(push)
			#pragma warning(disable: 4996) 
#endif
			auto ur_type3 = UniqueResource(
				fopen(test_filename, "w"),
				&fclose
			);

			fwrite(data_buffer, sizeof(char), sizeof(data_buffer), *ur_type3);

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
			#pragma warning(pop)
#endif
		}

		{
			ASSERT_TRUE(fs::exists(test_filename));
			ASSERT_EQ(fs::file_size(test_filename), sizeof(data_buffer));

			if (fs::exists(test_filename))
				fs::remove(test_filename);
		}
	}
}