#include "Map.h"

Map::Map(int width, int height) : _width(width), _height(height) {
    _tiles = new Tile[width * height];
    SetWall(30, 22);
    SetWall(50, 22);
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