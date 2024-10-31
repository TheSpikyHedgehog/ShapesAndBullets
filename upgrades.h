#ifndef UPGRADES_H
#define UPGRADES_H
#include <raylib.h>
#include "main.h"

class Button {
    public:
    Vector2 mousePoint;
    int btnState = 0;
    int btnAction = 0;
    Texture2D image;
    Rectangle btnBounds;
    int place;
    int screenwidth;
    int screenheight;
    Image CPUimage;
    Image* CPUimageptr;
    int type; // Type of upgrade

    Button();

    void draw();

    void destroy();

    void setBounds();

    void onClick();

    void update();

};

class SpeedUpgrade : public Button {
    public:
    float newSpeedToAdd;
    // int type;
    SpeedUpgrade(int lvl, int place, int screenwidth, int screenheight);

};
#endif