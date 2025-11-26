#pragma once

#include <string>


namespace allure { namespace model {

	class Parameter
	{
	public:
		Parameter();
		Parameter(const Parameter&);
		virtual ~Parameter() = default;

		std::string getName() const;
		std::string getValue() const;
		bool getExcluded() const;
		std::string getMode() const;

		void setName(const std::string&);
		void setValue(const std::string&);
		void setExcluded(bool);
		void setMode(const std::string&);

		virtual Parameter& operator= (const Parameter&);
		friend bool operator== (const Parameter& lhs, const Parameter& rhs);
		friend bool operator!= (const Parameter& lhs, const Parameter& rhs);

	private:
		std::string m_name;
		std::string m_value;
		bool m_excluded;
		std::string m_mode;
	};

}} // namespace allure::model
