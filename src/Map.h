#pragma once
#include "precompiled_header.h"

class Actor;

struct Tile {
    Tile() : _explored(false) {}
    bool _explored; // has the player already seen this tile
};

class Map {
public:
    int _width, _height;

    Map(int width, int height);
    ~Map();
    bool IsWall(int x, int y) const;
    bool IsInFov(int x, int y) const;
    bool IsInFov(Actor* actor) const;
    bool IsExplored(int x, int y) const;
    void ComputeFov();
    void Render() const;
    bool CanWalk(int x, int y) const;
    void AddMonster(int x, int y);
protected:
    TCODMap* _map;
    Tile* _tiles;
    friend class BspListener;

    void Dig(int x1, int y1, int x2, int y2);
    void CreateRoom(int room_number, int x1, int y1, int x2, int y2);
};