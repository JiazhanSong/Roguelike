#pragma once
#include "precompiled_header.h"

class Actor
{
    static const int default_x = 40;
    static const int default_y = 25;

    class Coordinates
    {
        friend Actor;
        Coordinates() {
            _x = 0;
            _y = 0;
        }

        Coordinates(int x, int y)
        {
            _x = x;
            _y = y;
        }

        int _x;
        int _y;

    public:
        int GetX() { return _x; }
        int GetY() { return _y; }
    };

public:
    Actor(int x, int y, int ch, const TCODColor& color)
    {
        _coordinates = Coordinates(x, y);
        _ch = ch;
        _color = color;
    }

    Actor(int ch, const TCODColor& color) : Actor(default_x, default_y, ch, color) {}

    Actor()
    {
        _coordinates = Coordinates(default_x, default_y);
    }

    void Move(TCOD_keycode_t direction, unsigned int delta);
    void Render() const;

    Coordinates _coordinates;
    int _ch; // ascii code
    TCODColor _color;
};