#include "precompiled_header.h"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;
static const int MAX_ROOM_ITEMS = 2;

class BspListener : public ITCODBspCallback {
private:
    Map& _map; // a map to dig
    int _roomNum; // room number
    int _lastx = -1;
    int _lasty = -1; // center of the last room
public:
    BspListener(Map& map) : _map(map), _roomNum(0) {}

    bool visitNode(TCODBsp* node, void* userData) {
        if (node->isLeaf()) {
            int x, y, w, h;

            // Create room dimensions
            TCODRandom* rng = TCODRandom::getInstance();
            w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
            h = rng->getInt(ROOM_MIN_SIZE, node->h - 2);
            x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
            y = rng->getInt(node->y + 1, node->y + node->h - h - 1);
            _map.CreateRoom(_roomNum, x, y, x + w - 1, y + h - 1);

            // dig a corridor from last room
            if (_roomNum != 0) {
                // horizontal
                _map.Dig(_lastx, _lasty, x + w / 2, _lasty);
                // vertical
                _map.Dig(x + w / 2, _lasty, x + w / 2, y + h / 2);
            }
            _lastx = x + w / 2;
            _lasty = y + h / 2;
            _roomNum++;
        }
        return true;
    }
};

Map::Map(int width, int height) : _width(width), _height(height)
{
    _tiles = new Tile[width * height];
    _map = new TCODMap(width, height);

    // Room tree
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(nullptr, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);

    // Traverse nodes and generate rooms
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, nullptr);
}

Map::~Map()
{
    delete[] _tiles;
    delete _map;
}

bool Map::IsWall(int x, int y) const
{
    return !_map->isWalkable(x, y);
}

bool Map::IsExplored(int x, int y) const
{
    return _tiles[x + y * _width]._explored;
}

void Map::Render() const {
    static const TCODColor maroonWall(102, 0, 51);
    static const TCODColor maroonGround(204, 0, 102);
    static const TCODColor goldWall(125, 100, 45);
    static const TCODColor goldGround(198, 180, 45);

    for (int x = 0; x < _width; x++) {
        for (int y = 0; y < _height; y++) {
            if (IsInFov(x, y)) {
                TCODConsole::root->setCharBackground(x, y,
                    IsWall(x, y) ? goldWall : goldGround);
            }
            else if (IsExplored(x, y)) {
                TCODConsole::root->setCharBackground(x, y,
                    IsWall(x, y) ? maroonWall : maroonGround);
            }
        }
    }
}

bool Map::IsInFov(int x, int y) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height)
    {
        return false;
    }
    if (_map->isInFov(x, y))
    {
        _tiles[x + y * _width]._explored = true;
        return true;
    }
    return false;
}

bool Map::IsInFov(Actor* actor) const
{
    return IsInFov(actor->_coordinates._x, actor->_coordinates._y);
}

void Map::ComputeFov() {
    _map->computeFov(kEngine._player->_coordinates._x, kEngine._player->_coordinates._y, kEngine._fovRadius);
}

bool Map::CanWalk(int x, int y) const
{
    if (IsWall(x, y)) {
        return false;
    }

    for (auto actor : kEngine._actors) {
        // detech collision
        if (actor->_blocks && actor->_coordinates._x == x && actor->_coordinates._y == y) {
            return false;
        }
    }
    return true;
}

void Map::AddMonster(int x, int y)
{
    TCODRandom* rng = TCODRandom::getInstance();
    if (rng->getInt(0, 100) < 80) {
        // create an orc
        Actor* orc = new Actor(x, y, 'O', "orc", TCODColor::desaturatedGreen);
        orc->_destructible = std::make_unique<MonsterDestructible>(10, 0, "orc body", 1);
        orc->_attacker = std::make_unique<Attacker>(3);
        orc->_ai = std::make_unique<MonsterAi>();
        kEngine._actors.push(orc);
    }
    else {
        // create a troll
        Actor* troll = new Actor(x, y, 'T', "troll", TCODColor::darkerGreen);
        troll->_destructible = std::make_unique<MonsterDestructible>(16, 1, "troll carcass", 2);
        troll->_attacker = std::make_unique<Attacker>(4);
        troll->_ai = std::make_unique<MonsterAi>();
        kEngine._actors.push(troll);
    }
}

void Map::AddItem(int x, int y)
{
    TCODRandom* rng = TCODRandom::getInstance();
    int dice = rng->getInt(0, 100);
    if (dice < 50) {
        Actor* healthPotion = new Actor(x, y, '!', "Health Potion", TCODColor::darkerViolet);
        healthPotion->_blocks = false;
        healthPotion->_item = std::make_unique<Healer>(4);
        kEngine._actors.push(healthPotion);
    }
    else if (dice < 75)
    {
        Actor* scrollOfLightningBolt = new Actor(x, y, '#', "Scroll of Thunder", TCODColor::lightYellow);
        scrollOfLightningBolt->_blocks = false;
        scrollOfLightningBolt->_item = std::make_unique<LightningBolt>(5, 20);
        kEngine._actors.push(scrollOfLightningBolt);
    }
    else
    {
        Actor* ringOfWeakness = new Actor(x, y, 'w', "Ring of Weakness", TCODColor::black);
        ringOfWeakness->_blocks = false;
        ringOfWeakness->_item = std::make_unique<RingOfWeakness>(3, 50);
        kEngine._actors.push(ringOfWeakness);
    }
}

void Map::Dig(int x1, int y1, int x2, int y2) 
{
    if (x2 < x1) {
        std::swap(x1, x2);
    }
    if (y2 < y1) {
        std::swap(y1, y2);
    }
    for (int tile_x = x1; tile_x <= x2; tile_x++) {
        for (int tile_y = y1; tile_y <= y2; tile_y++) {
            _map->setProperties(tile_x, tile_y, true, true);
        }
    }
}

void Map::CreateRoom(int room_number, int x1, int y1, int x2, int y2)
{
    Dig(x1, y1, x2, y2);

    if (room_number == 0) {
        // put the player in the first room
        kEngine._player->_coordinates._x = (x1 + x2) / 2;
        kEngine._player->_coordinates._y = (y1 + y2) / 2;
    }
    else {
        // Generate monsters
        TCODRandom* rng = TCODRandom::getInstance();
        int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (nbMonsters > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (CanWalk(x, y)) {
                AddMonster(x, y);
            }
            nbMonsters--;
        }

        // Generate items
        int nbItems = rng->getInt(0, MAX_ROOM_ITEMS);
        while (nbItems > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (CanWalk(x, y)) {
                AddItem(x, y);
            }
            nbItems--;
        }
    }
}