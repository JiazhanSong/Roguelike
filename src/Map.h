#pragma once
#include "precompiled_header.h"

struct Tile {
    bool _canWalk; // can an Actor walk through this tile
    Tile() : _canWalk(false) {}
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
    friend class BspListener;

    void SetWall(int x, int y);
    void Dig(int x1, int y1, int x2, int y2);
    void CreateRoom(int room_number, int x1, int y1, int x2, int y2);
};