#pragma once

#include "JSONAdapterInterface/IJSONAdapter.h"
#include <rapidjson/document.h>
#include <string>

namespace systelab { namespace json { namespace test_utility {

	// Simple JSON string comparison for tests
	// Note: This is a minimal implementation for GTestAllureUtilities tests
	// For production use, consider Systelab's full JSONAdapterTestUtilities
	inline bool compareJSONs(const std::string& expectedStr, const std::string& actualStr, const IJSONAdapter&)
	{
		::rapidjson::Document expected;
		::rapidjson::Document actual;

		expected.Parse(expectedStr.c_str());
		actual.Parse(actualStr.c_str());

		if (expected.HasParseError() || actual.HasParseError())
		{
			return false;
		}

		// Simple comparison
		return expected == actual;
	}

}}} // namespace systelab::json::test_utility
