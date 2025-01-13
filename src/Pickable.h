#pragma once

class Pickable {
public:
    virtual ~Pickable() {};

    // Requires non-nullptr owner
    bool Pick(Actor* owner, Actor* wearer);
    virtual bool Use(Actor* owner, Actor* wearer);
};

class Healer : public Pickable {
public:
    float _amount;

    Healer(float amount);
    bool Use(Actor* owner, Actor* wearer);
};

class LightningBolt : public Pickable {
public:
    float _range, _damage;
    LightningBolt(float range, float damage);
    bool Use(Actor* owner, Actor* wearer);
};

class RingOfWeakness : public Pickable {
public:
    float _range, _percentageDamage;
    RingOfWeakness(float range, float percentage_damage);
    bool Use(Actor* owner, Actor* wearer);
};