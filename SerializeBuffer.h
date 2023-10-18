#pragma once
#pragma once
#pragma once
#include <Windows.h>
#ifndef  __PACKET__
#define  __PACKET__

class SerializeBuffer
{
public:

	/*---------------------------------------------------------------
	Packet Enum.

	----------------------------------------------------------------*/
	enum en_PACKET
	{
		eBUFFER_DEFAULT = 1400		// ��Ŷ�� �⺻ ���� ������.
	};

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	SerializeBuffer();
	SerializeBuffer(int iBufferSize);

	virtual	~SerializeBuffer();


	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ  �ı�.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Release(void);


	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);


	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)��Ŷ ���� ������ ���.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void) { return m_iBufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return m_iWritePos - m_iReadPos; }



	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void) { return m_chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);






	/* ============================================================================= */
	// ������ �����ε�
	/* ============================================================================= */
	SerializeBuffer& operator = (SerializeBuffer& clSrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////
	SerializeBuffer& operator << (unsigned char byValue);
	SerializeBuffer& operator << (char chValue);

	SerializeBuffer& operator << (short shValue);
	SerializeBuffer& operator << (unsigned short wValue);

	SerializeBuffer& operator << (int iValue);
	SerializeBuffer& operator << (long lValue);
	SerializeBuffer& operator << (float fValue);

	SerializeBuffer& operator << (__int64 iValue);
	SerializeBuffer& operator << (double dValue);


	//////////////////////////////////////////////////////////////////////////
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////
	SerializeBuffer& operator >> (BYTE& byValue);
	SerializeBuffer& operator >> (char& chValue);

	SerializeBuffer& operator >> (short& shValue);
	SerializeBuffer& operator >> (WORD& wValue);

	SerializeBuffer& operator >> (int& iValue);
	SerializeBuffer& operator >> (DWORD& dwValue);
	SerializeBuffer& operator >> (float& fValue);

	SerializeBuffer& operator >> (__int64& iValue);
	SerializeBuffer& operator >> (double& dValue);




	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ���.
	//
	// Parameters: (char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char* chpSrc, int iSrcSize);




protected:
	char* m_chpBuffer;
	int	m_iBufferSize;

	int m_iReadPos;
	int m_iWritePos;

	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	int	m_iDataSize;


};



#endif
