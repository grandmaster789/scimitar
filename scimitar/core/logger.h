#pragma once

#include <string>
#include <vector>
#include "logger/log_category.h"
#include "logger/log_sink.h"

namespace scimitar::core::logger {
	class LogMessage;
}

namespace scimitar::core {
	class Logger {
	public:
		using LogSink      = logger::LogSink;
		using LogMessage   = logger::LogMessage;
		using eLogCategory = logger::eLogCategory;

		explicit Logger() = default;
		Logger(const std::string& filename); // log both to a file and to std::cout

		// this class provides a singleton interface, but can also be used as a regular object
		static Logger& instance();

		void   add(LogSink sink);
		void   removeAll();
		size_t getNumSinks() const;

		void flush(LogMessage* message);

		// create an associated LogMessage with the appropriate message metadata
		LogMessage operator()(
			eLogCategory       category,
			const std::string& source_file,
			unsigned int       source_line
		);

	private:
		std::vector<LogSink> m_Sinks;
	};
}

// macros to make it as painless as possible to log something
#define gLogCategory(category) \
	(::scimitar::core::Logger::instance()(::scimitar::core::logger::eLogCategory::category, __FILE__, __LINE__))

#define gLog (gLogCategory(info))

#define gLogDebug   (gLogCategory(debug))
#define gLogInfo    (gLogCategory(info))
#define gLogError   (gLogCategory(err))
#define gLogWarning (gLogCategory(warning))
#define gLogFatal   (gLogCategory(fatal))
