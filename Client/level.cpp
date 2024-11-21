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


Level::Level(sf::RenderWindow* window, int screen_width) {
    display_surface = window;
    level_height = level_map.size() * tileSize.y;
    level_width = level_map[0].size() * tileSize.x;
    
    // Create and configure the camera view
    gameView.reset(sf::FloatRect(0, 0, screen_width, screen_height));
    gameView.setCenter(screen_width/2, screen_height/2);
}

Level::Level() {}

void Level::setup_level(int screen_width) {
    for(int i = 0; i < level_map.size(); i++) {
        for(int j = 0; j < level_map[0].size(); j++) {
            int x = tileSize.x * j;
            int y = tileSize.y * i;
            if(level_map[i][j] == 'X') {
                Tile t(sf::Vector2f(x,y), tileSize);
                tiles.push_back(t);
            }
        }
    }
}

void Level::updateCamera() {
    static sf::Clock clock;
    float deltaTime = clock.restart().asSeconds();

    float desiredOffset = (1.0f / 3.0f) * gameView.getSize().x;
    float smoothing = 3.0f;

    // Determine offset based on facing direction
    float targetOffsetX = self.facing_right ? desiredOffset : -desiredOffset;

    // Smoothly interpolate camera offset
    cameraOffsetX += (targetOffsetX - cameraOffsetX) * smoothing * deltaTime;

    // Set target center based on player position plus offset
    sf::Vector2f targetCenter = self.coords;
    targetCenter.x += cameraOffsetX;

    // Clamp the camera within the level boundaries
    float halfViewWidth = gameView.getSize().x / 2.f;
    float halfViewHeight = gameView.getSize().y / 2.f;
    targetCenter.x = std::max(halfViewWidth, std::min(targetCenter.x, level_width - halfViewWidth));
    targetCenter.y = std::max(halfViewHeight, std::min(targetCenter.y, level_height - halfViewHeight));

    // Smooth camera movement
    sf::Vector2f currentCenter = gameView.getCenter();
    sf::Vector2f newCenter = currentCenter + (targetCenter - currentCenter) * smoothing * deltaTime;

    gameView.setCenter(newCenter);
}


void Level::x_collisions() {
    self.vel.x += self.acc.x;
    if(self.vel.x > self.maxspeed) {
        self.vel.x = self.maxspeed;
    }
    else if(self.vel.x < -self.maxspeed) {
        self.vel.x = -self.maxspeed;
    }

    self.coords.x += self.vel.x;
    self.pos.x = self.coords.x;  // Update screen position based on world coordinates

    for(Tile t : tiles) {
        if(colliding(t.surface, self.surface, t.coords, self.coords)) {
            float relVel = self.vel.x;
            
            if(relVel < 0) {
                self.coords.x = t.coords.x + tileSize.x;
                self.pos.x = self.coords.x;
                self.vel.x = 0;
            }
            else if(relVel > 0) {
                self.coords.x = t.coords.x - self.surface.getSize().x;
                self.pos.x = self.coords.x;
                self.vel.x = 0;
            }
        }
    }

    for(int i=0;i<other_players.size();i++){
        // cout<<"selfid:"<<self_id<<endl;
        // if(self_id-1==i){
        //     continue;
        // }
        
        Player* p = other_players[i];
        // cout<<"other:"<<p->coords.x<<" "<<p->coords.y<<endl;
        // float other_x = interpolation_buffer[i][interpolation_buffer[i].size()-1].pos.x;
        // float other_y = interpolation_buffer[i][interpolation_buffer[i].size()-1].pos.y;

        if(colliding(p->surface, self.surface, p->coords, self.coords)){
            // cout<<"colliding"<<endl;
            if(self.vel.x<0){
                self.coords.x = p->coords.x + p->surface.getSize().x;
                self.vel.x=0;
            }
            else if(self.vel.x>0){
                self.coords.x = p->coords.x - self.surface.getSize().x;
                self.vel.x=0;
            }
        }
        
    }

    
}

void Level::y_collisions() {
    self.acc.y = gravity;
    self.vel.y += self.acc.y;
    self.coords.y += self.vel.y;
    self.pos.y = self.coords.y;  // Update screen position based on world coordinates

    for(Tile t : tiles) {
        if(colliding(t.surface, self.surface, t.coords, self.coords)) {
            float relVel = self.vel.y;

            if(relVel < 0) {
                self.coords.y = t.coords.y + tileSize.y;
                self.pos.y = self.coords.y;
                self.vel.y = 0;
            }
            else if(relVel > 0) {
                self.coords.y = t.coords.y - self.surface.getSize().y;
                self.pos.y = self.coords.y;
                self.vel.y = 0;
                self.on_ground = true;
            }
        }
    }

    for(int i=0;i<other_players.size();i++){
        // if(self_id-1==i){
        //     continue;
        // }
        
        Player* p = other_players[i];
        // float other_x = interpolation_buffer[i][interpolation_buffer[i].size()-1].pos.x;
        // float other_y = interpolation_buffer[i][interpolation_buffer[i].size()-1].pos.y;
        // cout<<"other:"<<other_x<<" "<<other_y<<endl;

        if(colliding(p->surface, self.surface, p->coords, self.coords)){
            if(self.vel.y<0){
                self.coords.y = p->coords.y + p->surface.getSize().y;
                self.vel.y=0;
            }
            else if(self.vel.y>0){
                self.coords.y = p->coords.y - self.surface.getSize().y;
                self.vel.y=-15;
                // cout<<"colliding"<<endl;
            }
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

bool Level::colliding(sf::RectangleShape& rect1, sf::RectangleShape& rect2, sf::Vector2f coord1, sf::Vector2f coord2){
    if((coord1.x+rect1.getSize().x>coord2.x) && (coord1.x<coord2.x+rect2.getSize().x) && (coord1.y+rect1.getSize().y>coord2.y) && (coord1.y<coord2.y+rect2.getSize().y)){
        return true;
    }
    else{
        return false;
    }
}

void Level::applyLocalInput(vector<bool> &this_move, int camFlag) {
    self.prev_x_vel = self.vel.x;
    if(this_move[1] == 1) {
        self.acc.x = -self.runacc;
    }
    else if(this_move[3] == 1) {
        self.acc.x = self.runacc;
    }
    else {
        self.acc.x = 0;
        self.vel.x *= 0.9;
        if(abs(self.vel.x) < 0.1) {
            self.vel.x = 0;
        }
    }

    if(this_move[4] == 1) {
        if(self.on_ground) {
            self.vel.y -= 25;
            self.on_ground = false;
        }
    }

    x_collisions();
    y_collisions();
    
    self.surface.setPosition(self.coords);
    
    if(self.vel.x < 0) {
        self.facing_right = false;
    }
    else if(self.vel.x > 0) {
        self.facing_right = true;
    }

    if(camFlag == 1) {
        updateCamera();

    }

    // cout<<self.coords.x<<" "<<self.coords.y<<endl;
}
        
void Level::processPendingUpdates(){
    if(updates_buffer.size()>0){ //apply latest pending update one by one
        auto player_state = updates_buffer[updates_buffer.size()-1];
        // cout<<"seqnum:"<<player_state.last_processed_seq_num<<" "<<player_state.pos.x<<" "<<player_state.pos.y<<endl;
        // cout<<self.pos.x<<" "<<self.pos.y<<" "<<self.coords.x<<" "<<self.coords.y<<endl;
        self.coords.x = player_state.pos.x; //set pos to last acked state and replay all inputs from that point to the present
        self.coords.y = player_state.pos.y;
        // if(player_state.pos.x != movemap[player_state.last_processed_seq_num].x || player_state.pos.y != movemap[player_state.last_processed_seq_num].y){
        //     cout<<"mismatch"<<endl;
        //     cout << player_state.last_processed_seq_num << endl;
        //     cout << "pos x: " << player_state.pos.x << " pos y: " << player_state.pos.y << endl;
        //     cout << "movemap x: " << movemap[player_state.last_processed_seq_num].x << " movemap y: " << movemap[player_state.last_processed_seq_num].y << endl;
        //     cout << "-------------------------" << endl;
        // }
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

        // cout<<"apt"<<self.pos.x<<endl;
        newMove.seq_num=count++;
        newMove.pos.y=self.pos.y;
        newMove.pos.x=self.pos.x;
        move_history.push_back(newMove);
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
        movemap[count] = sf::Vector2f(self.coords.x, self.coords.y);
        cout << "seqnum:" << count-1 << " " << fixed << setprecision(self.coords.x == static_cast<int>(self.coords.x) ? 1 : 2) << self.coords.x << " y=" << setprecision(self.coords.y == static_cast<int>(self.coords.y) ? 1 : 2) << self.coords.y << endl;


}


void Level::InterpolateEntity(Player *player){
    long long current_time=setCurrentTimestamp();
    long long rqd_time=current_time-20;
    int player_id=player->get_id() - 1;
    auto i=interpolation_buffer[player_id].begin();
    float x,y;
    // cout << "arjit " << interpolation_buffer[player_id].size() << endl;
    while(((i+1)<interpolation_buffer[player_id].end()) && (i+1)->timestamp<rqd_time) i++;

    //Dropping older positions

    long long temp=i->timestamp;
    auto p=interpolation_buffer[player_id].begin();
    while(p!=interpolation_buffer[player_id].end() && p->timestamp<temp){
        p=interpolation_buffer[player_id].erase(p);
    }
    i=interpolation_buffer[player_id].begin();
    auto j=interpolation_buffer[player_id].begin();
    while(j<interpolation_buffer[player_id].end() && j->timestamp<=temp) j++;
    // if(i==interpolation_buffer[player_id].begin()) cout<<"yay"<<endl;

    //if no need for interpolation

    // cout<<"using:"<<j->pos.y<<" "<<i->pos.y<<endl;;
    // for(int k=0;k<interpolation_buffer[player_id].size();k++){
    //     cout<<interpolation_buffer[player_id][k].pos.y<<":"<<interpolation_buffer[player_id][k].timestamp<<" ";
    // }
    // cout<<endl;
    if((i+1)<interpolation_buffer[player_id].end() && i->timestamp==rqd_time){
        x=i->pos.x;
        y=i->pos.y;
        player->setCoords(x,y);
        return;
    }
    //interpolation logic
    if(i<interpolation_buffer[player_id].end() && j<interpolation_buffer[player_id].end()){
        x=i->pos.x+(((j->pos.x-i->pos.x)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
        y=i->pos.y+(((j->pos.y-i->pos.y)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
        player->setCoords(x,y);
        // cout<<"other:"<<x<<" "<<y<<endl;
    }
}
void Level::render() {
    display_surface->clear();
    
    // Set the view
    display_surface->setView(gameView);
    
    // Draw all game objects
    display_surface->draw(self.surface);
    for(auto others : other_players) {
        display_surface->draw(others->surface);
    }
    for(auto tile : tiles) {
        display_surface->draw(tile.surface);
    }
    
    display_surface->display();
} 

void Level::run(){
    // updatePlayer();
    processPendingUpdates();
    updatePlayer();
    for(auto player : other_players){
        InterpolateEntity(player);
        // player->moveCam(x_shift, y_shift);
    }
    render();
}



