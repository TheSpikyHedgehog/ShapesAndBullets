#ifndef MAIN_H
#define MAIN_H

class PlayerProperties {
    public:
    float playerSpeed;
    float pHealth;
    float pMaxHealth;
    float shootCooldown;
    int playerLvl;
    int bulletColor;
    int bulletDmg;

    PlayerProperties();
};

#endif