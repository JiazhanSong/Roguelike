#include "Map.h"
#include "Engine.h"
#include "Actor.h"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;

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

Map::Map(int width, int height) : _width(width), _height(height) {
    _tiles = new Tile[width * height];

    // Room tree
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);

    // Traverse nodes and generate rooms
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map() {
    delete[] _tiles;
}

bool Map::IsWall(int x, int y) const {
    return !_tiles[x + y * _width]._canWalk;
}

void Map::SetWall(int x, int y) {
    _tiles[x + y * _width]._canWalk = false;
}

void Map::Render() const {
    static const TCODColor darkWall(0, 0, 100);
    static const TCODColor darkGround(50, 50, 150);

    for (int x = 0; x < _width; x++) {
        for (int y = 0; y < _height; y++) {
            TCODConsole::root->setCharBackground(x, y,
                IsWall(x, y) ? darkWall : darkGround);
        }
    }
}

void Map::Dig(int x1, int y1, int x2, int y2) {
    if (x2 < x1) {
        std::swap(x1, x2);
    }
    if (y2 < y1) {
        std::swap(y1, y2);
    }
    for (int tile_x = x1; tile_x <= x2; tile_x++) {
        for (int tile_y = y1; tile_y <= y2; tile_y++) {
            _tiles[tile_x + tile_y * _width]._canWalk = true;
        }
    }
}

void Map::CreateRoom(int room_number, int x1, int y1, int x2, int y2) {
    Dig(x1, y1, x2, y2);

    if (room_number == 0) {
        // put the player in the first room
        kEngine._player->_coordinates._x = (x1 + x2) / 2;
        kEngine._player->_coordinates._y = (y1 + y2) / 2;
    }
    else {
        TCODRandom* rng = TCODRandom::getInstance();
        if (rng->getInt(0, 3) == 0) {
            kEngine._actors.push(new Actor((x1 + x2) / 2, (y1 + y2) / 2, '@',
                TCODColor::yellow));
        }
    }
}