#include "Actor.h"
#include "Engine.h"
#include "Map.h"
#include <stdio.h>

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

void Actor::Update() {
    printf("The %s growls!\n", _name.c_str());
}

bool Actor::MoveOrAttack(int x_delta, int y_delta) {
    int x = _coordinates._x + x_delta;
    int y = _coordinates._y + y_delta;

    if (kEngine._map->IsWall(x, y)) return false;

    for (auto actor : kEngine._actors)
    {
        if (actor->_coordinates._x == x && actor->_coordinates._y == y)
        {
            printf("The %s blocks your path!\n", actor->_name.c_str());
            return false;
        }
    }

    this->_coordinates._x = x;
    this->_coordinates._y = y;
    return true;
}