#include "pch.h"
#include "TimeSystem.h"

void TimeSystem::Init()
{
	QueryPerformanceCounter(&m_prevTime);
	QueryPerformanceFrequency(&m_frequency);
}

void TimeSystem::Update()
{
	// ���� ī��Ʈ �� ����
	QueryPerformanceCounter(&m_curTime);

	// ��Ÿ Ÿ�� ��� (�� ����)
	m_fDeltaTime = static_cast<float>(m_curTime.QuadPart - m_prevTime.QuadPart)
				  / static_cast<float>(m_frequency.QuadPart);

	// ���� ī��Ʈ ���� ���簪���� ����(�������� ����� ���ؼ�)
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
