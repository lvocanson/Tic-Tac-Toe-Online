#pragma once

class IManager
{
public:

    virtual ~IManager() {};

    virtual void Init() = 0;
    virtual void Clear() = 0;
};
