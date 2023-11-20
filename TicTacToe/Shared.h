#pragma once
#include <synchapi.h>

template <typename T> class Lock;
/// <summary>
/// A thread-safe wrapper for a resource.
/// </summary>
template <typename T>
class Shared final
{
public:
    Shared()
        : m_Resource(), m_CriticalSection()
    {
        InitializeCriticalSection(&m_CriticalSection);
    }
    ~Shared() noexcept(false)
    {
        DeleteCriticalSection(&m_CriticalSection);
    }
    /// <summary>
    /// Try to get the resource.
    /// </summary>
    Lock<T> TryGet() const
    {
        if (TryEnterCriticalSection(&m_CriticalSection))
        {
            return Lock(&m_Resource, &m_CriticalSection);
        }
        return Lock<T>();
    }
    /// <summary>
    /// Wait until the resource is available and get it.
    /// </summary>
    Lock<T> WaitGet() const
    {
        EnterCriticalSection(&m_CriticalSection);
        return m_Resource;
    }

private:
    Shared(const Shared&) = delete;
    Shared& operator=(const Shared&) = delete;

    T m_Resource;
    CRITICAL_SECTION m_CriticalSection;
};

template <typename T>
class Lock final
{
public:
    ~Lock()
    {
        if (m_CriticalSection != nullptr)
        {
            LeaveCriticalSection(m_CriticalSection);
        }
    }
    /// <summary>
    /// Check if the lock is valid.
    /// </summary>
    bool IsValid() const
    {
        return m_Resource != nullptr;
    }
    /// <summary>
    /// Get the resource.
    /// </summary>
    T& operator->()
    {
        return *m_Resource;
    }

private:
    Lock() = delete;
    Lock(const Lock&) = delete;
    Lock& operator=(const Lock&) = delete;

    friend class Shared<T>;
    Lock() : m_resource(nullptr), m_CriticalSection(nullptr)
    {
    }
    Lock(T* resource, CRITICAL_SECTION* criticalSection)
        : m_Resource(resource), m_CriticalSection(criticalSection)
    {
    }

    T* m_Resource;
    CRITICAL_SECTION* m_CriticalSection;
};
