#pragma once

#include "Parameter.h"
#include "Attachment.h"
#include <string>
#include <vector>
#include <memory>


namespace allure_cpp { namespace model {

	enum class Stage;
	enum class Status;
	enum class StepType;

	class Step
	{
	public:
		Step();
		Step(const Step&);
		virtual ~Step() = default;

		virtual StepType getStepType() const = 0;
		virtual Step* clone() const = 0;

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

		unsigned int getStepCount() const;
		const Step* getStep(unsigned int index) const;
		Step* getStep(unsigned int index);
		void addStep(std::unique_ptr<Step>);

		const std::vector<Parameter>& getParameters() const;
		void addParameter(const Parameter&);

		const std::vector<Attachment>& getAttachments() const;
		void addAttachment(const Attachment&);

		// Helper method to find the deepest running step (for nesting)
		Step* getRunningStep();
		const Step* getRunningStep() const;

		virtual Step& operator= (const Step&);
		friend bool operator== (const Step& lhs, const Step& rhs);
		friend bool operator!= (const Step& lhs, const Step& rhs);

	private:
		std::string m_name;
		Status m_status;
		Stage m_stage;
		time_t m_start;
		time_t m_stop;
		std::vector< std::unique_ptr<Step> > m_steps;
		std::vector<Parameter> m_parameters;
		std::vector<Attachment> m_attachments;
	};

}} // namespace allure_cpp::model
