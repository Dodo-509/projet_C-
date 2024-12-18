#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "personnage.h"
#include "joueur.h"
#include "labyrinthe.h"
#include <iterator>
#include <vector>
#include <raylib.h>


class Zombie : public Personnage
{
public:
    Zombie(const Labyrinthe& labyrinthe);
    void seDeplacer(const Joueur* joueur, const Labyrinthe& labyrinthe);
    void afficher_zombie(const Labyrinthe& labyrinthe);
    ~Zombie();
};

#endif