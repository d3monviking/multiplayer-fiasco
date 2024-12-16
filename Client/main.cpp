#include "level.h"

using namespace std;

using boost::asio::ip::udp;
boost::asio::io_context io_context;
udp::socket clientSocket(io_context);
udp::endpoint serverEndpoint(boost::asio::ip::make_address("127.0.0.1"), 8888);

int screen_width=1920;
int screen_height=1080;
sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "SFML works!");

auto window_ptr =  &window;

Level level(window_ptr, screen_width);

char recvBuf[1024];
std::size_t received=sizeof(recvBuf);
int self_id=-1;
int gameStart=0;

vector<SelfData> updates_buffer;
vector<vector<InterpolationData>> interpolation_buffer(4);

Player self(0, 0, 1);
vector<Player*> other_players;

void receiveFromSender(){
    while(true){

        memset(recvBuf, 0, sizeof(recvBuf));

        boost::system::error_code error;
        size_t len = clientSocket.receive_from(boost::asio::buffer(recvBuf), serverEndpoint, 0, error);
        
        if (error) {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            continue;
        }

        const uint8_t* buffer = reinterpret_cast<const uint8_t*>(recvBuf);

        flatbuffers::Verifier verifier(buffer, received);
        if(!Game::VerifyGameMessageBuffer(verifier)) {
            std::cerr << "Error: Buffer is not a valid FlatBuffer." << std::endl;
        }

        const Game::GameMessage* message = Game::GetGameMessage(buffer);
        const Game::ServerMessage* servermessage = static_cast<const Game::ServerMessage *>(message->data_type()); //unpack data

        if(servermessage->message_code()==0){
            cout<<"received id"<<endl;
            if(self_id==-1){
                self_id = servermessage->player_id();
                self.set_id(servermessage->player_id());
                // cout<<"id set"<<endl;
                if(!servermessage->player_data()){cout<<"null"<<endl;}
                cout<<"id:"<<self_id<<endl;
                for(int i=0;i<servermessage->player_data()->size();i++){
                    if(servermessage->player_data()->Get(i)->player_id()==self_id){
                        self.setPos(servermessage->player_data()->Get(i)->pos()->x(), servermessage->player_data()->Get(i)->pos()->y());
                        self.sprite.setPosition(servermessage->player_data()->Get(i)->pos()->x(), servermessage->player_data()->Get(i)->pos()->y());
                        self.setCoords(servermessage->player_data()->Get(i)->pos()->x(), servermessage->player_data()->Get(i)->pos()->y());
                        // self.sprite.setSize(sf::Vector2f(50, 50));
                        self.setSprite(self_id);
                    }
                }

            }
            if(servermessage->player_data()->size()>other_players.size()+1){
                for(int i=0;i<servermessage->player_data()->size();i++){
                    int f=0;
                    for(int j=0;j<other_players.size();j++){
                        if(servermessage->player_data()->Get(i)->player_id()==other_players[j]->get_id()){
                            f=1;//already have this player so don't add it
                            break;
                        }
                    }
                    if(f==0 && servermessage->player_data()->Get(i)->player_id()!=self_id){
                        Player* p = new Player(servermessage->player_data()->Get(i)->pos()->x(), servermessage->player_data()->Get(i)->pos()->y(), servermessage->player_data()->Get(i)->player_id());
                        other_players.push_back(p);
                        // cout<<p->getx()<<" "<<p->gety()<<endl;
                        // cout<<"now"<<other_players.size()<<endl;
                    }
                }
            }
            continue;
        }
        else if(servermessage->message_code()==1 && gameStart==0){
            gameStart=1;
            string terrainPath = "./TileMapFiles/level_terrain.csv";
            string collectiblePath = "./TileMapFiles/level_powerups.csv";
            string movingPlatformPath = "./TileMapFiles/level_moving_platform.csv";
            string rocksPath = "./TileMapFiles/level_rocks.csv";
            string waterPath = "./TileMapFiles/level_water.csv";
            string treePath = "./TileMapFiles/level_tree.csv";
            string backgroundPath = "./TileMapFiles/level_background.csv";
            string spikePath = "./TileMapFiles/level_spikes.csv";
            cout<<"Game started"<<endl;  
            level.set_id(self_id);

            level.setup_level(terrainPath, collectiblePath, movingPlatformPath, rocksPath, treePath, waterPath, backgroundPath, spikePath);
            continue;
            continue;
        }
        //if message code==2:
        else if(servermessage->message_code()==2){
            auto states = servermessage->player_data();
            for(int j=0;j<states->size();j++){
                auto player_state = states->Get(j);
                if(player_state->player_id()!=self_id){
                    struct InterpolationData data;
                    data.pos.x = player_state->pos()->x();
                    data.pos.y = player_state->pos()->y();
                    data.vel.x = player_state->vel()->x();
                    data.vel.y = player_state->vel()->y();
                    // Get the current time from the system clock
                    auto now = chrono::system_clock::now();

                    // Convert the current time to time since epoch
                    auto duration = now.time_since_epoch();

                    // Convert duration to milliseconds
                    long long milliseconds= chrono::duration_cast<chrono::milliseconds>(duration).count();
                    // cout<<"interpolate:"<<milliseconds<<" "<<data.pos.y<<endl;
                    data.timestamp = milliseconds;
                    interpolation_buffer[player_state->player_id()-1].push_back(data);
                }
                else{
                    struct SelfData data;
                    data.pos.x = player_state->pos()->x();
                    data.pos.y = player_state->pos()->y();
                    data.vel.x=player_state->vel()->x();
                    data.vel.y=player_state->vel()->y();
                    data.timestamp=player_state->timestamp();
                    data.last_processed_seq_num=player_state->last_processed_seq_number();
                    updates_buffer.push_back(data);
                }
            }
            continue;
        }
        

        for(int i=0;i<level.collictibles.size();i++){
            Collectibles* collectible=level.collictibles[i];
            auto states = servermessage->player_data();
            int flag=0;
            for(int j=0;j<states->size();j++){
                auto collectible_state = states->Get(j);
                if(collectible_state->pos()->x()==collectible->coords.x &&
                collectible_state->pos()->y()==collectible->coords.y){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                level.collictibles.erase(level.collictibles.begin()+i);
            }
        }
    }
}

int main(){
    // cout<<"bruh"<<endl;
    window.setPosition(sf::Vector2i(10, 50));
    window.setFramerateLimit(30);

    clientSocket.open(udp::v4());
    clientSocket.bind(udp::endpoint(udp::v4(), 8887)); 

    std::thread receive(receiveFromSender); 

    flatbuffers::FlatBufferBuilder builder(1024);

    // Creating PlayerData (empty for the initial message)

    auto player_id = -1;
    auto position = Game::Vec2(2.f, 2.f);
    auto vel = Game::Vec2(2.f, 2.f);
    auto timestamp = 0;
    auto seq_number = -1;

    auto selfData = CreatePlayerData(builder, player_id, &position, &vel,timestamp, seq_number);

    // Creating ClientMessage
    vector<bool> thisMove(4);
    thisMove[0]=true;
    auto player_input = builder.CreateVector(thisMove);
    auto clientMessage = Game::CreateClientMessage(builder, selfData, player_input, seq_number);

    // Creating GameMessage
    auto gameMessage = Game::CreateGameMessage(builder, Game::GameData_ClientMessage, clientMessage.Union());

    builder.Finish(gameMessage);

    uint8_t* buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();
    // cout<<size<<endl;
    // cout<<"hiiiiiiiiiii"<<endl;

    boost::system::error_code ec;
    clientSocket.send_to(boost::asio::buffer(buf, size), serverEndpoint, 0, ec);
    if(ec){
        cerr<<"Could not send initial message "<<ec.message()<<endl;
    }
    else{
        cout<<"sent initial message"<<endl;
    }

    while(window.isOpen()){

        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                clientSocket.close();
            }

        }

        if(gameStart==0){
           window.clear(sf::Color::Cyan);
           window.draw(self.sprite);
           for(auto others: other_players){
                window.draw(others->sprite);
            }
            window.display();
        }
        else{
            // cout<<"running level"<<endl;
            level.run();
        }

    }

    receive.join();
    return 0;

}