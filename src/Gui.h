#pragma once

class Gui {
public:
    Gui();
    ~Gui();
    void AppendMessage(const TCODColor& color, const std::string message);
    void Render();
    void RenderVictory();

private:
    struct Message {
        Message(const TCODColor& color, const std::string message);
        ~Message();
        std::string _message;
        TCODColor _color;
    };

    TCODList<Message*> _log;
    std::unique_ptr<TCODConsole> _console;

    void RenderBar(int x, int y, int width, const char* name, float value, float maxValue,
        const TCODColor& barColor, const TCODColor& backColor);
    void RenderMouseLook();
};