#include "precompiled_header.h"

Attacker::Attacker(float power) : _power(power) {}

void Attacker::Attack(Actor* owner, Actor* target)
{
    if (target->_destructible && !target->_destructible->IsDead())
    {
        if (_power - target->_destructible->_defense > 0)
        {
            fmt::print("{} attacks {} for {} hit-points.\n", owner->_name, target->_name,
                _power - target->_destructible->_defense);
        }
        else
        {
            fmt::print("{} attacks {} but its defense is strong!\n", owner->_name, target->_name);
        }
        target->_destructible->TakeDamage(target, _power);
    }
    else
    {
        fmt::print("{} attacks {} in vain...\n", owner->_name, target->_name);
    }
}