#include "TimeManager.h"

TimeManager::TimeManager() = default;
TimeManager::~TimeManager() = default;


void TimeManager::Tick()
{
    m_DeltaTime = m_Clock.restart().asSeconds();
    m_TotalTime += m_DeltaTime;
}
