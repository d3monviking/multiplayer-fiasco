#include<bits/stdc++.h>
#include "gamestructs.h"
#include "player.h"

using namespace std;

Player::Player(float x, float y, int id){
    this->pos.x=x;
    this->pos.y=y;
    this->id=id;
    this->surface.setPosition(x, y);
    this->surface.setSize(sf::Vector2f(50, 50));
    if(this->id==1){
        this->surface.setFillColor(sf::Color::Green);
    }
    if(this->id==2){
        this->surface.setFillColor(sf::Color::Blue);
    }
    if(this->id==3){
        this->surface.setFillColor(sf::Color::Red);
    }
    if(this->id==4){
        this->surface.setFillColor(sf::Color::Magenta);
    }
    
}
int Player::get_id() {return id;}

void Player::set_id(int id){this->id = id;}

sf::Vector2f Player::getPos() {return pos;}

void Player::setPos(float x,float y) {
    this->pos.x=x;
    this->pos.y=y;
    this->surface.setPosition(this->pos.x, this->pos.y);
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
}

