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

