#pragma once

#include <string>

namespace allure_cpp { namespace model {
	class Container;
}} // namespace allure_cpp::model

namespace allure_cpp { namespace service {

	class IContainerJSONSerializer
	{
	public:
		virtual ~IContainerJSONSerializer() = default;

		virtual std::string serialize(const model::Container&) const = 0;
	};

}} // namespace allure_cpp::service
