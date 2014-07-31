#include "stdafx.h"


#include "GameTimer.h"

#include <Windows.h>

GameTimer::GameTimer()
	: m_SecondsPerCount(0.0)
	, m_DeltaTime(-1.0)
	, m_BaseTime(0)
	, m_PausedTime(0)
	, m_StopTime(0)
	, m_PrevTime(0)
	, m_CurrTime(0)
	, m_Stopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	m_SecondsPerCount = 1.0 / (double)countsPerSec;
}


float GameTimer::GameTime() const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if( m_Stopped )
	{
		return (float)(((m_StopTime - m_PausedTime)-m_BaseTime)*m_SecondsPerCount);
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime
	
	else
	{
		return (float)(((m_CurrTime-m_PausedTime)-m_BaseTime)*m_SecondsPerCount);
	}
}

float GameTimer::DeltaTime() const	
{
	return (float)m_DeltaTime;
}

void GameTimer::Reset()
{
	__int64 curr_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);

	m_BaseTime = curr_time;
	m_PrevTime = curr_time;
	m_StopTime = 0;
	m_Stopped  = false;

}
void GameTimer::Start()
{
	if (m_Stopped)
	{
		long long start_time = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&start_time);

		m_PausedTime	+= (start_time - m_StopTime);
		m_StopTime		 = 0;
		m_PrevTime		 = start_time;
		m_Stopped		 = false;
	}
}
void GameTimer::Stop()
{
	if (!m_Stopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_StopTime);
		m_Stopped		= true;
	}
}
void GameTimer::Tick()
{
	if(m_Stopped)
	{
		m_DeltaTime = 0.0;
		return;
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrTime);

	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;
	m_PrevTime =  m_CurrTime;


	if (m_DeltaTime < 0.0)
		m_DeltaTime = 0.0;

}