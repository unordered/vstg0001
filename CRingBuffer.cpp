#include "CRingBuffer.h"
#include <cstdlib>
#include <corecrt_memcpy_s.h>
#include <minwinbase.h>
#include <synchapi.h>



void RingBuffer::Lock(void)
{
	//AcquireSRWLockExclusive(&m_SRWLock);
	EnterCriticalSection(&cs);
}

void RingBuffer::UnLock(void)
{
	//ReleaseSRWLockExclusive(&m_SRWLock);
	LeaveCriticalSection(&cs);
}

RingBuffer::RingBuffer() : ringBufferSize(15000)
{

	//InitializeSRWLock(&m_SRWLock);
	InitializeCriticalSection(&cs);

	// 4 x
	//begin = (char*)malloc(ringBufferSize);
	temp = new char[ringBufferSize + 4];
	begin = temp + 4;
	end = begin + ringBufferSize;
	readPointer = writePointer = begin;
}

RingBuffer::RingBuffer(int bufferSize) : ringBufferSize(bufferSize)
{
	//InitializeSRWLock(&m_SRWLock);
	InitializeCriticalSection(&cs);

	//begin = (char*)malloc(ringBufferSize);
	temp = new char[ringBufferSize + 4];
	begin = temp + 4;
	end = begin + ringBufferSize;
	readPointer = writePointer = begin;
}

int RingBuffer::GetBufferSize(void)
{

	return (int)(end - begin) - 1;
}

int RingBuffer::GetUseSize(void)
{
	char* writePointer1 = writePointer;
	char* readPointer1 = readPointer;

	if (writePointer1 >= readPointer1)
		return (int)(writePointer1 - readPointer1);
	else
		return (int)(writePointer1 - begin) + (int)(end - readPointer1);
}



int RingBuffer::GetFreeSize(void)
{
	char* writePointer1 = writePointer;
	char* readPointer1 = readPointer;

	if (writePointer1 >= readPointer1)
		return (int)(end - writePointer1) + (int)(readPointer1 - begin - 1);
	else
		return (int)(readPointer1 - writePointer1 - 1);
}



int RingBuffer::GetNotBrokenPutSize(void)
{
	char* writePointer1 = writePointer;
	char* readPointer1 = readPointer;
	// 여기 있는 순간에는 read > write
	if (writePointer1 >= readPointer1)
	{

		if (begin == readPointer1)
		{
			return (int)(end - writePointer1 - 1);
		}
		else
		{
			//return (int)(end - writePointer1);
			return (int)(end - writePointer1); // 수정 0511
		}
	}
	else
	{
		// 여기 있는 순간에는 read < write
		return (int)(readPointer1 - writePointer1 - 1);
	}
}


int RingBuffer::GetNotBrokenGetSize(void)
{
	char* writePointer1 = writePointer;
	char* readPointer1 = readPointer;

	if (writePointer1 >= readPointer1)
		return (int)(writePointer1 - readPointer1);
	return (int)(end - readPointer1);
}


int RingBuffer::Put(const char* buffer, int size)
{

	//////////////////////////// 새로운 버전 /////////////////////////////
	char* curWritePos = this->writePointer;
	char* curReadPos = this->readPointer;
	int FreeSize = 0;

	if (curWritePos >= curReadPos)
		FreeSize = (int)(end - curWritePos) + (int)(curReadPos - begin - 1);
	else
		FreeSize = (int)(curReadPos - curWritePos - 1);

	if (FreeSize < size)
	{
		return 0;
	}


	const char* temp = buffer;
	int directEnqueueSize;
	if (curWritePos >= curReadPos)
	{

		if (begin == curReadPos)
		{
			directEnqueueSize = (int)(end - curWritePos - 1);
		}
		else
		{
			directEnqueueSize = (int)(end - curWritePos);
		}
	}
	else
	{
		directEnqueueSize = (int)(curReadPos - curWritePos - 1);
	}


	if (directEnqueueSize >= size)
	{
		memcpy_s(curWritePos, size, buffer, size);
		MoveWritePos(size);
		return size;
	}


	memcpy_s(curWritePos, directEnqueueSize, temp, directEnqueueSize);
	temp += directEnqueueSize;
	int remainSize = size - directEnqueueSize;



	memcpy_s(begin, remainSize, temp, remainSize);
	MoveWritePos(size);



	return size;

	/////////////////////////////////////////////////////////////////////

	//if (GetFreeSize() < size)
	//{
	//	
	//	return 0;
	//}
	//const char* temp = buffer;
	//int directEnqueueSize = DirectEnqueueSize();


	//if (directEnqueueSize >= size)
	//{
	//	memcpy_s(writePointer, size, buffer, size);
	//	MoveWrite(size);
	//	return size;
	//	
	//}

	//memcpy_s(writePointer, directEnqueueSize, temp, directEnqueueSize);
	//temp += directEnqueueSize;
	//int remainSize = size - directEnqueueSize;

	//

	//memcpy_s(begin, remainSize, temp, remainSize);
	//MoveWrite(size);

	//return size;

}

int RingBuffer::Get(char* pDest, int size)
{
	////////////////////////////// 새로운 버전 /////////////////////////////
	char* curWritePos = this->writePointer;
	char* curReadPos = this->readPointer;

	int getUseSize = 0;
	if (curWritePos >= curReadPos)
		getUseSize = (int)(curWritePos - curReadPos);
	else
		getUseSize = (int)(curWritePos - begin) + (int)(end - curReadPos);

	if (getUseSize < size)
	{
		return 0;
	}

	char* pDestTemp = pDest;

	int directDequeueSize = 0;
	if (curWritePos >= curReadPos)
		directDequeueSize = (int)(curWritePos - curReadPos);
	else
		directDequeueSize = (int)(end - curReadPos);

	if (directDequeueSize >= size)
	{
		memcpy_s(pDest, size, curReadPos, size);

		curReadPos += size;
		if (curReadPos >= end)
		{
			int overFlow = (int)(curReadPos - end);
			curReadPos = (begin + overFlow);
		}
		this->readPointer = curReadPos;

		return size;
	}


	memcpy_s(pDestTemp, directDequeueSize, curReadPos, directDequeueSize);
	int remainSize = size - directDequeueSize;
	pDestTemp += directDequeueSize;

	memcpy_s(pDestTemp, remainSize, begin, remainSize);


	//curReadPos += size;
	//if (curReadPos >= end)
	//{
	//	int overFlow = curReadPos - end;
	//	curReadPos = begin + overFlow;
	//}
	//this->readPointer = curReadPos;
	RemoveData(size);

	return size;
	/////////////////////////////////////////////////////////////////////
	////EnterCriticalSection(&this->cs);


	//if (GetUseSize() < size)
	//{
	//	//LeaveCriticalSection(&this->cs);
	//	return 0;
	//}
	//
	//char* pDestTemp = pDest;
	//int directDequeueSize = DirectDequeueSize();

	//if (directDequeueSize >= size)
	//{
	//	memcpy_s(pDest, size, readPointer, size);
	//	MoveRead(size);
	//	return size;
	//}


	//memcpy_s(pDestTemp, directDequeueSize, readPointer, directDequeueSize);
	//int remainSize = size - directDequeueSize;
	//pDestTemp += directDequeueSize;

	//memcpy_s(pDestTemp, remainSize, begin, remainSize);

	//MoveRead(size);
	//return size;



}

int RingBuffer::Peek(char* chpDest, int iSize)
{
	if (GetUseSize() < iSize)
		return 0;
	if (GetNotBrokenGetSize() >= iSize)
	{
		memcpy_s(chpDest, iSize, readPointer, iSize);
		return iSize;
	}
	char* pFrontTemp = readPointer;
	char* pDestTemp = chpDest;
	int directDeqeueSize = GetNotBrokenGetSize();
	memcpy_s(pDestTemp, directDeqeueSize, readPointer, directDeqeueSize);
	RemoveData(directDeqeueSize);

	int remainSize = iSize - directDeqeueSize;
	pDestTemp += directDeqeueSize;
	memcpy_s(pDestTemp, remainSize, readPointer, remainSize);
	readPointer = pFrontTemp;

	return iSize;
}

int RingBuffer::MoveWritePos(int size)
{
	//EnterCriticalSection(&this->cs);
	char* temp = writePointer;
	temp += size;
	if (temp >= end)
	{
		int overflow = (int)(temp - end);
		temp = begin + overflow;
	}
	writePointer = temp;
	//	LeaveCriticalSection(&this->cs);
	return size;
}


int RingBuffer::RemoveData(int size)
{
	char* temp = readPointer;
	temp += size;
	if (temp >= end)
	{
		long long overFlow = temp - end;
		temp = begin + overFlow;
	}

	readPointer = temp;

	return size;
}



char* RingBuffer::GetWriteBufferPtr(void)
{
	return writePointer;
}

char* RingBuffer::GetReadBufferPtr(void)
{
	return readPointer;
}

RingBuffer::~RingBuffer()
{
	//delete[] begin;
	delete[] temp;
}


void RingBuffer::ClearBuffer(void)
{
	readPointer = writePointer = begin;
}