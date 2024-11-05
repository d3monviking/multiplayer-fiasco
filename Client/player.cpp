#include<bits/stdc++.h>
#include "gamestructs.h"
#include "player.h"

using namespace std;

Player::Player(float x, float y, int id){
    this->pos.x=x;
    this->pos.y=y;
    this->id=id;
    this->shape.setPosition(x, y);
    this->shape.setSize(sf::Vector2f(50, 50));
    if(this->id==1){
        this->shape.setFillColor(sf::Color::Green);
    }
    if(this->id==2){
        this->shape.setFillColor(sf::Color::Blue);
    }
    if(this->id==3){
        this->shape.setFillColor(sf::Color::Red);
    }
    if(this->id==4){
        this->shape.setFillColor(sf::Color::Magenta);
    }
    
}
int Player::get_id() {return id;}

void Player::set_id(int id){this->id = id;}

sf::Vector2f Player::getPos() {return pos;}

void Player::setPos(float x,float y) {
    this->pos.x=x;
    this->pos.y=y;
    this->shape.setPosition(this->pos.x, this->pos.y);
}

