#pragma once
#include "../Tool/TSingleton.h"

// ���� ���� ��� ��ü�� ������ �ð� �帧�� �����ؾ� �Ǳ� ������ �̱��� ���� ���

class TimeSystem : public TSingleton<TimeSystem>
{
	friend class TSingleton<TimeSystem>;

private:
	TimeSystem() = default;
	virtual ~TimeSystem() = default;

	TimeSystem(const TimeSystem&) = delete;
	TimeSystem& operator=(const TimeSystem&) = delete;

public:
	void Init();
	void Update();

	float GetfDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float _timeScale);


private:
	LARGE_INTEGER m_curTime = { 0 };
	LARGE_INTEGER m_prevTime = { 0 };
	LARGE_INTEGER m_frequency = { 0 };
	float m_fDeltaTime = 0.0f;
	float m_fTimeScale = 1.0f;		// �ð� ������ ����

};

