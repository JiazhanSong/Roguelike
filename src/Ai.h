#pragma once
class Actor;

class Ai {
public:
    virtual void Update(Actor* owner) = 0;
    virtual ~Ai() {};
};

class PlayerAi : public Ai {
public:
    void Update(Actor* owner);

protected:
    void HandleActionKey(Actor* owner, int ascii);
    Actor* SelectFromInventory(Actor* owner);
    bool MoveOrAttack(Actor* owner, int targetx, int targety);
};

class MonsterAi : public Ai {
public:
    void Update(Actor* owner);

protected:
    void MoveOrAttack(Actor* owner, int targetx, int targety);
};