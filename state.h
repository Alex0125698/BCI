#pragma once

#include "resources.h"
#include "variable.h"
#include "view.h"

namespace bci
{
	// TODO: support for resizeing m_vars
	class State : public QObject
	{
		Q_OBJECT

		// List all variables here - these represent physical quantities
		// TODO: move this outside class somehow
		enum class Vars
		{
			CH1,
			CH2,
			CH3,
			CH4,
			CH5,
			CH6,
			CH7,
			CH8,
			CH9,
			CH10,
			CH11,
			CH12,
			CH13,
			CH14,
			CH15,
			CH16,
			FREQ2,
			FREQ4,
			FREQ6,
			FREQ8,
			FREQ10,
			FREQ12,
			FREQ14,
			FREQ16,
			FREQ18,
			BATTERY_LEVEL,
			RUN_TIME,
			IO_FREQ,
			REFRESH_FREQ,
			END_OF_DATA
		};

	public:
		// the only instance of State ever to exist
		static State program;
		// use this to allocate the variable array *after* the main function has started
		static void init();

	public:
		// load vars into buffer
		void loadVars();
		// flush var buffer + update views
		void updateVars();
		// access fast var buffer
		double& operator[](size_t index)
		{
			return m_fast_var_buffer[index];
		}
		double& operator[](Vars index)
		{
			return (*this)[size_t(index)];
		}
		// store a non-atomic copy of the vars for fast arithematic
		std::vector<double> m_fast_var_buffer{ size_t(Vars::END_OF_DATA), 0.0 };

	public:
		// get a list of all variables with the given tags
		const std::vector<Variable*> searchVars(const std::vector<Tag> tags) const;
		// will generate a view for each variable passed in
		// However variabes with the same name + dimension are put into the same view
		const std::vector<View*> generateViews(const std::vector<Variable*> vars, View::Type viewType);
		//void save(std::string fileName);
		//void load(std::string fileName);

	signals:
		void sigVarUpdate();

	public slots:
		void slotViewReady();

	private:
		State() = default;
		~State() = default; // TODO: free stuff
		static State m_state;
		std::vector<Variable*> m_vars;
		// contained all generated views - variables may have multiple views
		// im using pointers here as they must not be invalidated with calls to emplace_back
		std::vector<View*> m_views;
		// we cannot copy the program
		State(const State&) = delete;
		State& operator=(const State&) = delete;
		Timer updateTimer;
	};

}