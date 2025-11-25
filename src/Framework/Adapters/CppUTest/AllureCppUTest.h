#pragma once

#include <memory>
#include <string>

namespace allure_cpp { namespace adapters { namespace cpputest {

class AllureCppUTest
{
public:
	explicit AllureCppUTest(const std::string& outputFolder = "allure-results");
	~AllureCppUTest();

	AllureCppUTest(const AllureCppUTest&) = delete;
	AllureCppUTest& operator=(const AllureCppUTest&) = delete;
	AllureCppUTest(AllureCppUTest&&) = delete;
	AllureCppUTest& operator=(AllureCppUTest&&) = delete;

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};

}}} // namespace allure_cpp::adapters::cpputest

namespace allure {
using AllureCppUTest = allure_cpp::adapters::cpputest::AllureCppUTest;
}
