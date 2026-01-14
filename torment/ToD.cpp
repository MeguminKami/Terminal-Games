#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "ToD.hpp"


void cli::gameStart::cycle(logic::Elements& elements) // Starting cycle , ask the user for a number of dragons
{      
    system("clear");
    printf("Welcome to Dragon's Bane!\nPlease insert the number of dragons (1-%d) => ",maxDragons);

    // Ask the user for a correct number of dragons
    cin >> elements.numDragons;
    while(!(1 <= elements.numDragons && elements.numDragons <= maxDragons))
    {
        cout << "\033[1F" << "\033[2K";
        printf("You can only have 1 to %d Dragons =>  ",maxDragons);
        cin >> elements.numDragons;
    }
    system("clear");
}

void cli::gamePlay::printMap(logic::Game game, logic::Elements element) // Prints the map
{
    system("clear");

    // Prints the map
    for (int i = 0; i < mapSize; i++)
    {
        for (int j = 0; j < mapSize; j++) 
        {
            switch (game.map[i][j])
            {
            case 'X':
                printf("\033[1;32m%c\033[0m",game.map[i][j]);
            break;

            case 'F':
            case 'D':
                printf("\033[1;31m%c\033[0m",game.map[i][j]);
            break;

            case 'H':
            case 'A':
                printf("\033[1;34m%c\033[0m",game.map[i][j]);
            break;

            case 'K':
                printf("\033[1;33m%c\033[0m",game.map[i][j]);
            break;

            case 'S':
                printf("\033[1;36m%c\033[0m",game.map[i][j]);
            break;
            
            default:
                cout << game.map[i][j];
            break;
            }  
            if((i==1) && (j==mapSize-1)) printf("   \033[7mINVENTORY\033[0m");
            if((i==2) && (j==mapSize-1) && element.hero.hasKey) printf("  \033[7m>>Master Key\033[0m");
            if((i==3) && (j==mapSize-1) && element.hero.hasSword) printf("  \033[7m>>DragonKiller\033[0m");
            if((i==(mapSize-1)) && (j==mapSize-1)) printf("   \033[7mDragons Alive %d/%d\033[0m",(element.numDragons-element.killedDragons),element.numDragons);
            cout << " \n"[j == (mapSize-1)];
            
        }     
        
    }
                  
                
                
}

void cli::gamePlay::updateMap(logic::Game& game, logic::Elements elements) // Updates the map of every element new position
{   
    // Updates hero pos
    game.map[elements.hero.pos.i][elements.hero.pos.j] = elements.hero.symbol;

    // Updates key pos
    if(!elements.hero.hasKey) game.map[elements.key.pos.i][elements.key.pos.j] = elements.key.symbol;

    // Updates sword pos
    if(!elements.hero.hasSword) game.map[elements.sword.pos.i][elements.sword.pos.j] = elements.sword.symbol;

    // Updates dragons positions
    for(int i = 0; i < elements.numDragons; i++)
    {
        if(elements.dragons[i].alive) game.map[elements.dragons[i].pos.i][elements.dragons[i].pos.j] = elements.dragons[i].symbol;
    }
}

void cli::gamePlay::cycle(logic::Game& game,logic::Elements elements) // Gameplay cycle
{      
    elements.placeDragons(game);
    elements.placeExit(game);

    while (1)
    {   
        updateMap(game,elements);
        printMap(game,elements);

        // Deletes from the map hero and dragon elements so a new position can be determined
        game.map[elements.hero.pos.i][elements.hero.pos.j] = ' ';  

        // Hero move 
        elements.moveHero(game);
        game.map[elements.hero.pos.i][elements.hero.pos.j] = elements.hero.symbol;  
        if(game.updateGame(elements)) 
        {
            printMap(game,elements);
            return;
        }
        
        // Deletes dragons to be replaced
        for(int i = 0; i < elements.numDragons; i++) game.map[elements.dragons[i].pos.i][elements.dragons[i].pos.j] = ' ';

        // Dragons move
        for(int i = 0; i < elements.numDragons; i++)
        {
            if(elements.dragons[i].alive)
            {
                elements.moveDragon(game,i);
                game.map[elements.dragons[i].pos.i][elements.dragons[i].pos.j] =  elements.dragons[i].symbol;
                if(game.updateGame(elements)) 
                {
                    updateMap(game,elements);
                    printMap(game,elements);
                    return;
                }
            }
        }  
    }   
}

void logic::Elements::moveHero(logic::Game game) // Moves the hero in a inpured direction
{   
    // Variables
    coordenadas newPos;
    newPos.i = 0; newPos.j = 0;
    int endCycle = 1;

    // Asks for a input
    char pressedKey;
    cout << "\nCommand: ";
    cin >> pressedKey; 
    while(endCycle)
    {
        // Determines new coordinates based on the input
        switch (tolower(pressedKey))
        {
        case 'a': // Left
            newPos.i = hero.pos.i;
            newPos.j = hero.pos.j - 1;
        break;

        case 'd': // Right
            newPos.i = hero.pos.i;
            newPos.j = hero.pos.j + 1;
        break;

        case 'w': // Up
            newPos.i = hero.pos.i - 1;
            newPos.j = hero.pos.j;
        break;

        case 's': // Down
            newPos.i = hero.pos.i + 1;
            newPos.j = hero.pos.j;
        break;

        default: 
        break;
        }   

        // Checks if the move is valid
        switch (game.map[newPos.i][newPos.j])
        {
        case 'S': // Sword
            hero.hasSword = true;
            hero.symbol = 'A';
            hero.pos = newPos;
            endCycle = 0;
        break;
        
        case 'K': // Key
            hero.hasKey = true;
            hero.pos = newPos;
            endCycle = 0;
        break;
        
        case 'E': // Exit
            if(hero.hasKey && allDragonsDead())
            {
                hero.pos = newPos;
                endCycle = 0;
            }
            else
            {
                cout << "\033[1F" << "\033[2K";
                printf("You need the key and to kill all dragons to leave, command: ");
                cin >> pressedKey; 
            }
        break;

        case ' ': // Empty space
            hero.pos = newPos;
            endCycle = 0;
        break;

        default: // Not a valid input asks the user for a new input
            cout << "\033[1F" << "\033[2K";
            printf("Please insert a valid command: ");
            cin >> pressedKey; 
        break;
        }  
    }
}

void logic::Elements::moveDragon(logic::Game game, int dragonID) // Moves a dragon randomly
{   
    // Variables
    vector<coordenadas> validMoves;
    checkValidMoves(validMoves,game,dragons[dragonID]);
    if(validMoves.size() == 0) return;
    int validMoveNumb;
    int endCycle = 1;
   
   
    srand(time(NULL));

    while(endCycle)
    {   
        if(validMoves.size() == 1) validMoveNumb = 0;
        else validMoveNumb = rand() % validMoves.size() ;
        
        // Checks if a move is valid if so the cycle ends
        switch (game.map[validMoves[validMoveNumb].i][validMoves[validMoveNumb].j])
        {
        case 'S': // Sword
            dragons[dragonID].symbol = 'F';
            dragons[dragonID].pos = validMoves[validMoveNumb];
            endCycle = 0;
        break;
        
        case 'K': // Key
            dragons[dragonID].symbol = 'F';
            dragons[dragonID].pos = validMoves[validMoveNumb];
            endCycle = 0;
        break;

        case ' ': // Empty space
            dragons[dragonID].symbol = 'D';
            dragons[dragonID].pos = validMoves[validMoveNumb];
            endCycle = 0;
        break;
        
        default:
        break;

        }  
    }
}

int logic::Game::updateGame(logic::Elements& elements) // Updates the game status
{
    // If the the gero is within 1 space of a dragon and that dragon is alive
    for(int i = 0; i < elements.numDragons; i++)
    {
         if(is_close(elements.dragons[i].pos,elements.hero.pos) && elements.dragons[i].alive)
        {   
            // If the hero has the sword he kills the dragon, if not he dies
            if(elements.hero.hasSword) 
            {   
                elements.dragons[i].alive = 0 ;
                elements.killedDragons++;
                map[elements.dragons[i].pos.i][elements.dragons[i].pos.j] = ' ';
            }
            else gameStatus = -1; // Game lost

            return gameStatus; // Returns game status (if the hero just killed the dragon, or died, he can't be in the exit)
        }   

    }
   
    gameStatus = is_equal(elements.exit.pos,elements.hero.pos); // If the hero is in the exit means he finished the game
    return gameStatus; // Returns gameStatus (0 if nothing happened) (1 if hero wins) (-1 if hero dies)
}   

void logic::Elements::placeDragons(logic::Game game) // Places the dragons in their FIRST positions
{   
    // Variables
    Dragon auxDragon;
    coordenadas aux;
    srand(time(NULL));

    // Cycle to fill every dragon position
    for(int i = 0; i < numDragons; i++)
    {
        while(1)
        {   
            // Inside the walls
            aux.j = 1 + rand() % (mapSize-2);
            aux.i = 1 + rand() % (mapSize-2);

            // Not a valid position if is the same as a wall
            if(game.map[aux.i][aux.j] == 'X') continue;

            // If the hero is within 1 square of the key or the sword
            // and within 2 squares of the hero
            // is NOT a valid position
            if(is_close(aux,key.pos) || is_close(aux,sword.pos) || is_close_dis(aux,hero.pos,2) ) continue;
            if(is_equal(aux,key.pos) || is_equal(aux,sword.pos)) continue;

            // If theres already a dragon in the position
            // is Not a valid position
            for(int j = 0; j < i; j++)
                if(is_equal(aux,dragons[j].pos)) continue;
            
            break;
        }

        // If the cycle ends it means it fond a valid position for the dragon
        // Initiates a new dragon
        auxDragon.alive = true;
        auxDragon.pos = aux;
        auxDragon.symbol = 'D';

        // Adds the dragon to the dragon vector
        dragons.push_back(auxDragon);
    }
}

void logic::Elements::placeExit(logic::Game& game) // Finds a valid place for the exit
{   
    srand(time(NULL));
    
    // Cycle to find a valid exit position
    while(1)
    {   
        // Generates a position in the lateral walls
        exit.pos.j = (mapSize-1) * (rand() % 2);
        exit.pos.i = 1 + rand() % (mapSize-2);

        // If the exit is generated within 1 square of the hero or the key
        // is NOT a valid position
        if(is_close(exit.pos,hero.pos) || is_close(exit.pos,key.pos)) continue;

        // If the exit is generated within 1 square of a dragon
        // is NOT a valid position
        for(int i = 0; i < numDragons; i++)
            if(is_close(exit.pos,dragons[i].pos)) continue;
        
        // If HERE means it generated a valid exit position
        break; 
    }

    // Updates exit pos
    game.map[exit.pos.i][exit.pos.j] = exit.symbol;
}

int logic::Elements::allDragonsDead() // Checks if all dragons are dead
{
    for(int i = 0; i < numDragons; i++)
        if(dragons[i].alive) return 0;

    return 1;
}

int is_close(coordenadas A, coordenadas B) // Checks if 2 elements are within 1 quare of 1 another (not diagonally)
{
    if(sqrt(abs(A.i - B.i) + abs(A.j - B.j)) == 1) return 1;
    else return 0;
}

int is_equal(coordenadas A, coordenadas B) // Checks if 2 elemenrs are in the same position
{
  if(A.i == B.i && A.j == B.j) return 1;
  else return 0;
}

int is_close_dis(coordenadas A, coordenadas B, int dist) // Checks if 2 elements are in an "Euclidean Distance" dist of 1 another
{
   if(sqrt(abs(A.i - B.i) + abs(A.j - B.j)) <= dist) return 1;
   else return 0;  
}

void checkValidMoves(vector<coordenadas>& validMoves, logic::Game game, logic::Dragon dragon) // Checks all the valid moves of the dragon in the argument
{   
    coordenadas validMove = {0,0};
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {   
            if(i == 0 && j == 0) continue;
            validMove.i = i + dragon.pos.i;
            validMove.j = j + dragon.pos.j;
            switch (game.map[validMove.i][validMove.j])
            {
            case ' ':
            case 'S':
            case 'K':
                validMoves.push_back(validMove);
            break;
        
            default:
            break;
            }
        }
    }
}

int elementsAround(logic::Game game,coordenadas element)
{
    int count  = 0;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {   
            if(i == 0 && j == 0) continue;
            if(abs(i) == abs(j)) if(game.map[element.i + i][element.j + j] != ' ') count++;
        }
    }
    if(count > 1) return 1;
    else return 0;
}

int main()
{      
    // Variables
    cli::gameStart start;
    cli::gamePlay play;
    logic::Elements elements;
    logic::Game game;
    srand(time(NULL));
    elements.killedDragons = 0;

    // Hero
    elements.hero.pos = {1,1};
    elements.hero.hasSword = false;
    elements.hero.hasKey = false;
    elements.hero.symbol = 'H';

    // Key
    elements.key.pos = {swordkeyOffset + rand() % (mapSize-(swordkeyOffset*2)),swordkeyOffset + rand() % (mapSize-(swordkeyOffset*2))};
    elements.key.symbol = 'K';

    // Sword
    elements.sword.pos = {swordkeyOffset + rand() % (mapSize-(swordkeyOffset*2)),swordkeyOffset + rand() % (mapSize-(swordkeyOffset*2))};
    elements.sword.symbol = 'S';

    // In case the sword and key are genarated in the same position
    // this cycle will make sure both are in different positions
    while(is_equal(elements.sword.pos,elements.key.pos)) elements.sword.pos = {swordkeyOffset + rand() % (mapSize-(swordkeyOffset*2)),swordkeyOffset + rand() % (mapSize-(swordkeyOffset*2))};

    // Exit
    elements.exit.symbol = 'E';

    
    // Outside Walls
    for(int i = 0; i < mapSize; i++)
    {
        for(int j = 0; j < mapSize; j++)
        {
            if(i == 0 || i == (mapSize-1)) game.map[i][j] = 'X';
            else if(j == 0 || j == (mapSize-1)) game.map[i][j] = 'X';
            else game.map[i][j] = ' ';
        }
    }

    // Inside walls
    int maxInsideWalls = (mapSize-1)*(mapSize-1) * wallPercentage;
    coordenadas wallPos;
    for(int i = 0; i < maxInsideWalls; i++)
    {
        wallPos.i = 1 + rand() % (mapSize-2);
        wallPos.j = 1 + rand() % (mapSize-2);
        if(elementsAround(game,wallPos))
        {
            i--;
            continue;
        }
        if(game.map[wallPos.i][wallPos.j] == ' ') game.map[wallPos.i][wallPos.j] = 'X';
    }

    // Gamestatus 0 is game running;
    game.gameStatus = 0;

    // Starting game cycle
    start.cycle(elements);

    // Gameplay cycle
    play.cycle(game,elements);  

    // Presents the result, victory or lose
    if(game.gameStatus == 1) printf("You won the game\n");
    else printf("You lost the game\n");


}