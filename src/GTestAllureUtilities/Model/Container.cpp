#include "Container.h"


namespace allure_cpp { namespace model {

	// FixtureStep implementation
	FixtureStep::FixtureStep()
		:m_name("")
		,m_status(Status::UNKNOWN)
		,m_stage(Stage::PENDING)
		,m_start(0)
		,m_stop(0)
		,m_parameters()
		,m_attachments()
		,m_steps()
	{
	}

	FixtureStep::FixtureStep(const FixtureStep& other)
		:m_name(other.m_name)
		,m_status(other.m_status)
		,m_stage(other.m_stage)
		,m_start(other.m_start)
		,m_stop(other.m_stop)
		,m_parameters(other.m_parameters)
		,m_attachments(other.m_attachments)
		,m_steps()
	{
		for (const auto& step : other.m_steps)
		{
			m_steps.push_back(std::unique_ptr<FixtureStep>(new FixtureStep(*step)));
		}
	}

	std::string FixtureStep::getName() const
	{
		return m_name;
	}

	Status FixtureStep::getStatus() const
	{
		return m_status;
	}

	Stage FixtureStep::getStage() const
	{
		return m_stage;
	}

	time_t FixtureStep::getStart() const
	{
		return m_start;
	}

	time_t FixtureStep::getStop() const
	{
		return m_stop;
	}

	void FixtureStep::setName(const std::string& name)
	{
		m_name = name;
	}

	void FixtureStep::setStatus(Status status)
	{
		m_status = status;
	}

	void FixtureStep::setStage(Stage stage)
	{
		m_stage = stage;
	}

	void FixtureStep::setStart(time_t start)
	{
		m_start = start;
	}

	void FixtureStep::setStop(time_t stop)
	{
		m_stop = stop;
	}

	const std::vector<Parameter>& FixtureStep::getParameters() const
	{
		return m_parameters;
	}

	void FixtureStep::addParameter(const Parameter& parameter)
	{
		m_parameters.push_back(parameter);
	}

	const std::vector<Attachment>& FixtureStep::getAttachments() const
	{
		return m_attachments;
	}

	void FixtureStep::addAttachment(const Attachment& attachment)
	{
		m_attachments.push_back(attachment);
	}

	unsigned int FixtureStep::getStepCount() const
	{
		return (unsigned int) m_steps.size();
	}

	const FixtureStep* FixtureStep::getStep(unsigned int index) const
	{
		return m_steps[index].get();
	}

	FixtureStep* FixtureStep::getStep(unsigned int index)
	{
		return m_steps[index].get();
	}

	void FixtureStep::addStep(std::unique_ptr<FixtureStep> step)
	{
		m_steps.push_back(std::move(step));
	}

	FixtureStep& FixtureStep::operator= (const FixtureStep& other)
	{
		m_name = other.m_name;
		m_status = other.m_status;
		m_stage = other.m_stage;
		m_start = other.m_start;
		m_stop = other.m_stop;
		m_parameters = other.m_parameters;
		m_attachments = other.m_attachments;

		m_steps = std::vector< std::unique_ptr<FixtureStep> >();
		for (const auto& step : other.m_steps)
		{
			m_steps.push_back(std::unique_ptr<FixtureStep>(new FixtureStep(*step)));
		}

		return *this;
	}

	bool operator== (const FixtureStep& lhs, const FixtureStep& rhs)
	{
		if ((lhs.m_name != rhs.m_name) ||
			(lhs.m_status != rhs.m_status) ||
			(lhs.m_stage != rhs.m_stage) ||
			(lhs.m_start != rhs.m_start) ||
			(lhs.m_stop != rhs.m_stop) ||
			(lhs.m_parameters != rhs.m_parameters) ||
			(lhs.m_attachments != rhs.m_attachments) ||
			(lhs.m_steps.size() != rhs.m_steps.size()))
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

	bool operator!= (const FixtureStep& lhs, const FixtureStep& rhs)
	{
		return !(lhs == rhs);
	}

	// Container implementation
	Container::Container()
		:m_uuid("")
		,m_name("")
		,m_start(0)
		,m_stop(0)
		,m_children()
		,m_befores()
		,m_afters()
	{
	}

	Container::Container(const Container& other)
		:m_uuid(other.m_uuid)
		,m_name(other.m_name)
		,m_start(other.m_start)
		,m_stop(other.m_stop)
		,m_children(other.m_children)
		,m_befores(other.m_befores)
		,m_afters(other.m_afters)
	{
	}

	std::string Container::getUUID() const
	{
		return m_uuid;
	}

	std::string Container::getName() const
	{
		return m_name;
	}

	time_t Container::getStart() const
	{
		return m_start;
	}

	time_t Container::getStop() const
	{
		return m_stop;
	}

	void Container::setUUID(const std::string& uuid)
	{
		m_uuid = uuid;
	}

	void Container::setName(const std::string& name)
	{
		m_name = name;
	}

	void Container::setStart(time_t start)
	{
		m_start = start;
	}

	void Container::setStop(time_t stop)
	{
		m_stop = stop;
	}

	const std::vector<std::string>& Container::getChildren() const
	{
		return m_children;
	}

	void Container::addChild(const std::string& testCaseUUID)
	{
		m_children.push_back(testCaseUUID);
	}

	const std::vector<FixtureStep>& Container::getBefores() const
	{
		return m_befores;
	}

	void Container::addBefore(const FixtureStep& before)
	{
		m_befores.push_back(before);
	}

	const std::vector<FixtureStep>& Container::getAfters() const
	{
		return m_afters;
	}

	void Container::addAfter(const FixtureStep& after)
	{
		m_afters.push_back(after);
	}

	Container& Container::operator= (const Container& other)
	{
		m_uuid = other.m_uuid;
		m_name = other.m_name;
		m_start = other.m_start;
		m_stop = other.m_stop;
		m_children = other.m_children;
		m_befores = other.m_befores;
		m_afters = other.m_afters;

		return *this;
	}

	bool operator== (const Container& lhs, const Container& rhs)
	{
		return (lhs.m_uuid == rhs.m_uuid) &&
			   (lhs.m_name == rhs.m_name) &&
			   (lhs.m_start == rhs.m_start) &&
			   (lhs.m_stop == rhs.m_stop) &&
			   (lhs.m_children == rhs.m_children) &&
			   (lhs.m_befores == rhs.m_befores) &&
			   (lhs.m_afters == rhs.m_afters);
	}

	bool operator!= (const Container& lhs, const Container& rhs)
	{
		return !(lhs == rhs);
	}

}} // namespace allure_cpp::model
