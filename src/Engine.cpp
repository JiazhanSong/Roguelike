#include "precompiled_header.h"

static const int kDisplayWidth = 80;
static const int kDisplayHeight = 50;

// Global engine
Engine kEngine(kDisplayWidth, kDisplayHeight);

Engine::Engine(int _displayWidth, int _displayHeight) : _fovRadius(10), _gameStatus(GameStatus::STARTUP),
  _displayWidth(_displayWidth), _displayHeight(_displayHeight)
{
    TCODConsole::initRoot(_displayWidth, _displayHeight, "Roguelike!", false);
    Init();
}

Engine::~Engine()
{
    _actors.clearAndDelete();
}

void Engine::Init()
{
    _player = Actor::InitPlayer();
    _actors.push(_player);

    _map = std::make_unique<Map>(80, 45);
    _map->ComputeFov();

    _gui = std::make_unique<Gui>();
    _gui->AppendMessage(TCODColor::gold, "Behold adventurer, the Pyramid of Ashkan!");
}

void Engine::Update()
{
    if (_gameStatus == STARTUP) _map->ComputeFov();
    if (_gameStatus != GameStatus::VICTORY) _gameStatus = GameStatus::IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &_lastKey, &_mouse);

    if (_lastKey.vk == TCODK_ESCAPE)
    {
        kEngine._gameStatus = GameStatus::EXIT;
        return;
    }

    _player->Update();

    if (_gameStatus == NEW_TURN)
    {
        bool victorious = true;
        for (auto actor : _actors)
        {
            if (actor != _player)
            {
                actor->Update();
                if (actor->_destructible && !actor->_destructible->IsDead())
                {
                    victorious = false;
                }
            }
        }

        if (victorious) kEngine._gameStatus = GameStatus::VICTORY;
    }
}

void Engine::Render()
{
    TCODConsole::root->clear();

    _map->Render();

    for (auto actor : _actors)
    {
        if (_map->IsInFov(actor->_coordinates._x, actor->_coordinates._y))
        {
            actor->Render();
        }
    }

    if (kEngine._gameStatus == Engine::GameStatus::VICTORY)
    {
        _gui->RenderVictory();
    }
    else
    {
        _gui->Render();
    }
}

void Engine::SendToBack(Actor* actor)
{
    _actors.remove(actor);
    _actors.insertBefore(actor, 0);
}

Actor* Engine::GetClosestMonster(int x, int y, float range) const
{
    Actor* closest = nullptr;
    float bestDistance = std::numeric_limits<float>::max();

    for (auto actor : _actors)
    {
        if (actor != _player && actor->_destructible && !actor->_destructible->IsDead())
        {
            float distance = actor->GetDistance(x, y);
            if (distance < bestDistance && (distance <= range || range == 0.0f))
            {
                bestDistance = distance;
                closest = actor;
            }
        }
    }
    return closest;
}