#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;


struct coordenadas{
    int i ;
    int j ;
};

int is_close(coordenadas A, coordenadas B)
{
    if(sqrt(abs(A.i - B.i) + abs(A.j - B.j)) == 1) return 1;
    else return 0;
}

coordenadas exit_place(coordenadas H, coordenadas K, coordenadas D)
{   
    coordenadas aux;
    srand(time(NULL));
    
    while(1)
    {
        aux.j = 9 * (rand() % 2);
        aux.i = 1 + rand() % 8;
        if(is_close(aux,H) || is_close(aux,K) || is_close(aux,D)) continue;
        else break; 
    }

    return aux;
}

int game_state_update(coordenadas H, coordenadas D, coordenadas K, coordenadas E, int game_state)
{
    if(is_close(D,H)) return -1;
    if((game_state != 1) && (K.i == H.i && K.j == H.j)) return 1;
    else if((game_state == 1) && (E.i == H.i && E.j == H.j)) return 2;
    else return game_state;
}

int can_exit(coordenadas H, coordenadas E, int game_state, int i, int j)
{   
    if((game_state == 0) && (E.i == (H.i+i) && E.j == (H.j+j))) return 0;
    else return 1;
    
}


namespace cli 
{

}
int main()
{      
    srand(time(NULL));

    coordenadas H ;
    H.i= 1 ; H.j = 1;
    coordenadas K ;
    K.i = 8 ; K.j = 1;
    coordenadas D ;
    D.i = 5 ; D.j = 4;
    coordenadas E = exit_place(H,K,D);

    int game_state = 0;

    char pressed_key ;

    char maze[10][10] = {
        {'X','X','X','X','X','X','X','X','X','X'},
        {'X',' ',' ',' ',' ',' ',' ',' ',' ','X'},
        {'X',' ','X','X',' ','X',' ','X',' ','X'},
        {'X',' ','X','X',' ','X',' ','X',' ','X'},
        {'X',' ','X','X',' ','X',' ','X',' ','X'},
        {'X',' ',' ',' ',' ',' ',' ','X',' ','X'},
        {'X',' ','X','X',' ','X',' ','X',' ','X'},
        {'X',' ','X','X',' ','X',' ','X',' ','X'},
        {'X',' ','X','X',' ',' ',' ',' ',' ','X'},
        {'X','X','X','X','X','X','X','X','X','X'}
    };
    
    maze[H.i][H.j] = 'H';
    maze[E.i][E.j] = 'E';
    maze[K.i][K.j] = 'K';
    maze[D.i][D.j] = 'D';


while(tolower(pressed_key) != 'f')
{
    system("clear");
    cout << "   HERE BE DRAGONS\n";
    cout << "   press F to start\n";
    cin >> pressed_key;
}

system("clear");

while(1)
{   
    maze[H.i][H.j] = 'H';

    for (int i = 0; i < 10; i++)
       for (int j = 0; j < 10; j++)
        cout << maze[i][j] << " \n"[j == 9];
    
    game_state = game_state_update(H,D,K,E,game_state);
    
    switch (game_state)
    {
    
    case -1:
        cout << "\n You lost the game!!\n";
        return 1;
    break;

    case 2:
        cout << "\n You won the game!!\n";
        return 1;
    break;

    default:
    break;
    }

    maze[H.i][H.j]= ' ';

    cout << "Command: ";
    cin >> pressed_key;
    
    switch (tolower(pressed_key))
    {
    case 'a':
        if(maze[H.i][H.j-1] != 'X' && can_exit(H,E,game_state,0,-1)) H.j--;
    break;

    case 'd':
        if(maze[H.i][H.j+1] != 'X' && can_exit(H,E,game_state,0,1)) H.j++;
    break;

    case 'w':
       if(maze[H.i-1][H.j] != 'X' && can_exit(H,E,game_state,-1,0)) H.i--;
    break;

    case 's':
        if(maze[H.i+1][H.j] != 'X' && can_exit(H,E,game_state,1,0)) H.i++;
    break;

    default:
    break;
    }

    system("clear");
}
     
    return 0;
}