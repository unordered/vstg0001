#include "SerializeBuffer.h"

int SerializeBuffer::GetData(char* chpDest, int iSize)
{
	if (m_iDataSize - iSize < 0)
	{
		return -1;
	}
	else
	{
		memcpy(chpDest, &m_chpBuffer[m_iReadPos], iSize);
		m_iDataSize -= iSize;
		m_iReadPos += iSize;
		return iSize;
	}
}

int SerializeBuffer::PutData(char* chpSrc, int iSrcSize)
{
	if (m_iWritePos + iSrcSize > eBUFFER_DEFAULT)
	{
		return -1;
	}
	else
	{
		memcpy(&m_chpBuffer[m_iWritePos], chpSrc, iSrcSize);
		m_iDataSize += iSrcSize;
		m_iWritePos += iSrcSize;
		return iSrcSize;
	}
}

// 쓰기한 바이트 만큼 이동
int SerializeBuffer::MoveWritePos(int iSize)
{
	if (m_iWritePos + iSize > eBUFFER_DEFAULT)
	{
		return 0;
	}
	else
	{
		m_iDataSize += iSize;
		m_iWritePos += iSize;
		return iSize;
	}
}

int SerializeBuffer::MoveReadPos(int iSize)
{
	if (m_iDataSize - iSize < 0)
	{
		return 0;
	}
	else
	{
		m_iDataSize -= iSize;
		m_iReadPos += iSize;
		return iSize;
	}
}

SerializeBuffer& SerializeBuffer::operator=(SerializeBuffer& clSrcPacket)
{
	// TODO: 여기에 return 문을 삽입합니다.
	memcpy(this->m_chpBuffer, clSrcPacket.m_chpBuffer, clSrcPacket.GetDataSize());
	this->m_iDataSize = clSrcPacket.GetDataSize();
	this->m_iReadPos = clSrcPacket.m_iReadPos;
	this->m_iWritePos = clSrcPacket.m_iWritePos;
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(unsigned char byValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &byValue, sizeof(unsigned char));
	m_iDataSize += sizeof(unsigned char);
	m_iWritePos += sizeof(unsigned char);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(char chValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &chValue, sizeof(char));
	m_iDataSize += sizeof(char);
	m_iWritePos += sizeof(char);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(short shValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &shValue, sizeof(short));
	m_iDataSize += sizeof(short);
	m_iWritePos += sizeof(short);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(unsigned short wValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &wValue, sizeof(unsigned short));
	m_iDataSize += sizeof(unsigned short);
	m_iWritePos += sizeof(unsigned char);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(int iValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &iValue, sizeof(int));
	m_iDataSize += sizeof(int);
	m_iWritePos += sizeof(int);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(long lValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &lValue, sizeof(long));
	m_iDataSize += sizeof(long);
	m_iWritePos += sizeof(long);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(float fValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &fValue, sizeof(float));
	m_iDataSize += sizeof(float);
	m_iWritePos += sizeof(float);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(__int64 iValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &iValue, sizeof(__int64));
	m_iDataSize += sizeof(__int64);
	m_iWritePos += sizeof(__int64);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator<<(double dValue)
{
	memcpy(&this->m_chpBuffer[m_iWritePos], &dValue, sizeof(double));
	m_iDataSize += sizeof(double);
	m_iWritePos += sizeof(double);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(BYTE& byValue)
{
	memcpy(&byValue, &this->m_chpBuffer[m_iReadPos], sizeof(BYTE));
	m_iDataSize -= sizeof(BYTE);
	m_iReadPos += sizeof(BYTE);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(char& chValue)
{
	memcpy(&chValue, &this->m_chpBuffer[m_iReadPos], sizeof(char));
	m_iDataSize -= sizeof(char);
	m_iReadPos += sizeof(char);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(short& shValue)
{
	memcpy(&shValue, &this->m_chpBuffer[m_iReadPos], sizeof(short));
	m_iDataSize -= sizeof(short);
	m_iReadPos += sizeof(short);
	return *this;;
}

SerializeBuffer& SerializeBuffer::operator>>(WORD& wValue)
{
	memcpy(&wValue, &this->m_chpBuffer[m_iReadPos], sizeof(WORD));
	m_iDataSize -= sizeof(WORD);
	m_iReadPos += sizeof(WORD);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(int& iValue)
{

	memcpy(&iValue, &this->m_chpBuffer[m_iReadPos], sizeof(int));
	m_iDataSize -= sizeof(int);
	m_iReadPos += sizeof(int);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(DWORD& dwValue)
{
	memcpy(&dwValue, &this->m_chpBuffer[m_iReadPos], sizeof(DWORD));
	m_iDataSize -= sizeof(DWORD);
	m_iReadPos += sizeof(DWORD);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(float& fValue)
{
	memcpy(&fValue, &this->m_chpBuffer[m_iReadPos], sizeof(float));
	m_iDataSize -= sizeof(float);
	m_iReadPos += sizeof(float);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(__int64& iValue)
{
	memcpy(&iValue, &this->m_chpBuffer[m_iReadPos], sizeof(__int64));
	m_iDataSize -= sizeof(__int64);
	m_iReadPos += sizeof(__int64);
	return *this;
}

SerializeBuffer& SerializeBuffer::operator>>(double& dValue)
{
	memcpy(&dValue, &this->m_chpBuffer[m_iReadPos], sizeof(double));
	m_iDataSize -= sizeof(double);
	m_iReadPos += sizeof(double);
	return *this;
}


// 생성자
SerializeBuffer::SerializeBuffer()
{
	m_chpBuffer = new char[eBUFFER_DEFAULT];
	m_iBufferSize = 0;
	m_iDataSize = 0;
	m_iReadPos = 0;
	m_iWritePos = 0;
}

SerializeBuffer::SerializeBuffer(int iBufferSize)
{
	m_chpBuffer = new char[iBufferSize];
	m_iBufferSize = 0;
	m_iDataSize = 0;
	m_iReadPos = 0;
	m_iWritePos = 0;
}

SerializeBuffer::~SerializeBuffer()
{
	if (m_chpBuffer != nullptr)
		delete[] m_chpBuffer;
}

void SerializeBuffer::Release(void)
{
	delete[] m_chpBuffer;
	m_chpBuffer = nullptr;
}

void SerializeBuffer::Clear(void)
{
	m_iDataSize = 0;
	m_iReadPos = 0;
	m_iWritePos = 0;
}
