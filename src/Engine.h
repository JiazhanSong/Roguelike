#pragma once
#include "precompiled_header.h"

class Actor;
class Map;

class Engine {
public:
    enum GameStatus {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    };

    TCODList<Actor*> _actors;
    Actor* _player;
    Map* _map;
    int _fovRadius;
    GameStatus _gameStatus;

    Engine();
    ~Engine();
    void Update();
    void Render();
};

extern Engine kEngine;