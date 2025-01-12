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