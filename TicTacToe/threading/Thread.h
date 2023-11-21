#pragma once
#include <WinBase.h>

class Thread final
{
public:
    /// <summary>
    /// Create a new thread.
    /// </summary>
    /// <param name="start">The function to start the thread with.</param>
    /// <param name="param">The parameter to pass to the thread function.</param>
    /// <param name="startImmediately">If false, the thread will be paused until Start() is called.</param>
    /// <returns>The created thread, or nullptr if creation failed.</returns>
    template <typename Param>
    static Thread* Create(LPTHREAD_START_ROUTINE start, Param* param, bool startImmediately)
    {
        LPVOID paramAlloc = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Param));
        if (paramAlloc == nullptr)
        {
            // Allocation failed
            return nullptr;
        }
        // Copy the parameter
        memcpy(paramAlloc, (const void*)param, sizeof(Param));

        DWORD threadID = 0;
        HANDLE thread = CreateThread(
            nullptr,
            0,
            start,
            paramAlloc,
            startImmediately ? 0 : CREATE_SUSPENDED,
            &threadID);

        if (thread == nullptr)
        {
            // Creation failed, free the memory
            HeapFree(GetProcessHeap(), 0, paramAlloc);
            return nullptr;
        }

        return new Thread(thread, threadID);
    }

    static Thread* Create(LPTHREAD_START_ROUTINE start, bool startImmediately)
    {
        DWORD threadID = 0;
        HANDLE thread = CreateThread(
            nullptr,
            0,
            start,
            nullptr,
            startImmediately ? 0 : CREATE_SUSPENDED,
            &threadID);

        if (thread == nullptr)
        {
            // Creation failed
            return nullptr;
        }

        return new Thread(thread, threadID);
    }

public:
    ~Thread()
    {
        TerminateThread(m_Thread, 0);
        CloseHandle(m_Thread);
    }
    /// <summary>
    /// Start (or resume) the thread.
    /// </summary>
    void Start()
    {
        ResumeThread(m_Thread);
    }
    /// <summary>
    /// Pause the thread.
    /// </summary>
    void Pause()
    {
        SuspendThread(m_Thread);
    }
    /// <summary>
    /// Wait for the thread to finish.
    /// </summary>
    void Wait(DWORD timeout = INFINITE) const
    {
        WaitForSingleObject(m_Thread, timeout);
    }
    /// <summary>
    /// Check if the thread is running.
    /// </summary>
    bool IsRunning() const
    {
        return WaitForSingleObject(m_Thread, 0) == WAIT_TIMEOUT;
    }
    /// <summary>
    /// Returns the thread ID.
    /// </summary>
    DWORD GetID() const
    {
        return m_ThreadID;
    }

private:
    friend class ThreadCreator;
    Thread(HANDLE thread, DWORD threadID)
        : m_Thread(thread), m_ThreadID(threadID)
    {
    }

private:
    HANDLE m_Thread = nullptr;
    DWORD m_ThreadID = 0;
};