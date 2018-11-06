#include "resources.h"
#include "cytoninterface.h"
#include <QSerialPort>

// automatically add line & file macros
#define SIGERROR(x) sigError(x,__FILE__,__LINE__)

bci::CytonInterface::~CytonInterface()
{
	stop();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	while (m_connected)
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	DEBUG_PRINTLN("Cyton Interface Stopped");
}

void bci::CytonInterface::start_helper()
{
	firstPacket = true;
	m_stats_avaliable = true;
	if (m_serialPort->isOpen())
	{
		if (!m_error_state)
			DEBUG_PRINTLNY("CytonInterface already open", MSG_TYPE::WARNING);
		return;
	}

	m_serialPort->setPortName(m_port);
	bool open = m_serialPort->open(QIODevice::ReadWrite);

	if (open)
	{
		connect(m_serialPort, &QSerialPort::readyRead, this, &CytonInterface::slotReadyRead, Qt::QueuedConnection);
		connect(m_serialPort, &QSerialPort::bytesWritten, this, &CytonInterface::slotBytesWritten, Qt::QueuedConnection);
		connect(m_serialPort, &QSerialPort::errorOccurred, this, &CytonInterface::slotCOMerror, Qt::QueuedConnection);
		std::this_thread::sleep_for(std::chrono::milliseconds(2600));
		m_connected = true;
		write("b");
	}
	else
	{
		m_error_state = true;
		emit SIGERROR(QString("Failed to open serial port: %2, error: %1").arg(m_serialPort->errorString()).arg(m_port));
		stop();
	}
}

char bci::CytonInterface::getNibbleDigit(char c)
{
	c = c & 0x0Fu;

	if (c < 10)
		return c + '0';
	else
		return c + 'A' - 10;
}

void bci::CytonInterface::processByte(quint8 byte)
{
	m_pkt_rdy = false;

	if (m_pkt_byte_num == 0)
	{
		if (quint8(byte) == quint8(0xA0u))
			m_counting = true;
		else
			m_counting = false;
	}
	else if (m_pkt_byte_num == 32)
	{
		if (uint8_t(byte & 0xF0u) != (uint8_t)0xC0u)
		{
			emit SIGERROR(QString("incorrect end of packet byte: %1").arg(byte & 0xF0u));
			stop();
		}
		m_pkt_rdy = true;
		m_type = byte & 0xF0;
		m_pkt_byte_num = 0;
		m_counting = false;
	}
	else
		m_pkt_raw[m_pkt_byte_num - 1] = byte;

	if (m_counting) ++m_pkt_byte_num;
}

void bci::CytonInterface::decode()
{
	auto sampleNum = m_pkt_raw[0];

	uint8_t requiredSample = uint8_t(m_rxPacket.sampleNum + 1);

	if ((sampleNum != requiredSample) && (!firstPacket))
	{
		DEBUG_PRINTLNY(QString("Dropped packed, ecpected %1, got %2").arg(requiredSample).arg(sampleNum), MSG_TYPE::WARNING);
	}

	firstPacket = false;
	m_rxPacket.sampleNum = sampleNum;
	   
	auto get24Signed = [&](int i)
	{
		return int32_t((uint32_t((int8_t)m_pkt_raw[i]) << 16u) | (m_pkt_raw[i + 1] << 8u) | m_pkt_raw[i + 2]);
	};

	double V_REF = 4.5; //volts
	double MAX_RAW = 1 << 23;
	double GAIN = 24;

	double SCALE = V_REF / (MAX_RAW*GAIN);

	int chByte = 1;
	for (int i=0; i<8; ++i)
	{
		double tmp = SCALE*get24Signed(chByte);
		chByte += 3;
		{
			if ((m_rxPacket.sampleNum & 0x01u) == 0)
				m_rxPacket.channels[i] = tmp;
			else
				m_rxPacket.channels[i + 8] = tmp;
		}
	}

	// todo chByte = 25
	/*
	for (auto& aux : pkt.aux)
	{
		aux = m_pkt_raw[chByte];
		++chByte;
	}
	*/
}

void bci::CytonInterface::stop_helper()
{
	m_pkt_rdy = false;
	m_counting = false;
	m_pkt_byte_num = 0;
	m_timer->stop();
	if (m_serialPort->isOpen())
	{
		if (!m_error_state) write("s");
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
		m_serialPort->close();
		disconnect(m_serialPort);
		DEBUG_PRINTLN("Closed " + m_port);
	}
	m_error_state = false;
	m_connected = false;
}

void bci::CytonInterface::init()
{
	m_serialPort = new QSerialPort();
	m_serialPort->setBaudRate(QSerialPort::Baud115200);

	m_timer = std::make_unique<QTimer>();
	m_timer->setSingleShot(true);
	connect(m_timer.get(), &QTimer::timeout, this, &CytonInterface::slotTimeout, Qt::QueuedConnection);
}

void bci::CytonInterface::slotTimeout()
{
	m_error_state = true;
	stop();
	emit SIGERROR(QString("CytonInterface write timeout on Serial port %1; possible error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()));
}

void bci::CytonInterface::slotBytesWritten(qint64 bytes)
{
	m_bytesWritten += bytes;
	if (m_bytesWritten == m_msg.size())
	{
		m_timer->stop();
		m_bytesWritten = 0;
	}
}

void bci::CytonInterface::slotCOMerror()
{
	if (!m_error_state)
		emit SIGERROR(QString("An I/O error occurred while writing the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()));
	m_error_state = true;
	stop();
	m_serialPort->clearError();
}

void bci::CytonInterface::write(QString&& msg)
{
	m_msg = msg;
	const qint64 bytesWritten = m_serialPort->write(msg.toLocal8Bit());

	if (bytesWritten == -1)
	{
		m_error_state = true;
		stop();
		emit SIGERROR(QString("Failed to write the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()));
		return;
	} 
	else if (bytesWritten != msg.size())
	{
		m_error_state = true;
		stop();
		emit SIGERROR(QString("Failed to write all the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()));
		return;
	}

	m_timer->start(200);
}

void bci::CytonInterface::slotReadyRead()
{
	QByteArray rx = std::move(m_serialPort->readAll());

	for (auto& byte : rx)
	{
		processByte(byte);

		if (m_counting || m_pkt_rdy)
		{
			//m_pkt_hex += getNibbleDigit(byte >> 4u);
			//m_pkt_hex += getNibbleDigit(byte);
		}
		else
		{
			m_info += byte;
		}

		if (m_pkt_rdy)
		{
			if (m_info != "")
			{
				DEBUG_PRINTLN(m_info);
				m_info = "";
			}
			decode();
			if ((m_rxPacket.sampleNum & 0x01u) == 1)
			{
				{
					std::lock_guard<std::mutex> lock(m_ch_mtx);
					if (m_channel.size() >= 256)
					{
						stop();
						emit SIGERROR("Controller failed to keep up with BCI");
					}
					m_channel.emplace(m_rxPacket.channels.begin(), m_rxPacket.channels.end());
				}
				
				emit sigDataReady();
			}

			//ui->textReceive->appendPlainText(s);
			//ui->textReceive->appendPlainText(plain);
			//m_pkt_hex = "";
		}
	}

	if (m_info != "")
	{
		DEBUG_PRINTLN(m_info);
		m_info = "";
	}
}
