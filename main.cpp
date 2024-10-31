#include <iostream>
#include <raylib.h>
#include <list>
#include <cmath>
#include <cstring> 
#include <string>
#include "bosses.h"
#include "upgrades.h"
#include "main.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else       // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

#define SCREENWIDTH                 1260
#define SCREENHEIGHT                820

using namespace std; // Use std namespace

bool hitboxes_enabled = false;

typedef struct Sprite {
    Texture2D texture;
    Rectangle rect;
} Sprite;

class MenuStar
{
    public:
    float x;
    float y;
    int radius;
    MenuStar(float x, float y) {
        this -> x = x;
        this -> y = y;
        this -> radius = (2) + (rand() % 5);
    }

    void draw() {
        DrawCircle(this->x, this->y, this->radius, WHITE);
    }
};

class Indicator 
{

public:
    float x;
    float y;
    const char * text;
    float decayTime; // Time it takes for the text to disappear.
    int fontSize;
    Color color;
    float startTime;

    Indicator(float x, float y, const char * text, float decayTime, int fontSize, int id) {
        this -> x = x;
        this -> y = y;
        this -> text = text;
        this -> decayTime = decayTime;
        this -> fontSize = fontSize;
        this -> startTime = GetTime();
        if (id == 0) {
            color = WHITE;
        }
        else if (id == 1)
        {
            color = YELLOW;
        }
        else if (id == 2)
        {
            color = RED;
        }
        else if (id == 3)
        {
            color = GREEN;
        }
        else if (id == 4)
        {
            color = ORANGE;
        }
        
    }

    bool update() {
        float curTime = GetTime();
        if (curTime - startTime> decayTime) {
            return true; // If return true, then in main loop remove instance from list and destroy it
        } else {
            DrawText(text, (int) x, (int) y, fontSize, color);
            return false;
        }
        
    }

};

// ------------------------ GOTO `main.h` -----------------------------   
PlayerProperties::PlayerProperties() {
    this->playerSpeed = 3.0f;
    this->pHealth = 20;
    this->pMaxHealth = 20;
    this->shootCooldown = 1.0f;
    this->playerLvl = 0;
    this->bulletColor = 1;
    this->bulletDmg = 5;
};       


class Enemy
{
public:
    float x;
    float y;
    int lvl; // 0=Triangle, 1 = Square, 2 = Pentagon, ...  The higher the number, the more powerful;
    float speed;
    Vector2 pos;
    Rectangle hitbox;
    float health;
    float maxHealth;
    int xp;
    int sides;
    Enemy(float x, float y, int lvl)
    {
        this->x = x;
        this->y = y;
        this->lvl = lvl;
        this->pos = {x, y};
        if (lvl == 0) {
            this->speed = 1.5f;
            this->hitbox = {x - 20, y - 20, 20.0f, 20.0f};
            health = 10;
            maxHealth = 10;
            xp = 4;
            sides = 3;
            
        }
        else if (lvl == 1) {
            this->speed = 1.7f;
            hitbox = {x - 20, y - 20, 20.0f, 20.0f};
            health = 15;
            maxHealth = 15;
            xp = 5;
            sides = 4;
        }
        
    }

    void draw(Vector2 playerPos)
    {
        update(playerPos);
        switch (lvl)
        {
        case 0:
            DrawPoly(pos, sides, 20.0f, 0.0f, WHITE);
        case 1:
            DrawPoly(pos, sides, 20.0f, 0.0f, WHITE);
        }
        
        
        if (hitboxes_enabled) {
            DrawRectangleRec(hitbox, RED);
        }   
    }

    void update(Vector2 playerPos)
    {

        float distanceX = playerPos.x - x;
        float distanceY = playerPos.y - y;

        float distance = sqrt(distanceX * distanceX + distanceY * distanceY);
        if (distance > 0) {
            distanceX /= distance;
            distanceY /= distance;
            x += distanceX * speed;
            y += distanceY * speed;
        }
        pos.x = x;
        pos.y = y;
        hitbox.x = x - 10;
        hitbox.y = y - 10;
    }

    void attack()
    {
    
    }
};

class Bullet
{
public:
    Rectangle bulletHitbox;
    float x;
    float y;
    float speed = 5.0f;
    float angle;
    bool harmfulToPlayer;
    Color bulletColor = YELLOW;
    int dmg;
    Bullet(float x, float y, float angle, bool harmfulToPlayer, int color, int dmg)
    {
        // Color = 0 WHITE
        // Color = 1 YELLOW
        // Color = 2 ORANGE
        // Color = 3 RED
        // Color = 4 PINK
        // Color = 5 DARK_PINK
        this->x = x;
        this->y = y;
        this->angle = angle;
        this->harmfulToPlayer = harmfulToPlayer;
        this->dmg = dmg;
        if (color == 0) {
            this->bulletColor = WHITE;
        }
        else if (color == 1) {
            this->bulletColor = YELLOW;
        }
        else if (color == 2) {
            this->bulletColor = ORANGE;
        }
        else if (color == 3) {
            this->bulletColor = RED;
        }
        else if (color == 4) {
            this->bulletColor = PINK;
        }
        else if (color == 5) {
            this->bulletColor = MAGENTA;
        }
        bulletHitbox = {x - 10, y - 10, 10.0f, 10.0f};
    }

    void draw()
    {
        update(); 
        DrawCircle(this->x, this->y, 4, this->bulletColor);
        if (hitboxes_enabled) {
            DrawRectangleRec(bulletHitbox, RED);
        }   
    }

    void update()
    {
        this->x += cos(this->angle) * speed;
        this->y += sin(this->angle) * speed;
        this->bulletHitbox.x = x - 5;
        this->bulletHitbox.y = y - 5;
    }
};

char* convertIntegerToChar(int N)  // ... ;-;
{
 
    // Count digits in number N
    int m = N;
    int digit = 0;
 
    while (m > 0) {
        digit++;
        m /= 10;
    }
 

    char* arr = new char[digit];

    char arr1[digit + 1];
 
    int index = 0;
 
    while (N > 0) {
        index++;
        arr1[index] = static_cast<char>(N % 10 + 48);
        N /= 10;
    }
 
    // Reverse the array for result
    for (int i = 0; i < index; i++) {
        arr[i] = arr1[index - i];
    }
 
    // Char array truncate by null
    arr[index] = '\0';
 
    // Return char array
    return arr;
}

// Pause menu 
int pause() {
    // Declare list of stars in the menu
    std::list<MenuStar> stars{}; // Stars! :D
    for (int i=0;i<100;i++) {
        float randX = rand() % SCREENWIDTH;
        float randY = rand() % SCREENHEIGHT;
        // Push the star to the list
        stars.push_front(MenuStar(randX, randY));
    }

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        for (std::list<MenuStar>::iterator j = stars.begin(); j != stars.end(); ++j) {
            j->draw();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

int singleplayer() {
    int enemyCap = 4; // But will increase as time goes on.
    // Lists! :)
    std::list<Bullet> bullets{};
    std::list<Enemy> enemies{};
    std::list<Indicator> indicators{};
    // indicators.push_front(Indicator(200.0f, 200.0f, "test", 1, 50, 0));
    //----- Player Setup --------
    float lastTimeShot = 0.0f;
    PlayerProperties playerVars = PlayerProperties();
    Vector2 playerPos = {(float)SCREENWIDTH / 2, (float)SCREENHEIGHT / 2};
    float lastTimeDmged = 0.0f;
    float lastEnemySpawned = 0.0f;
    float spawnTime = 2.0f;
    float iFrame = 0.5f;
    int enemiesKilled = 0;
    int exp = 0;
    string username = "Dev";
    bool paused = false;
    bool upgrade_mn = false;
    Rectangle playerHitbox = {playerPos.x, playerPos.y, 22.0f, 22.0f};
    indicators.push_front(Indicator(SCREENWIDTH/2 - 250, SCREENHEIGHT/2, "Press \"SPACE\" to open upgrade menu!", 5, 25, 0));
    Button upgrade1 = SpeedUpgrade(1, 1, SCREENWIDTH, SCREENHEIGHT);
    Button upgrade2;
    Button upgrade3;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime() * 60; // To make smooth movements and not make a crappy game
        // Update
        // ---------------------------------------------------------------------------
        float currentTime = GetTime();

        if (!upgrade_mn) {
            if (IsKeyDown(KEY_D) && playerPos.x < SCREENWIDTH - 17.5)
                playerPos.x += playerVars.playerSpeed * dt;
            if (IsKeyDown(KEY_A) && playerPos.x > 17.5)
                playerPos.x -= playerVars.playerSpeed * dt;
            if (IsKeyDown(KEY_W) && playerPos.y > 17.5)
                playerPos.y -= playerVars.playerSpeed * dt;
            if (IsKeyDown(KEY_S) && playerPos.y < SCREENHEIGHT - 17.5)
                playerPos.y += playerVars.playerSpeed * dt;
            if (IsKeyPressed(KEY_H)) {
                if (hitboxes_enabled) {
                    hitboxes_enabled = false;
                } else {
                    hitboxes_enabled = true;
                } 
            }
            if (IsKeyPressed(KEY_P)) {
                pause();
                break;
            }
            if (IsKeyPressed(KEY_SPACE)) {
                upgrade_mn = true;
            }
            playerHitbox.x = playerPos.x - 11;
            playerHitbox.y = playerPos.y - 11;

            double rotation = atan2(GetMouseY() - playerPos.y, GetMouseX() - playerPos.x);
            // Draw
            // ---------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(BLACK);
            DrawRing(playerPos, 15.0f, 20.0f, 0.0f, 360.0f, 0, YELLOW);
            string stat = "Exp: " + to_string(exp) + "  Enemies Killed: " + to_string(enemiesKilled);
            const int length = stat.length(); 
            // declaring character array (+1 for null terminator) 
            char* char_array = new char[length + 1]; 
        
            // copying the contents of the 
            // string to char array 
            strcpy(char_array, stat.c_str()); 
            DrawText(char_array, SCREENWIDTH/2 - length*5, 20, 25, WHITE);

            string tag = username + ": " + to_string((int)playerVars.pHealth) + "/" + to_string((int)playerVars.pMaxHealth);
            const int taglength = tag.length(); 
            // declaring character array (+1 for null terminator) 
            char* array = new char[taglength + 1]; 
        
            // copying the contents of the 
            // string to char array 
            strcpy(array, tag.c_str());
            DrawText(array, playerPos.x-taglength*6.5, playerPos.y-45, 25, WHITE);
            if (hitboxes_enabled) {
                DrawRectangleRec(playerHitbox, RED);
            }   
            if (enemies.size() <= 4) {
                if (currentTime - lastEnemySpawned > spawnTime) {
                    int randX = rand() % SCREENWIDTH;
                    int randY = rand() % SCREENHEIGHT;
                    if (enemiesKilled >= 0 && enemiesKilled < 30) {
                        enemies.push_front(Enemy(randX, randY, 0));
                    }
                    if (enemiesKilled >= 30 && enemiesKilled < 80) {
                        enemies.push_front(Enemy(randX, randY, 1));
                    }
                    
                    lastEnemySpawned = GetTime();
                }
            }

            if (currentTime - lastTimeShot > playerVars.shootCooldown)
            { // Firing Cooldown
                bullets.push_front(Bullet(playerPos.x, playerPos.y, rotation, false, playerVars.bulletColor, playerVars.bulletDmg));
                lastTimeShot = GetTime();
            }
            for (std::list<Enemy>::iterator i = enemies.begin(); i != enemies.end(); ++i)
            {
                if (i->x > playerPos.x + SCREENWIDTH || i->x < playerPos.x - SCREENWIDTH || i->y > playerPos.y + SCREENHEIGHT || i->y < playerPos.y - SCREENHEIGHT)
                {
                    i = enemies.erase(i);
                }
                else
                {
                    i->draw(playerPos);
                    if (CheckCollisionRecs(i->hitbox, playerHitbox))
                    {
                        if (currentTime - lastTimeDmged > iFrame) {
                            playerVars.pHealth -= 5;
                            lastTimeDmged = GetTime();
                            int randX = (playerPos.x-20) + (rand() % 20);
                            indicators.push_front(Indicator(randX, playerPos.y-40, "5", 1, 25, 1));
                            cout << "[DEBUG]   Player Damaged!" << endl;
                        }  
                    }
                    for (std::list<Bullet>::iterator j = bullets.begin(); j != bullets.end(); ++j) {
                        if (CheckCollisionRecs(i->hitbox, j->bulletHitbox) && !j->harmfulToPlayer) {
                            i->health -= j->dmg;
                            j = bullets.erase(j);
                            int randX = (i->x-20) + (rand() % 20);

                            indicators.push_front(Indicator(randX, i->y-40, convertIntegerToChar(playerVars.bulletDmg), 1, 25, 0));
                            if (i->health<=0) {
                                exp += i->xp;
                                enemiesKilled += 1;
                                i = enemies.erase(i);
                            }
                        }
                    }
                }
            }
            for (std::list<Bullet>::iterator j = bullets.begin(); j != bullets.end(); ++j)
            {
                if (j->x > playerPos.x + SCREENWIDTH || j->x < playerPos.x - SCREENWIDTH || j->y > playerPos.y + SCREENHEIGHT || j->y < playerPos.y - SCREENHEIGHT)
                {
                    j = bullets.erase(j);
                }
                else
                {
                    j->draw();
                }
            }
            for (std::list<Indicator>::iterator j = indicators.begin(); j != indicators.end(); ++j) {
                if (j -> update()) {
                    j = indicators.erase(j);
                } else {

                }
            }
            EndDrawing();
            delete[] char_array;
            delete[] array; 
        }   
        else {
            // ---------------- UPDATE -----------------
            upgrade1.update();
            if (IsKeyPressed(KEY_SPACE)) {
                upgrade_mn = false;
            }
            // ---------------- DRAW -------------------
            BeginDrawing();
            ClearBackground(BLACK);
            upgrade1.draw();  
            DrawText("Press \"SPACE\" to EXIT.", 420, 100, 35, WHITE);
            EndDrawing();
        }

    }
    upgrade1.destroy();
    upgrade2.destroy();
    upgrade3.destroy();
    CloseWindow();
    CloseAudioDevice();
    return 0;
}

int main()
{   
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Shapes And Bullets");
    InitAudioDevice();
    SetTargetFPS(60);
    // SetWindowState(FLAG_WINDOW_RESIZABLE);
    while(true) {
        singleplayer();
        break;
    }
}