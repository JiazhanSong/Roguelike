#include "Actor.h"
#include "Map.h"
#include "Engine.h"

int main()
{
    while (!TCODConsole::isWindowClosed()) {
        kEngine.Update();
        kEngine.Render();
        TCODConsole::flush();
    }
    return 0;
}