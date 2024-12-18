#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <stack>


#define DEBUT_QUADRILLAGE 2


using namespace std;


//déclaration avant propos
class Joueur;
class Zombie;

//déclaration de la structure cellule qui est l'unité du quadrillage
struct cell {
    Vector2 position = {0, 0};
    bool murs[4] = {true, true, true, true};
    bool visitee = false;
};

class Labyrinthe
{
private:
    Texture2D image;
    int colonnes;
    int lignes;
    vector<cell> quadrillage;
    Rectangle img_portion;
    bool initialise;
    int taille_cellule;
    int index_cellule(int x, int y) const;
    void initialisation_du_quadrillage(cell **courant);
    void tracer_du_labyrinthe();
    cell *trouver_les_cellules_non_visitees(cell& cellule);
    void casser_les_murs(cell& courant, cell& suivant);
    void tracer_les_murs_exterieurs();

public:
    Labyrinthe(const char *chemin_acces, int colonnes, int lignes, int taille_cellule);
    void initialisation_du_labyrinthe();
    void afficher_le_labyrinthe();
    friend class Zombie;
    friend class Joueur;

    ~Labyrinthe();
};



#endif