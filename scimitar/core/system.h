#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <functional>
#include <iostream>

#include "../dependencies.h"

namespace scimitar::core {
	class System {
	private:
		struct JsonProperties {
			std::string                                m_VariableName;
			std::function<nlohmann::json()>            m_GetFn;
			std::function<void(const nlohmann::json&)> m_SetFn;
		};

	public:
		friend class Engine;
		friend class Settings;

		using Dependencies = std::vector<std::string>; // based on System names (which must be unique)
		using Settings = std::vector<JsonProperties>;

		explicit System(const std::string& unique_system_name);
		virtual ~System() = default;

		// no-copy, move-only semantics
		System             (const System&) = delete;
		System& operator = (const System&) = delete;
		System             (System&&) noexcept = default;
		System& operator = (System&&) noexcept = default;

		virtual void start();
		virtual void update();
		virtual void stop();

		std::string_view getName() const;

		const Dependencies& getDependencies() const;
		const Settings&     getSettings()     const;

		friend std::ostream& operator << (std::ostream& os, const System& s);

	protected:
		void add_dependency(std::string_view system_name);

		template <typename T>
		void register_setting(
			std::string_view json_key, 
			T*               member_variable
		);

		class Engine* m_Engine = nullptr;

	private:
		std::string  m_Name;
		Dependencies m_Dependencies;
		Settings     m_Settings;
	};
}

#include "system.inl"