#include "state.h"

sdtft DTFT_Shared_Data;

bci::State bci::State::program;

void bci::State::init()
{
	assert(bci::State::program.m_vars.size() == 0);
	bci::State::program.m_vars =
	{
		new Variable("TIME", Dimension::VOLTAGE,{ Tag::STATUS_BAR }),
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
		new Variable("MU1", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU2", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU3", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU4", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU5", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU6", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU7", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU8", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU9", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU10", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU11", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU12", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU13", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU14", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU15", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("MU16", Dimension::VOLTAGE,{ Tag::MCU_INPUT, Tag::FREQ_BAND }),
		new Variable("Battery Level", Dimension::NONE,{ Tag::MCU_INPUT }),
		new Variable("Run TIme", Dimension::TIME,{ Tag::STATUS_BAR }),
		new Variable("IO Freq", Dimension::FREQUENCY,{ Tag::STATUS_BAR }),
		new Variable("Refresh Freq", Dimension::FREQUENCY,{ Tag::STATUS_BAR }),
		new Variable("Used Time %", Dimension::NONE,{ Tag::STATUS_BAR })
	};
	assert(bci::State::program.m_vars.size() == size_t(Vars::END_OF_DATA));
	State::program.m_fast_var_buffer.resize(State::program.m_vars.size(), 0.0);
}

void bci::State::loadVars()
{
	for (size_t i = 0; i<m_fast_var_buffer.size(); ++i)
	{
		//m_fast_var_buffer[i] = m_vars[i]->data();
	}
}

void bci::State::updateVars()
{
	bool update = false;
	if (updateTimer.getDuration() > 0.08)
	{
		State::program[Vars::REFRESH_FREQ] = 1.0 / updateTimer.getDuration();
		update = true;
		updateTimer.restart();
	}

	for (size_t i = 0; i<m_fast_var_buffer.size(); ++i)
	{
		// synchronised data copy
		if (!m_vars[i]->checkTag(Tag::USER_INPUT))
		{
			//m_vars[i]->data() = m_fast_var_buffer[i];
			if (update) m_vars[i]->refresh();
		}
	}
	static int c = 0;

	//qDebug() << c << ' ' << m_vars[1]->getValue();
	++c;
	if (m_reset)
	{
		m_reset = false;
		static std::vector<bool> used;
		used.resize(m_vars.size(), false);

		for (size_t i = 0; i < m_vars.size(); ++i)
		{
			// check for matching names
			for (size_t j = i + 1; j < m_vars.size(); ++j)
			{
				if (used[j]) continue;

				if (m_vars[j]->getName() != m_vars[i]->getName()) continue;
				{
					used[j] = true;
					used[i] = true;

					// figure out if we have a setpoint + value pair
					if (m_vars[j]->checkTag(Tag::USER_INPUT) && (!m_vars[i]->checkTag(Tag::USER_INPUT)))
						m_vars[j]->setValue(m_vars[i]->getValue());
					else if (m_vars[i]->checkTag(Tag::USER_INPUT) && (!m_vars[j]->checkTag(Tag::USER_INPUT)))
						m_vars[i]->setValue(m_vars[j]->getValue());
					else
					{
						assert(false); // probably a mistake
					}
					break;
				}
			}

			if (!used[i])
			{
				//m_vars[i]->setValue(0);
				// TODO: default reset value here
			}
		}
		emit sigVarReset();
	}
	

	if (update) emit sigViewUpdate();
	//emit sigVarUpdate(m_vars[(size_t)Vars::TIME]->data().load());
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
		else if (viewType == View::Type::SLIDER)
		{
			m_views.push_back(new SliderView{ relatedVars,{} });
			result.push_back(m_views[m_views.size() - 1]);
		}
		else
			assert(false); // not supported yet	
	}

	return std::move(result);
}

const std::vector<View*> bci::State::generateView(Variable* var, View::Type viewType, const std::vector<QString>&& names)
{
	if (viewType != View::Type::BUTTONS)
	{
		assert(false);
	}
	std::vector<Variable*> vars{ var };
	m_views.push_back(new ButtonsView(vars, std::move(names)));
	return std::vector<View*>{m_views[m_views.size() - 1]};
}

void bci::State::slotViewReady()
{
}
