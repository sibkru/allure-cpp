#include "Step.h"

#include "Stage.h"
#include "Status.h"


namespace allure_cpp { namespace model {

	Step::Step()
		:m_name("")
		,m_status(Status::UNKNOWN)
		,m_stage(Stage::PENDING)
		,m_start(0)
		,m_stop(0)
		,m_steps()
		,m_parameters()
		,m_attachments()
	{
	}

	Step::Step(const Step& other)
		:m_name(other.m_name)
		,m_status(other.m_status)
		,m_stage(other.m_stage)
		,m_start(other.m_start)
		,m_stop(other.m_stop)
		,m_steps()
		,m_parameters(other.m_parameters)
		,m_attachments(other.m_attachments)
	{
		for (const auto& step : other.m_steps)
		{
			m_steps.push_back(std::unique_ptr<Step>(step->clone()));
		}
	}

	std::string Step::getName() const
	{
		return m_name;
	}

	model::Status Step::getStatus() const
	{
		return m_status;
	}

	model::Stage Step::getStage() const
	{
		return m_stage;
	}

	time_t Step::getStart() const
	{
		return m_start;
	}

	time_t Step::getStop() const
	{
		return m_stop;
	}

	void Step::setName(const std::string& name)
	{
		m_name = name;
	}

	void Step::setStatus(Status status)
	{
		m_status = status;
	}

	void Step::setStage(Stage stage)
	{
		m_stage = stage;
	}

	void Step::setStart(time_t start)
	{
		m_start = start;
	}

	void Step::setStop(time_t stop)
	{
		m_stop = stop;
	}

	unsigned int Step::getStepCount() const
	{
		return (unsigned int) m_steps.size();
	}

	const Step* Step::getStep(unsigned int index) const
	{
		return m_steps[index].get();
	}

	Step* Step::getStep(unsigned int index)
	{
		return m_steps[index].get();
	}

	void Step::addStep(std::unique_ptr<Step> step)
	{
		m_steps.push_back(std::move(step));
	}

	const std::vector<Parameter>& Step::getParameters() const
	{
		return m_parameters;
	}

	void Step::addParameter(const Parameter& parameter)
	{
		m_parameters.push_back(parameter);
	}

	const std::vector<Attachment>& Step::getAttachments() const
	{
		return m_attachments;
	}

	void Step::addAttachment(const Attachment& attachment)
	{
		m_attachments.push_back(attachment);
	}

	Step* Step::getRunningStep()
	{
		// If this step is running, check if any of its nested steps are also running
		if (getStage() == Stage::RUNNING)
		{
			// Check nested steps first (depth-first search for the deepest running step)
			for (auto& step : m_steps)
			{
				Step* runningNestedStep = step->getRunningStep();
				if (runningNestedStep != nullptr)
				{
					return runningNestedStep;
				}
			}
			// No nested running step found, this is the deepest running step
			return this;
		}
		return nullptr;
	}

	const Step* Step::getRunningStep() const
	{
		// If this step is running, check if any of its nested steps are also running
		if (getStage() == Stage::RUNNING)
		{
			// Check nested steps first (depth-first search for the deepest running step)
			for (const auto& step : m_steps)
			{
				const Step* runningNestedStep = step->getRunningStep();
				if (runningNestedStep != nullptr)
				{
					return runningNestedStep;
				}
			}
			// No nested running step found, this is the deepest running step
			return this;
		}
		return nullptr;
	}

	Step& Step::operator= (const Step& other)
	{
		m_name = other.m_name;
		m_status = other.m_status;
		m_stage = other.m_stage;
		m_start = other.m_start;
		m_stop = other.m_stop;

		m_steps = std::vector< std::unique_ptr<Step> >();
		for (const auto& step : other.m_steps)
		{
			m_steps.push_back(std::unique_ptr<Step>(step->clone()));
		}

		m_parameters = other.m_parameters;
		m_attachments = other.m_attachments;

		return *this;
	}

	bool operator== (const Step& lhs, const Step& rhs)
	{
		if ((lhs.m_name != rhs.m_name) ||
			(lhs.m_status != rhs.m_status) ||
			(lhs.m_stage != rhs.m_stage) ||
			(lhs.m_start != rhs.m_start) ||
			(lhs.m_stop != rhs.m_stop) ||
			(lhs.m_steps.size() != rhs.m_steps.size()) ||
			(lhs.m_parameters != rhs.m_parameters) ||
			(lhs.m_attachments != rhs.m_attachments))
		{
			return false;
		}

		unsigned int nSteps = (unsigned int) lhs.m_steps.size();
		for (unsigned int i = 0; i < nSteps; i++)
		{
			if ((*lhs.m_steps[i]) != (*rhs.m_steps[i]))
			{
				return false;
			}
		}

		return true;
	}

	bool operator!= (const Step& lhs, const Step& rhs)
	{
		return !(lhs == rhs);
	}

}} // namespace allure_cpp::model
