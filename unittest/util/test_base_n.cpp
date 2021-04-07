#include "../unittest.h"

namespace test {
	using namespace scimitar::util::codec;
	using scimitar::util::ByteString;
	using scimitar::util::toByteString;

	TEST(util, base64) {
		std::string hello = Base64::encode(toByteString("Hello World\n"));

		ASSERT_EQ(hello, std::string("SGVsbG8gV29ybGQK"));

		std::string leviathan =
			"Man is distinguished, not only by his reason, but by this singular passion from other animals, "
			"which is a lust of the mind, that by a perseverance of delight in the continuedand indefatigable "
			"generation of knowledge, exceeds the short vehemence of any carnal pleasure.";

		std::string leviathan_base64 =
			"TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc"
			"3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZX"
			"ZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkYW5kIGluZGVmYXRpZ2FibGUgZ2VuZXJhdGlvbiBvZiBrbm9"
			"3bGVkZ2UsIGV4Y2VlZHMgdGhlIHNob3J0IHZlaGVtZW5jZSBvZiBhbnkgY2FybmFsIHBsZWFzdXJlLg==";

		auto encoded = Base64::encode(toByteString(leviathan));

		ASSERT_EQ(encoded, leviathan_base64);

		ByteString decoded;
		Base64::decode(
			std::begin(leviathan_base64), 
			std::end(leviathan_base64), 
			std::back_inserter(decoded)
		);

	}
}