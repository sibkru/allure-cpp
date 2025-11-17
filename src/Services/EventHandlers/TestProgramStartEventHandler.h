#pragma once

#include "ITestProgramStartEventHandler.h"

#include <memory>


namespace allure_cpp { namespace model {
	class TestProgram;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class TestProgramStartEventHandler : public ITestProgramStartEventHandler
	{
	public:
		TestProgramStartEventHandler(model::TestProgram&);
		virtual ~TestProgramStartEventHandler();

		void handleTestProgramStart() const;

	private:
		model::TestProgram& m_testProgram;
	};

}} // namespace allure_cpp::service
