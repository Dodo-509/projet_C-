#include "labyrinthe.h"

Labyrinthe::Labyrinthe(const char *chemin_acces, int colonnes, int lignes, int taille_cellule) 
{
        image = LoadTexture(chemin_acces);
        this->colonnes = colonnes;
        this->lignes = lignes;
        img_portion = {0, 0, (float)taille_cellule, (float)taille_cellule};
        initialise = false;
        this->taille_cellule = taille_cellule; 
}

Labyrinthe::~Labyrinthe()
{
        UnloadTexture(image);
}

int Labyrinthe::index_cellule (int x, int y) const{
    if (x < 0 || y < 0 || x >= colonnes + DEBUT_QUADRILLAGE || y >= lignes + DEBUT_QUADRILLAGE) return -1;
    return (x + y * colonnes);
}

void Labyrinthe::initialisation_du_quadrillage (cell **courant) {
    if (!initialise) {
        for (int y = DEBUT_QUADRILLAGE; y < lignes + DEBUT_QUADRILLAGE; y++) {
            for (int x = DEBUT_QUADRILLAGE; x < colonnes + DEBUT_QUADRILLAGE; x++) {
                cell cellule;
                cellule.position.x = x;
                cellule.position.y = y;
                quadrillage.push_back(cellule);
            }
        }
        int i = 0;    

        *courant = &quadrillage[0];
        initialise = true;
    }
}

void Labyrinthe::tracer_du_labyrinthe() {
    for (vector<cell>::iterator it = quadrillage.begin(); it != quadrillage.end(); it++)
    {
        int x = (int)(it->position.x * taille_cellule);
        int y = (int)(it->position.y * taille_cellule);
        Vector2 position = {(float) x, (float) y};

        if (it->murs[0]) DrawLine(x, y, x + taille_cellule, y, RAYWHITE);               // Mur du haut
        if (it->murs[1]) DrawLine(x + taille_cellule, y, x + taille_cellule, y + taille_cellule, RAYWHITE); // Mur de droite
        if (it->murs[2]) DrawLine(x, y + taille_cellule, x + taille_cellule, y + taille_cellule, RAYWHITE); // Mur du bas
        if (it->murs[3]) DrawLine(x, y, x, y + taille_cellule, RAYWHITE);               // Mur de gauche

        
        if(it->visitee){
           // DrawRectangle(x, y, TAILLECELLULE, TAILLECELLULE, RED);  
            DrawTextureRec(image, img_portion, position, WHITE);
        }
    }
}

cell *Labyrinthe::trouver_les_cellules_non_visitees(cell& cellule) {
    vector<cell*> voisins;

    int haut = index_cellule(cellule.position.x - DEBUT_QUADRILLAGE, cellule.position.y -(DEBUT_QUADRILLAGE + 1));
    int droite = index_cellule(cellule.position.x - DEBUT_QUADRILLAGE + 1, cellule.position.y - DEBUT_QUADRILLAGE);
    int bas = index_cellule(cellule.position.x - DEBUT_QUADRILLAGE, cellule.position.y - DEBUT_QUADRILLAGE + 1);
    int gauche = index_cellule(cellule.position.x - (DEBUT_QUADRILLAGE + 1) , cellule.position.y - DEBUT_QUADRILLAGE);

    if (haut != -1 && !quadrillage[haut].visitee) voisins.push_back(&quadrillage[haut]);
    if (droite != -1 && !quadrillage[droite].visitee) voisins.push_back(&quadrillage[droite]);
    if (bas != -1 && !quadrillage[bas].visitee) voisins.push_back(&quadrillage[bas]);
    if (gauche != -1 && !quadrillage[gauche].visitee) voisins.push_back(&quadrillage[gauche]);

    if (!voisins.empty()) {
        int index_aleatoire = rand() % voisins.size();
        return voisins[index_aleatoire];
    }
    return nullptr;
}

void Labyrinthe::casser_les_murs(cell& courant, cell& suivant) {
    int dx = courant.position.x - suivant.position.x;
    int dy = courant.position.y - suivant.position.y;

    if (dx == 1) {
        courant.murs[3] = false; // Enlever le mur de gauche
        suivant.murs[1] = false;    // Enlever le mur de droite de la cellule suivante
    } else if (dx == -1) {
        courant.murs[1] = false; // Enlever le mur de droite
        suivant.murs[3] = false;    // Enlever le mur de gauche de la cellule suivante
    } else if (dy == 1) {
        courant.murs[0] = false; // Enlever le mur du haut
        suivant.murs[2] = false;    // Enlever le mur du bas de la cellule suivante
    } else if (dy == -1) {
        courant.murs[2] = false; // Enlever le mur du bas
        suivant.murs[0] = false;    // Enlever le mur du haut de la cellule suivante
    } 
}

void Labyrinthe::tracer_les_murs_exterieurs() {
    DrawLine(DEBUT_QUADRILLAGE * taille_cellule, DEBUT_QUADRILLAGE * taille_cellule, (DEBUT_QUADRILLAGE + colonnes) * taille_cellule, DEBUT_QUADRILLAGE * taille_cellule, BLACK); //ligne du haut
    DrawLine(DEBUT_QUADRILLAGE * taille_cellule, (DEBUT_QUADRILLAGE + lignes) * taille_cellule, (DEBUT_QUADRILLAGE + colonnes) * taille_cellule, (DEBUT_QUADRILLAGE + lignes) * taille_cellule, BLACK); //ligne du bas
    DrawLine(DEBUT_QUADRILLAGE * taille_cellule, DEBUT_QUADRILLAGE * taille_cellule, DEBUT_QUADRILLAGE * taille_cellule, (DEBUT_QUADRILLAGE + lignes) * taille_cellule, BLACK);
    DrawLine((DEBUT_QUADRILLAGE + colonnes) * taille_cellule, DEBUT_QUADRILLAGE * taille_cellule , (DEBUT_QUADRILLAGE + colonnes) * taille_cellule, (DEBUT_QUADRILLAGE + lignes) * taille_cellule, BLACK); 
}

void Labyrinthe::initialisation_du_labyrinthe() {
        
        if(!initialise){
                cell *courant;
                initialisation_du_quadrillage(&courant);

                // Initialisation pour DFS
                courant->visitee = true;
                stack<cell*> stack;
                int i =0;
            
                do {   
                    
                    // Génération du labyrinthe en temps réel
                    cell* suivant = trouver_les_cellules_non_visitees(*courant);
                    if (suivant) {
                        suivant->visitee = true;
                        stack.push(suivant);
                        casser_les_murs(*courant, *suivant);
                        courant = suivant;
                    } else if (!stack.empty()) {
                        courant = stack.top();
                        stack.pop();
                    }
                    i++;
                } while(!stack.empty());
        }
        
    
}

void Labyrinthe::afficher_le_labyrinthe() {
    // Affiche le labyrinthe
        tracer_du_labyrinthe();
        tracer_les_murs_exterieurs();
}