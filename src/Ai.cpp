#include "precompiled_header.h"

static const int INVENTORY_WIDTH = 50;
static const int INVENTORY_HEIGHT = 28;

void PlayerAi::Update(Actor* owner)
{
    if (owner->_destructible && owner->_destructible->IsDead()) return;

    int dx = 0, dy = 0;
    switch (kEngine._lastKey.vk)
    {
    case TCODK_UP: dy = -1; break;
    case TCODK_DOWN: dy = 1; break;
    case TCODK_LEFT: dx = -1; break;
    case TCODK_RIGHT: dx = 1; break;
    case TCODK_CHAR: HandleActionKey(owner, kEngine._lastKey.c); break;
    default: break;
    }

    if (dx != 0 || dy != 0)
    {
        kEngine._gameStatus = Engine::GameStatus::NEW_TURN;
        if (MoveOrAttack(owner, owner->_coordinates._x + dx, owner->_coordinates._y + dy))
        {
            kEngine._map->ComputeFov();
        }
    }
}

void PlayerAi::HandleActionKey(Actor* owner, int ascii) {
    switch (ascii) {
    case 'g': // pickup item
    {
        bool found = false;

        for (Actor** iterator = kEngine._actors.begin(); iterator != kEngine._actors.end(); iterator++)
        {
            Actor* actor = *iterator;
            if (actor->_pickable && actor->_coordinates == owner->_coordinates)
            {
                if (actor->_pickable->Pick(actor, owner))
                {
                    found = true;
                    kEngine._gui->AppendMessage(TCODColor::lightGrey, fmt::format("You pick up the {}.", actor->_name));
                    break;
                }
                else if (!found)
                {
                    found = true;
                    kEngine._gui->AppendMessage(TCODColor::red, "Your inventory is full.");
                }
            }
        }
        if (!found) {
            kEngine._gui->AppendMessage(TCODColor::lightGrey, "There's nothing here that you can pick up.");
        }

        kEngine._gameStatus = Engine::NEW_TURN;
        break;
    }
    case 'i': // display inventory
    {
        Actor* actor = SelectFromInventory(owner);
        if (actor)
        {
            actor->_pickable->Use(actor, owner);
            kEngine._gameStatus = Engine::NEW_TURN;
        }
    }
    break;
    }
}

Actor* PlayerAi::SelectFromInventory(Actor* owner)
{
    static TCODConsole con(INVENTORY_WIDTH, INVENTORY_HEIGHT);

    con.setDefaultForeground(TCODColor(200, 180, 50));
    con.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Inventory");

    // display the items with their keyboard shortcut 
    con.setDefaultForeground(TCODColor::white);
    char shortcut = 'a';
    int y = 1;
    for (auto actor : owner->_container->_inventory)
    {
        auto item_line = fmt::format("({}) {}", shortcut, actor->_name);
        con.print(2, y, item_line.c_str());
        y++;
        shortcut++;
    }

    // blit the inventory console on the root console
    TCODConsole::blit(&con, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, TCODConsole::root,
        kEngine._displayWidth / 2 - INVENTORY_WIDTH / 2, kEngine._displayHeight / 2 - INVENTORY_HEIGHT / 2);
    TCODConsole::flush();

    // wait for a key press
    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

    if (key.vk == TCODK_CHAR)
    {
        int actorIndex = key.c - 'a';
        if (actorIndex >= 0 && actorIndex < owner->_container->_inventory.size())
        {
            return owner->_container->_inventory.get(actorIndex);
        }
    }
    return nullptr;
}

bool PlayerAi::MoveOrAttack(Actor* owner, int targetx, int targety) {
    if (kEngine._map->IsWall(targetx, targety)) return false;

    // Attack
    for (auto actor : kEngine._actors)
    {
        if (actor->_destructible && !actor->_destructible->IsDead()
            && actor->_coordinates._x == targetx && actor->_coordinates._y == targety)
        {
            owner->_attacker->Attack(owner, actor);
            return false;
        }
    }

    // Interactable actors
    for (auto actor : kEngine._actors)
    {
        bool isCorpse = actor->_destructible && actor->_destructible->IsDead();
        bool isItem = actor->_pickable != nullptr;

        if ((isCorpse || isItem) && actor->_coordinates._x == targetx && actor->_coordinates._y == targety)
        {
            kEngine._gui->AppendMessage(TCODColor::darkerSky, fmt::format("There is a {}\n", actor->_name));
        }
    }

    owner->_coordinates._x = targetx;
    owner->_coordinates._y = targety;
    return true;
}

void MonsterAi::Update(Actor* owner) {
    if (owner->_destructible && owner->_destructible->IsDead()) return;

    if (kEngine._map->IsInFov(owner->_coordinates._x, owner->_coordinates._y))
    {
        // we can see the player. move towards him
        MoveOrAttack(owner, kEngine._player->_coordinates._x, kEngine._player->_coordinates._y);
    }
}
void MonsterAi::MoveOrAttack(Actor* owner, int targetx, int targety) {
    int dx = targetx - owner->_coordinates._x;
    int dy = targety - owner->_coordinates._y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance >= 2)
    {
        dx = (int)(round(dx / distance));
        dy = (int)(round(dy / distance));

        if (kEngine._map->CanWalk(owner->_coordinates._x + dx, owner->_coordinates._y + dy)) {
            owner->_coordinates._x += dx;
            owner->_coordinates._y += dy;
        }
    }
    else if (owner->_attacker)
    {
        owner->_attacker->Attack(owner, kEngine._player);
    }
}