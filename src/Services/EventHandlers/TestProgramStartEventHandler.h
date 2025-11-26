#pragma once

#include "ITestProgramStartEventHandler.h"

#include <memory>


namespace allure { namespace model {
	class TestProgram;
}} // namespace allure::service

namespace allure { namespace service {

	class TestProgramStartEventHandler : public ITestProgramStartEventHandler
	{
	public:
		TestProgramStartEventHandler(model::TestProgram&);
		virtual ~TestProgramStartEventHandler();

		void handleTestProgramStart() const;

	private:
		model::TestProgram& m_testProgram;
	};

}} // namespace allure::service
