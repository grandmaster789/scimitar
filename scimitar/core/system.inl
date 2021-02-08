#pragma once

#include "system.h"

namespace scimitar::core {
	template <typename T>
	void System::register_setting(
		const std::string& json_key,
		T*                 member_variable
	) {
		JsonProperties entry;

		entry.m_VariableName = json_key;

		// NOTE this relies on the member variable being memory-stable!
		entry.m_GetFn = [json_key, member_variable] {
			nlohmann::json result;
			result[json_key] = *member_variable;
			return result;
		};

		entry.m_SetFn = [this, json_key, member_variable](const nlohmann::json& parsed_json) {
			auto it = parsed_json.find(json_key);

			if (it != parsed_json.end())
				*member_variable = *it;
			else
				std::cout << get_name() << ": setting " << json_key << " was not found\n";
		};

		m_Settings.push_back(std::move(entry));
	}
}