#include "Game.h"
#include "PF.h"
#include "TerrainEditor.h"
#include "Gamagochi.h"
#include "Fruit.h"

int main()
{

    Game game;

    // game.getEntities().push_back(new PF());
    game.getEntities().push_back(new TerrainEditor());
    game.getEntities().push_back(new Gamagochi(vec2(10, 10)));
    
    while (game.isRunning())
    {
        game.poll();
        game.update();
        game.render();
    }
    
    return EXIT_SUCCESS;
}