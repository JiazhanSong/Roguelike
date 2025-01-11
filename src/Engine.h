#pragma once
#include "precompiled_header.h"

class Actor;
class Map;

class Engine {
public:
    TCODList<Actor*> _actors;
    Actor* _player;
    Map* _map;
    int _fovRadius;

    Engine();
    ~Engine();
    void Update();
    void Render();

private:
    bool _computeFov;
};

extern Engine kEngine;