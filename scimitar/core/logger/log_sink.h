#pragma once

#include "log_message.h"
#include "../../preprocessor.h"
#include <memory>
#include <filesystem>

namespace scimitar::core::logger {
	/*
	* Type-erased interface; this allows storage in a vector
	*/

	template <typename T>
	concept c_LogSink = requires(
		T                           sink,
		const LogMessage::MetaInfo& info,
		const std::string&          message
	) {
		{ sink(info, message) } -> std::convertible_to<void>;
	};

	class LogSink {
	public:
		template <c_LogSink T>
		LogSink(T&& impl); // maybe this should be explicit?

		// no-copy, move-default
		LogSink            (const LogSink&)     = delete;
		LogSink& operator= (const LogSink&)     = delete;
		LogSink            (LogSink&&) noexcept = default;
		LogSink& operator= (LogSink&&) noexcept = default;

		void write(
			const LogMessage::MetaInfo& info,
			const std::string&          message
		);

	private:
		struct Prototype {
			virtual ~Prototype() = default;

			virtual void operator()(
				const LogMessage::MetaInfo& meta,
				const std::string&          message
			) = 0;
		};

		template <c_LogSink T>
		struct Wrapper: 
			Prototype 
		{
			Wrapper(T&& impl);

			virtual void operator()(
				const LogMessage::MetaInfo& meta,
				const std::string&          message
			) override; 

			T m_Impl;
		};

		std::unique_ptr<Prototype> m_Wrapper;
	};

	LogSink makeStdOutSink();   // [logLevel] [message] [\n] ~> std::cout
	LogSink makeStdErrSink();   // [logLevel] [message] [\n] ~> std::cerr
	
	LogSink makeFileSink(const std::filesystem::path& p); // [timestamp] [loglevel] [message] ([sourcefile]:[linenumber]) [\n] ~> p
	
#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
	LogSink makeDebuggerSink(); // [loglevel] [message] [\n] ~> OutputDebugStringA
#endif
}

#include "log_sink.inl"