#include "precompiled_header.h"

// Global engine
Engine kEngine(kDisplayWidth, kDisplayHeight);

Engine::Engine(int _displayWidth, int _displayHeight) : _fovRadius(10), _gameStatus(GameStatus::STARTUP),
  _displayWidth(_displayWidth), _displayHeight(_displayHeight)
{
    TCODConsole::initRoot(_displayWidth, _displayHeight, "Roguelike!", false);
    _player = new Actor(40, 25, '@', "player", TCODColor::white);
    _player->_destructible = std::make_unique<Destructible>(30, 2, "your cadaver");
    _player->_attacker = std::make_unique<Attacker>(5);
    _player->_ai = std::make_unique<PlayerAi>();
    _actors.push(_player);
    _map = new Map(80, 45);
    _map->ComputeFov();
}

Engine::~Engine()
{
    _actors.clearAndDelete();
    delete _map;
}

void Engine::Update()
{
    if (_gameStatus == STARTUP) _map->ComputeFov();
    _gameStatus = IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &_lastKey, NULL);

    _player->Update();

    if (_gameStatus == NEW_TURN) {
        for (auto actor : _actors) {
            if (actor != _player) {
                actor->Update();
            }
        }
    }
}

void Engine::Render()
{
    TCODConsole::root->clear();

    // Draw map
    _map->Render();

    for (auto actor : _actors) {
        if (_map->IsInFov(actor->_coordinates._x, actor->_coordinates._y)) {
            actor->Render();
        }
    }

    // show the player's stats
    TCODConsole::root->print(1, _displayHeight - 2, "HP : %d/%d",
        (int)_player->_destructible->_hp, (int)_player->_destructible->_maxHp);
}

void Engine::SendToBack(Actor* actor)
{
    _actors.remove(actor);
    _actors.insertBefore(actor, 0);
}