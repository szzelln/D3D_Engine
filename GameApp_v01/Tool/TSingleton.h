#pragma once

template <typename T>
class TSingleton
{
protected:
	TSingleton() = default;
	virtual ~TSingleton() = default;

private:
	TSingleton(const TSingleton&) = delete;				// ��������� delete
	TSingleton& operator=(const TSingleton&) = delete;	// ������Կ����� delete

public:
	static T* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T();
		}

		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static T* m_pInstance;

};

template <typename T>
T* TSingleton<T>::m_pInstance = nullptr;