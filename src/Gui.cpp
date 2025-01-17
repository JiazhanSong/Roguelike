#include "precompiled_header.h"

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_HEIGHT = PANEL_HEIGHT - 1;
static const int MSG_X = BAR_WIDTH + 2;
static const int VICTORY_MSG_X = 5;
static const int VICTORY_MSG_Y = PANEL_HEIGHT / 2;

Gui::Gui()
{
    _console = std::make_unique<TCODConsole>(kEngine._displayWidth, PANEL_HEIGHT);
}

Gui::~Gui()
{
    _log.clearAndDelete();
}


Gui::Message::Message(const TCODColor& color, const std::string message) :
    _message(message), _color(color)
{
}

Gui::Message::~Message()
{
}

void Gui::AppendMessage(const TCODColor& color, const std::string message) {
    std::stringstream ss(message);
    std::string line;

    while (std::getline(ss, line, '\n'))
    {
        if (_log.size() == MSG_HEIGHT)
{
            Message* toRemove = _log.get(0);
            _log.remove(toRemove);
            delete toRemove;
        }

        Message* msg = new Message(color, line);
        _log.push(msg);
    }
}

void Gui::Render() {
    _console->setDefaultBackground(TCODColor::black);
    _console->clear();

    // draw leftside status bars
    {
        RenderBar(1, 1, BAR_WIDTH, "HP", kEngine._player->_destructible->_hp,
            kEngine._player->_destructible->_maxHp, TCODColor::lightRed, TCODColor::darkerRed);

        auto& exp = kEngine._player->_experience;
        RenderBar(1, 2, BAR_WIDTH, "EXP", exp->_exp, exp->GetNextExperienceThreshold(),
            TCODColor::amber, TCODColor::darkAmber);

        std::string statusText = fmt::format("LV({}) ATK({})", exp->GetLevel(), kEngine._player->_attacker->_power);
        RenderStatusText(1, 3, BAR_WIDTH, statusText, TCODColor::darkerBlue);
    }

    // draw the message log, iterate in reverse for top-down
    float fadeRatio = 1.0f;
    for (Message** it = (_log.end() - 1); it >= _log.begin(); it--)
    {
        Message* message = *it;
        _console->setDefaultForeground(message->_color * fadeRatio);
        _console->print(MSG_X, _log.end() - it, message->_message.c_str());

       fadeRatio = std::max(0.0f, fadeRatio - 0.2f);
    }

    RenderMouseLook();

    // copy GUI into root console
    TCODConsole::blit(_console.get(), 0, 0, kEngine._displayWidth, PANEL_HEIGHT,
        TCODConsole::root, 0, kEngine._displayHeight - PANEL_HEIGHT);
}

void Gui::RenderVictory() {
    _console->setDefaultBackground(TCODColor::gold);
    _console->clear();

    std::string victoryMessage = "You have cleared the dundgeon, hurrah! Your reward: 1 Bitcoin";

    _console->setDefaultForeground(TCODColor::black);
    _console->print(VICTORY_MSG_X, 1, victoryMessage.c_str());

    // copy GUI into root console
    TCODConsole::blit(_console.get(), 0, 0, kEngine._displayWidth, PANEL_HEIGHT,
        TCODConsole::root, 0, kEngine._displayHeight - PANEL_HEIGHT);
}

void Gui::RenderBar(int x, int y, int width, std::string name, float value, float maxValue,
    const TCODColor & barColor, const TCODColor & backColor)
{
    // fill background
    _console->setDefaultBackground(backColor);
    _console->rect(x, y, width, 1, false, TCOD_BKGND_SET);

    int barWidth = (int)(value / maxValue * width);
    if (barWidth > 0)
    {
        _console->setDefaultBackground(barColor);
        _console->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
    }

    std::string formattedText = fmt::format("{} : {}/{}", name, value, maxValue);

    // print text on top of the bar
    _console->setDefaultForeground(TCODColor::white);
    _console->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, formattedText.c_str());
}

void Gui::RenderStatusText(int x, int y, int width, std::string text, const TCODColor& backColor)
{
    _console->setDefaultBackground(backColor);
    _console->rect(x, y, width, 1, false, TCOD_BKGND_SET);

    _console->setDefaultForeground(TCODColor::white);
    _console->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, text.c_str());
}

void Gui::RenderMouseLook() {
    auto& mouse = kEngine._mouse;
    if (!kEngine._map->IsInFov(mouse.cx, mouse.cy)) {
        return;
    }

    std::string output;

    bool first = true;
    for (auto actor : kEngine._actors)
    {
        // find actors under the mouse cursor
        if (actor->_coordinates._x == mouse.cx && actor->_coordinates._y == mouse.cy) {
            if (!first)
            {
                output += ", ";
            }
            else
            {
                first = false;
            }

            output += actor->_name;
        }
    }

    _console->setDefaultForeground(TCODColor::lightGrey);
    _console->print(1, 0, output.c_str());
}