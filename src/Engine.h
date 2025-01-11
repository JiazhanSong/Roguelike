#pragma once
#include "precompiled_header.h"

class Actor;
class Map;

class Engine {
public:
    TCODList<Actor*> _actors;
    Actor* _player;
    Map* _map;

    Engine();
    ~Engine();
    void Update();
    void Render();
};

extern Engine kEngine;