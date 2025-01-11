#pragma once
#include "precompiled_header.h"

struct Tile {
    bool _canWalk; // can we walk through this tile?
    Tile() : _canWalk(true) {}
};

class Map {
public:
    int _width, _height;

    Map(int width, int height);
    ~Map();
    bool IsWall(int x, int y) const;
    void Render() const;
protected:
    Tile* _tiles;

    void SetWall(int x, int y);
};