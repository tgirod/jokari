#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "rlgl.h"
#include <stdio.h>
#include "extras/physac.h"

// point d'entrée du programme
int main(void) {

  // on définit quelques constantes ...
  const int screenWidth = 800;
  const int screenHeight = 800;

  // structure pour représenter le pointeur. j'aurai pu mettre juste le vecteur
  // de position, mais si jamais je veux lui ajouter quelque chose j'ai juste à
  // modifier ici.
  struct {
    Vector2 pos;
  } ptr = {{0}};

  // structure pour représenter le jokari. ici, j'ai besoin de la position
  // (vecteur depuis l'origine) et de la vélocité (vecteur de mouvement)
  struct {
    Vector2 pos; // position
    Vector2 vel; // vélocité
  } jok = {
      // on colle des valeurs initiales pour les deux vecteurs
      {(float)screenWidth / 2, (float)screenHeight / 2},
      {0, 0},
  };

  // on va calculer un vecteur de force qui représente l'élastique entre le
  // pointeur et le jokari
  Vector2 force = {0, 0};
  // et un vecteur de gravité constant
  const Vector2 gravite = {0, 10};

  // on créé la fenêtre aux dimensions demandées
  InitWindow(screenWidth, screenHeight, "Extreme Jokari");

  InitPhysics();

  // fonction un peu plus avancée : je définis une caméra qui va me permettre
  // de scroller sur un écran virtuel plus grand (mais tu connais, c'est
  // similaire dans pico)
  Camera2D camera = {0};
  camera.zoom = 1;

  SetTargetFPS(60); // 60 FPS, c'est bien

  // la boucle principale !
  while (!WindowShouldClose()) // termine si on appuie sur ESC
  {

    // je récupère les coordonnées de la souris sur l'écran. Vu que j'utilise
    // une caméra, je dois convertir les coordonnées sur l'écran en coordonnées
    // dans le "monde"
    ptr.pos = GetScreenToWorld2D(GetMousePosition(), camera);

    // calcul du vecteur de force. c'est un peu long mais ça fait ce qu'on veut

    // on part du vecteur entre les deux points
    force = Vector2Subtract(ptr.pos, jok.pos);
    // on le réduit beaucoup, parce que sinon le jokari va juste se téléporter
    // sur la souris
    force = Vector2Scale(force, 0.05);

    // on ajoute cette force a la vélocité actuelle
    jok.vel = Vector2Add(jok.vel, force);

    // et on ajoute aussi la gravité parce qu'elle est toujours là
    jok.vel = Vector2Add(jok.vel, gravite);

    // et on réduit un peu la vélocité pour que les mouvements s'atténuent
    jok.vel = Vector2Scale(jok.vel, 0.9);

    // finalement on ajoute le vecteur de vélocité à la position pour déplacer
// le jokari
    jok.pos = Vector2Add(jok.pos, jok.vel);

    // scroller l'écran à droite quand on appuie sur la flèche droite
    if (IsKeyDown(KEY_RIGHT)) {
      camera.offset.x -= 10;
    }

    // scroller l'écran à gauche quand on appuie sur la flèche gauche
    if (IsKeyDown(KEY_LEFT)) {
      camera.offset.x += 10;
    }

    // il est temps de dessiner tout ça !
    BeginDrawing();

    // cls()
    ClearBackground(BLACK);

    // on utilise une caméra, alors il faut passer en Mode2D
    BeginMode2D(camera);

    // je dessine une grille pour voir le mouvement. j'ai recopié le code ;)
    rlPushMatrix();
    rlTranslatef(0, 50 * 200, 0);
    rlRotatef(90, 1, 0, 0);
    DrawGrid(100, 200);
    rlPopMatrix();

    DrawLineV(ptr.pos, jok.pos, BLUE); // l'élastique
    DrawCircleV(ptr.pos, 10, RAYWHITE); // le pointeur
    DrawCircleV(jok.pos, 30, RED); // le jokari

    EndMode2D();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
