#include "Game.h"
#include "PF.h"

int main()
{

    Game game;

    game.getEntities().push_back(new PF());
    
    while (game.isRunning())
    {
        game.poll();
        game.update();
        game.render();
    }
    
    return EXIT_SUCCESS;
}