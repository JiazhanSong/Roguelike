#pragma once
class Actor;

//
class Attacker {
public:
    float _power;

    Attacker(float power);
    void Attack(Actor* owner, Actor* target);
};