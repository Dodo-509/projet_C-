#include "zombie.h"

Zombie::Zombie(const Labyrinthe& labyrinthe)
{
    x = (float) labyrinthe.quadrillage[0].position.x * labyrinthe.taille_cellule;
    y = (float) labyrinthe.quadrillage[0].position.y * labyrinthe.taille_cellule;
    img_portion = {0,0,40,50};
    nom = "Zombie";

   for(int i = 0; i < 9; i++){

        char numero = '0' + 5;  // '5' en caractère
        char numeroStr[2] = { numero, '\0' };  // Convertir en chaîne de caractères

        char down[50];
        snprintf(down, sizeof(down), "images/zombie/zombie_marche_down_%s.png", numeroStr);

        char up[50];
        snprintf(up, sizeof(up), "images/zombie/zombie_marche_up_%s.png", numeroStr);

        char left[50];
        snprintf(left, sizeof(left), "images/zombie/zombie_marche_left_%s.png", numeroStr);

        char right[50];
        snprintf(right, sizeof(right), "images/zombie/zombie_marche_right_%s.png", numeroStr);

            marche_down[i] = LoadTexture(down);
            marche_up[i] = LoadTexture(up);
            marche_left[i] = LoadTexture(left);
            marche_right[i] = LoadTexture(right);
    }  
}

void Zombie::seDeplacer(const Joueur* joueur, const Labyrinthe& labyrinthe) {
    
    while (!joueur->gagner(labyrinthe) && !WindowShouldClose())
    {
        for (vector<cell>::const_iterator it = joueur->chemin_parcouru.begin(); it < joueur->chemin_parcouru.end(); it++)
        {
            x =(int) it->position.x;
            y = (int) it->position.y;
            WaitTime(0.09);

            if(x == joueur->x && y == joueur->y)
            {
                cout<<"Vous avez perdu!!"<<endl;
                x = (float) labyrinthe.quadrillage[0].position.x * labyrinthe.taille_cellule;
                y = (float) labyrinthe.quadrillage[0].position.y * labyrinthe.taille_cellule;
                break;
            }
        }
    }
    
    
}

void Zombie::afficher_zombie(const Labyrinthe& labyrinthe) {
    if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[1])
        DrawTextureRec(marche_right[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
    else if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[2])
        DrawTextureRec(marche_down[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
    else if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[3])
        DrawTextureRec(marche_left[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
    else if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[0])
        DrawTextureRec(marche_up[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
}

Zombie::~Zombie()
{
    for(int i = 0; i < 9; i++){

        UnloadTexture(marche_down[i]);
        UnloadTexture(marche_up[i]);
        UnloadTexture(marche_left[i]);
        UnloadTexture(marche_right[i]);
    }   
cout<<"zombie détruit";
}