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

			auto ur_type3 = UniqueResource(
				fopen(test_filename, "w"),
				&fclose
			);

			fwrite(data_buffer, sizeof(char), sizeof(data_buffer), *ur_type3);
		}

		{
			ASSERT_TRUE(fs::exists(test_filename));
			ASSERT_EQ(fs::file_size(test_filename), sizeof(data_buffer));

			if (fs::exists(test_filename))
				fs::remove(test_filename);
		}
	}
}