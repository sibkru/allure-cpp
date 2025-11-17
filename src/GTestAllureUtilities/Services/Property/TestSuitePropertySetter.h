#pragma once

#include "ITestSuitePropertySetter.h"

#include <memory>


namespace allure_cpp { namespace model {
	class TestProgram;
	class TestSuite;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class TestSuitePropertySetter : public ITestSuitePropertySetter
	{
	public:
		TestSuitePropertySetter(model::TestProgram&);
		virtual ~TestSuitePropertySetter();

		void setProperty(const std::string& name, const std::string& value) const override;

	private:
		model::TestSuite& getRunningTestSuite() const;

	private:
		model::TestProgram& m_testProgram;
	};

}} // namespace allure_cpp::service
