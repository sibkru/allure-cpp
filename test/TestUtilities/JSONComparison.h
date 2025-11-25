#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace systelab { namespace json { namespace test_utility {

	// Simple JSON string comparison for tests
	// Note: This is a minimal implementation for allure-cpp tests
	inline bool compareJSONs(const std::string& expectedStr, const std::string& actualStr)
	{
		try
		{
			nlohmann::json expected = nlohmann::json::parse(expectedStr);
			nlohmann::json actual = nlohmann::json::parse(actualStr);

			// nlohmann::json has built-in comparison operators
			return expected == actual;
		}
		catch (const nlohmann::json::exception&)
		{
			return false;
		}
	}

}}} // namespace systelab::json::test_utility
