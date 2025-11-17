#pragma once

#include <string>


namespace allure_cpp { namespace service {

	class IUUIDGeneratorService
	{
	public:
		virtual ~IUUIDGeneratorService() = default;

		virtual std::string generateUUID() const = 0;
	};

}} // namespace allure_cpp::service
