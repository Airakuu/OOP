#pragma once

class figure
{
public:
    virtual void draw() = 0;
    virtual void erase() = 0;
    virtual ~figure() = default;
};
