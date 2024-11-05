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




Level::Level(sf::RenderWindow* window){
    display_surface=window;
}

Level::Level(){}

void Level::set_id(int id){self_id=id;}

long long Level::setCurrentTimestamp() {
    // Get the current time since epoch (in milliseconds)
    auto now = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());

    // Set the timestamp to the current time
    return duration.count();  // This will give you the time in milliseconds
}

void Level::applyLocalInput(vector<bool> &this_move){
    for(int i = 0; i < 4; i++){
        if(this_move[i] == 1){
            switch(i){
                case 0: self.pos.y-=2;break;//W
                case 1: self.pos.x=self.pos.x-2; break;//A
                case 2: self.pos.y+=2; break;//S
                case 3: self.pos.x=self.pos.x+2; break;//D
                // case 4: self.vel.y -=15;//Jump in case space is pressed
            }
        }
    }

    self.shape.setPosition(self.pos);
}
        
void Level::processPendingUpdates(){
    for(int i=0;i<updates_buffer.size();i++){ //apply all pending updates one by one
        auto player_state = updates_buffer[i];
        self.pos.x = player_state.pos.x; //set pos to last acked state and replay all inputs from that point to the present
        self.pos.y = player_state.pos.y;
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
            applyLocalInput(move_history[j].thisMove); //calculate state for this input
        }
        move_history=local_history;
    }

    updates_buffer.clear();
}

void Level::updatePlayer(){
    Move newMove;
    int flag=0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        flag=1;
        newMove.thisMove[0] = 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        flag=1;
        newMove.thisMove[1] = 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        flag=1;
        newMove.thisMove[2] = 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        flag=1;
        newMove.thisMove[3] = 1;
    }
    

    // Creating FlatBuffer variables to store and transmit data

    if(flag==1){
        newMove.seq_num=count++;
        newMove.pos.y=self.pos.y;
        newMove.pos.x=self.pos.x;
        move_history.push_back(newMove);

        flatbuffers::FlatBufferBuilder builder(1024);

        // Creating PlayerData

        auto player_id = self_id;
        auto position = Game::Vec2(self.pos.x, self.pos.y);
        auto timestamp = std::time(nullptr);
        auto seq_number = newMove.seq_num;

        auto selfData = CreatePlayerData(builder, player_id, &position, timestamp, seq_number);

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

        applyLocalInput(newMove.thisMove);

        
    }
    
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
    display_surface->draw(self.shape);
    for(auto others: other_players){
        display_surface->draw(others->shape);
    }
    display_surface->display();
}

void Level::run(){
    processPendingUpdates();
    updatePlayer();
    for(auto player : other_players){
        InterpolateEntity(player);
    }
    render();
}

