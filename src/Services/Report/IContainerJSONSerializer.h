#pragma once

#include <string>

namespace allure { namespace model {
	class Container;
}} // namespace allure::model

namespace allure { namespace service {

	class IContainerJSONSerializer
	{
	public:
		virtual ~IContainerJSONSerializer() = default;

		virtual std::string serialize(const model::Container&) const = 0;
	};

}} // namespace allure::service
