#pragma once

// Base class for game actors.
class Actor
{
    class Coordinates
    {
    public:
        Coordinates() {}
        Coordinates(int x, int y) : _x(x), _y(y) {}

        bool operator==(const Coordinates& other) const
        {
            return _x == other._x && _y == other._y;
        }

        int _x = 0;
        int _y = 0;
    };

public:
    Actor(int x, int y, int ch, const char* name, const TCODColor& color)
    {
        _coordinates = Coordinates(x, y);
        _ch = ch;
        _color = color;
        _name = std::string(name);
        _blocks = true;
    }
    ~Actor() {}

    static Actor* InitPlayer();
    void Update();
    bool MoveOrAttack(int x_delta, int y_delta);
    void Move(TCOD_keycode_t direction, unsigned int delta);
    void Render() const;
    float GetDistance(int cx, int cy) const;

    Coordinates _coordinates;
    int _ch; // ascii code
    TCODColor _color;
    std::string _name;
    bool _blocks; // blocks movement

    // Owned attributes, may be nullptr. Avoiding inheritance for flexibility.
    std::unique_ptr<Attacker> _attacker = nullptr;
    std::unique_ptr<Destructible> _destructible = nullptr;
    std::unique_ptr<Item> _item = nullptr; // able to be picked up by player
    std::unique_ptr<Ai> _ai = nullptr; // movement
    std::unique_ptr<Container> _container = nullptr;
    std::unique_ptr<Experience> _experience = nullptr;
};