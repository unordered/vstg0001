#pragma once
#pragma once
#pragma once
#pragma once

#include <math.h>
#include <time.h>
#include <synchapi.h>
#include <cstdio>



class Fps {

	int mStartTime;         //측정개시시각
	int mCount = 0;             //카운터
	float mFps;             //fps
	//HWND g_hWnd;

	static const int N = 25;//평균을 얻기위한 샘플 값
	static const int FPS = 25;	//설정하려는 FPS



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
		if (mCount == 0) { //1프레임이라면 시각을 기억

			mStartTime = GetNowCount();
		}

		if (mCount == N) { //60프레임이라면 평균을 계산

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

		int tookTime = GetNowCount() - mStartTime;	//걸린시간

		int waitTime = mCount * 1000 / FPS - tookTime;	//지연시켜야 하는 시간

		if (waitTime > 0) {
			// Sleep(waitTime);
			return true;	//스킵을 해야 하는 상황

		}
		else
		{
			return false;
		}

	}

};

