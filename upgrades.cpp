#include "upgrades.h"
#include <raylib.h>
#include <iostream>
#include "main.h"
using namespace std;

// Button Base Class

Button::Button() {

};

void Button::setBounds() {
    if (place == 1) {
        this->btnBounds = {screenwidth/3.0f - 300, screenheight/3.0f - image.height/2.0f, (float) image.width, (float) image.height};
    }   
    else if (place == 2) {
        this->btnBounds = {screenwidth/2.0f - 300, screenheight/2.0f - image.height/2.0f, (float) image.width, (float) image.height};
    }
    else {
        this->btnBounds = {(int) screenwidth - image.width/1.0f, screenheight - image.height/2.0f, (float) image.width, (float) image.height};
    }   
};
    
void Button::draw() {
    DrawTextureRec(image, {0, 0, 200, 200}, (Vector2) {btnBounds.x, btnBounds.y}, WHITE);
    // DrawRectangleRec(btnBounds, RED);
    // DrawTextureRec(image, btnBounds, (Vector2) {btnBounds.x, btnBounds.y}, WHITE);
}
    
void Button::destroy() {
    UnloadTexture(image);
    delete CPUimageptr;
}


void Button::update() {
    mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, btnBounds)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            onClick();
            return;
        }
    }
}

void Button::onClick() {
    // if (type == 0) {
    //     cout << "works";
    // }
}



SpeedUpgrade::SpeedUpgrade(int lvl, int place, int screenwidth, int screenheight) {   
    newSpeedToAdd = lvl / 1000;
    this->place = place;
    this->screenwidth = screenwidth;
    this->screenheight = screenheight;
    setBounds();
    CPUimage = LoadImage("data/images/speed_upgrade.png");
    this->image = LoadTextureFromImage(CPUimage);
    // this->type = 0;
};

 // PlayerProperties player