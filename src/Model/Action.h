#pragma once

#include "Step.h"


namespace allure { namespace model {

	class Action : public Step
	{
	public:
		Action();
		Action(const Action&);
		virtual ~Action() = default;

		StepType getStepType() const override;
		Step* clone() const override;

	};

}} // namespace allure::model
