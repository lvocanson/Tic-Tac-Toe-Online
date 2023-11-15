#pragma once
class IDGenerator
{
public:
    static int  GenerateID()
    {
        static int id = 1;
        return id++;
    }
};

