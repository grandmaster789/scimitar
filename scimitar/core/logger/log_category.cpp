#include "log_category.h"
#include <ostream>

namespace scimitar::core::logger {
	std::ostream& operator << (std::ostream& os, eLogCategory cat) {

		switch (cat) {
		case eLogCategory::debug:   os << "[dbg] "; break;
		case eLogCategory::info:    os << "      "; break; // should be the most common, and hence the least annotated
		case eLogCategory::warning: os << "*wrn* "; break;

		case eLogCategory::err:     os << "< ERROR >     "; break;
		case eLogCategory::fatal:   os << "<## FATAL ##> "; break;

		default:
			os << "[???] ";
			break;
		}

		return os;
	}
}