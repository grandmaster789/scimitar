#include "log_message.h"
#include "../logger.h"
#include <ostream>

namespace scimitar::core::logger {
	LogMessage::LogMessage(
		Logger*            owner,
		eLogCategory       category,
		const std::string& source_file,
		unsigned int       source_line,
		util::ThreadID     thread_id,
		const std::string& thread_name
	):
		m_Owner(owner),
		m_MetaInfo{
			category,
			source_file,
			source_line,
			thread_id,
			thread_name
		}
	{
	}

	LogMessage::~LogMessage() {
		if (m_Owner)
			m_Owner->flush(this);
	}


	// allow iostream manipulator functions to work on this as well (such as std::endl and std::boolalpha)
	LogMessage& LogMessage::operator << (std::ostream& (*fn)(std::ostream&)) {
		(*fn)(m_Buffer);
		return *this;
	}
}