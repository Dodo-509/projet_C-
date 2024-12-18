#include "raylib.h"
#include "gestionnaireDeJeux.h"

int main() {
    InitWindow(800, 600, "Simuler le bouton Maximiser");
    InitAudioDevice(); 
    SetTargetFPS(60);
    char icon[] = "images/logo.png";
    GestionnaireDeJeux gestionnaire(icon);
    
    

    while (!WindowShouldClose()) {
        

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (IsKeyPressed(KEY_F11)) { gestionnaire.toggleMaximize();}
        
        gestionnaire.refresh();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
