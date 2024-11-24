#include<SFML/Graphics.hpp>
class Tile{
    protected:
        sf::Vector2f pos;
        sf::Vector2f coords;
        sf::Vector2f vel;
        sf::Vector2f acc;
    public:
        sf::Sprite surface;
        Tile(sf::Sprite sprite);

        void update(float x_shit, float y_shift);

    friend class Level;
};
class Collectibles{
protected:
    sf::Vector2f coords;
    public:
    sf::Sprite surface;
    virtual char getType()=0;
    Collectibles(sf::Vector2f coords, sf::Sprite newSprite);
    virtual ~Collectibles(){};
    friend class Level;
};
class Shell:public Collectibles{
private:
    bool kicked;
    bool held;
    char type;
public:
    Shell(sf::Vector2f coords,char type,bool kicked,bool held,sf::Sprite newSprite);
    // void moveX(float shift, float player_x, float player_coords_x);
    // void moveY(float shift, float player_y, float player_coords_y);
    char getType();
};
class PowerUp:public Collectibles{
    private:
        float speedBoost;
        char type;
        sf::Clock boostClock;
    public:
        bool isBoostActive=false;
        PowerUp(sf::Vector2f coords,float speedBoost,char type,sf::Sprite newSprite);
        void applyBoost(float &player_speed);
        void updateBoost(float &player_speed, float original_speed); 
        float getBoost();
        char getType();
};
class MovingPlatform:Tile{
    private:
    float minHeight=100;
    float maxHeight=400;
    bool movingUp=false;
    sf::Clock movementClock;
    public:
    MovingPlatform(sf::Sprite sprite);
    void movePlatform();
};