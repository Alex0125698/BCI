#include "cytoninterface.h"
#include <QTimer>
#include <QSerialPort>

bci::CytonInterface::CytonInterface()
{
	m_serialPort = std::make_unique<QSerialPort>("COM3");
	m_timer = std::make_unique<QTimer>();

	m_timer->setSingleShot(true);
	m_serialPort->setBaudRate(QSerialPort::Baud115200);

	connect(m_serialPort.get(), &QSerialPort::readyRead, this, &CytonInterface::slotReadyRead, Qt::QueuedConnection);
	connect(m_serialPort.get(), &QSerialPort::bytesWritten, this, &CytonInterface::slotBytesWritten, Qt::QueuedConnection);
	connect(m_serialPort.get(), &QSerialPort::errorOccurred, this, &CytonInterface::slotCOMerror, Qt::QueuedConnection);
	connect(m_timer.get(), &QTimer::timeout, this, &CytonInterface::slotTimeout, Qt::QueuedConnection);
}

bci::CytonInterface::~CytonInterface()
{
	try
	{
		stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		m_serialPort->close();
	}
	catch (...)
	{
	}
}

void bci::CytonInterface::start()
{
	m_serialPort->open(QIODevice::ReadWrite);

	if (!m_serialPort->isOpen())
	{
		throw DETAILEDEXCEPTION("Failed to open serial port");
	}
	write("b");
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
		assert((byte >> 4u) == (quint8)0xC);
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
	//pkt.sampleNum = m_pkt_raw[0];

	auto get24Signed = [&](int i)
	{
		return int32_t((uint32_t(m_pkt_raw[i]) << 16u) | (uint8_t(m_pkt_raw[i + 1]) << 8u) | uint8_t(m_pkt_raw[i + 2]));
	};

	int chByte = 1;
	/*
	for (auto& ch : pkt.channel)
	{
		ch = get24Signed(chByte) / double(1u << 23u);
		chByte += 3;
	}
	*/
	// todo chByte = 25
	/*
	for (auto& aux : pkt.aux)
	{
		aux = m_pkt_raw[chByte];
		++chByte;
	}
	*/
}

void bci::CytonInterface::stop()
{
	write("s");
}

void bci::CytonInterface::slotTimeout()
{
	QString err = QString("CytonInterface write timeout on Serial port %1; possible error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
	throw DETAILEDEXCEPTION(err);
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
	throw DETAILEDEXCEPTION(QString("An I/O error occurred while writing the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()));
}

void bci::CytonInterface::write(QString&& msg)
{
	m_msg = msg;
	const qint64 bytesWritten = m_serialPort->write(msg.toLocal8Bit());

	if (bytesWritten == -1)
	{
		throw DETAILEDEXCEPTION(QString("Failed to write the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()));
	} 
	else if (bytesWritten != msg.size())
	{
		throw DETAILEDEXCEPTION(QString("Failed to write all the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()));
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
			m_pkt_hex += getNibbleDigit(byte >> 4u);
			m_pkt_hex += getNibbleDigit(byte);
		}
		else
		{
			m_info += byte;
		}

		if (m_pkt_rdy)
		{
			decode();
			if (m_info != "")
			{
				//DEBUG_PRINTLN(info);
				m_info = "";
			}
			//ui->textReceive->appendPlainText(s);
			//ui->textReceive->appendPlainText(plain);
			m_pkt_hex = "";
		}
	}

	if (m_info != "")
	{
		//DEBUG_PRINTLN(info);
		m_info = "";
	}
}
