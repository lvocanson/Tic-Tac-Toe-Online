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
        : m_Resource(), m_CriticalSection(new CRITICAL_SECTION)
    {
        InitializeCriticalSection(m_CriticalSection);
    }
    Shared(const T& resource)
        : m_Resource(resource), m_CriticalSection(new CRITICAL_SECTION)
    {
        InitializeCriticalSection(m_CriticalSection);
    }
    ~Shared() noexcept(false)
    {
        DeleteCriticalSection(m_CriticalSection);
        delete m_CriticalSection;
    }
    /// <summary>
    /// Try to get the resource.
    /// </summary>
    Lock<T> TryGet()
    {
        if (TryEnterCriticalSection(m_CriticalSection))
        {
            return Lock(&m_Resource, &m_CriticalSection);
        }
        return Lock<T>();
    }
    /// <summary>
    /// Wait until the resource is available and get it.
    /// </summary>
    Lock<T> WaitGet()
    {
        EnterCriticalSection(m_CriticalSection);
        return Lock(&m_Resource, &m_CriticalSection);
    }

private:

    T m_Resource;
    LPCRITICAL_SECTION m_CriticalSection;
};

/// <summary>
/// A lock on a resource. The resource is unlocked when the lock goes out of scope.
/// </summary>
template <typename T>
class Lock final
{
public:
    ~Lock()
    {
        if (m_Resource != nullptr)
        {
            LeaveCriticalSection(*m_CriticalSection);
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
    T& Get()
    {
        return *m_Resource;
    }
    /// <summary>
    /// Get the resource.
    /// </summary>
    T* operator->()
    {
        return m_Resource;
    }

private:
    Lock(const Lock&) = delete;
    Lock& operator=(const Lock&) = delete;

    friend class Shared<T>;
    Lock() : m_Resource(nullptr), m_CriticalSection(nullptr)
    {
    }
    Lock(T* resource, LPCRITICAL_SECTION* criticalSection)
        : m_Resource(resource), m_CriticalSection(criticalSection)
    {
    }

    T* m_Resource;
    LPCRITICAL_SECTION* m_CriticalSection;
};
