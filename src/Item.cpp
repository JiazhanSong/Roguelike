#include "precompiled_header.h"

bool Item::Pick(Actor* owner, Actor* wearer)
{
    if (wearer->_container && wearer->_container->Add(owner))
    {
        kEngine._actors.remove(owner);
        return true;
    }
    return false;
}

bool Item::Use(Actor* owner, Actor* wearer)
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
        return Item::Use(owner, wearer);
    }
    return false;
}

LightningBolt::LightningBolt(float range, float damage) : _range(range), _damage(damage)
{
}

bool LightningBolt::Use(Actor* owner, Actor* wearer)
{
    Actor* closestMonster = kEngine.GetClosestMonster(wearer->_coordinates._x, wearer->_coordinates._y, _range);
    if (!closestMonster) {
        kEngine._gui->AppendMessage(TCODColor::lightGrey, "No enemy is close enough to strike.");
        return false;
    }

    // hit closest monster for <damage> hit points
    kEngine._gui->AppendMessage(TCODColor::lightBlue, fmt::format(
        "A lighting bolt strikes the {} with a loud thunder!\nThe damage is {} hit points.",
        closestMonster->_name, _damage));
    closestMonster->_destructible->TakeDamage(closestMonster, _damage);

    return Item::Use(owner, wearer);
}

RingOfWeakness::RingOfWeakness(float range, float percentageDamage) : _range(range), _percentageDamage(percentageDamage)
{
}

bool RingOfWeakness::Use(Actor* owner, Actor* wearer)
{
    Actor* closestMonster = kEngine.GetClosestMonster(wearer->_coordinates._x, wearer->_coordinates._y, _range);
    if (!closestMonster) {
        kEngine._gui->AppendMessage(TCODColor::lightGrey, "No enemy is close enough to strike.");
        return false;
    }

    // hit closest monster for <damage> hit points
    auto monsterName = closestMonster->_name;
    auto damage = closestMonster->_destructible->TakePercentageDamage(closestMonster, _percentageDamage);

    kEngine._gui->AppendMessage(TCODColor::lightBlue, fmt::format(
        "The Ring of Weakness cripples {}!\nThe damage is {} hit points.",
        monsterName, damage));

    return Item::Use(owner, wearer);
}