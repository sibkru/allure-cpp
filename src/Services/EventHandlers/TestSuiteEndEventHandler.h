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
	class IContainerJSONSerializer;
	class IFileService;

	class TestSuiteEndEventHandler : public ITestSuiteEndEventHandler
	{
	public:
		TestSuiteEndEventHandler(model::TestProgram&,
								 std::unique_ptr<ITimeService>,
								 std::unique_ptr<IContainerJSONSerializer>,
								 std::unique_ptr<IFileService>);
		virtual ~TestSuiteEndEventHandler() = default;

		void handleTestSuiteEnd(model::Status) const override;

	private:
		model::TestSuite& getRunningTestSuite() const;
		void addTMSLink(model::TestSuite&) const;
		void writeContainerJSON(const model::TestSuite& testSuite) const;

	private:
		model::TestProgram& m_testProgram;
		std::unique_ptr<ITimeService> m_timeService;
		std::unique_ptr<IContainerJSONSerializer> m_containerJSONSerializer;
		std::unique_ptr<IFileService> m_fileService;
	};

}} // namespace allure_cpp::service
