#pragma once
#pragma once
#pragma once
#pragma once

#include <math.h>
#include <time.h>
#include <synchapi.h>
#include <cstdio>



class Fps {

	int mStartTime;         //�������ýð�
	int mCount = 0;             //ī����
	float mFps;             //fps
	//HWND g_hWnd;

	static const int N = 25;//����� ������� ���� ��
	static const int FPS = 25;	//�����Ϸ��� FPS



public:

	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	int getmFps()
	{
		return (int)mFps;
	}

	int GetNowCount()
	{

		return clock();
	}

	int flowFrame = 0;
	bool Update() {

		flowFrame++;
		if (mCount == 0) { //1�������̶�� �ð��� ���

			mStartTime = GetNowCount();
		}

		if (mCount == N) { //60�������̶�� ����� ���

			int t = GetNowCount();
			mFps = 1000.f / ((t - mStartTime) / (float)N);
			mCount = 0;
			//printf("flow FPS: %4d\n", (int)flowFrame);
			flowFrame = 0;
			mStartTime = t;

		}

	//	mCount++;

		return true;

	}

	void FrameIncrement()
	{
		mCount++;
	}

	void Draw() {
		//WCHAR title[50];

		//swprintf_s(title, 50, L"fps: %4d", (int)mFps);
	//	if ((int)mFps != FPS - 1)
			printf("FPS: %4d\n", (int)mFps);
		//SetWindowTextW(g_hWnd, title);
	}



	bool Wait() {

		int tookTime = GetNowCount() - mStartTime;	//�ɸ��ð�

		int waitTime = mCount * 1000 / FPS - tookTime;	//�������Ѿ� �ϴ� �ð�

		if (waitTime > 0) {
			// Sleep(waitTime);
			return true;	//��ŵ�� �ؾ� �ϴ� ��Ȳ

		}
		else
		{
			return false;
		}

	}

};

