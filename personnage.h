#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include <raylib.h>
#include <string>

using namespace std;

//declaration avant-propos
class Labyrinthe;
class Joueur;

class Personnage
{
protected:
    int x;
    int y;
    
    string nom;
    Texture2D marche_up[9];
    Texture2D marche_down[9];
    Texture2D marche_left[9];
    Texture2D marche_right[9];
    Rectangle img_portion;
    

};


#endif