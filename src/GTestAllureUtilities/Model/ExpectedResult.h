#pragma once

#include "Step.h"


namespace allure_cpp { namespace model {

	class ExpectedResult : public Step
	{
	public:
		ExpectedResult();
		ExpectedResult(const ExpectedResult&);
		virtual ~ExpectedResult() = default;

		StepType getStepType() const override;
		Step* clone() const override;
	};

}} // namespace allure_cpp::model
