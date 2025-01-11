#include "Actor.h"
#include "Map.h"
#include "Engine.h"

// Global engine
Engine kEngine;

Engine::Engine() : _fovRadius(10), _gameStatus(GameStatus::STARTUP) {
    TCODConsole::initRoot(80, 50, "Roguelike!", false);
    _player = new Actor(40, 25, '@', "player", TCODColor::white);
    _actors.push(_player);
    _map = new Map(80, 45);
    _map->ComputeFov();
}

Engine::~Engine() {
    _actors.clearAndDelete();
    delete _map;
}

void Engine::Update() {
    TCOD_key_t key;
    if (_gameStatus == STARTUP) _map->ComputeFov();
    _gameStatus = IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

    int dx = 0, dy = 0;
    switch (key.vk) {
        case TCODK_UP: dy = -1; break;
        case TCODK_DOWN: dy = 1; break;
        case TCODK_LEFT: dx = -1; break;
        case TCODK_RIGHT: dx = 1; break;
        default:break;
    }

    if (dx != 0 || dy != 0) {
        _gameStatus = NEW_TURN;
        if (_player->MoveOrAttack(dx, dy)) {
            _map->ComputeFov();
        }
    }

    if (_gameStatus == NEW_TURN) {
        for (auto actor : _actors) {
            if (actor != _player && _map->IsInFov(actor)) {
                actor->Update();
            }
        }
    }
}

void Engine::Render() {
    TCODConsole::root->clear();

    // Draw map
    _map->Render();

    for (auto actor : _actors) {
        if (_map->IsInFov(actor->_coordinates._x, actor->_coordinates._y)) {
            actor->Render();
        }
    }
}