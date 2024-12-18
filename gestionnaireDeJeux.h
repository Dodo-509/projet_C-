#ifndef GESTIONNAIREDEJEUX_H
#define GESTIONNAIREDEJEUX_H

#include <string>
#include "raylib.h"
#include "labyrinthe.h"
#include "joueur.h"


using namespace std;

class GestionnaireDeJeux
{
private:
    Font police;
    string nomJeu;
    int gameState = 0;

    //menu related variables
    string options[4] = {"Jouer", "Score", "Parametres", "Credits"};
    Rectangle optionsCadre[4];

    //window related variables
    int windowWidth;
    int windowHeight;
    int widthSubdivision;
    int heightSubdivision;

    //icon related variable
    Image logo;

    //audio related variables
    Music titre;
    FilePathList musiques;
    double duration;
    double starting_time;
    int titre_number = 0;
    bool audioLoaded = false;
    int volume;

    //timer related variables
    double chrono;
    double starting_game;
    string chronometre();

    //background animation related variables
    Image fond;
    Texture2D fondTexture;
    int frames;
    unsigned int nextFrameDataOffset;  
    int currentAnimFrame;      
    int frameDelay;           
    int frameCounter;     
    bool animationOngoing = true;
    bool fondLoaded = false;

    //text animation related variables
    Color couleurTexte;
    int tailleTexte;
    float alpha;

    //credits related variables
    FilePathList creditsPath;
    string creditsTexte;
    Font creditsPolice;
    bool creditsLoaded = false;

    //parameters related variables
    string levelText[3] = {"FACILE", "MOYEN", "DIFFICILE"};
    Rectangle parametersCadre[6];
    bool musiquePlay = true;
    int level = 2;

    //game related variables
    Labyrinthe *labyrinthe_jeux;
    Joueur *joueur_1;
    bool isGameLaunched = false;
    Rectangle gameOptionsCadre[2];
    Font policeGameOptions;
    bool isGameFinished = false;
    string chronoText;
    

    //les fonctions privées
    void setIcon(char *icon);
    bool isMaximized;
    void playMusic();
    void chargementDeMusique();

    void mainMenu();
    void checkButtonMenu();

    void animatedTextFading(string texte, int duration);
    void animationDebut();
    void chargementDuFond();
    void misAJour();
    
    void afficherScrores();
    void afficherParametres();
    void chargementCredits();
    void checkParameters();
    void afficherCredits();
    void checkCredits();
    void lancerPartie();
    void checkDeplacement();
    void checkResultat();
    void gameOptions();
    void checkGameOptions();

public:
    GestionnaireDeJeux(char * icon);
    void toggleMaximize();
    void refresh();
    ~GestionnaireDeJeux();
};

#endif