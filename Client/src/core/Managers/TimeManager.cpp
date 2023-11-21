#include "TimeManager.h"

TimeManager::TimeManager() = default;
TimeManager::~TimeManager() = default;

void TimeManager::Init()
{
    m_TotalTime = 0.0f;
    m_Clock.restart();
}

void TimeManager::Clear()
{

}

void TimeManager::Tick()
{
    m_DeltaTime = m_Clock.restart().asSeconds();
    m_TotalTime += m_DeltaTime;
}
