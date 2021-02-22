#include "../unittest.h"

namespace test {
	using scimitar::util::UniqueResource;

	TEST(util, unique_resource) {
		namespace fs = std::filesystem;

		const fs::path    test_file   = "123.xyz";
		const std::string test_buffer = "abacadabra";

		{
			UniqueResource ur(
				fopen(test_file.string().c_str(), "w"),
				[](FILE* x) { fclose(x); }
			);

			fwrite(test_buffer.data(), 1, test_buffer.size(), *ur);
			fwrite(test_buffer.data(), 1, test_buffer.size(), ur.get());
		}

		{
			ASSERT_TRUE(fs::exists(test_file));
			ASSERT_TRUE(fs::file_size(test_file) == 2 * test_buffer.size());
		}
	}
}