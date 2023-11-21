#pragma once
#include "IManager.h"
#include <SFML/System/Clock.hpp>

class TimeManager : public IManager
{
public:

    TimeManager();
    ~TimeManager() override;

    void Init() override;
    void Clear() override;

    void Tick();

    static float GetDeltaTime() { return m_DeltaTime; }
    static float GetTotalTime() { return m_TotalTime; }

private:

    inline static sf::Clock m_Clock;
    inline static float m_TotalTime;
    inline static float m_DeltaTime;

};
