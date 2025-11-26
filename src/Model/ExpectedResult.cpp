#include "ExpectedResult.h"

#include "StepType.h"


namespace allure { namespace model {

	ExpectedResult::ExpectedResult()
		:Step()
	{
	}

	ExpectedResult::ExpectedResult(const ExpectedResult& other)
		:Step(other)
	{
	}

	StepType ExpectedResult::getStepType() const
	{
		return StepType::EXPECTED_RESULT_STEP;
	}

	Step* ExpectedResult::clone() const
	{
		return new ExpectedResult(*this);
	}

}} // namespace allure::model
