#pragma once

#include "Stage.h"
#include "Status.h"
#include "Step.h"
#include "Parameter.h"
#include "Attachment.h"

#include <memory>
#include <string>
#include <vector>


namespace allure { namespace model {

	// FixtureStep represents a setup or teardown step in a test container
	// It has the same structure as a regular step
	class FixtureStep
	{
	public:
		FixtureStep();
		FixtureStep(const FixtureStep&);
		virtual ~FixtureStep() = default;

		std::string getName() const;
		Status getStatus() const;
		Stage getStage() const;
		time_t getStart() const;
		time_t getStop() const;

		void setName(const std::string&);
		void setStatus(Status);
		void setStage(Stage);
		void setStart(time_t);
		void setStop(time_t);

		const std::vector<Parameter>& getParameters() const;
		void addParameter(const Parameter&);

		const std::vector<Attachment>& getAttachments() const;
		void addAttachment(const Attachment&);

		unsigned int getStepCount() const;
		const FixtureStep* getStep(unsigned int index) const;
		FixtureStep* getStep(unsigned int index);
		void addStep(std::unique_ptr<FixtureStep>);

		virtual FixtureStep& operator= (const FixtureStep&);
		friend bool operator== (const FixtureStep& lhs, const FixtureStep& rhs);
		friend bool operator!= (const FixtureStep& lhs, const FixtureStep& rhs);

	private:
		std::string m_name;
		Status m_status;
		Stage m_stage;
		time_t m_start;
		time_t m_stop;
		std::vector<Parameter> m_parameters;
		std::vector<Attachment> m_attachments;
		std::vector< std::unique_ptr<FixtureStep> > m_steps;
	};

	// Container represents a test suite/fixture container in Allure 2 format
	class Container
	{
	public:
		Container();
		Container(const Container&);
		virtual ~Container() = default;

		std::string getUUID() const;
		std::string getName() const;
		time_t getStart() const;
		time_t getStop() const;

		void setUUID(const std::string&);
		void setName(const std::string&);
		void setStart(time_t);
		void setStop(time_t);

		const std::vector<std::string>& getChildren() const;
		void addChild(const std::string& testCaseUUID);

		const std::vector<FixtureStep>& getBefores() const;
		void addBefore(const FixtureStep&);

		const std::vector<FixtureStep>& getAfters() const;
		void addAfter(const FixtureStep&);

		virtual Container& operator= (const Container&);
		friend bool operator== (const Container& lhs, const Container& rhs);
		friend bool operator!= (const Container& lhs, const Container& rhs);

	private:
		std::string m_uuid;
		std::string m_name;
		time_t m_start;
		time_t m_stop;
		std::vector<std::string> m_children;  // UUIDs of test case result files
		std::vector<FixtureStep> m_befores;   // Setup steps
		std::vector<FixtureStep> m_afters;    // Teardown steps
	};

}} // namespace allure::model
