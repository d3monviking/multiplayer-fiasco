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
    this->animState = PLAYER_ANIMATION_STATES::IDLE;
    this->animationTimer.restart();
    this->animationSwitch = true;
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
   // cout<<pos.x<<" "<<pos.y<<endl;
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
    if(this->id==2){
        this->dim = sf::Vector2u(69, 80);
        if(!this->movingTexture.loadFromFile("../Sprites/Archer/Run.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }
        if(!this->idleTexture.loadFromFile("../Sprites/Archer/Idle.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        } 
        if(!this->jumpTexture.loadFromFile("../Sprites/Archer/Jump.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }  
        this->initialTextureIdlePos = 21;
        this->initialTextureMovePos = 28;
        this->initialTextureJumpPos = 23;
        this->jumpFrameWidth = 1050;
        this->currentFrameIdle = sf::IntRect(21, 50, dim.x, dim.y);
        this->currentFrameMove = sf::IntRect(28, 51, dim.x, dim.y);
        this->currentFrameJump = sf::IntRect(23, 47, dim.x, dim.y);
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(this->currentFrameIdle);
    }
    if(this->id==4){
        this->dim = sf::Vector2u(56, 94);
        if(!this->movingTexture.loadFromFile("../Sprites/Musketeer/Run.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }
        if(!this->idleTexture.loadFromFile("../Sprites/Musketeer/Idle.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }   
        if(!this->jumpTexture.loadFromFile("../Sprites/Musketeer/Jump.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }
        this->initialTextureIdlePos = 36;
        this->initialTextureMovePos = 24;
        this->initialTextureJumpPos = 416;
        this->jumpFrameWidth = 700;
        this->currentFrameIdle = sf::IntRect(36, 54, dim.x, dim.y);
        this->currentFrameMove = sf::IntRect(24, 55, dim.x, dim.y);
        this->currentFrameJump = sf::IntRect(416, 34, dim.x, dim.y);
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(this->currentFrameIdle);
    }
    if(this->id==3){
        this->dim = sf::Vector2u(55, 81);
        if(!this->movingTexture.loadFromFile("../Sprites/Wizard/Run.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }
        if(!this->idleTexture.loadFromFile("../Sprites/Wizard/Idle.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        } 
        if(!this->jumpTexture.loadFromFile("../Sprites/Wizard/Jump.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }
        this->initialTextureIdlePos = 37;
        this->initialTextureMovePos = 37;
        this->initialTextureJumpPos = 28;
        this->jumpFrameWidth = 1300;
        this->currentFrameIdle = sf::IntRect(37, 52, dim.x, dim.y);
        this->currentFrameMove = sf::IntRect(37, 52, dim.x, dim.y);
        this->currentFrameJump = sf::IntRect(28, 47, dim.x, dim.y);
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(this->currentFrameIdle);
    }
    if(this->id==1){
        this->dim = sf::Vector2u(57, 80);
        if(!this->movingTexture.loadFromFile("../Sprites/Swordsman/Run.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }
        if(!this->idleTexture.loadFromFile("../Sprites/Swordsman/Idle.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        } 
        if(!this->jumpTexture.loadFromFile("../Sprites/Swordsman/Jump.png")){
            cout << "ERROR: PLAYER:: Could not load the player sheet\n";
        }
        this->initialTextureIdlePos = 37;
        this->initialTextureJumpPos = 38;
        this->initialTextureMovePos = 36;
        this->jumpFrameWidth = 940;
        this->currentFrameIdle = sf::IntRect(37, 54, dim.x, dim.y);
        this->currentFrameMove = sf::IntRect(38, 54, dim.x, dim.y);
        this->currentFrameJump = sf::IntRect(36, 48, dim.x, dim.y);
        this->sprite.setTexture(idleTexture);
        this->sprite.setTextureRect(this->currentFrameIdle);
    }
}

void Player::applyPowerUp(long long time){
    if(powerups.size()!=0){
        float originalSpeed=vel.x;
        // cout<<vel.x<<endl;
        (*powerups.begin())->applyBoost(vel.x);
        boostActive=true;
        boostStart=time;
        cout<<"boostStart:"<<time<<endl;
        //(*powerups.begin())->updateBoost(vel.x,originalSpeed);
        // cout<<vel.x<<endl;
        powerups.erase(powerups.begin());
        cout<<"power up applied"<<endl;
    }
}

void Player::addShell(Shell* shell){
    shells.push_back(shell);
}
void Player::addPowerUps(PowerUp* powerup){
    powerups.push_back(powerup);
}

const bool Player::getAnimSwitch(){
    bool anim_switch = this->animationSwitch;
    if(this->animationSwitch){
        this->animationSwitch = false;
    }
    return anim_switch;
}

void Player::resetAnimationTimer(){
    this->animationTimer.restart();
    this->animationSwitch = true;
}

void Player::updateAnimation(){
    if(!this->on_ground){
        this->animState = PLAYER_ANIMATION_STATES::JUMPING;
    }
    else if(this->vel.x < -0.4){
        this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else if(this->vel.x > 0.4){
        this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
    }
    else {
        this->animState = PLAYER_ANIMATION_STATES::IDLE;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::IDLE){
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.08f || this->getAnimSwitch()){
            this->currentFrameIdle.left += 128.f;
            if(this->currentFrameIdle.left >= 640.f){
                this->currentFrameIdle.left = this->initialTextureIdlePos;
            }  
            this->animationTimer.restart();
            this->sprite.setTexture(this->idleTexture);
            this->sprite.setTextureRect(this->currentFrameIdle);
        }
    }
    else if (this->animState == PLAYER_ANIMATION_STATES::MOVING_RIGHT){
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.08f ){
            this->currentFrameMove.left += 128.f;
            if(this->currentFrameMove.left >= 768.f){
                this->currentFrameMove.left = this->initialTextureMovePos;
            }   
            this->animationTimer.restart();
            this->sprite.setTexture(this->movingTexture);
            this->sprite.setTextureRect(this->currentFrameMove);
        } 
        this->sprite.setScale(1.f, 1.f);
        this->sprite.setOrigin(0.f, 0.f);
    }
    else if (this->animState == PLAYER_ANIMATION_STATES::MOVING_LEFT){
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.08f ){
            this->currentFrameMove.left += 128.f;
            if(this->currentFrameMove.left >= 768.f){
                this->currentFrameMove.left = this->initialTextureMovePos;
            }   
            this->animationTimer.restart();
            this->sprite.setTexture(this->movingTexture);
            this->sprite.setTextureRect(this->currentFrameMove);
        } 
        this->sprite.setScale(-1.f, 1.f);
        this->sprite.setOrigin(this->sprite.getGlobalBounds().width, 0.f);
    }
    if (this->animState == PLAYER_ANIMATION_STATES::JUMPING){
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.08f ){
            this->currentFrameJump.left += 128.f;
            if(this->currentFrameJump.left >= this->jumpFrameWidth){
                this->currentFrameJump.left = this->initialTextureJumpPos;
            }   
            this->animationTimer.restart();
            this->sprite.setTexture(this->jumpTexture);
            this->sprite.setTextureRect(this->currentFrameJump);
        }
        if(this->vel.x > 0){
            this->sprite.setScale(1.f, 1.f);
            this->sprite.setOrigin(0.f, 0.f);            
        }
        else if(this->vel.x < 0){
            this->sprite.setScale(-1.f, 1.f);
            this->sprite.setOrigin(this->sprite.getGlobalBounds().width, 0.f);            
        } 
    }    
}