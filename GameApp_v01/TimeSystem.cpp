#include "pch.h"
#include "TimeSystem.h"

void TimeSystem::Init()
{
	QueryPerformanceCounter(&m_prevTime);
	QueryPerformanceFrequency(&m_frequency);
}

void TimeSystem::Update()
{
	// 현재 카운트 값 측정
	QueryPerformanceCounter(&m_curTime);

	// 델타 타임 계산 (초 단위)
	m_fDeltaTime = static_cast<float>(m_curTime.QuadPart - m_prevTime.QuadPart)
				  / static_cast<float>(m_frequency.QuadPart);

	// 이전 카운트 값을 현재값으로 갱신(다음번에 계산을 위해서)
	m_prevTime = m_curTime;
}

float TimeSystem::GetfDeltaTime()
{
	return m_fDeltaTime * m_fTimeScale;
}

float TimeSystem::GetTimeScale()
{
	return m_fTimeScale;
}

void TimeSystem::SetTimeScale(float _timeScale)
{
	m_fTimeScale = _timeScale;
}
