#pragma once

#include "Step.h"


namespace allure { namespace model {

	class ExpectedResult : public Step
	{
	public:
		ExpectedResult();
		ExpectedResult(const ExpectedResult&);
		virtual ~ExpectedResult() = default;

		StepType getStepType() const override;
		Step* clone() const override;
	};

}} // namespace allure::model
