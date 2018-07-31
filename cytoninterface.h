#pragma once

#include "resources.h"
#include "bciinterface.h"

class QTimer;
class QSerialPort;
class QThread;

namespace bci
{

class CytonInterface : public bci::Interface
{
	Q_OBJECT

public:
	CytonInterface();
	virtual ~CytonInterface();
	// acquire resources + connect to device + start stream
	virtual void start_helper() override;
	
	// free resources + disconnect
	virtual void stop_helper() override;

protected:
	virtual void init() override;

private slots:
	void slotTimeout();
	void slotBytesWritten(qint64 bytes);
	void slotCOMerror();
	void slotReadyRead();
	
private:
	void write(QString&& msg);
	char getNibbleDigit(char c);
	void processByte(quint8 byte);
	void decode();

private:
	QSerialPort* m_serialPort{ nullptr };
	// used to ensure data sent on time
	std::unique_ptr<QTimer> m_timer;
	// stores how many send bytes written so far
	uint64_t m_bytesWritten{ 0 };
	// stores the message to send to Cyton
	QString m_msg;
	// st to true each time end of packet byte received
	bool m_pkt_rdy{ false };
	// true if currently storing packet
	bool m_counting{ false };
	// which byte number are we processing?
	uint32_t m_pkt_byte_num{ 0 };
	// stores the raw packet bytes
	std::array<uint8_t, 32> m_pkt_raw{ {0} };
	// the aux data type
	int m_type{ 0 };
	// stores current packet in hexadecimal
	QString m_pkt_hex;
	// stores any non packet received data
	QString m_info;
	// block using the serial port
	bool m_error_state{ false };
	bool m_interleave{ false };
};

}