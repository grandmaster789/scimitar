#include "../unittest.h"

namespace test {
	using namespace scimitar::util::codec;

	TEST(util, base64) {
		//ASSERT_EQ(Base64::encode("Hello World\n"), scimitar::util::ByteString("SGVsbG8gV29ybGQK"));

		std::string leviathan =
			"Man is distinguished, not only by his reason, but by this singular passion from other animals, "
			"which is a lust of the mind, that by a perseverance of delight in the continuedand indefatigable"
			"generation of knowledge, exceeds the short vehemence of any carnal pleasure.";

		std::string leviathan_base64 =
			"TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz"
			"IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
			"dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu"
			"dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo"
			"ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4 =";

		auto encoded = Base64::encode(scimitar::util::toByteString(leviathan));
		bool b = true;
	}
}