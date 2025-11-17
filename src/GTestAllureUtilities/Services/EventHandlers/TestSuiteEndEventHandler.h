#pragma once

#include "ITestSuiteEndEventHandler.h"

#include <memory>


namespace allure_cpp { namespace model {
	class TestProgram;
	class TestSuite;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class ITimeService;
	class IUUIDGeneratorService;

	class TestSuiteEndEventHandler : public ITestSuiteEndEventHandler
	{
	public:
		TestSuiteEndEventHandler(model::TestProgram&,
								 std::unique_ptr<ITimeService>);
		virtual ~TestSuiteEndEventHandler() = default;

		void handleTestSuiteEnd(model::Status) const override;

	private:
		model::TestSuite& getRunningTestSuite() const;
		void addTMSLink(model::TestSuite&) const;

	private:
		model::TestProgram& m_testProgram;
		std::unique_ptr<ITimeService> m_timeService;
	};

}} // namespace allure_cpp::service
