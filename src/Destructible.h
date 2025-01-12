#pragma once
class Actor;

class Destructible {
public:
    float _maxHp;
    float _hp; // current health points
    float _defense; // damage negation
    std::string _corpseName; // the actor's name once dead

    Destructible(float maxHp, float defense, const char* corpseName);
    virtual ~Destructible() {};
    bool IsDead() { return _hp <= 0; }
    float TakeDamage(Actor* owner, float damage);
    virtual void Die(Actor* owner);
    float Heal(float amount);
};

class MonsterDestructible : public Destructible {
public:
    MonsterDestructible(float maxHp, float defense, const char* corpseName) :
        Destructible(maxHp, defense, corpseName)
    {
    }

    void Die(Actor* owner);
};

class PlayerDestructible : public Destructible {
public:
    PlayerDestructible(float maxHp, float defense, const char* corpseName) :
        Destructible(maxHp, defense, corpseName)
    {
    }

    void Die(Actor* owner);
};