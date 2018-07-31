#include "state.h"

bci::State bci::State::program;

void bci::State::init()
{
	assert(bci::State::program.m_vars.size() == 0);
	bci::State::program.m_vars =
	{
		new Variable("CH1", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH2", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH3", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH4", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH5", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH6", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH7", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH8", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH9", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH10", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH11", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH12", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH13", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH14", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH15", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("CH16", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::DATA_CHANNEL }),
		new Variable("FREQ2", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ4", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ6", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ8", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ10", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ12", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ14", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ16", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("FREQ18", Dimension::VOLTAGE,{ Tag::FREQ_BAND }),
		new Variable("Battery Level", Dimension::NONE,{ Tag::MCU_INPUT }),
		new Variable("Run TIme", Dimension::TIME,{ Tag::STATUS_BAR }),
		new Variable("IO Freq", Dimension::FREQUENCY,{ Tag::STATUS_BAR }),
		new Variable("Refresh Freq", Dimension::FREQUENCY,{ Tag::STATUS_BAR, Tag::EDITABLE, Tag::USER_INPUT })
	};
	assert(bci::State::program.m_vars.size() == size_t(Vars::END_OF_DATA));
}

void bci::State::loadVars()
{
	for (size_t i = 0; i<m_fast_var_buffer.size(); ++i)
	{
		m_fast_var_buffer[i] = m_vars[i]->data();
	}
}

void bci::State::updateVars()
{
	for (size_t i = 0; i<m_fast_var_buffer.size(); ++i)
	{
		// synchronised data copy
		if (!m_vars[i]->checkTag(Tag::USER_INPUT))
			m_vars[i]->data() = m_fast_var_buffer[i];
	}
}

const std::vector<Variable*> bci::State::searchVars(const std::vector<Tag> tags) const
{
	std::vector<Variable*> search;
	search.reserve(m_vars.size());
	for (size_t i=0; i<m_vars.size(); ++i)
	{
		assert(m_vars[i] != nullptr);
		if (m_vars[i]->checkTags(tags))
		{
			search.push_back(m_vars[i]);
		}
	}
	return std::move(search);
}

const std::vector<View*> bci::State::generateViews(const std::vector<Variable*> vars, View::Type viewType)
{
	std::vector<View*> result;
	result.reserve(vars.size());
	std::vector<Variable*> relatedVars;
	relatedVars.reserve(vars.size());
	// used to ensure each var added to 1 view only
	std::vector<bool> varUsed(vars.size(), false);

	for (size_t i = 0; i < vars.size(); ++i)
	{
		relatedVars.clear();
		auto currName = vars[i]->getName(); // COPY
		auto currDim = vars[i]->getDimension();

		for (size_t j = i; j < vars.size(); ++j)
		{
			// related vars have matching dimensions and names but may differ in wither they are input or output
			if (!varUsed[j] && (vars[j]->getName() == currName) && (vars[j]->getDimension() == currDim))
			{
				relatedVars.push_back(vars[j]);
				varUsed[j] = true;
			}
		}
		if (relatedVars.size() == 0) continue;

		if (viewType == View::Type::TABLE)
		{
			m_views.push_back(new TableView{ relatedVars,{} });
			result.push_back(m_views[m_views.size() - 1]);
		}
		else
			assert(false); // not supported yet	
	}

	return std::move(result);
}

void bci::State::slotViewReady()
{
}
