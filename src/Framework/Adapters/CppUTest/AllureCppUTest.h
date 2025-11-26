#pragma once

#include <memory>
#include <string>

namespace allure { namespace adapters { namespace cpputest {

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

}}} // namespace allure::adapters::cpputest

namespace allure {
using AllureCppUTest = allure::adapters::cpputest::AllureCppUTest;
}
