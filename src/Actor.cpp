#include "precompiled_header.h"

Actor* Actor::InitPlayer()
{
    auto player = new Actor(40, 25, '@', "Player", TCODColor::white);
    player->_destructible = std::make_unique<PlayerDestructible>(30, 2, "your lifeless cadaver");
    player->_attacker = std::make_unique<Attacker>(5);
    player->_ai = std::make_unique<PlayerAi>();
    player->_container = std::make_unique<Container>(26);
    player->_experience = std::make_unique<PlayerExperience>(player);
    return player;
}

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

void Actor::Render() const
{
    TCODConsole::root->setChar(_coordinates._x, _coordinates._y, _ch);
    TCODConsole::root->setCharForeground(_coordinates._x, _coordinates._y, _color);
}

void Actor::Update()
{
    if (_ai) _ai->Update(this);
}

bool Actor::MoveOrAttack(int x_delta, int y_delta)
{
    int x = _coordinates._x + x_delta;
    int y = _coordinates._y + y_delta;

    if (kEngine._map->IsWall(x, y)) return false;

    for (auto actor : kEngine._actors)
    {
        if (actor->_coordinates._x == x && actor->_coordinates._y == y)
        {
            kEngine._gui->AppendMessage(TCODColor::amber, fmt::format("The {} blocks your path!\n", actor->_name));
            return false;
        }
    }

    this->_coordinates._x = x;
    this->_coordinates._y = y;
    return true;
}

float Actor::GetDistance(int cx, int cy) const {
    int dx = _coordinates._x - cx;
    int dy = _coordinates._y - cy;
    return sqrt(dx * dx + dy * dy);
}