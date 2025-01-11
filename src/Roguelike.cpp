#include "precompiled_header.h"

int main()
{
    while (!TCODConsole::isWindowClosed()) {
        kEngine.Update();
        kEngine.Render();
        TCODConsole::flush();
    }
    return 0;
}