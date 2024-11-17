#include "level.h"
using namespace std;

using boost::asio::ip::udp;
extern boost::asio::io_context io_context;
extern udp::socket clientSocket;
extern udp::endpoint serverEndpoint;

extern vector<SelfData> updates_buffer;
extern vector<vector<InterpolationData>> interpolation_buffer;
vector<Move> move_history;
extern Player self;
extern vector<Player*> other_players;
extern int screen_width;
extern int screen_height;


Level::Level(sf::RenderWindow* window, int screen_width){
    display_surface=window;
    level_height=level_map.size()*tileSize.y;
    level_width=level_map[0].size()*tileSize.x;
    // setup_level(screen_width);
}

Level::Level(){}

void Level::setup_level(int screen_width){
    right_calibration=level_width;
    for(int i=0;i<level_map.size();i++){
        for(int j=0;j<level_map[0].size();j++){
            int x = tileSize.x*j;
            int y = tileSize.y*i;
            if(level_map[i][j]=='X'){
                Tile t(sf::Vector2f(x,y), tileSize);
                tiles.push_back(t);
            }
            // else if(level_map[i][j]=='P'){
            //     // cout<<"setplayerpos"<<endl;
            //     self.setPos(x, y);
            //     self.coords.x=x;
            //     self.coords.y=y;
            // }

            cout<<"playerpoapt:"<<self.pos.x<<endl;
        }
    }


}

void Level::set_id(int id){self_id=id;}

long long Level::setCurrentTimestamp() {
    // Get the current time since epoch (in milliseconds)
    auto now = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());

    // Set the timestamp to the current time
    return duration.count();  // This will give you the time in milliseconds
}

void Level::scroll_x(){
    x_shift=0;
    auto vel = self.prev_x_vel;
    if(self.facing_right){
        if(self.coords.x<screen_width/3 || right_calibration<screen_width){
            x_shift=0;
        }
        else if(self.pos.x>screen_width/3){
            x_shift=-3*(vel)/2;
            self.pos.x-=(3*(vel)/2);
        }
        else{
            x_shift=-vel;
            self.pos.x-=vel;
        }
    }
    else{
        if(left_calibration>0 || self.coords.x>level_width-screen_width/3){
            x_shift=0;
        }
        else if(self.pos.x<2*screen_width/3){
            x_shift=-3*(vel)/2;
            self.pos.x-=(3*(vel)/2);
        }
        else{
            x_shift=-vel;
            self.pos.x-=vel;
        }
    }

    right_calibration+=x_shift;
    left_calibration+=x_shift;
}

void Level::scroll_y(){
    // y_shift=0;
    auto vel = self.vel.y;
    // cout<<"vel:"<<vel<<endl;
    // if(self.pos.y < screen_width/3){
    //     y_shift=-vel;
    //     self.pos.y-=vel;
    // }
    // else if(self.pos.y > 2*screen_width/3){
    //     y_shift=-vel;
    //     self.pos.y-=vel;
    // }
    

    // // cout<<y_shift<<endl;

    if(self.pos.y<screen_height/3 && vel<0 && self.coords.y>level_height/3){
        y_shift = -vel;
        self.pos.y += y_shift;
        if(!shifted){
            start_ypos = self.coords.y;
        }
        shifted=true;
    }
    else if(self.pos.y > 2*screen_height/3 && vel>0 && self.coords.y<level_height-screen_height/3){
        y_shift = -vel;
        self.pos.y += y_shift;
        shifted=true;
    }
    else{
        if(vel<0){
            if(shifted && ((!self.on_ground) && (self.coords.y<start_ypos))){
                y_shift = -vel;
                self.pos.y += y_shift;
            }
            else{
                y_shift=0;
                shifted=false;
            }
        }
        else{
            if(shifted && ((!self.on_ground) && (self.coords.y>start_ypos) && (start_ypos<2*screen_height/3))){
                y_shift = -vel;
                self.pos.y += y_shift;
            }
            else{
                y_shift=0;
                shifted=false;
            }
        }
    }


}

bool Level::colliding(sf::RectangleShape& rect1, sf::RectangleShape& rect2, sf::Vector2f coord1, sf::Vector2f coord2){
    if((coord1.x+rect1.getSize().x>coord2.x) && (coord1.x<coord2.x+rect2.getSize().x) && (coord1.y+rect1.getSize().y>coord2.y) && (coord1.y<coord2.y+rect2.getSize().y)){
        return true;
    }
    else{
        return false;
    }
}

void Level::x_collisions(){
    self.vel.x+=self.acc.x;
    if(self.vel.x>self.maxspeed){
        self.vel.x=self.maxspeed;
    }
    else if(self.vel.x<-self.maxspeed){
        self.vel.x=-self.maxspeed;
    }

    self.pos.x+=self.vel.x;
    self.coords.x+=self.vel.x;


    for(Tile& t:this->tiles){
        t.update(x_shift, 0);
    }

    for(Tile t:this->tiles){ //player-tile collisions
        if(colliding(t.surface, self.surface, t.coords, self.coords)){
            // cout<<"collision"<<endl;

            float relVel = self.vel.x-t.vel.x;
            
            if(relVel<0){
                self.pos.x = t.pos.x + tileSize.x;
                self.coords.x = t.coords.x + tileSize.x;
                self.vel.x=0;
            }
            else if(relVel>0){
                self.pos.x = t.pos.x - self.surface.getSize().x;
                self.coords.x = t.coords.x - self.surface.getSize().x;
                self.vel.x=0;
            }
        }
        // else{
        //     cout<<"no collision"<<endl;
        // }


    }
}

void Level::y_collisions(){
    self.acc.y = gravity;
    self.vel.y+=self.acc.y;
    self.pos.y+=self.vel.y;
    self.coords.y+=self.vel.y;
    for(Tile& t:this->tiles){
        t.update(0, y_shift);
    }

    for(Tile t:this->tiles){ //player-tile collisions
        if(colliding(t.surface, self.surface, t.coords, self.coords)){
            float relVel = self.vel.y-t.vel.y;

            if(relVel<0){
                self.pos.y = t.pos.y + tileSize.y;
                self.coords.y = t.coords.y + tileSize.y;
                self.vel.y=0;
            }
            else if(relVel>0){
                self.pos.y = t.pos.y - self.surface.getSize().y;
                self.coords.y = t.coords.y - self.surface.getSize().y;
                self.vel.y=0;
                self.on_ground=true;
            }
        }
    } 

}

void Level::applyLocalInput(vector<bool> &this_move, int camFlag){
    self.prev_x_vel=self.vel.x;
    if(this_move[1]==1){
        self.acc.x=-self.runacc;
    }
    else if(this_move[3]==1){
        self.acc.x=self.runacc;
    }
    else{
        self.acc.x=0;
        self.vel.x*=0.9;
        if(abs(self.vel.x)<0.1){
            self.vel.x=0;
        }
    }

    if(this_move[4]==1){
        if(self.on_ground){
            self.vel.y-=15;
            self.on_ground=false;
        }
    }
    if(camFlag==1){
        // scroll_x();
    }
    x_collisions();
    if(camFlag==1){
        // scroll_y();
    }
    y_collisions();
    self.surface.setPosition(self.coords);
    if(self.vel.x<0){
        self.facing_right=false;
    }
    else if(self.vel.x>0){
        self.facing_right=true;
    }

    // cout<<self.coords.x<<" "<<self.coords.y<<endl;
}
        
void Level::processPendingUpdates(){
    if(updates_buffer.size()>0){ //apply latest pending update one by one
        auto player_state = updates_buffer[updates_buffer.size()-1];
        cout<<"seqnum:"<<player_state.last_processed_seq_num<<" "<<player_state.pos.x<<" "<<player_state.pos.y<<endl;
        cout<<self.pos.x<<" "<<self.pos.y<<" "<<self.coords.x<<" "<<self.coords.y<<endl;
        self.coords.x = player_state.pos.x; //set pos to last acked state and replay all inputs from that point to the present
        self.coords.y = player_state.pos.y;
        self.vel.x=player_state.vel.x;
        self.vel.y=player_state.vel.y;
        if(player_state.last_processed_seq_num!=-1){
            self.pos.x = movemap[player_state.last_processed_seq_num].x;
            self.pos.y = movemap[player_state.last_processed_seq_num].y;
        }
        int k=0;
        while(k<move_history.size()){
            if(move_history[k].seq_num<=player_state.last_processed_seq_num){
                k++;
            }
            else{
                break;
            }
        }
        vector<Move> local_history;
        for(int j=k;j<move_history.size();j++){
            local_history.push_back(move_history[j]);
        }
        move_history=local_history;
        
        for(int j=0;j<move_history.size();j++){
            applyLocalInput(move_history[j].thisMove, 0); //calculate state for this input
        }
        move_history=local_history;
        updates_buffer.clear();
    }

    
}

void Level::updatePlayer(){
    Move newMove;
    int flag=0;
    // if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
    //     flag=1;
    //     newMove.thisMove[0] = 1;
    // }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        flag=1;
        newMove.thisMove[1] = 1;
    }
    // if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
    //     flag=1;
    //     newMove.thisMove[2] = 1;
    // }
   else  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        flag=1;
        newMove.thisMove[3] = 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        flag=1;
        newMove.thisMove[4] = 1;
    }
    

    // Creating FlatBuffer variables to store and transmit data

        cout<<"apt"<<self.pos.x<<endl;
        newMove.seq_num=count++;
        newMove.pos.y=self.pos.y;
        newMove.pos.x=self.pos.x;
        move_history.push_back(newMove);
        movemap[count] = sf::Vector2f(self.pos.x, self.pos.y);
        // cout<<"storing:"<<self.pos.x<<endl;

        flatbuffers::FlatBufferBuilder builder(1024);

        // Creating PlayerData

        auto player_id = self_id;
        auto position = Game::Vec2(self.pos.x, self.pos.y);
        auto velocity = Game::Vec2(self.vel.x, self.vel.y);
        auto timestamp = std::time(nullptr);
        auto seq_number = newMove.seq_num;

        auto selfData = CreatePlayerData(builder, player_id, &position, &velocity, timestamp, seq_number);

        // Creating ClientMessage
        auto player_input = builder.CreateVector(newMove.thisMove);
        auto clientMessage = Game::CreateClientMessage(builder, selfData, player_input, seq_number);

        // Creating GameMessage
        auto gameMessage = Game::CreateGameMessage(builder, Game::GameData_ClientMessage, clientMessage.Union());

        builder.Finish(gameMessage);
        uint8_t* buf = builder.GetBufferPointer();
        size_t size = builder.GetSize();

        boost::system::error_code ec;
        clientSocket.send_to(boost::asio::buffer(buf, size), serverEndpoint, 0, ec);
    
        if (ec) {
            cerr << "Send failed: " << ec.message() << endl;
        }


        
    
    applyLocalInput(newMove.thisMove, 1);

    
}


void Level::InterpolateEntity(Player *player){

    long long current_time=setCurrentTimestamp();
    long long rqd_time=current_time-100;
    int player_id=player->get_id() - 1;
    auto i=interpolation_buffer[player_id].begin();
    auto j=interpolation_buffer[player_id].begin();
    float x,y;
    while(((i+1)<interpolation_buffer[player_id].end()) && (i+1)->timestamp<rqd_time) i++;
    
    while(j<interpolation_buffer[player_id].end() && j->timestamp<rqd_time) j++;
    //if no need for interpolation
    if((i+1)<interpolation_buffer[player_id].end() && i->timestamp==rqd_time){
        x=i->pos.x;
        y=i->pos.y;
        player->setPos(x,y);
        return;
    }
    //interpolation logic
    if(i<interpolation_buffer[player_id].end() && j<interpolation_buffer[player_id].end()){
        x=i->pos.x+(((j->pos.x-i->pos.x)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
        y=i->pos.y+(((j->pos.y-i->pos.y)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
        player->setPos(x,y);

    }

}

void Level::render(){
    display_surface->clear();
    display_surface->draw(self.surface);
    for(auto others: other_players){
        display_surface->draw(others->surface);
    }

    for(auto tile:tiles){
        display_surface->draw(tile.surface);
    }
    display_surface->display();
}

void Level::run(){
    processPendingUpdates();
    updatePlayer();
    for(auto player : other_players){

        // InterpolateEntity(player);
        player->moveCam(x_shift, y_shift);

    }
    render();
}

