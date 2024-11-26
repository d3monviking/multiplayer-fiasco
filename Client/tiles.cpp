#include <bits/stdc++.h>
#include "tiles.h"

using namespace std;

Tile::Tile(sf::Sprite sprite){
    // this->pos=pos;
    this->coords=sprite.getPosition();
    this->vel=sf::Vector2f(0,0);
    this->acc=sf::Vector2f(0,0);
    // this->surface.setFillColor(sf::Color(120, 83, 38));
    // this->surface.setPosition(pos);
    // this->surface.setTextureRect(sf::IntRect(texturePos.x, texturePos.y, 16, 16));
    this->surface = sprite;
}

void Tile::update(float x_shift, float y_shift){
    this->pos.x += x_shift;
    this->pos.y += y_shift;
    // cout<<"tile:"<<y_shift<<" "<<pos.y<<endl;
    this->surface.setPosition(pos);
}

Collectibles::Collectibles(sf::Vector2f coords, sf::Sprite newSprite){
        this->coords=coords;
        this->surface.setPosition(coords);
        this->surface = newSprite;
    }
    PowerUp::PowerUp(sf::Vector2f coords,float speedBoost, char type, sf::Sprite newSprite)
     :Collectibles(coords, newSprite)
     {
            // this->surface.setFillColor(sf::Color(29, 22, 128));
            // this->surface.setSize(tileSize);
            this->speedBoost=speedBoost;
            this->type=type;
            this->surface = newSprite;
        }
    void PowerUp::applyBoost(float &player_speed) {
        if (!isBoostActive) {
            player_speed *= speedBoost; 
            isBoostActive = true;      
            boostClock.restart();      
        }
    //     if (isBoostActive && boostClock.getElapsedTime().asSeconds() >= 2.0f) {
    //         isBoostActive = false;      
    //         // self.boostActive=false; 
    //         // player_speed /w2= speedBoost; 
    // }
    }

    // Update function to revert the boost
    void PowerUp::updateBoost(float &player_speed, float original_speed) {
        if (isBoostActive && boostClock.getElapsedTime().asSeconds() >= 5.0f) {
            player_speed = original_speed; // Revert to original speed
            isBoostActive = false;        // Reset the boost flag
        }
    }
    float PowerUp::getBoost(){return speedBoost;}
    char PowerUp::getType(){return 'P';}

    Shell::Shell(sf::Vector2f coords,char type,bool kicked,bool held, sf::Sprite newSprite)
    :Collectibles(coords, newSprite){
        // this->surface.setFillColor(sf::Color(128, 22, 23));
        // this->surface.setSize(tileSize);
        this->type=type;
        this->kicked=kicked;
        this->held=held;
    };

    char Shell::getType(){return 'S';}
MovingPlatform::MovingPlatform(sf::Sprite sprite):Tile(sprite){
    this->vel.x = 1;
    this->initialCoords = coords;
    movementClock.restart();
}
float MovingPlatform::velocity = 1;
bool MovingPlatform::mvLeft = true; // Static member definition
void MovingPlatform::movePlatform(){
    if(movingLeft){
        if(coords.x<=initialCoords.x-100){
            coords.x=initialCoords.x-100;
            movingLeft=false;
            movementClock.restart();
            return;
        }
            coords.x-=vel.x;
            mvLeft=true;
    }   
        else{
            if(coords.x>=initialCoords.x+100){
                coords.x=initialCoords.x+100;
                movingLeft=true;
                movementClock.restart();
                return;
            }
                coords.x+=vel.x;
                mvLeft=false;
        }

        
    surface.setPosition(coords);
}
void MovingPlatform::updateAllPlatforms(std::vector<MovingPlatform*>& movingPlatforms) {
    for (auto* platform : movingPlatforms) {
        if (platform) {
            platform->movePlatform();
        }
    }
}

