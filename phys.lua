width = 800
height = 600
pi = 3.14

rl.SetTraceLogLevel(rl.LOG_WARNING)
rl.SetConfigFlags(rl.FLAG_VSYNC_HINT)
rl.InitWindow(width, height, "physique");
rl.SetTargetFPS(60)

rl.InitPhysics()

-- murs pour délimiter la zone

local bas = rl.CreatePhysicsBodyRectangle({width/2,height},width,1,10)
bas.enabled = false

local haut = rl.CreatePhysicsBodyRectangle({width/2,0},width,1,10)
haut.enabled = false

local gauche = rl.CreatePhysicsBodyRectangle({0,height/2},1,height,10)
gauche.enabled = false

local droite = rl.CreatePhysicsBodyRectangle({width, height/2},1,height,10)
droite.enabled = false

local ptr = rl.CreatePhysicsBodyPolygon({width/2, height/2}, 20, 4, 1)
ptr.enabled = false
rl.SetPhysicsBodyRotation(ptr, pi/4)

local jok = rl.CreatePhysicsBodyPolygon({200,200}, 20, 5, 1)

local damping = 0.95 -- ralentissement du mouvement
local longueur = 100 -- longueur de l'élastique
local raideur = 1 -- raideur de l'élastique

rl.SetPhysicsGravity(0,5)

while not rl.WindowShouldClose() do
    -- mise à jour de la position du pointeur
    ptr.position = {x=rl.GetMouseX(), y=rl.GetMouseY()}

    local elastique = (ptr.position - jok.position)
    local len = rl.Vector2Length(elastique)
    elastique = rl.Vector2Normalize(elastique)
    local coef = math.max(0, len - longueur) * raideur
    rl.PhysicsAddForce(jok, elastique * coef )

    -- ralentissement des mouvements en l'absence de contact
    jok.velocity = jok.velocity * damping
    jok.angularVelocity = jok.angularVelocity * damping
  
    rl.UpdatePhysics()
    rl.BeginDrawing()

    rl.ClearBackground(rl.BLACK)

    -- dessiner une ligne entre ptr et jok
    rl.DrawLineV(ptr.position, jok.position, rl.RED)
    
    -- dessiner les objets physiques
    local count = rl.GetPhysicsBodiesCount()
    for i=0,count-1 do
        local body = rl.GetPhysicsBody(i)

        if body ~= nil then
          local vertexCount = rl.GetPhysicsShapeVerticesCount(i)
          for j=0,vertexCount-1 do
            local vertexA = rl.GetPhysicsShapeVertex(body, j);

            local jj = ((j + 1) < vertexCount) and (j + 1) or 0
            local vertexB = rl.GetPhysicsShapeVertex(body, jj);

            rl.DrawLineV(vertexA, vertexB, rl.GREEN)
          end
        end
    end
    rl.EndDrawing()
end

rl.ClosePhysics()

rl.CloseWindow()
