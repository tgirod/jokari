#include <stdio.h>

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "rlgl.h"

#define PHYSAC_IMPLEMENTATION
#include "extras/physac.h"

// point d'entrée du programme
int main(void) {

  // on définit quelques constantes ...
  const int screenWidth = 800;
  const int screenHeight = 800;
  const float longueur_elastique = 50;
  const float ralentissement = 0.95;

  // structure pour représenter le pointeur. j'aurai pu mettre juste le vecteur
  // de position, mais si jamais je veux lui ajouter quelque chose j'ai juste à
  // modifier ici.
  struct {
    PhysicsBody body;
  } ptr;

  ptr.body = CreatePhysicsBodyRectangle(
      (Vector2){(float)screenWidth / 2, (float)screenHeight / 2}, 40, 20, 1);
  ptr.body->enabled = false;

  // structure pour représenter le jokari. ici, j'ai besoin de la position
  // (vecteur depuis l'origine) et de la vélocité (vecteur de mouvement)
  struct {
    PhysicsBody body;
  } jok;

  jok.body = CreatePhysicsBodyPolygon(
      (Vector2){(float)screenWidth / 2, (float)screenHeight / 2}, 20, 5, 1);

  Vector2 force;

  // on créé la fenêtre aux dimensions demandées
  InitWindow(screenWidth, screenHeight, "Extreme Jokari");

  InitPhysics();

  SetTargetFPS(60); // 60 FPS, c'est bien

  // moteur physique
  InitPhysics();

  // la boucle principale !
  while (!WindowShouldClose()) // termine si on appuie sur ESC
  {

    // je récupère les coordonnées de la souris sur l'écran. Vu que j'utilise
    // une caméra, je dois convertir les coordonnées sur l'écran en coordonnées
    // dans le "monde"
    ptr.body->position = GetMousePosition();

    // calcul du vecteur de force. c'est un peu long mais ça fait ce qu'on veut

    force = Vector2Subtract(ptr.body->position, jok.body->position);
    const float length = Vector2Length(force);
    force = Vector2Normalize(force);

    force = Vector2Scale(force, Clamp(length - longueur_elastique, 0, 1000) * 0.5);
    PhysicsAddForce(jok.body, force);

    // et on réduit un peu la vélocité pour que les mouvements s'atténuent
    jok.body->velocity = Vector2Scale(jok.body->velocity, ralentissement);
    jok.body->angularVelocity *= ralentissement;

    UpdatePhysics();

    // il est temps de dessiner tout ça !
    BeginDrawing();

    // cls()
    ClearBackground(BLACK);

    for (int i = 0; i < GetPhysicsBodiesCount(); i++) {
      const PhysicsBody body = GetPhysicsBody(i);
      const int vCount = GetPhysicsShapeVerticesCount(i);
      for (int j = 0; j < vCount; j++) {
        const int jj = (j + 1) % vCount;
        DrawLineV(GetPhysicsShapeVertex(body, j),
                  GetPhysicsShapeVertex(body, jj), GREEN);
      }
    }

    DrawLineV(ptr.body->position, jok.body->position, RED);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
