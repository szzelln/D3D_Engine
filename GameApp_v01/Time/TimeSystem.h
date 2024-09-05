#pragma once
#include "../Tool/TSingleton.h"

// 게임 내의 모든 객체가 동일한 시간 흐름을 참조해야 되기 때문에 싱글톤 패턴 사용

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
	float m_fTimeScale = 1.0f;		// 시간 빠르기 조절

};

