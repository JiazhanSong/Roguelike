#include "precompiled_header.h"

int main()
{
    while (!TCODConsole::isWindowClosed() && kEngine._gameStatus != Engine::GameStatus::EXIT) {
        kEngine.Update();
        kEngine.Render();
        TCODConsole::flush();
    }

    // Gamestate persistence? serialize classes to disk

    return 0;
}