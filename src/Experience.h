#pragma once
class Actor;

class Experience {
public:
    std::vector<int> _intervals; // do not resize
    std::vector<int>::iterator _it;
    int _exp = 0;
    Actor* _owner;

    virtual ~Experience() {};

    int GetLevel();
    int GetNextExperienceThreshold();
    virtual void Gain(int experience) = 0;
};

class PlayerExperience : public Experience {
public:
    PlayerExperience(Actor* owner);
    void Gain(int experience);
};