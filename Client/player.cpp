#include<bits/stdc++.h>
#include "gamestructs.h"
#include "player.h"

using namespace std;

Player::Player(float x, float y, int id){
    this->pos.x=x;
    this->pos.y=y;
    this->id=id;
    this->sprite.setPosition(x, y);
    this->setSprite(id);
    
}
int Player::get_id() {return id;}

void Player::set_id(int id){this->id = id;}

sf::Vector2f Player::getPos() {return pos;}

void Player::setPos(float x,float y) {
    this->pos.x=x;
    this->pos.y=y;
    this->sprite.setPosition(this->pos.x, this->pos.y);
    // cout<<"set:"<<x<<" "<<y<<endl;
}

void Player::moveCam(float x_shift, float y_shift){
    cout<<pos.x<<" "<<pos.y<<endl;
    this->setPos(this->pos.x+x_shift, this->pos.y+y_shift);
    cout<<pos.x<<" "<<pos.y<<endl;
    cout<<"----------"<<endl;
}

void Player::setCoords(float x, float y){
    this->coords.x=x;
    this->coords.y=y;
    this->sprite.setPosition(x, y);
}

sf::Vector2u Player::getDim(){
    return this->dim;
}

void Player::setSprite(int id){
    if(this->id==1){
        this->dim = sf::Vector2u(67, 77);
        this->idleTexture.loadFromFile("../Sprites/Archer/Idle.png");
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(sf::IntRect(18, 50, dim.x, dim.y));
    }
    if(this->id==2){
        this->dim = sf::Vector2u(74, 68);
        this->idleTexture.loadFromFile("../Sprites/Knight/Idle.png");
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(sf::IntRect(25, 60, dim.x, dim.y));
    }
    if(this->id==3){
        this->dim = sf::Vector2u(55, 76);
        this->idleTexture.loadFromFile("../Sprites/Wizard/Idle.png");
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(sf::IntRect(37, 52, dim.x, dim.y));
    }
    if(this->id==4){
        this->dim = sf::Vector2u(57, 74);
        this->idleTexture.loadFromFile("../Sprites/Swordsman/Idle.png");
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(sf::IntRect(37, 54, dim.x, dim.y));
    }
}


