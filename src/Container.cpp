#include "precompiled_header.h"

Container::Container(int size) : _size(size)
{
}

Container::~Container()
{
    _inventory.clearAndDelete();
}

bool Container::Add(Actor* actor)
{
    if (_size > 0 && _inventory.size() >= _size) {
        // inventory full
        return false;
    }

    _inventory.push(actor);
    return true;
}

void Container::Remove(Actor* actor)
{
    _inventory.remove(actor);
}