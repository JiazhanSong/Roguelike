#include "Actor.h"

void Actor::Move(TCOD_keycode_t direction, unsigned int delta)
{
    switch (direction)
    {
    case TCODK_UP: _coordinates._y -= delta; break;
    case TCODK_DOWN: _coordinates._y += delta; break;
    case TCODK_LEFT: _coordinates._x -= delta; break;
    case TCODK_RIGHT: _coordinates._x += delta; break;
    default: break;
    }
}

void Actor::Render() const {
    TCODConsole::root->setChar(_coordinates._x, _coordinates._y, _ch);
    TCODConsole::root->setCharForeground(_coordinates._x, _coordinates._y, _color);
}