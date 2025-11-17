#include "Parameter.h"


namespace allure_cpp { namespace model {

	Parameter::Parameter()
		:m_name("")
		,m_value("")
		,m_excluded(false)
		,m_mode("default")
	{
	}

	Parameter::Parameter(const Parameter& other)
		:m_name(other.m_name)
		,m_value(other.m_value)
		,m_excluded(other.m_excluded)
		,m_mode(other.m_mode)
	{
	}

	std::string Parameter::getName() const
	{
		return m_name;
	}

	std::string Parameter::getValue() const
	{
		return m_value;
	}

	bool Parameter::getExcluded() const
	{
		return m_excluded;
	}

	std::string Parameter::getMode() const
	{
		return m_mode;
	}

	void Parameter::setName(const std::string& name)
	{
		m_name = name;
	}

	void Parameter::setValue(const std::string& value)
	{
		m_value = value;
	}

	void Parameter::setExcluded(bool excluded)
	{
		m_excluded = excluded;
	}

	void Parameter::setMode(const std::string& mode)
	{
		m_mode = mode;
	}

	Parameter& Parameter::operator= (const Parameter& other)
	{
		m_name = other.m_name;
		m_value = other.m_value;
		m_excluded = other.m_excluded;
		m_mode = other.m_mode;
		return *this;
	}

	bool operator== (const Parameter& lhs, const Parameter& rhs)
	{
		return (lhs.m_name == rhs.m_name) &&
			   (lhs.m_value == rhs.m_value) &&
			   (lhs.m_excluded == rhs.m_excluded) &&
			   (lhs.m_mode == rhs.m_mode);
	}

	bool operator!= (const Parameter& lhs, const Parameter& rhs)
	{
		return !(lhs == rhs);
	}

}} // namespace allure_cpp::model