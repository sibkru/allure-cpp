#include "TestCase.h"


namespace allure { namespace model {

	TestCase::TestCase()
		:m_uuid("")
		,m_name("")
		,m_fullName("")
		,m_historyId("")
		,m_testCaseId("")
		,m_description("")
		,m_descriptionHtml("")
		,m_status(Status::UNKNOWN)
		,m_stage(Stage::PENDING)
		,m_start(0)
		,m_stop(0)
		,m_statusMessage("")
		,m_statusTrace("")
		,m_statusKnown(false)
		,m_statusMuted(false)
		,m_statusFlaky(false)
		,m_steps()
		,m_parameters()
		,m_labels()
		,m_links()
		,m_attachments()
	{
	}

	TestCase::TestCase(const TestCase& other)
		:m_uuid(other.m_uuid)
		,m_name(other.m_name)
		,m_fullName(other.m_fullName)
		,m_historyId(other.m_historyId)
		,m_testCaseId(other.m_testCaseId)
		,m_description(other.m_description)
		,m_descriptionHtml(other.m_descriptionHtml)
		,m_status(other.m_status)
		,m_stage(other.m_stage)
		,m_start(other.m_start)
		,m_stop(other.m_stop)
		,m_statusMessage(other.m_statusMessage)
		,m_statusTrace(other.m_statusTrace)
		,m_statusKnown(other.m_statusKnown)
		,m_statusMuted(other.m_statusMuted)
		,m_statusFlaky(other.m_statusFlaky)
		,m_steps()
		,m_parameters(other.m_parameters)
		,m_labels(other.m_labels)
		,m_links(other.m_links)
		,m_attachments(other.m_attachments)
	{
		for (const auto& step : other.m_steps)
		{
			m_steps.push_back(std::unique_ptr<Step>(step->clone()));
		}
	}

	std::string TestCase::getUUID() const
	{
		return m_uuid;
	}

	std::string TestCase::getName() const
	{
		return m_name;
	}

	std::string TestCase::getFullName() const
	{
		return m_fullName;
	}

	std::string TestCase::getHistoryId() const
	{
		return m_historyId;
	}

	std::string TestCase::getTestCaseId() const
	{
		return m_testCaseId;
	}

	std::string TestCase::getDescription() const
	{
		return m_description;
	}

	std::string TestCase::getDescriptionHtml() const
	{
		return m_descriptionHtml;
	}

	Status TestCase::getStatus() const
	{
		return m_status;
	}

	Stage TestCase::getStage() const
	{
		return m_stage;
	}

	time_t TestCase::getStart() const
	{
		return m_start;
	}

	time_t TestCase::getStop() const
	{
		return m_stop;
	}

	void TestCase::setUUID(const std::string& uuid)
	{
		m_uuid = uuid;
	}

	void TestCase::setName(const std::string& name)
	{
		m_name = name;
	}

	void TestCase::setFullName(const std::string& fullName)
	{
		m_fullName = fullName;
	}

	void TestCase::setHistoryId(const std::string& historyId)
	{
		m_historyId = historyId;
	}

	void TestCase::setTestCaseId(const std::string& testCaseId)
	{
		m_testCaseId = testCaseId;
	}

	void TestCase::setDescription(const std::string& description)
	{
		m_description = description;
	}

	void TestCase::setDescriptionHtml(const std::string& descriptionHtml)
	{
		m_descriptionHtml = descriptionHtml;
	}

	void TestCase::setStatus(Status status)
	{
		m_status = status;
	}

	void TestCase::setStage(Stage stage)
	{
		m_stage = stage;
	}

	void TestCase::setStart(time_t start)
	{
		m_start = start;
	}

	void TestCase::setStop(time_t stop)
	{
		m_stop = stop;
	}

	unsigned int TestCase::getStepCount() const
	{
		return (unsigned int) m_steps.size();
	}

	const Step* TestCase::getStep(unsigned int index) const
	{
		return m_steps[index].get();
	}

	Step* TestCase::getStep(unsigned int index)
	{
		return m_steps[index].get();
	}

	void TestCase::addStep(std::unique_ptr<Step> step)
	{
		m_steps.push_back(std::move(step));
	}

	Step* TestCase::getRunningStep()
	{
		// Search through all top-level steps for a running step
		for (auto& step : m_steps)
		{
			Step* runningStep = step->getRunningStep();
			if (runningStep != nullptr)
			{
				return runningStep;
			}
		}
		return nullptr;
	}

	const Step* TestCase::getRunningStep() const
	{
		// Search through all top-level steps for a running step
		for (const auto& step : m_steps)
		{
			const Step* runningStep = step->getRunningStep();
			if (runningStep != nullptr)
			{
				return runningStep;
			}
		}
		return nullptr;
	}

	const std::vector<Parameter>& TestCase::getParameters() const
	{
		return m_parameters;
	}

	void TestCase::addParameter(const Parameter& parameter)
	{
		m_parameters.push_back(parameter);
	}

	const std::vector<Label>& TestCase::getLabels() const
	{
		return m_labels;
	}

	void TestCase::addLabel(const Label& label)
	{
		m_labels.push_back(label);
	}

	const std::vector<Link>& TestCase::getLinks() const
	{
		return m_links;
	}

	void TestCase::addLink(const Link& link)
	{
		m_links.push_back(link);
	}

	const std::vector<Attachment>& TestCase::getAttachments() const
	{
		return m_attachments;
	}

	void TestCase::addAttachment(const Attachment& attachment)
	{
		m_attachments.push_back(attachment);
	}

	void TestCase::clearSteps()
	{
		m_steps.clear();
	}

	void TestCase::clearParameters()
	{
		m_parameters.clear();
	}

	void TestCase::clearLabels()
	{
		m_labels.clear();
	}

	void TestCase::clearLinks()
	{
		m_links.clear();
	}

	void TestCase::clearAttachments()
	{
		m_attachments.clear();
	}

	TestCase& TestCase::operator= (const TestCase& other)
	{
		m_uuid = other.m_uuid;
		m_name = other.m_name;
		m_fullName = other.m_fullName;
		m_historyId = other.m_historyId;
		m_testCaseId = other.m_testCaseId;
		m_description = other.m_description;
		m_descriptionHtml = other.m_descriptionHtml;
		m_status = other.m_status;
		m_stage = other.m_stage;
		m_start = other.m_start;
		m_stop = other.m_stop;
		m_statusMessage = other.m_statusMessage;
		m_statusTrace = other.m_statusTrace;
		m_statusKnown = other.m_statusKnown;
		m_statusMuted = other.m_statusMuted;
		m_statusFlaky = other.m_statusFlaky;

		m_steps = std::vector< std::unique_ptr<Step> >();
		for (const auto& step : other.m_steps)
		{
			m_steps.push_back(std::unique_ptr<Step>(step->clone()));
		}

		m_parameters = other.m_parameters;
		m_labels = other.m_labels;
		m_links = other.m_links;
		m_attachments = other.m_attachments;

		return *this;
	}

	bool operator== (const TestCase& lhs, const TestCase& rhs)
	{
		if ((lhs.m_uuid != rhs.m_uuid) ||
			(lhs.m_name != rhs.m_name) ||
			(lhs.m_fullName != rhs.m_fullName) ||
			(lhs.m_historyId != rhs.m_historyId) ||
			(lhs.m_testCaseId != rhs.m_testCaseId) ||
			(lhs.m_description != rhs.m_description) ||
			(lhs.m_descriptionHtml != rhs.m_descriptionHtml) ||
			(lhs.m_status != rhs.m_status) ||
			(lhs.m_stage != rhs.m_stage) ||
			(lhs.m_start != rhs.m_start) ||
			(lhs.m_stop != rhs.m_stop) ||
			(lhs.m_statusMessage != rhs.m_statusMessage) ||
			(lhs.m_statusTrace != rhs.m_statusTrace) ||
			(lhs.m_statusKnown != rhs.m_statusKnown) ||
			(lhs.m_statusMuted != rhs.m_statusMuted) ||
			(lhs.m_statusFlaky != rhs.m_statusFlaky) ||
			(lhs.m_steps.size() != rhs.m_steps.size()) ||
			(lhs.m_parameters != rhs.m_parameters) ||
			(lhs.m_labels != rhs.m_labels) ||
			(lhs.m_links != rhs.m_links) ||
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

	bool operator!= (const TestCase& lhs, const TestCase& rhs)
	{
		return !(lhs == rhs);
	}

	std::string TestCase::getStatusMessage() const
	{
		return m_statusMessage;
	}

	std::string TestCase::getStatusTrace() const
	{
		return m_statusTrace;
	}

	void TestCase::setStatusMessage(const std::string& message)
	{
		m_statusMessage = message;
	}

	void TestCase::setStatusTrace(const std::string& trace)
	{
		m_statusTrace = trace;
	}

	bool TestCase::getStatusKnown() const
	{
		return m_statusKnown;
	}

	bool TestCase::getStatusMuted() const
	{
		return m_statusMuted;
	}

	bool TestCase::getStatusFlaky() const
	{
		return m_statusFlaky;
	}

	void TestCase::setStatusKnown(bool known)
	{
		m_statusKnown = known;
	}

	void TestCase::setStatusMuted(bool muted)
	{
		m_statusMuted = muted;
	}

	void TestCase::setStatusFlaky(bool flaky)
	{
		m_statusFlaky = flaky;
	}

}} // namespace allure::model
