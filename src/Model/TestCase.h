#pragma once

#include "Stage.h"
#include "Status.h"
#include "Step.h"
#include "Parameter.h"
#include "Label.h"
#include "Link.h"
#include "Attachment.h"

#include <memory>
#include <string>
#include <vector>


namespace allure_cpp { namespace model {

	class TestCase
	{
	public:
		TestCase();
		TestCase(const TestCase&);
		virtual ~TestCase() = default;

		std::string getUUID() const;
		std::string getName() const;
		std::string getFullName() const;
		std::string getHistoryId() const;
		std::string getTestCaseId() const;
		std::string getDescription() const;
		std::string getDescriptionHtml() const;
		Status getStatus() const;
		Stage getStage() const;
		time_t getStart() const;
		time_t getStop() const;
		std::string getStatusMessage() const;
		std::string getStatusTrace() const;
		bool getStatusKnown() const;
		bool getStatusMuted() const;
		bool getStatusFlaky() const;

		void setUUID(const std::string&);
		void setName(const std::string&);
		void setFullName(const std::string&);
		void setHistoryId(const std::string&);
		void setTestCaseId(const std::string&);
		void setDescription(const std::string&);
		void setDescriptionHtml(const std::string&);
		void setStatus(Status);
		void setStage(Stage);
		void setStart(time_t);
		void setStop(time_t);
		void setStatusMessage(const std::string&);
		void setStatusTrace(const std::string&);
		void setStatusKnown(bool);
		void setStatusMuted(bool);
		void setStatusFlaky(bool);

		unsigned int getStepCount() const;
		const Step* getStep(unsigned int index) const;
		Step* getStep(unsigned int index);
		void addStep(std::unique_ptr<Step>);

	// Helper method to find the deepest running step (for nesting)
	Step* getRunningStep();
	const Step* getRunningStep() const;

		const std::vector<Parameter>& getParameters() const;
		void addParameter(const Parameter&);

		const std::vector<Label>& getLabels() const;
		void addLabel(const Label&);

		const std::vector<Link>& getLinks() const;
		void addLink(const Link&);

		const std::vector<Attachment>& getAttachments() const;
		void addAttachment(const Attachment&);

		// Clear methods to free memory after persisting to JSON
		void clearSteps();
		void clearParameters();
		void clearLabels();
		void clearLinks();
		void clearAttachments();

		virtual TestCase& operator= (const TestCase&);
		friend bool operator== (const TestCase& lhs, const TestCase& rhs);
		friend bool operator!= (const TestCase& lhs, const TestCase& rhs);

	private:
		std::string m_uuid;
		std::string m_name;
		std::string m_fullName;
		std::string m_historyId;
		std::string m_testCaseId;
		std::string m_description;
		std::string m_descriptionHtml;
		Status m_status;
		Stage m_stage;
		time_t m_start;
		time_t m_stop;
		std::string m_statusMessage;
		std::string m_statusTrace;
		bool m_statusKnown;
		bool m_statusMuted;
		bool m_statusFlaky;

		std::vector< std::unique_ptr<Step> > m_steps;
		std::vector<Parameter> m_parameters;
		std::vector<Label> m_labels;
		std::vector<Link> m_links;
		std::vector<Attachment> m_attachments;
	};

}} // namespace allure_cpp::model
