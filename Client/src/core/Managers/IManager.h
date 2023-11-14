#pragma once

class IManager
{
public:

    virtual ~IManager() = 0;

    virtual void Init() = 0;
    virtual void Clear() = 0;
};
