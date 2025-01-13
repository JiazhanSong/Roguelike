#pragma once

class Engine {
public:
    enum GameStatus {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    };

    GameStatus _gameStatus;

    TCODList<Actor*> _actors;
    Actor* _player;

    std::unique_ptr<Map> _map;
    std::unique_ptr<Gui> _gui;

    TCOD_mouse_t _mouse;
    TCOD_key_t _lastKey;

    int _fovRadius;
    int _displayWidth;
    int _displayHeight;

    Engine(int _displayWidth, int _displayHeight);
    ~Engine();
    void Update();
    void Render();
    void SendToBack(Actor* actor);
    Actor* GetClosestMonster(int x, int y, float range) const;
};

extern Engine kEngine;