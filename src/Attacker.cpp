#include "precompiled_header.h"

Attacker::Attacker(float power) : _power(power) {}

void Attacker::Attack(Actor* owner, Actor* target)
{
    if (target->_destructible && !target->_destructible->IsDead())
    {
        if (_power - target->_destructible->_defense > 0)
        {
            auto color = owner->_name != "player" ? TCODColor::darkerRed : TCODColor::blue;
            kEngine._gui->AppendMessage(color, fmt::format("{} attacks {} for {} hit-points.\n",
                owner->_name, target->_name, _power - target->_destructible->_defense));
        }
        else
        {
            kEngine._gui->AppendMessage(TCODColor::lightGrey,
                fmt::format("{} attacks {} but its defense is strong!\n", owner->_name, target->_name));
        }
        target->_destructible->TakeDamage(target, _power);
    }
    else
    {
        kEngine._gui->AppendMessage(TCODColor::lightGrey,
            fmt::format("{} attacks {} in vain...\n", owner->_name, target->_name));
    }
}