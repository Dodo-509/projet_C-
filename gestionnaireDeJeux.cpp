#include "gestionnaireDeJeux.h"
#include <iostream>
GestionnaireDeJeux::GestionnaireDeJeux(char *icon)
{   
    police = LoadFont("fonts/Creepster/Creepster-Regular.ttf");  // Charger la police
    policeGameOptions = LoadFont("fonts/Playfair_Display/static/PlayfairDisplay-Bold.ttf");
    nomJeu = "Labyrinthe 1";
    isMaximized = false;
    frames = 0;
    alpha = 0.0;

    setIcon(icon);
    toggleMaximize();
}

void GestionnaireDeJeux::refresh() {
    
    playMusic();
    animationDebut();
    if(!animationOngoing && gameState == 0) {
        mainMenu();
        if(IsMouseButtonPressed(0))
            checkButtonMenu();
    } else if(!animationOngoing && gameState == 1) {
            if (!isGameLaunched)
                lancerPartie();
            else {
                labyrinthe_jeux->afficher_le_labyrinthe(); 
                joueur_1->afficher_joueur(*labyrinthe_jeux);
                gameOptions();
                if(IsMouseButtonPressed(0))
                    checkGameOptions();
                if(GetKeyPressed != 0)
                    checkDeplacement();
                checkResultat();
            }
        } else if(!animationOngoing && gameState == 2) {

        } else if(!animationOngoing && gameState == 3) {
            afficherParametres();
            if(IsMouseButtonPressed(0))
                checkParameters();
        } else if(!animationOngoing && gameState == 4) {
            afficherCredits();
            if(IsMouseButtonPressed(0))
                checkCredits();
        }   
}



void GestionnaireDeJeux::setIcon(char *icon){
    logo = LoadImage(icon);
    ImageFormat(&logo, 7); //conversion dans le format R8G8B8A8
    SetWindowIcon(logo);
}

void GestionnaireDeJeux::chargementDeMusique() {
    //chargement des titres qui vont être jouées en arrière plan
        char repertoire[8] = "musique";
        if(DirectoryExists(repertoire))
        {
            musiques = LoadDirectoryFiles(repertoire);
        }

        titre = LoadMusicStream(musiques.paths[titre_number]);
        PlayMusicStream(titre);
        starting_time = GetTime();
        duration = GetMusicTimeLength(titre);
        audioLoaded = true;
}
void GestionnaireDeJeux::playMusic(){ 
    if(audioLoaded){
        UpdateMusicStream(titre);
        ///change de musique
        if(GetTime() > starting_time + duration)
        {
            //on check si le numéro est valide
            if(titre_number < ((int)musiques.count - 1))
                ++titre_number;
            else
                titre_number = 0;
            
            //charge la prochaine musique
            chargementDeMusique();
        }

    } else {
        chargementDeMusique();
    }
    
}

string GestionnaireDeJeux::chronometre() {
        chrono = GetTime() - starting_game;
        int minute = 0, heure = 0, seconde = (int)chrono ;
        char chronoText[10];

        if(chrono > 60) {
            minute = seconde / 60;
            seconde %= 60; 
        }
        if(minute > 60){
            heure = minute/60;
            minute %= 60; 
        }
        
        sprintf(chronoText, "%.2d:%.2d:%.2d", heure, minute, seconde);

        string text = chronoText;
        return text;
}

void GestionnaireDeJeux::animationDebut() {
     if(animationOngoing) {
        if(!fondLoaded) chargementDuFond();

        misAJour();
        DrawTexturePro(fondTexture, {0.0,0.0,(float)fond.width, (float)fond.height}, {0.0, 0.0, (float)windowWidth, (float)windowHeight}, {0.0,0.0}, 0.0, WHITE);
        animatedTextFading(nomJeu, 4);
    }
}

void GestionnaireDeJeux::animatedTextFading(string texte, int duration) {
    if(alpha < 1 )
    {
        alpha += 1.0 / 60 * duration;
        couleurTexte = Fade (WHITE, alpha);
    }
    tailleTexte = MeasureText(texte.c_str(), windowWidth/20);
    DrawTextEx(police, texte.c_str(), {(float)(windowWidth - tailleTexte)/2, (float)windowHeight/2}, (float) windowWidth/20, 10.0, couleurTexte);
}

void GestionnaireDeJeux::misAJour() {
        frameCounter++;
        if (frameCounter >= frameDelay)
        {
            currentAnimFrame++;
            if (currentAnimFrame >= frames) {
                currentAnimFrame = 0;
                animationOngoing = false;
            } 
            nextFrameDataOffset = fond.width*fond.height*4*currentAnimFrame;
            UpdateTexture(fondTexture, ((unsigned char *)fond.data) + nextFrameDataOffset);
            frameCounter = 0;
        }
}
void GestionnaireDeJeux::chargementDuFond() {
    char path[50] = "images/animee/fond.gif";
    fond = LoadImageAnim(path, &frames);
    fondTexture = LoadTextureFromImage(fond);
    
    //pour une animation plus fluide
    frameDelay = 60/ (frames / 15); 

    if (frameDelay < 1) frameDelay = 1;

    currentAnimFrame = 0;         
    frameCounter = 0; 
    fondLoaded = true;
}

void GestionnaireDeJeux::mainMenu() {

    Color fontColor = BLUE;
    float fontSize = windowWidth / 25;

    //affichage du nom du jeu
    DrawText(nomJeu.c_str(), (windowWidth - MeasureText(nomJeu.c_str(), fontSize))/2, 2 *heightSubdivision, fontSize, fontColor);

    //affichage de l'option Jouer    
    optionsCadre[0].x = (windowWidth - MeasureText(options[0].c_str(), fontSize))/2 ;
    optionsCadre[0].y = 3 *heightSubdivision;
    optionsCadre[0].width = MeasureText(options[0].c_str(), fontSize);
    optionsCadre[0].height = heightSubdivision;
    DrawText(options[0].c_str(), optionsCadre[0].x, optionsCadre[0].y, fontSize, fontColor);
    
    //affichage de l'option Scores
    optionsCadre[1].x = (windowWidth - MeasureText(options[1].c_str(), fontSize))/2 ;
    optionsCadre[1].y = 5 *heightSubdivision;
    optionsCadre[1].width = MeasureText(options[1].c_str(), fontSize);
    optionsCadre[1].height = heightSubdivision;
    DrawText(options[1].c_str(), optionsCadre[1].x, optionsCadre[1].y, fontSize, fontColor);
    
    //affichage de l'option parametres
    optionsCadre[2].x = (windowWidth - MeasureText(options[2].c_str(), fontSize))/2 ;
    optionsCadre[2].y = 6 *heightSubdivision;
    optionsCadre[2].width = MeasureText(options[2].c_str(), fontSize);
    optionsCadre[2].height = heightSubdivision;
    DrawText(options[2].c_str(), optionsCadre[2].x, optionsCadre[2].y, fontSize, fontColor);
    
    //affichage de l'option credits
    optionsCadre[3].x = (windowWidth - MeasureText(options[3].c_str(), fontSize))/2 ;
    optionsCadre[3].y = 7 *heightSubdivision;
    optionsCadre[3].width = MeasureText(options[3].c_str(), fontSize);
    optionsCadre[3].height = heightSubdivision;
    DrawText(options[3].c_str(), optionsCadre[3].x, optionsCadre[3].y, fontSize, fontColor);
}

void GestionnaireDeJeux::checkButtonMenu(){
    Vector2 cursor_position = GetMousePosition();
    
    if((optionsCadre[0].x <= cursor_position.x && optionsCadre[0].x + optionsCadre[0].width>= cursor_position.x) && (optionsCadre[0].y <= cursor_position.y && optionsCadre[0].y + optionsCadre[0].height>= cursor_position.y))
        gameState = 1;
    else if((optionsCadre[1].x <= cursor_position.x && optionsCadre[1].x + optionsCadre[1].width>= cursor_position.x) && (optionsCadre[1].y <= cursor_position.y && optionsCadre[1].y + optionsCadre[1].height>= cursor_position.y))
        gameState = 2;
    else if((optionsCadre[2].x <= cursor_position.x && optionsCadre[2].x + optionsCadre[2].width>= cursor_position.x) && (optionsCadre[2].y <= cursor_position.y && optionsCadre[2].y + optionsCadre[2].height>= cursor_position.y))
        gameState = 3;
    else if((optionsCadre[3].x <= cursor_position.x && optionsCadre[3].x + optionsCadre[3].width>= cursor_position.x) && (optionsCadre[3].y <= cursor_position.y && optionsCadre[3].y + optionsCadre[3].height>= cursor_position.y))
        gameState = 4;
}

void GestionnaireDeJeux::afficherScrores(){}

void GestionnaireDeJeux::afficherParametres(){
    Color fontColor = BLUE;
    float fontSize = windowWidth / 25;

    //affichage du nom du jeu
    DrawText("--Niveau--", (windowWidth - MeasureText("--Niveau--", fontSize))/2, heightSubdivision, fontSize, fontColor);

    //Retour
    parametersCadre[5].x = 7 * widthSubdivision;
    parametersCadre[5].y = heightSubdivision;
    parametersCadre[5].width = MeasureText("Retour", fontSize);
    parametersCadre[5].height = heightSubdivision;
    DrawText("Retour", 7 * widthSubdivision, heightSubdivision, fontSize, fontColor);


    //affichage du niveau facile  
    parametersCadre[0].x = (windowWidth - MeasureText(levelText[0].c_str(), fontSize))/2 ;
    parametersCadre[0].y = 2 * heightSubdivision;
    parametersCadre[0].width = MeasureText(levelText[0].c_str(), fontSize);
    parametersCadre[0].height = heightSubdivision;
    DrawText(levelText[0].c_str(), parametersCadre[0].x, parametersCadre[0].y, fontSize, fontColor);
    
    //affichage du niveau moyen
    parametersCadre[1].x = (windowWidth - MeasureText(levelText[1].c_str(), fontSize))/2 ;
    parametersCadre[1].y = 3 *heightSubdivision;
    parametersCadre[1].width = MeasureText(levelText[1].c_str(), fontSize);
    parametersCadre[1].height = heightSubdivision;
    DrawText(levelText[1].c_str(), parametersCadre[1].x, parametersCadre[1].y, fontSize, fontColor);
    
    //affichage du niveau Difficile
    parametersCadre[2].x = (windowWidth - MeasureText(levelText[2].c_str(), fontSize))/2 ;
    parametersCadre[2].y = 4 *heightSubdivision;
    parametersCadre[2].width = MeasureText(levelText[2].c_str(), fontSize);
    parametersCadre[2].height = heightSubdivision;
    DrawText(levelText[2].c_str(), parametersCadre[2].x, parametersCadre[2].y, fontSize, fontColor);
    
    //Joue la musique?
    DrawText("--MUSIQUE--", (windowWidth - MeasureText("--MUSIQUE--", fontSize))/2, 5 * heightSubdivision, fontSize, fontColor);

    parametersCadre[3].x = (windowWidth - MeasureText("ON", fontSize))/2 ;
    parametersCadre[3].y = 6 *heightSubdivision;
    parametersCadre[3].width = MeasureText("ON", fontSize);
    parametersCadre[3].height = heightSubdivision;
    DrawText("ON", parametersCadre[3].x, parametersCadre[3].y, fontSize, fontColor);

    
    parametersCadre[4].x = (windowWidth - MeasureText("OFF", fontSize))/2 ;
    parametersCadre[4].y = 7 *heightSubdivision;
    parametersCadre[4].width = MeasureText("OFF", fontSize);
    parametersCadre[4].height = heightSubdivision;
    DrawText("OFF", parametersCadre[4].x, parametersCadre[4].y, fontSize, fontColor);
}

void GestionnaireDeJeux::checkParameters() {
    Vector2 cursor_position = GetMousePosition();
    
    if((parametersCadre[0].x <= cursor_position.x && parametersCadre[0].x + parametersCadre[0].width>= cursor_position.x) && (parametersCadre[0].y <= cursor_position.y && parametersCadre[0].y + parametersCadre[0].height>= cursor_position.y))
        level = 1;
    else if((parametersCadre[1].x <= cursor_position.x && parametersCadre[1].x + parametersCadre[1].width>= cursor_position.x) && (parametersCadre[1].y <= cursor_position.y && parametersCadre[1].y + parametersCadre[1].height>= cursor_position.y))
        level = 2;
    else if((parametersCadre[2].x <= cursor_position.x && parametersCadre[2].x + parametersCadre[2].width>= cursor_position.x) && (parametersCadre[2].y <= cursor_position.y && parametersCadre[2].y + parametersCadre[2].height>= cursor_position.y))
        level = 3;
    else if((parametersCadre[3].x <= cursor_position.x && parametersCadre[3].x + parametersCadre[3].width>= cursor_position.x) && (parametersCadre[3].y <= cursor_position.y && parametersCadre[3].y + parametersCadre[3].height>= cursor_position.y))
        PlayMusicStream(titre);
    else if((parametersCadre[4].x <= cursor_position.x && parametersCadre[4].x + parametersCadre[4].width>= cursor_position.x) && (parametersCadre[4].y <= cursor_position.y && parametersCadre[4].y + parametersCadre[4].height>= cursor_position.y))
        StopMusicStream(titre);
    else if((parametersCadre[5].x <= cursor_position.x && parametersCadre[5].x + parametersCadre[5].width>= cursor_position.x) && (parametersCadre[5].y <= cursor_position.y && parametersCadre[5].y + parametersCadre[5].height>= cursor_position.y))
        gameState = 0;
}

void GestionnaireDeJeux::chargementCredits() {
        string path = "credits";
        creditsPath = LoadDirectoryFiles(path.c_str());
        string credits;
        for (int i = 0; i < creditsPath.count ; i++)
        {   
            char *texte = LoadFileText(creditsPath.paths[i]);
            credits.append(texte);
            UnloadFileText(texte);
        }

    creditsTexte.append(credits.c_str());
    UnloadDirectoryFiles(creditsPath);
    creditsPolice = LoadFont("fonts/Playfair_Display/static/PlayfairDisplay-Bold.ttf");
    creditsLoaded = true;
}


void GestionnaireDeJeux::afficherCredits() {
    if (!creditsLoaded){
        chargementCredits();
    }
    else 
    {   
        //Retour
        parametersCadre[5].x = (float)(windowWidth - (MeasureText("Retour", 25.0) + 20));
        parametersCadre[5].y = 0.0;
        parametersCadre[5].width = MeasureText("Retour", 25.0);
        parametersCadre[5].height = heightSubdivision;
        DrawTextEx(creditsPolice, "Retour", {(float)(windowWidth - (MeasureText("Retour", 25.0) + 20) ), 0.0}, 25.0, 2.0, BLACK);

        DrawTextEx(creditsPolice, creditsTexte.c_str(), {0.0, 20.0}, 20.0, 2.0, BLACK);
    }
}
void GestionnaireDeJeux::checkCredits() {
    Vector2 cursor_position = GetMousePosition();
    if((parametersCadre[5].x <= cursor_position.x && parametersCadre[5].x + parametersCadre[5].width>= cursor_position.x) && (parametersCadre[5].y <= cursor_position.y && parametersCadre[5].y + parametersCadre[5].height>= cursor_position.y))
        gameState = 0;
}

void GestionnaireDeJeux::toggleMaximize() {
    
            if (!isMaximized) {
                SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
                SetWindowPosition(0, 0);
            } else {
                SetWindowSize(800, 600);  // Retour à la taille par défaut
                SetWindowPosition((GetMonitorWidth(0) - 800) / 2, (GetMonitorHeight(0) - 600) / 2);
            }
            isMaximized = !isMaximized;

            //recuperation des dimensions de la fenetre
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();
            widthSubdivision = windowWidth / 9;
            heightSubdivision = windowHeight / 8;
}


void GestionnaireDeJeux::lancerPartie() {
        switch (level)
        {
            case 1:
                labyrinthe_jeux = new Labyrinthe("images/brick_brown_0.png", 15, 15, 50);
                cout<<"1"<<endl;
                break;
            case 2: 
                labyrinthe_jeux = new Labyrinthe("images/brick_brown_0.png", 20, 18, 50);
                cout<<"2"<<endl;
                break;
            case 3: 
                labyrinthe_jeux = new Labyrinthe("images/brick_brown_0.png", 30, 18, 50);
                cout<<"3"<<endl;
                break;
            default:
                break;
        }
        labyrinthe_jeux->initialisation_du_labyrinthe();
        joueur_1 = new Joueur(*(labyrinthe_jeux));
        starting_game = GetTime();
        StopMusicStream(titre);
        isGameLaunched = true;
        isGameFinished = false;
}

void GestionnaireDeJeux::checkDeplacement() {

        if(!isGameFinished) {
            if (IsKeyDown(KEY_RIGHT)) joueur_1->seDeplacer(*labyrinthe_jeux, 1);
            if (IsKeyDown(KEY_LEFT)) joueur_1->seDeplacer(*labyrinthe_jeux, 3);
            if (IsKeyDown(KEY_UP)) joueur_1->seDeplacer(*labyrinthe_jeux, 0);
            if (IsKeyDown(KEY_DOWN)) joueur_1->seDeplacer(*labyrinthe_jeux, 2);
        }
}

void GestionnaireDeJeux::checkResultat() {
    if(joueur_1->gagner(*labyrinthe_jeux))
    {
        DrawTextEx(policeGameOptions, "Vous avez gagné", {(float)(windowWidth - (MeasureText("Vous avez gagné", 25.0) + 20) ), 100.0}, 25.0, 2.0, BLACK);
        isGameFinished = true;
    } 
}

void GestionnaireDeJeux::gameOptions(){

    
    Color couleur = BLUE;
    float fontSize = windowWidth / 45;

    //met à jour le temps si le jeux n'est pas encore fini
    if (!isGameFinished)
       chronoText =  chronometre();

    
    DrawTextEx(policeGameOptions, chronoText.c_str(), {(float)(8 * widthSubdivision), (float)heightSubdivision}, fontSize, 2.0, couleur);

    gameOptionsCadre[0].x = (float)(windowWidth - (MeasureText("Reconmmencer", fontSize) + 20));
    gameOptionsCadre[0].y = 4 * heightSubdivision;
    gameOptionsCadre[0].width = MeasureText("Reconmmencer", fontSize);
    gameOptionsCadre[0].height = heightSubdivision/2;
    DrawTextEx(policeGameOptions, "Reconmmencer", {(float)(windowWidth - MeasureText("Reconmmencer", fontSize) ), (float)(4.0 * heightSubdivision)}, fontSize, 2.0, couleur);
    
    gameOptionsCadre[1].x = (float)(windowWidth - (MeasureText("Retour", fontSize) + 20));
    gameOptionsCadre[1].y = 5 * heightSubdivision;
    gameOptionsCadre[1].width = MeasureText("Retour", fontSize);
    gameOptionsCadre[1].height = heightSubdivision/2;
    DrawTextEx(policeGameOptions, "Retour", {(float)(windowWidth - (MeasureText("Retour", fontSize) + 20)), (float)(5.0 * heightSubdivision)}, fontSize, 2.0, couleur);

    
}

void GestionnaireDeJeux::checkGameOptions() {

    Vector2 cursor_position = GetMousePosition();
    if((gameOptionsCadre[0].x <= cursor_position.x && gameOptionsCadre[0].x + gameOptionsCadre[0].width>= cursor_position.x) && (gameOptionsCadre[0].y <= cursor_position.y && gameOptionsCadre[0].y + gameOptionsCadre[0].height>= cursor_position.y))
        {
            isGameLaunched = false;
            isGameFinished = false;
        }
    else if((gameOptionsCadre[1].x <= cursor_position.x && gameOptionsCadre[1].x + gameOptionsCadre[1].width>= cursor_position.x) && (gameOptionsCadre[1].y <= cursor_position.y && gameOptionsCadre[1].y + gameOptionsCadre[1].height>= cursor_position.y))
        {
            gameState = 0;
            isGameLaunched = false;
            isGameFinished = true;
            if(!isGameLaunched)
                PlayMusicStream(titre);
        }

    checkResultat();
}


/*void GestionnaireDeJeux::afficherScores() {
    char directory[8] = "scores";
    char file[28] = "scores/meilleurs_scores.txt";
    string texte;
    if(DirectoryExists(directory))
        if(FileExists(file))
            texte = LoadFileText(file);
    
}*/
GestionnaireDeJeux::~GestionnaireDeJeux()
{
    UnloadFont(police);
    UnloadFont(policeGameOptions);
    UnloadImage(logo);


    if(fondLoaded) {
        UnloadTexture(fondTexture);
        UnloadImage(fond);
    } 
    if(audioLoaded){
        UnloadMusicStream(titre);
        UnloadDroppedFiles(musiques);
    }
    if(creditsLoaded) {
        UnloadFont(creditsPolice);
    }

    if(labyrinthe_jeux != nullptr)
        delete labyrinthe_jeux;
    if(joueur_1 != nullptr)
        delete joueur_1;
}