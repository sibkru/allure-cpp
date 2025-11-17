#pragma once

#include "ITestCasePropertySetter.h"

#include <memory>


namespace allure_cpp { namespace model {
	class TestProgram;
	class TestSuite;
	class TestCase;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class TestCasePropertySetter : public ITestCasePropertySetter
	{
	public:
		TestCasePropertySetter(model::TestProgram&);
		virtual ~TestCasePropertySetter();

		void setProperty(const std::string& name, const std::string& value) const override;

	private:
		model::TestCase& getRunningTestCase() const;
		model::TestSuite& getRunningTestSuite() const;

	private:
		model::TestProgram& m_testProgram;
	};

}} // namespace allure_cpp::service
