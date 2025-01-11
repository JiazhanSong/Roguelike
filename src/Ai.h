#pragma once
class Actor;

class Ai {
public:
    virtual void Update(Actor* owner) = 0;
};

class PlayerAi : public Ai {
public:
    void Update(Actor* owner);

protected:
    bool MoveOrAttack(Actor* owner, int targetx, int targety);
};

class MonsterAi : public Ai {
public:
    void Update(Actor* owner);

protected:
    void MoveOrAttack(Actor* owner, int targetx, int targety);
};