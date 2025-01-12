#include "precompiled_header.h"

bool Pickable::Pick(Actor* owner, Actor* wearer)
{
    if (wearer->_container && wearer->_container->Add(owner))
    {
        kEngine._actors.remove(owner);
        return true;
    }
    return false;
}

bool Pickable::Use(Actor* owner, Actor* wearer)
{
    if (wearer->_container)
    {
        wearer->_container->Remove(owner);
        delete owner;
        return true;
    }
    return false;
}

Healer::Healer(float amount) : _amount(amount)
{
}

bool Healer::Use(Actor* owner, Actor* wearer)
{
    if (wearer->_destructible && wearer->_destructible->Heal(_amount) > 0)
    {
        return Pickable::Use(owner, wearer);
    }
    return false;
}