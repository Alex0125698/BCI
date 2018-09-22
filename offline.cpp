#include "offline.h"
#include "csvio.h"
#include <QTimer>

// automatically add line & file macros
#define SIGERROR(x) sigError(x,__FILE__,__LINE__)

bci::Offline::Offline(const std::string&& fileName, uint32_t freq_Hz)
	: m_fileName(fileName), m_freq_hz(freq_Hz)
{
}

// TODO: it may be worth profiling this
// TODO: figure out how to make this faster
void bci::Offline::openFile()
{
	try
	{
		CSVreader in;
		in.open(m_fileName);
		DEBUG_PRINTLN(QString("Loading file: %1 ...").arg(m_fileName.c_str()));
		{
			// number of cols assumed to be number of channels
			size_t num_cols = 0;
			// get channel names + number of channels
			while (true)
			{
				// read the next element
				const auto str = in.read();
				if (str == "\n")
					break;
				else if (str == "")
				{
					emit SIGERROR(QString("premature end of data input file: %1").arg(m_fileName.c_str()));
					stop_helper();
					return;
				}

				// TODO: check positions
				++num_cols;
			}
			m_num_channels = num_cols;
		}

		{
			// now we load the data
			m_data.clear();
			std::vector<double> tmp(m_num_channels, 0.0);

			while (true)
			{	
				for (size_t i = 0; i < m_num_channels; ++i)
				{
					// read the next element
					const auto str = in.read();

					if (str == "\n")
					{
						DEBUG_PRINTLNY("row found without required column count", MSG_TYPE::WARNING);
						goto END;
					}
					if (str == "")
					{
						DEBUG_PRINTLNY("data input file ended before row complete", MSG_TYPE::WARNING);
						goto END;
					}
					else
						tmp[i] = std::stod(str);
				}

				m_data.emplace_back(tmp);

				// get rid of the newline
				const auto str = in.read();
				if (str == "")
				{
					break;
				}
				else if (str != "\n")
				{
					DEBUG_PRINTLNY("row found with too many elments", MSG_TYPE::WARNING);
					break;
				}
			}
			// needed to get out of nested loop
		END: ;
		}
		DEBUG_PRINTLN(QString("file load completed with %1 channels and %2 rows").arg(m_num_channels).arg(m_data.size()));
	}
	catch (DetailedException& e)
	{
		emit SIGERROR(QString("Data aquisition file: %1 could not be loaded").arg(m_fileName.c_str()));
		stop_helper();
	}
	catch (std::exception& e)
	{
		emit SIGERROR(QString("STD error when loading data: ") + e.what());
		stop_helper();
	}
}

void bci::Offline::start_helper()
{
	// unfortunately Qt timers have a minimum interval of 1 millisecond
	assert(uint64_t(1000.0 / m_freq_hz) >= 1);
	openFile();
	// TODO: should I be using a semaphore for this ???
	m_stats_avaliable = true;
	m_data_counter = 0;
	m_timer->setInterval(std::chrono::milliseconds(uint64_t(1000.0 / m_freq_hz)));
	m_timer->start();
}

void bci::Offline::stop_helper()
{
	m_timer->stop();
}

void bci::Offline::init()
{
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &bci::Offline::slotGetSample, Qt::QueuedConnection);
}

void bci::Offline::slotGetSample()
{
	size_t dataSize = 0, chDataSize = 0;

	{
		std::lock_guard<std::mutex> lock(m_ch_mtx);
		dataSize = m_data.size();
		chDataSize = m_channel.size();
	}

	if (m_data_counter >= dataSize || dataSize == 0)
	{
		// no more data left so we stop
		// m_connected set to false
		DEBUG_PRINTLN("offline data has ended");
		stop();
		return;
	}

	if (chDataSize > 512)
	{
		SIGERROR("Controller was unable to keep up with BCI");
		return;
	}

	{
		std::lock_guard<std::mutex> lock(m_ch_mtx);
		m_channel.push(m_data[m_data_counter]);
	}
	++m_data_counter;

	emit sigDataReady();
}
