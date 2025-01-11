#include "Actor.h"
#include "Map.h"
#include "Engine.h"

// Global engine
Engine kEngine;

Engine::Engine() : _fovRadius(10), _computeFov(true) {
    TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
    _player = new Actor(40, 25, '@', TCODColor::white);
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
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

    auto& coordinates = _player->_coordinates;
    switch (key.vk) {
    case TCODK_UP:
        if (!_map->IsWall(coordinates._x, coordinates._y - 1)) {
            _player->Move(TCODK_UP, 1);
        }
        break;
    case TCODK_DOWN:
        if (!_map->IsWall(coordinates._x, coordinates._y + 1)) {
            _player->Move(TCODK_DOWN, 1);
        }
        break;
    case TCODK_LEFT:
        if (!_map->IsWall(coordinates._x - 1, coordinates._y)) {
            _player->Move(TCODK_LEFT, 1);
        }
        break;
    case TCODK_RIGHT:
        if (!_map->IsWall(coordinates._x + 1, coordinates._y)) {
            _player->Move(TCODK_RIGHT, 1);
        }
        break;
    default:
        _computeFov = true;
        break;
    }

    if (_computeFov) {
        _map->ComputeFov();
        _computeFov = false;
    }
}

void Engine::Render() {
    TCODConsole::root->clear();
    // draw the map
    _map->Render();
    // draw the actors
    for (auto actor : _actors) {
        if (_map->IsInFov(actor->_coordinates._x, actor->_coordinates._y)) {
            actor->Render();
        }
    }
}