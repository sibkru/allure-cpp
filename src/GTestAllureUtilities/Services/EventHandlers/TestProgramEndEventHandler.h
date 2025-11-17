#pragma once

#include "ITestProgramEndEventHandler.h"

#include <memory>


namespace allure_cpp { namespace model {
	class TestProgram;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class ITestProgramJSONBuilder;

	class TestProgramEndEventHandler : public ITestProgramEndEventHandler
	{
	public:
		TestProgramEndEventHandler(model::TestProgram&,
								   std::unique_ptr<ITestProgramJSONBuilder>);
		virtual ~TestProgramEndEventHandler() = default;

		void handleTestProgramEnd() const;

	private:
		model::TestProgram& m_testProgram;
		std::unique_ptr<ITestProgramJSONBuilder> m_testProgramJSONBuilderService;
	};

}} // namespace allure_cpp::service
