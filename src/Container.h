#pragma once

class Container {
public:
    int _size; // max size
    TCODList<Actor*> _inventory;

    Container(int size);
    ~Container();
    bool Add(Actor* actor);
    void Remove(Actor* actor);
};