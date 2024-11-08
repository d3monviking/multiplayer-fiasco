#include <SFML/Graphics.hpp>
class Player{
    private:
        int id=-1;
        sf::Vector2f pos;
        sf::Vector2f coords;
        sf::Vector2f vel = sf::Vector2f(0,0);
        sf::Vector2f acc=sf::Vector2f(0,0);
        float speed = 2.0f;
        float runacc=2;
        float maxspeed=8;
        bool on_ground=true;
        bool facing_right=true;
        float prev_x_vel=0;
        
    public:
        sf::RectangleShape surface;
        long long count=0;

        Player(float x, float y, int id);
        int get_id();
        void set_id(int id);
        sf::Vector2f getPos();
        void setPos(float x, float y);

    friend class Level;
};