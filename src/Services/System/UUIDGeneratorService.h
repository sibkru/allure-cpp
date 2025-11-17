#pragma once

#include "IUUIDGeneratorService.h"


namespace allure_cpp { namespace service {

	class UUIDGeneratorService : public IUUIDGeneratorService
	{
	public:
		UUIDGeneratorService();
		virtual ~UUIDGeneratorService() = default;

		std::string generateUUID() const;

	private:
		std::string generateHex(unsigned int length) const;
		unsigned char generateRandomChar() const;
	};

}} // namespace allure_cpp::service
