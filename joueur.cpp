#include "joueur.h"

Joueur::Joueur(const Labyrinthe& labyrinthe)
{
    x = labyrinthe.quadrillage[0].position.x;
    y = labyrinthe.quadrillage[0].position.y;
    img_portion = {0,0,25,50};
    nom = "Player 1";

   for(int i = 0; i < 9; i++){

        char numero = '0' + 5;  // '5' en caractère
        char numeroStr[2] = { numero, '\0' };  // Convertir en chaîne de caractères

        char down[50];
        snprintf(down, sizeof(down), "images/joueur/marche_down_%s.png", numeroStr);

        char up[50];
        snprintf(up, sizeof(up), "images/joueur/marche_up_%s.png", numeroStr);

        char left[50];
        snprintf(left, sizeof(left), "images/joueur/marche_left_%s.png", numeroStr);

        char right[50];
        snprintf(right, sizeof(right), "images/joueur/marche_right_%s.png", numeroStr);

            marche_down[i] = LoadTexture(down);
            marche_up[i] = LoadTexture(up);
            marche_left[i] = LoadTexture(left);
            marche_right[i] = LoadTexture(right);
    }  
}

void Joueur::afficher_joueur(const Labyrinthe& labyrinthe) {
    if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[1])
        DrawTextureRec(marche_right[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
    else if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[2])
        DrawTextureRec(marche_down[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
    else if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[3])
        DrawTextureRec(marche_left[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
    else if (!labyrinthe.quadrillage[labyrinthe.index_cellule(x,y)].murs[0])
        DrawTextureRec(marche_up[0], img_portion, {(float)(x * labyrinthe.taille_cellule),(float)(y * labyrinthe.taille_cellule)}, WHITE);
}
void Joueur::seDeplacer(const Labyrinthe& labyrinthe, int direction) {

    int courant = labyrinthe.index_cellule(x - DEBUT_QUADRILLAGE, y - DEBUT_QUADRILLAGE );
    chemin_parcouru.push_back(labyrinthe.quadrillage[courant]);
    switch (direction)
    {
        case 0:{
                if(!labyrinthe.quadrillage[courant].murs[0])
                    y--; 
                break;
            }
        case 1: {
                if(!labyrinthe.quadrillage[courant].murs[1])
                    x++;
                break;
        }
        case 2:{
                if(!labyrinthe.quadrillage[courant].murs[2])
                    y++;
                break;
        }    
        case 3:{
                if(!labyrinthe.quadrillage[courant].murs[3])
                    x--;
                break;
        }
    }   
    WaitTime(0.09);
}

bool Joueur::gagner(const Labyrinthe& labyrinthe) const {
    int colonnes = labyrinthe.colonnes;
    int lignes = labyrinthe.lignes;

    if(x == labyrinthe.quadrillage[colonnes * lignes - 1].position.x && y == labyrinthe.quadrillage[colonnes * lignes - 1].position.y)
        return true;
    else
        return false;
    
}

Joueur::~Joueur()
{
   for(int i = 0; i < 9; i++){

        UnloadTexture(marche_down[i]);
        UnloadTexture(marche_up[i]);
        UnloadTexture(marche_left[i]);
        UnloadTexture(marche_right[i]);
    }   
cout<<"joueur détruit";
}