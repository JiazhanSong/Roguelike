#include "precompiled_header.h"

void PlayerAi::Update(Actor* owner) {
    if (owner->_destructible && owner->_destructible->IsDead()) return;

    int dx = 0, dy = 0;
    switch (kEngine._lastKey.vk)
    {
    case TCODK_UP: dy = -1; break;
    case TCODK_DOWN: dy = 1; break;
    case TCODK_LEFT: dx = -1; break;
    case TCODK_RIGHT: dx = 1; break;
    default: break;
    }

    if (dx != 0 || dy != 0)
    {
        kEngine._gameStatus = Engine::GameStatus::NEW_TURN;
        if (MoveOrAttack(owner, owner->_coordinates._x + dx, owner->_coordinates._y + dy))
        {
            kEngine._map->ComputeFov();
        }
    }
}

bool PlayerAi::MoveOrAttack(Actor* owner, int targetx, int targety) {
    if (kEngine._map->IsWall(targetx, targety)) return false;

    // Attack
    for (auto actor : kEngine._actors)
    {
        if (actor->_destructible && !actor->_destructible->IsDead()
            && actor->_coordinates._x == targetx && actor->_coordinates._y == targety)
        {
            owner->_attacker->Attack(owner, actor);
            return false;
        }
    }

    // Corpses
    for (auto actor : kEngine._actors)
    {
        if (actor->_destructible && actor->_destructible->IsDead()
            && actor->_coordinates._x == targetx && actor->_coordinates._y == targety)
        {
            kEngine._gui->AppendMessage(TCODColor::darkerSky, fmt::format("There is a {}\n", actor->_name));
        }
    }

    owner->_coordinates._x = targetx;
    owner->_coordinates._y = targety;
    return true;
}

void MonsterAi::Update(Actor* owner) {
    if (owner->_destructible && owner->_destructible->IsDead()) return;

    if (kEngine._map->IsInFov(owner->_coordinates._x, owner->_coordinates._y))
    {
        // we can see the player. move towards him
        MoveOrAttack(owner, kEngine._player->_coordinates._x, kEngine._player->_coordinates._y);
    }
}
void MonsterAi::MoveOrAttack(Actor* owner, int targetx, int targety) {
    int dx = targetx - owner->_coordinates._x;
    int dy = targety - owner->_coordinates._y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance >= 2)
    {
        dx = (int)(round(dx / distance));
        dy = (int)(round(dy / distance));

        if (kEngine._map->CanWalk(owner->_coordinates._x + dx, owner->_coordinates._y + dy)) {
            owner->_coordinates._x += dx;
            owner->_coordinates._y += dy;
        }
    }
    else if (owner->_attacker)
    {
        owner->_attacker->Attack(owner, kEngine._player);
    }
}