#pragma once

#include "log_category.h"
#include <iosfwd>
#include <sstream>
#include <string>
#include "../../util/threads.h"

namespace scimitar::core {
	class Logger;
}

namespace scimitar::core::logger {
	class LogSink;

	// string buffer, accumulates the full message and flushes it during destruction
	//
	class LogMessage {
	private:
		friend class Logger;
		friend class LogSink;

		LogMessage(
			Logger*            owner,
			eLogCategory       category,
			const std::string& source_file,
			unsigned int       source_line,
			util::ThreadID     thread_id,
			const std::string& thread_name
		);

		// no-copy, move-default (moving is questionable though)
		LogMessage             (const LogMessage&) = delete;
		LogMessage& operator = (const LogMessage&) = delete;
		LogMessage             (LogMessage&&)      = default;
		LogMessage& operator = (LogMessage&&)      = default;

	public:
		~LogMessage();

		// allow additional values to accumulate here
		template <typename T>
		LogMessage& operator << (const T& value);

		// allow iostream manipulator functions to work on this as well (such as std::endl and std::boolalpha)
		LogMessage& operator << (std::ostream& (*fn)(std::ostream&));

		struct MetaInfo {
			eLogCategory   m_Category;
			std::string    m_SourceFile;
			unsigned int   m_SourceLine;
			util::ThreadID m_ThreadID;
			std::string    m_ThreadName;
		};

	private:
		std::ostringstream m_Buffer;
		Logger*            m_Owner = nullptr;
		MetaInfo           m_MetaInfo;
	};
}

#include "log_message.inl"