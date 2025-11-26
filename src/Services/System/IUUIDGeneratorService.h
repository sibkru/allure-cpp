#pragma once

#include <string>


namespace allure { namespace service {

	class IUUIDGeneratorService
	{
	public:
		virtual ~IUUIDGeneratorService() = default;

		virtual std::string generateUUID() const = 0;
	};

}} // namespace allure::service
