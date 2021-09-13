#pragma once

#include <iosfwd>
//#include <format>

namespace scimitar::core::logger {
	enum class eLogCategory {
		debug,
		info,
		err,
		warning,
		fatal
	};

	std::ostream& operator << (std::ostream& os, eLogCategory cat);
}

// user-defined types for std::format is... kind of complicated
// [NOTE] encountered a bug in the MSVC STL v14.29.30133; looks like it'll be fixed in Visual Studio 2022
//        but I'm not about to switch to a preview IDE just yet...
/*namespace std {
	template <>
	struct std::formatter<scimitar::core::logger::eLogCategory>:
		std::formatter<std::string>
	{
		auto format(
			const scimitar::core::logger::eLogCategory& cat, 
			format_context&                             ctx
		) {
			using scimitar::core::logger::eLogCategory;

			auto&& out = ctx.out();

			switch (cat) {
			default:                    return format_to(out, "[???] ");

			case eLogCategory::debug:   return format_to(out, "[dbg] "); 
			case eLogCategory::info:    return format_to(out, "      ");
			case eLogCategory::warning: return format_to(out, "*wrn* ");

			case eLogCategory::err:     return format_to(out, "< ERROR >     ");
			case eLogCategory::fatal:   return format_to(out, "<## FATAL ##> ");			
			}
		}
	};
}*/