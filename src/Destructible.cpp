#include "precompiled_header.h"

Destructible::Destructible(float maxHp, float defense, const char* corpseName) :
    _maxHp(maxHp), _hp(maxHp), _defense(defense), _corpseName(corpseName)
{
}

float Destructible::TakeDamage(Actor* owner, float damage)
{
    float calculated_damage = std::max(damage - _defense, 0.f);

    _hp -= calculated_damage;
    if (_hp <= 0) {
        Die(owner);
    }

    return calculated_damage;
}

void Destructible::Die(Actor* owner)
{
    owner->_ch = '%';
    owner->_color = TCODColor::darkRed;
    owner->_name = _corpseName;
    owner->_blocks = false;

    // Send to back to be rendered first
    kEngine.SendToBack(owner);
}

void MonsterDestructible::Die(Actor* owner)
{
    kEngine._gui->AppendMessage(TCODColor::red,
        fmt::format("{} died a gruesome and painful death.\n", owner->_name));
    Destructible::Die(owner);
}

void PlayerDestructible::Die(Actor* owner)
{
    kEngine._gui->AppendMessage(TCODColor::darkRed,
        fmt::format("You died!\n"));
    Destructible::Die(owner);
    kEngine._gameStatus = Engine::DEFEAT;
}

float Destructible::Heal(float amount) {
    _hp = _hp + amount;
    auto amount_healed = _hp > _maxHp ? (amount - (_hp - _maxHp)) : amount;
    _hp = std::min(_maxHp, _hp);
    return amount_healed;
}