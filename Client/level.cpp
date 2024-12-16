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
    level_height = MAX_ROWS * tileSize.y;
    level_width = MAX_COLS * tileSize.x;
    
    // Create and configure the camera view
    gameView.reset(sf::FloatRect(0, 0, screen_width, screen_height));
    gameView.setCenter(screen_width/2, screen_height/2);
}

Level::Level() {}

void Level::setup_level(string terrainPath, string collectiblePath, string movingPlatformPath, string rocksPath, string treePath, string waterPath, string backgroundPath, string spikePath) {

    //Loading tileSheet 
    if (!tileSheet.loadFromFile(tileSetPath)){
        cerr << "Error loading the texture!\n";
        return;
    }

    if (!movingPlatImage.loadFromFile(movingPlatPath)){
        cerr << "Error loading the texture!\n";
        return;
    }

    if(!treeImage.loadFromFile(treeImagePath)){
        cerr << "Error loading the texture!\n";
        return;        
    }

    if(!rocksImage.loadFromFile(this->rocksImagePath)){
        cerr << "Error loading the texture!\n";
        return;        
    }    

    if(!spikeImage.loadFromFile(spikeImagePath)){
        cerr << "Error loading the texture!\n";
        return;        
    }        
    //Going through terrain sprites
    ifstream pathFile(terrainPath);
    if(!pathFile.is_open()){
        cerr << "Error opening file!\n";
        return;
    }
    string line;
    int row = 0;
    while(getline(pathFile, line) && row < MAX_ROWS){
        stringstream ss(line);
        string cell;
        int col = 0;
        while(getline(ss, cell, ',') && col < MAX_COLS){
            level[row][col] = stoi(cell);
            col++;
        }
        row++;
    }
    pathFile.close();
    sf::Sprite s;
    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            s.setTexture(tileSheet);
            int tileNumber = level[i][j];
            if(tileNumber == -1){
                continue;
            }
            int tu = tileNumber % (tileSheet.getSize().x / tileSize.x);
            int tv = tileNumber / (tileSheet.getSize().x / tileSize.x);
            s.setTextureRect(sf::IntRect(tu * tileSize.x, tv * tileSize.y, 16, 16));
            s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));      
            Tile newTile(s);      
            tiles.push_back(newTile);
        }
    }

    //Adding Collectables
    ifstream collectiblePathFile(collectiblePath);
    if(!collectiblePathFile.is_open()){
        cerr << "Error opening file!\n";
        return;        
    }
    row = 0;
    while(getline(collectiblePathFile, line) && row < MAX_ROWS){
        stringstream ss(line);
        string cell;
        int col = 0;
        while(getline(ss, cell, ',') && col < MAX_COLS){
            level[row][col] = stoi(cell);
            col++;
        }
        row++;
    }
    collectiblePathFile.close();

    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            s.setTexture(tileSheet);
            int tileNumber = level[i][j];
            if(tileNumber == -1){
                continue;
            }
            int tu = tileNumber % (tileSheet.getSize().x / tileSize.x);
            int tv = tileNumber / (tileSheet.getSize().x / tileSize.x);
            s.setTextureRect(sf::IntRect(tu * tileSize.x, tv * tileSize.y, 16, 16));
            s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));      
            Collectibles* newCollectible = new PowerUp(sf::Vector2f(j * tileSize.x, i * tileSize.y), 2.f, 'P', s);      
            this->collictibles.push_back(newCollectible);
        }
    }
    finishTexture.loadFromFile(finishLinePath);
    ifstream finishPath("./TileMapFiles/level_finishLine.csv");
    if(!finishPath.is_open()){
        cerr << "Error opening file!\n";
        return;        
    }
    row = 0;
    while(getline(finishPath, line) && row < MAX_ROWS){
        stringstream ss(line);
        string cell;
        int col = 0;
        while(getline(ss, cell, ',') && col < MAX_COLS){
            level[row][col] = stoi(cell);
            col++;
        }
        row++;
    }
    finishPath.close();

    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            s.setTexture(finishTexture);
            int tileNumber = level[i][j];
            if(tileNumber == -1){
                continue;
            }
            int tu = tileNumber % (finishTexture.getSize().x / tileSize.x);
            int tv = tileNumber / (finishTexture.getSize().x / tileSize.x);
            s.setTextureRect(sf::IntRect(tu * tileSize.x, tv * tileSize.y, 16, 16));
            s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));      
            Tile t(s);
            tiles.push_back(s);
        }
    }

    //Adding Moving Platform

    ifstream movingPlatformPathFile(movingPlatformPath);
    if(!movingPlatformPathFile.is_open()){
        cerr << "Error opening file here!\n";
        return;        
    }
    row = 0;
    while(getline(movingPlatformPathFile, line) && row < MAX_ROWS){
        stringstream ss(line);
        string cell;
        int col = 0;
        while(getline(ss, cell, ',') && col < MAX_COLS){
            level[row][col] = stoi(cell);
            col++;
        }
        row++;
    }
    movingPlatformPathFile.close();

    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            s.setTexture(movingPlatImage);
            int tileNumber = level[i][j];
            if(tileNumber == -1){
                continue;
            }
            int tu = tileNumber % (movingPlatImage.getSize().x / tileSize.x);
            int tv = tileNumber / (movingPlatImage.getSize().x / tileSize.x);
            s.setTextureRect(sf::IntRect(tu * tileSize.x, tv * tileSize.y, 48, 16));
            s.setScale(sf::Vector2f(3.f, 1.2f));
            s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));  
            MovingPlatform* newPlatform = new MovingPlatform(s);    
            this->movingPlatforms.push_back(newPlatform);
        }
    }


    ifstream treeFile(treePath);
    if(!treeFile.is_open()){
        cerr << "Error opening file here!\n";
        return;        
    }
    row = 0;
    while(getline(treeFile, line) && row < MAX_ROWS){
        stringstream ss(line);
        string cell;
        int col = 0;
        while(getline(ss, cell, ',') && col < MAX_COLS){
            level[row][col] = stoi(cell);
            col++;
        }
        row++;
    }
    treeFile.close();
    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            s.setTexture(treeImage);
            int tileNumber = level[i][j];
            if(tileNumber == -1){
                continue;
            }
            int tu = tileNumber % (treeImage.getSize().x / tileSize.x);
            int tv = tileNumber / (treeImage.getSize().x / tileSize.x);
            s.setTextureRect(sf::IntRect(tu * tileSize.x, tv * tileSize.y, 16, 16));
            s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));  
            Tile newTile(s);
            tiles.push_back(newTile);
        }
    }

    // ifstream rockFile(treePath);
    // int rockArray[MAX_ROWS][MAX_COLS];
    // if(!rockFile.is_open()){
    //     cerr << "Error opening file here!\n";
    //     return;        
    // }
    // row = 0;
    // while(getline(rockFile, line) && row < MAX_ROWS){
    //     stringstream ss(line);
    //     string cell;
    //     int col = 0;
    //     while(getline(ss, cell, ',') && col < MAX_COLS){
    //         rockArray[row][col] = stoi(cell);
    //         col++;
    //     }
    //     row++;
    // }
    // rockFile.close();

    // for(int i = 0; i < MAX_ROWS; i++) {
    //     for(int j = 0; j < MAX_COLS; j++) {
    //         s.setTexture(this->rocksImage);
    //         int tileNumber = rockArray[i][j];
    //         if(tileNumber == -1){
    //             continue;
    //         }
    //         int tu = tileNumber % (rocksImage.getSize().x / tileSize.x);
    //         int tv = tileNumber / (rocksImage.getSize().x / tileSize.x);
    //         s.setTextureRect(sf::IntRect(tu * tileSize.x, tv * tileSize.y, 16, 16));
    //         s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));  
    //         Tile newTile(s);
    //         cout << tileNumber << endl;
    //         tiles.push_back(newTile);
    //     }
    // }

    ifstream waterFile(waterPath);
    if(!waterFile.is_open()){
        cerr << "Error opening file here!\n";
        return;        
    }
    row = 0;
    while(getline(waterFile, line) && row < MAX_ROWS){
        stringstream ss(line);
        string cell;
        int col = 0;
        while(getline(ss, cell, ',') && col < MAX_COLS){
            level[row][col] = stoi(cell);
            col++;
        }
        row++;
    }
    waterFile.close();

    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            s.setTexture(tileSheet);
            int tileNumber = level[i][j];
            if(tileNumber == -1){
                continue;
            }
            int tu = tileNumber % (tileSheet.getSize().x / tileSize.x);
            int tv = tileNumber / (tileSheet.getSize().x / tileSize.x);
            s.setTextureRect(sf::IntRect(tu * tileSize.x, tv * tileSize.y, 16, 16));
            s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));  
            Tile newTile(s);
            killingThings.push_back(newTile);
        }
    }

    ifstream spikeFile(spikePath);
    if(!spikeFile.is_open()){
        cerr << "Error opening file here!\n";
        return;        
    }
    row = 0;
    while(getline(spikeFile, line) && row < MAX_ROWS){
        stringstream ss(line);
        string cell;
        int col = 0;
        while(getline(ss, cell, ',') && col < MAX_COLS){
            level[row][col] = stoi(cell);
            col++;
        }
        row++;
    }
    spikeFile.close();

    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            s.setTexture(spikeImage);
            int tileNumber = level[i][j];
            if(tileNumber == -1){
                continue;
            }
            // int tu = tileNumber % (tileSheet.getSize().x / tileSize.x);
            // int tv = tileNumber / (tileSheet.getSize().x / tileSize.x);
            s.setTextureRect(sf::IntRect(0, 0, 16, 16));
            s.setPosition(sf::Vector2f(j * tileSize.x, i * tileSize.y));  
            Tile newTile(s);
            killingThings.push_back(newTile);
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
    if(!self.boostActive){
        if(self.vel.x > self.maxspeed) {
            self.vel.x = self.maxspeed;
        }
        else if(self.vel.x < -self.maxspeed) {
            self.vel.x = -self.maxspeed;
        }
    }
    if(self.onPlatform){
        cout<<self.coords.x<<endl;
        if(MovingPlatform::mvLeft) {
           // cout<<"enteredL"<<endl;
            self.coords.x-=1;
        }
        else {
           // cout<<"enteredR"<<endl;
            self.coords.x+=1;
        }
    }
    
    self.coords.x += self.vel.x;
    self.pos.x = self.coords.x;  // Update screen position based on world coordinates

    for(Tile t : tiles) {
        if(colliding(t.surface, self.sprite, t.coords, self.coords)) {
            float relVel = self.vel.x;
            //cout<<"col"<<endl;
            if(relVel < 0) {
                self.coords.x = t.coords.x + tileSize.x;
                self.pos.x = self.coords.x;
                self.vel.x = 0;
            }
            else if(relVel > 0) {
                self.coords.x = t.coords.x - self.getDim().x;
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

        if(colliding(p->sprite, self.sprite, p->coords, self.coords)){
            // cout<<"colliding"<<endl;
            if(self.vel.x<0){
                self.coords.x = p->coords.x + p->getDim().x;
                self.vel.x=0;
            }
            else if(self.vel.x>0){
                self.coords.x = p->coords.x - self.getDim().x;
                self.vel.x=0;
            }
        }
        
    }

    for(auto collect=collictibles.begin();collect!=collictibles.end();){
            if(colliding((*collect)->surface, self.sprite, (*collect)->coords, self.coords)){
                if((*collect)->getType()=='S')
                    self.addShell(dynamic_cast<Shell*>(*collect));
                else if((*collect)->getType()=='P') 
                    self.addPowerUps(dynamic_cast<PowerUp*>(*collect));
                collect=collictibles.erase(collect);
            }
            else{
                collect++;
            }
        }

    
}

void Level::y_collisions() {
    self.acc.y = gravity;
    self.vel.y += self.acc.y;
    self.coords.y += self.vel.y;
    self.pos.y = self.coords.y;  // Update screen position based on world coordinates
    int flag=0;
    for(Tile t : tiles) {
        if(colliding(t.surface, self.sprite, t.coords, self.coords)) {
            float relVel = self.vel.y;

            if(relVel < 0) {
                self.coords.y = t.coords.y + tileSize.y;
                self.pos.y = self.coords.y;
                self.vel.y = 0;
            }
            else if(relVel > 0) {
                self.coords.y = t.coords.y - self.getDim().y;
                self.pos.y = self.coords.y;
                self.vel.y = 0;
                self.on_ground = true;
                //cout<<t.type<<endl;
            }
        }
    }
    for(MovingPlatform* mp:movingPlatforms){
        float relVel = self.vel.y;
        if(colliding(mp->surface, self.sprite, mp->coords, self.coords)){
            if(relVel < 0) {
                self.coords.y = mp->coords.y + 19.2;
                self.pos.y = self.coords.y;
                self.vel.y = 0;
            }
            else if(relVel > 0) {
                self.coords.y = mp->coords.y - self.getDim().y;
                self.pos.y = self.coords.y;
                self.vel.y = 0;
                self.on_ground = true;
                //cout<<t.type<<endl;
                flag=1;
                self.onPlatform=true;
        }
    }
    }
    // cout<<"flag :"<<flag<<endl;
    // if(self.onPlatform) cout<<"yo"<<endl;
    if(self.onPlatform && flag==0){
        self.onPlatform=false;
    }
    for(int i=0;i<other_players.size();i++){
        // if(self_id-1==i){
        //     continue;
        // }
        
        Player* p = other_players[i];
        // float other_x = interpolation_buffer[i][interpolation_buffer[i].size()-1].pos.x;
        // float other_y = interpolation_buffer[i][interpolation_buffer[i].size()-1].pos.y;
        // cout<<"other:"<<other_x<<" "<<other_y<<endl;

        if(colliding(p->sprite, self.sprite, p->coords, self.coords)){
            if(self.vel.y<0){
                self.coords.y = p->coords.y + p->getDim().y;
                self.vel.y=0;
            }
            else if(self.vel.y>0){
                self.coords.y = p->coords.y - self.getDim().y;
                self.vel.y=-15;
                // cout<<"colliding"<<endl;
            }
        }
        
    }

    for(auto collect=collictibles.begin();collect!=collictibles.end();){
            if(colliding((*collect)->surface, self.sprite, (*collect)->coords, self.coords)){
                if((*collect)->getType()=='S')
                    self.addShell(dynamic_cast<Shell*>(*collect));
                else if((*collect)->getType()=='P') 
                    self.addPowerUps(dynamic_cast<PowerUp*>(*collect));
                collect=collictibles.erase(collect);
            }
            else{
                collect++;
            }
        }
//     for(auto mv_platform=movingPlatforms.begin();mv_platform!=movingPlatforms.end();mv_platform++){

//     }
}

void Level::set_id(int id){self_id=id;}

long long Level::setCurrentTimestamp() {
    // Get the current time since epoch (in milliseconds)
    auto now = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());

    // Set the timestamp to the current time
    return duration.count();  // This will give you the time in milliseconds
}

bool Level::colliding(sf::Sprite& rect1, sf::Sprite& rect2, sf::Vector2f coord1, sf::Vector2f coord2){
    // if(rect1.getGlobalBounds().intersects(rect2.getGlobalBounds())){
    //     return true;
    // }
    // else {
    //     return false;
    // }
    if((coord1.x+rect1.getGlobalBounds().width>coord2.x) && (coord1.x<coord2.x+rect2.getGlobalBounds().width) && (coord1.y+rect1.getGlobalBounds().height>coord2.y) && (coord1.y<coord2.y+rect2.getGlobalBounds().height)){
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
    bool isClockStarted = false;
    if (this_move[0] == 1) { // Player presses 'W'
        // Restart the clock for timing
            if (!self.boostActive) {
                long long time = setCurrentTimestamp();
                self.applyPowerUp(time);
                pUp++;
            
        }
    } else if (clock1.getElapsedTime().asSeconds() >= 5.0f) { 
        isClockStarted = false; // Reset after 5 seconds of inactivity
    }
    // cout<<self.vel.x<<endl;
    

    x_collisions();
    y_collisions();
    cout << self.coords.x << " " << self.coords.y << endl;
    
    self.sprite.setPosition(self.coords);
    
    if(self.vel.x < 0) {
        self.facing_right = false;
    }
    else if(self.vel.x > 0) {
        self.facing_right = true;
    }

    if(camFlag == 1) {
        updateCamera();

    }
    long long curr = setCurrentTimestamp();
    // cout<<curr-self.boostStart<<endl;
    if(curr-self.boostStart>30){
        self.boostActive=false;
    }
    self.updateAnimation();
    // cout<<self.boostActive<<endl;

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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        flag=1;
        if(!self.boostActive){
        newMove.thisMove[0] = 1;
        }
        else{
            cout<<"rej"<<endl;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        flag=1;
        newMove.thisMove[1] = 1;
    }
    else  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        flag=1;
        newMove.thisMove[3] = 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        flag=1;
        newMove.thisMove[2] = 1;
    }
   
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        flag=1;
        newMove.thisMove[4] = 1;
    }
    

    // Creating FlatBuffer variables to store and transmit data

        cout<<"apt"<<self.pos.y<<endl;
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
        //cout << "seqnum:" << count-1 << " " << fixed << setprecision(self.coords.x == static_cast<int>(self.coords.x) ? 1 : 2) << self.coords.x << " y=" << setprecision(self.coords.y == static_cast<int>(self.coords.y) ? 1 : 2) << self.coords.y << endl;

}

void Level::InterpolateEntity(Player* player) {
    long long current_time = setCurrentTimestamp(); // Time in milliseconds
    const long long INTERPOLATION_DELAY = 50; // Delay in milliseconds
    long long target_time = current_time - INTERPOLATION_DELAY;

    int player_id = player->get_id() - 1;
    auto& buffer = interpolation_buffer[player_id];
    if (buffer.empty()) return;

    // Ensure the buffer is sorted by timestamp
    std::sort(buffer.begin(), buffer.end(), [](const InterpolationData& a, const InterpolationData& b) {
        return a.timestamp < b.timestamp;
    });

    // Remove old data from the buffer
    const long long MAX_BUFFER_AGE = 2000; // Maximum age in milliseconds
    buffer.erase( 
        buffer.begin(),std::lower_bound(buffer.begin(), buffer.end(), current_time - MAX_BUFFER_AGE,[](const InterpolationData& entry, long long time) { 
                return entry.timestamp < time; 
            })
    );

    // Find interpolation points
    auto next_it = std::lower_bound(buffer.begin(), buffer.end(), target_time, 
        [](const InterpolationData& entry, long long time) {
            return entry.timestamp < time;
        });

    InterpolationData predictedState;

    if (next_it == buffer.begin()) {
        // Extrapolate before first sample
        if (buffer.size() >= 2) {
            auto& p1 = buffer[0];
            auto& p2 = buffer[1];
            long long dt = p2.timestamp - p1.timestamp;
            if (dt > 0) {
                float factor = static_cast<float>(target_time - p1.timestamp) / dt;
                predictedState.pos.x = p1.pos.x + factor * (p2.pos.x - p1.pos.x);
                predictedState.pos.y = p1.pos.y + factor * (p2.pos.y - p1.pos.y);
                predictedState.vel = p1.vel;
            } else {
                predictedState.pos = p1.pos;
                predictedState.vel = p1.vel;
            }
        } else {
            predictedState.pos = buffer[0].pos;
            predictedState.vel = buffer[0].vel;
        }
    } else if (next_it == buffer.end()) {
        // Extrapolate after last sample
        if (buffer.size() >= 2) {
            auto& p1 = buffer[buffer.size() - 2];
            auto& p2 = buffer.back();
            long long dt = p2.timestamp - p1.timestamp;
            if (dt > 0) {
                float factor = static_cast<float>(target_time - p2.timestamp) / dt;
                predictedState.pos.x = p2.pos.x + factor * (p2.pos.x - p1.pos.x);
                predictedState.pos.y = p2.pos.y + factor * (p2.pos.y - p1.pos.y);
                predictedState.vel = p2.vel;
            } else {
                predictedState.pos = p2.pos;
                predictedState.vel = p2.vel;
            }
        } else {
            predictedState.pos = buffer.back().pos;
            predictedState.vel = buffer.back().vel;
        }
    } else {
        // Interpolate between prev_it and next_it
        auto prev_it = std::prev(next_it);
        long long dt = next_it->timestamp - prev_it->timestamp;
        if (dt == 0) {
            predictedState.pos = next_it->pos;
            predictedState.vel = next_it->vel;
        } else {
            float alpha = static_cast<float>(target_time - prev_it->timestamp) / dt;
            predictedState.pos.x = prev_it->pos.x + alpha * (next_it->pos.x - prev_it->pos.x);
            predictedState.pos.y = prev_it->pos.y + alpha * (next_it->pos.y - prev_it->pos.y);
            predictedState.vel.x = prev_it->vel.x + alpha * (next_it->vel.x - prev_it->vel.x);
            predictedState.vel.y = prev_it->vel.y + alpha * (next_it->vel.y - prev_it->vel.y);
        }
    }

    // Perform collision checks
    sf::Vector2f oldPos = player->coords;
    player->setCoords(predictedState.pos.x, predictedState.pos.y);

    // Check collisions with tiles
    for (Tile& tile : tiles) {
        if (colliding(tile.surface, player->sprite, tile.coords, player->coords)) {
            // Handle collision resolution
            player->setCoords(oldPos.x, oldPos.y);
            predictedState.vel = {0, 0};
            break;
        }
    }

    // Check collisions with other players
    for (Player* other_player : other_players) {
        if (other_player->get_id() != player->get_id()) {
            if (colliding(other_player->sprite, player->sprite, other_player->coords, player->coords)) {
                // Handle collision resolution
                player->setCoords(oldPos.x, oldPos.y);
                predictedState.vel = {0, 0};
                break;
            }
        }
    }

    // Update player velocity
    player->vel = predictedState.vel;
}

void Level::render() {
    display_surface->clear(sf::Color::Cyan);
    
    // Set the view
    display_surface->setView(gameView);
    
    // Draw all game objects
    // sf::Texture bgTexture;
    // if (!bgTexture.loadFromFile("../Sprites/bg1.png")) {
    //     std::cerr << "Error loading background image!" << std::endl;
    // }    
    // sf::Sprite backgroundSprite;
    // backgroundSprite.setTexture(bgTexture);
    // backgroundSprite.setScale(4.5f, 7.f);
    // display_surface->draw(backgroundSprite);
    display_surface->draw(self.sprite);
    for(auto others : other_players) {
        cout << " x: " << others->coords.x << " y: " << others->coords.y << endl;
        display_surface->draw(others->sprite);
    }

    for(auto killingYou: killingThings){
        display_surface->draw(killingYou.surface);
    }
    for(auto tile : tiles) {
        display_surface->draw(tile.surface);
    }
    for(auto collectible: collictibles){
        display_surface->draw(collectible->surface);
    }
    for(auto movingPlat: movingPlatforms){
        display_surface->draw(movingPlat->surface);
    }

    display_surface->display();
} 

void Level::run(){
    // updatePlayer();
    processPendingUpdates();
   // cout << tiles.size() << endl;
    updatePlayer();
    for(auto player : other_players){
        InterpolateEntity(player);
        // player->moveCam(x_shift, y_shift);
    }
    // cout << self.powerups.size() << endl;
    MovingPlatform::updateAllPlatforms(movingPlatforms);
    render();
     //MovingPlatform::updateAllPlatforms(movingPlatforms);
    
}



