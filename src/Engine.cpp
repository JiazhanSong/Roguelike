#include "precompiled_header.h"

// Global engine
Engine kEngine(kDisplayWidth, kDisplayHeight);

Engine::Engine(int _displayWidth, int _displayHeight) : _fovRadius(10), _gameStatus(GameStatus::STARTUP),
  _displayWidth(_displayWidth), _displayHeight(_displayHeight)
{
    TCODConsole::initRoot(_displayWidth, _displayHeight, "Roguelike!", false);
    _player = new Actor(40, 25, '@', "player", TCODColor::white);
    _player->_destructible = std::make_unique<PlayerDestructible>(30, 2, "your cadaver");
    _player->_attacker = std::make_unique<Attacker>(5);
    _player->_ai = std::make_unique<PlayerAi>();
    _actors.push(_player);
    _map = std::make_unique<Map>(80, 45);
    _map->ComputeFov();
    _gui = std::make_unique<Gui>();

    _gui->AppendMessage(TCODColor::gold,
        "Behold adventurer, the Pyramid of Ashkan!");
}

Engine::~Engine()
{
    _actors.clearAndDelete();
}

void Engine::Update()
{
    if (_gameStatus == STARTUP) _map->ComputeFov();
    _gameStatus = IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &_lastKey, &_mouse);

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

    _map->Render();

    for (auto actor : _actors) {
        if (_map->IsInFov(actor->_coordinates._x, actor->_coordinates._y)) {
            actor->Render();
        }
    }

    _gui->Render();
}

void Engine::SendToBack(Actor* actor)
{
    _actors.remove(actor);
    _actors.insertBefore(actor, 0);
}