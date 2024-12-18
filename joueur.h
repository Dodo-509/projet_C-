#ifndef JOUEUR_H
#define JOUEUR_H

#include "personnage.h"
#include "labyrinthe.h"
#include <iostream>

//declaration avant-propos
class Zombie;

class Joueur : public Personnage
{

private:
    vector<cell> chemin_parcouru;

public:
    Joueur(const Labyrinthe& labyrinthe);
    void seDeplacer(const Labyrinthe& labyrinthe, int direction);
    void afficher_joueur(const Labyrinthe& labyrinthe);
    bool gagner(const Labyrinthe& labyrinthe) const;
    friend class Zombie;
    ~Joueur();
};



#endif