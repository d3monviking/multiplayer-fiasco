// #include <SFML/Graphics.hpp>
// #include <bits/stdc++.h>
// #include <SFML/Network.hpp>
// #include <iostream>
// #include <string>
// #include <cstring>
// #include<chrono>
// #include<thread>
// #include "message_generated.h"
// #include <boost/asio.hpp>

// using namespace std;

// using boost::asio::ip::udp;
// boost::asio::io_context io_context;
// udp::socket clientSocket(io_context);
// udp::endpoint serverEndpoint(boost::asio::ip::make_address("172.20.10.5"), 8888);

// struct InterpolationData{
//     sf::Vector2f pos;
//     long timestamp;
// };

// struct Move{
//     long long seq_num;
//     vector<bool> thisMove; // Just declare the vector here
//     Move() : thisMove(4) {}
// };


// int self_id = -1;//will be assigned by server
// vector<const Game::ServerMessage*> updates_buffer;
// vector<vector<InterpolationData>> interpolation_buffer{4};
// flatbuffers::FlatBufferBuilder builder;
// int gameStart=0;

// sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");


// char buf[4096];
// char recvBuf[104];
// int coord = 13;
// std::size_t received;
// sf::IpAddress serverAddress = "172.20.10.5"; // Change to your sender IP
// unsigned short serverPort = 8888;

// // sf::UdpSocket clientSocket;


// long long setCurrentTimestamp() {
//     // Get the current time since epoch (in milliseconds)
//     auto now = chrono::system_clock::now();
//     auto duration = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());

//     // Set the timestamp to the current time
//     return duration.count();  // This will give you the time in milliseconds
// }

// class Player{
//     private:
//         int id=-1;
//         sf::Vector2f pos;
//         float speed = 2.0f;
//         vector<Move> move_history;
        
//     public:
//         sf::CircleShape shape;
//         Player(float x, float y, int id){
//             this->pos.x=x;
//             this->pos.y=y;
//             this->id=id;
//             this->shape.setPosition(x, y);
//             this->shape.setRadius(50);
//             if(this->id==1){
//                 this->shape.setFillColor(sf::Color::Green);
//             }
//             if(this->id==2){
//                 this->shape.setFillColor(sf::Color::Blue);
//             }
//             if(this->id==3){
//                 this->shape.setFillColor(sf::Color::Red);
//             }
//             if(this->id==4){
//                 this->shape.setFillColor(sf::Color::Magenta);
//             }
            
//         }
//         int get_id() {return id;}

//         void set_ID(int id){this->id = id;}

//         sf::Vector2f getPos() {return pos;}
//         float getx() {return pos.x;}
//         float gety() {return pos.y;}


//         void setPos(float x,float y) {
//             this->pos.x=x;
//             this->pos.y=y;
//         }
//         void processPendingUpdates(){
//             cout<<"updates:"<<updates_buffer.size()<<endl;
//             for(int i=0;i<updates_buffer.size();i++){ //apply all pending updates one by one
//                 cout<<"update no:"<<i<<endl;
//                 auto message = updates_buffer[i];
//                 auto states = message->player_data();

//                 for(int j=0;j<states->size();j++){ //find state of self by going through the array sent by the server
//                     auto player_state = states->Get(j);
//                     if(player_state->player_id()==id){
//                         pos.x = player_state->pos()->x(); //set pos to last acked state and replay all inputs from that point to the present
//                         pos.y = player_state->pos()->y();
//                         for(int k=0;k<move_history.size();k++){ //go through move history and replay inputs
//                             cout<<"applying move:"<<k<<endl;
//                             if(move_history[k].seq_num<=player_state->last_processed_seq_number()){
//                                 move_history.erase(move_history.begin()+k); //remove this input since it was already dealt with
//                             }
//                             else{
//                                 applyLocalInput(move_history[k].thisMove); //calculate state for this input
//                             }
//                         }
//                     }
//                 }
//             }

//         }

//         void updateSelf(){
//             // cout<<"chal be"<<endl;
//             Move newMove;
//             newMove.seq_num = move_history.size();
//             int flag=0;
//             if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
//                 flag=1;
//                 newMove.thisMove[0] = 1;
//             }
//             if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
//                 flag=1;
//                 newMove.thisMove[1] = 1;
//             }
//             if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
//                 flag=1;
//                 newMove.thisMove[2] = 1;
//             }
//             if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
//                 flag=1;
//                 newMove.thisMove[3] = 1;
//             }
//             move_history.push_back(newMove);
//             applyLocalInput(newMove.thisMove);

//             // Creating FlatBuffer variables to store and transmit data

//             if(flag==1){

//             flatbuffers::FlatBufferBuilder builder(1024);

//             // Creating PlayerData

//             auto player_id = id;
//             auto position = Game::Vec2(pos.x, pos.y);
//             auto timestamp = std::time(nullptr);
//             auto seq_number = newMove.seq_num;

//             auto selfData = CreatePlayerData(builder, player_id, &position, timestamp, seq_number);

//             // Creating ClientMessage
//             auto player_input = builder.CreateVector(newMove.thisMove);
//             auto clientMessage = Game::CreateClientMessage(builder, selfData, player_input, seq_number);

//             // Creating GameMessage
//             auto gameMessage = Game::CreateGameMessage(builder, Game::GameData_ClientMessage, clientMessage.Union());

//             builder.Finish(gameMessage);
//             uint8_t* buf = builder.GetBufferPointer();
//             size_t size = builder.GetSize();
//             // if(clientSocket.send(buf, size, serverAddress, serverPort) != sf::Socket::Done){
//             //     cerr << "Connection losta\n";
//             // }


           
//             boost::system::error_code ec;
//             clientSocket.send_to(boost::asio::buffer(buf, size), serverEndpoint, 0, ec);
        
//              if (ec) {
//             cerr << "Send failed: " << ec.message() << endl;
//         }
//         else{
//             cout<<"send"<<endl;
//         }
//             }
            
//         }

//         void applyLocalInput(vector<bool> &this_move){
//             for(int i = 0; i < 4; i++){
//                 if(this_move[i] == 1){
//                     switch(i){
//                         case 0: pos.y = pos.y - speed; break;
//                         case 1: pos.x = pos.x - speed; break;
//                         case 2: pos.y = pos.y + speed; break;
//                         case 3: pos.x = pos.x + speed; break;
//                     }
//                 }
//             }

//             this->shape.setPosition(pos);
//             cout<<shape.getPosition().x<<" "<<shape.getPosition().y<<endl;
//         }
// };

// vector<Player> other_players;
// Player self(0,0,1);



// void InterpolateEntity(Player &player){
//     int current_time=setCurrentTimestamp();
//     int rqd_time=current_time-100;
//     int player_id=player.get_id();
//     auto i=interpolation_buffer[player_id].begin();
//     float x,y;
//     while((i+1)->timestamp<rqd_time) i++;
//     auto j=i+1;
//     //Dropping older positions
//     for(auto p=interpolation_buffer[player_id].begin();p!=interpolation_buffer[player_id].end();p++){
//         if(p->timestamp<i->timestamp) p=interpolation_buffer[player_id].erase(p);
//     }
//     //if no need for interpolation
//     if(i->timestamp==rqd_time){
//         x=i->pos.x;
//         y=i->pos.y;
//         player.setPos(x,y);
//         return;
//     }
//     //interpolation logic
//     x=i->pos.x+(((j->pos.x-i->pos.x)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
//     y=i->pos.y+(((j->pos.y-i->pos.y)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
//     player.setPos(x,y);
// }


// void renderGame(){
//     // cout<<self.shape.getPosition().x<<" "<<self.shape.getPosition().y<<endl;
//     // cout << self.shape.getRadius() << endl;
//     window.clear();
//     window.draw(self.shape);
//     for(auto others: other_players){
//         // cout<<others.getx()<<" "<<others.gety()<<endl;
//         window.draw(others.shape);
//     }
//     window.display();
// }

// void updateClient(){
//     // cout<<"rendering"<<endl;
//     while(true){

//         if(gameStart==1){
//             // self.processPendingUpdates();
            
//             self.updateSelf();

//             for(auto player : other_players){
//                 // InterpolateEntity(player);
//             }
//         }

//         renderGame();
//     }
// }

// void receiveFromSender(){
//     while(true){

//       memset(recvBuf, 0, sizeof(recvBuf));


//         // if (clientSocket.receive(recvBuf, sizeof(recvBuf), received, serverAddress, serverPort) != sf::Socket::Done)
//         // {
//         //     cout << "Cannot receive" << endl;
//         // }

//         udp::endpoint senderEndpoint;

//         boost::system::error_code error;
//         size_t len = clientSocket.receive_from(boost::asio::buffer(recvBuf), senderEndpoint, 0, error);
        
//         if (error) {
//             std::cerr << "Receive failed: " << error.message() << std::endl;
//             continue;
//         }

//         const uint8_t* buffer = reinterpret_cast<const uint8_t*>(recvBuf);
    
//         // Verify the buffer is valid before using it
//         // flatbuffers::Verifier verifier(buffer, received);
//         // if (!Game::VerifyGameMessageBuffer(verifier)) {
//         //     std::cerr << "Error: Buffer is not a valid FlatBuffer." << std::endl;
//         // }



//         const Game::GameMessage* message = Game::GetGameMessage(buffer);
//         const Game::ServerMessage* servermessage = static_cast<const Game::ServerMessage *>(message->data_type()); //unpack data

//         if(servermessage->message_code()==0){
//             cout<<"received id"<<endl;
//             if(self_id==-1){
//                 cout<<"id"<<endl;
//                 self_id = servermessage->player_id();
//                 self.set_ID(servermessage->player_id());

//                 for(int i=0;i<servermessage->player_data()->size();i++){
//                     if(servermessage->player_data()->Get(i)->player_id()==self_id){
//                         self.setPos(servermessage->player_data()->Get(self_id-1)->pos()->x(), servermessage->player_data()->Get(self_id-1)->pos()->y());
//                         self.shape.setPosition(servermessage->player_data()->Get(self_id-1)->pos()->x(), servermessage->player_data()->Get(self_id-1)->pos()->y());
//                         self.shape.setRadius(50.f);
//                         if(self_id==1){
//                             self.shape.setFillColor(sf::Color::Green);
//                         }
//                         else if(self_id==2){
//                             self.shape.setFillColor(sf::Color::Blue);
//                         }
//                         else if(self_id==3){
//                             self.shape.setFillColor(sf::Color::Red);
//                         }
//                         else if(self_id==4){
//                             self.shape.setFillColor(sf::Color::Magenta);
//                         }
//                     }
//                 }

//             }
//             if(servermessage->player_data()->size()>other_players.size()+1){
//                 cout<<"psizeserver:"<<servermessage->player_data()->size()<<endl;
//                 cout<<"psizeclient"<<other_players.size()<<endl;
//                 for(int i=other_players.size()+2;i<=servermessage->player_data()->size();i++){
//                     Player p(servermessage->player_data()->Get(i-1)->pos()->x(), servermessage->player_data()->Get(i-1)->pos()->y(), servermessage->player_data()->Get(i-1)->player_id());
//                     other_players.push_back(p);
//                     cout<<p.getx()<<" "<<p.gety()<<endl;
//                     cout<<"now"<<other_players.size()<<endl;
//                 }
//             }
//             continue;
//         }
//         else if(servermessage->message_code()==1 && gameStart==0){
//             gameStart=1;
//             cout<<"Game started"<<endl;
            

//             continue;
//         }

//         // if(servermessage->message_code()==2){
//         //     cout<<"code 2"<<endl;
//         //     continue;
//         // }
//         if(servermessage->player_data()){
//             updates_buffer.push_back(servermessage);
//             auto states = servermessage->player_data();
//             cout<<states->size()<<endl;
//             cout<<"hi"<<endl;
//             for(int j=0;j<states->size();j++){
//                 cout<<"j:"<<j<<endl; 
//                 auto player_state = states->Get(j);
//                     if(player_state->player_id()!=self_id){
//                         cout<<"nahi"<<endl;
//                         struct InterpolationData data;
//                         data.pos.x = player_state->pos()->x();
//                         data.pos.y = player_state->pos()->y();
//                         // Get the current time from the system clock
//                         auto now = chrono::system_clock::now();

//                         // Convert the current time to time since epoch
//                         auto duration = now.time_since_epoch();

//                         // Convert duration to milliseconds
//                         auto milliseconds= chrono::duration_cast<chrono::milliseconds>(duration).count();
//                         data.timestamp = milliseconds;
//                         interpolation_buffer[player_state->player_id()].push_back(data);
//                     }
//             }
//             cout<<"no probles here"<<endl;
//         }
//         else{
//             cout<<"empty"<<endl;
//         }
//     }
// }


// int main()
// {
//     window.setPosition(sf::Vector2i(10, 50));
//     window.setFramerateLimit(60);
//     clientSocket.open(udp::v4());
//     clientSocket.bind(udp::endpoint(udp::v4(), 8887));  
    
//     // if (clientSocket.bind(8887) != sf::Socket::Done)
//     // {
//     //     cout << "Cannot bind" << endl;
//     //     return 1;
//     // }
//     flatbuffers::FlatBufferBuilder builder(1024);

//     // Creating PlayerData

//     auto player_id = -1;
//     auto position = Game::Vec2(2.f, 2.f);
//     auto timestamp = setCurrentTimestamp();
//     auto seq_number = 1;

//     auto selfData = CreatePlayerData(builder, player_id, &position, timestamp, seq_number);

//     // Creating ClientMessage
//     vector<bool> thisMove(4);
//     thisMove[0]=true;
//     auto player_input = builder.CreateVector(thisMove);
//     auto clientMessage = Game::CreateClientMessage(builder, selfData, player_input, seq_number);

//     // Creating GameMessage
//     auto gameMessage = Game::CreateGameMessage(builder, Game::GameData_ClientMessage, clientMessage.Union());

//     builder.Finish(gameMessage);

//     uint8_t* buf = builder.GetBufferPointer();
//     size_t size = builder.GetSize();

//     auto message = Game::GetGameMessage(builder.GetBufferPointer());
//     const Game::ClientMessage* servermessage = static_cast<const Game::ClientMessage *>(message->data_type()); 
//     boost::system::error_code ec;
//     clientSocket.send_to(boost::asio::buffer(buf, size), serverEndpoint, 0, ec);
//      if (ec) {
//             cerr << "Could not send initial message " << ec.message() << endl;
//         }

//     // if(clientSocket.send(buf, size, serverAddress, serverPort) != sf::Socket::Done){
//     //     cerr << "Connection lost\n";
//     // }


//     std::thread receive(receiveFromSender); 
//     std::thread runGame(updateClient);

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//                 clientSocket.close();
//             }

//         }
//     }
//     return 0;
// }
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include<chrono>
#include<thread>
#include "message_generated.h"
#include <boost/asio.hpp>

using namespace std;

using boost::asio::ip::udp;
boost::asio::io_context io_context;
udp::socket clientSocket(io_context);
udp::endpoint serverEndpoint(boost::asio::ip::make_address("172.20.10.5"), 8888);

struct InterpolationData{
    sf::Vector2f pos;
    long timestamp;
};

struct Move{
    long long seq_num;
    vector<bool> thisMove; // Just declare the vector here
    Move() : thisMove(4) {}
};


int self_id = -1;//will be assigned by server
vector<const Game::ServerMessage*> updates_buffer;
vector<vector<InterpolationData>> interpolation_buffer{4};
flatbuffers::FlatBufferBuilder builder;
int gameStart=0;



char buf[4096];
char recvBuf[104];
int coord = 13;
std::size_t received;
sf::IpAddress serverAddress = "172.20.10.5"; // Change to your sender IP
unsigned short serverPort = 8888;

// sf::UdpSocket clientSocket;


long long setCurrentTimestamp() {
    // Get the current time since epoch (in milliseconds)
    auto now = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());

    // Set the timestamp to the current time
    return duration.count();  // This will give you the time in milliseconds
}

class Player{
    private:
        int id=-1;
        sf::Vector2f pos;
        float speed = 2.0f;
        vector<Move> move_history;
        
    public:
        sf::CircleShape shape;
        Player(float x, float y, int id){
            this->pos.x=x;
            this->pos.y=y;
            this->id=id;
            this->shape.setPosition(x, y);
            this->shape.setRadius(50);
            if(this->id==1){
                this->shape.setFillColor(sf::Color::Green);
            }
            if(this->id==2){
                this->shape.setFillColor(sf::Color::Blue);
            }
            if(this->id==3){
                this->shape.setFillColor(sf::Color::Red);
            }
            if(this->id==4){
                this->shape.setFillColor(sf::Color::Magenta);
            }
            
        }
        int get_id() {return id;}

        void set_ID(int id){this->id = id;}

        sf::Vector2f getPos() {return pos;}
        float getx() {return pos.x;}
        float gety() {return pos.y;}


        void setPos(float x,float y) {
            this->pos.x=x;
            this->pos.y=y;
        }
        void processPendingUpdates(){
            cout<<"updates:"<<updates_buffer.size()<<endl;
            for(int i=0;i<updates_buffer.size();i++){ //apply all pending updates one by one
                cout<<"update no:"<<i<<endl;
                auto message = updates_buffer[i];
                auto states = message->player_data();

                for(int j=0;j<states->size();j++){ //find state of self by going through the array sent by the server
                    auto player_state = states->Get(j);
                    if(player_state->player_id()==id){
                        pos.x = player_state->pos()->x(); //set pos to last acked state and replay all inputs from that point to the present
                        pos.y = player_state->pos()->y();
                        for(int k=0;k<move_history.size();k++){ //go through move history and replay inputs
                            cout<<"applying move:"<<k<<endl;
                            if(move_history[k].seq_num<=player_state->last_processed_seq_number()){
                                move_history.erase(move_history.begin()+k); //remove this input since it was already dealt with
                            }
                            else{
                                applyLocalInput(move_history[k].thisMove); //calculate state for this input
                            }
                        }
                    }
                }
            }

        }

        void updateSelf(){
            // cout<<"chal be"<<endl;
            Move newMove;
            newMove.seq_num = move_history.size();
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
            move_history.push_back(newMove);
            applyLocalInput(newMove.thisMove);

            // Creating FlatBuffer variables to store and transmit data

            if(flag==1){

            flatbuffers::FlatBufferBuilder builder(1024);

            // Creating PlayerData

            auto player_id = id;
            auto position = Game::Vec2(pos.x, pos.y);
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
            // if(clientSocket.send(buf, size, serverAddress, serverPort) != sf::Socket::Done){
            //     cerr << "Connection losta\n";
            // }


           
            boost::system::error_code ec;
            clientSocket.send_to(boost::asio::buffer(buf, size), serverEndpoint, 0, ec);
        
             if (ec) {
            cerr << "Send failed: " << ec.message() << endl;
        }
        else{
            cout<<"send"<<endl;
        }
            }
            
        }

        void applyLocalInput(vector<bool> &this_move){
            for(int i = 0; i < 4; i++){
                if(this_move[i] == 1){
                    switch(i){
                        case 0: pos.y = pos.y - speed; break;
                        case 1: pos.x = pos.x - speed; break;
                        case 2: pos.y = pos.y + speed; break;
                        case 3: pos.x = pos.x + speed; break;
                    }
                }
            }

            this->shape.setPosition(pos);
            cout<<shape.getPosition().x<<" "<<shape.getPosition().y<<endl;
        }
};

vector<Player> other_players;
Player self(0,0,1);



void InterpolateEntity(Player &player){
    int current_time=setCurrentTimestamp();
    int rqd_time=current_time-100;
    int player_id=player.get_id();
    auto i=interpolation_buffer[player_id].begin();
    float x,y;
    while((i+1)->timestamp<rqd_time) i++;
    auto j=i+1;
    //Dropping older positions
    for(auto p=interpolation_buffer[player_id].begin();p!=interpolation_buffer[player_id].end();p++){
        if(p->timestamp<i->timestamp) p=interpolation_buffer[player_id].erase(p);
    }
    //if no need for interpolation
    if(i->timestamp==rqd_time){
        x=i->pos.x;
        y=i->pos.y;
        player.setPos(x,y);
        return;
    }
    //interpolation logic
    x=i->pos.x+(((j->pos.x-i->pos.x)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
    y=i->pos.y+(((j->pos.y-i->pos.y)/(j->timestamp-i->timestamp))*(rqd_time-i->timestamp));
    player.setPos(x,y);
}


void renderGame(sf::RenderWindow &window){
    // cout<<self.shape.getPosition().x<<" "<<self.shape.getPosition().y<<endl;
    // cout << self.shape.getRadius() << endl;
    window.clear();
    window.draw(self.shape);
    for(auto others: other_players){
        // cout<<others.getx()<<" "<<others.gety()<<endl;
        window.draw(others.shape);
    }
    window.display();
}

void updateClient(){
    cout<<"thread"<<endl;

sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");
window.setPosition(sf::Vector2i(10, 50));
    window.setFramerateLimit(60);

    // cout<<"rendering"<<endl;
    while(window.isOpen()){

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                clientSocket.close();
            }

        }

        if(gameStart==1){
            // self.processPendingUpdates();
            
            self.updateSelf();

            for(auto player : other_players){
                // InterpolateEntity(player);
            }
        }

        renderGame(window);
    }
}

void receiveFromSender(){
    while(true){

      memset(recvBuf, 0, sizeof(recvBuf));


        // if (clientSocket.receive(recvBuf, sizeof(recvBuf), received, serverAddress, serverPort) != sf::Socket::Done)
        // {
        //     cout << "Cannot receive" << endl;
        // }

        udp::endpoint senderEndpoint;

        boost::system::error_code error;
        size_t len = clientSocket.receive_from(boost::asio::buffer(recvBuf), senderEndpoint, 0, error);
        
        if (error) {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            continue;
        }

        const uint8_t* buffer = reinterpret_cast<const uint8_t*>(recvBuf);
    
        // Verify the buffer is valid before using it
        // flatbuffers::Verifier verifier(buffer, received);
        // if (!Game::VerifyGameMessageBuffer(verifier)) {
        //     std::cerr << "Error: Buffer is not a valid FlatBuffer." << std::endl;
        // }



        const Game::GameMessage* message = Game::GetGameMessage(buffer);
        const Game::ServerMessage* servermessage = static_cast<const Game::ServerMessage *>(message->data_type()); //unpack data

        if(servermessage->message_code()==0){
            cout<<"received id"<<endl;
            if(self_id==-1){
                cout<<"id"<<endl;
                self_id = servermessage->player_id();
                self.set_ID(servermessage->player_id());

                for(int i=0;i<servermessage->player_data()->size();i++){
                    if(servermessage->player_data()->Get(i)->player_id()==self_id){
                        self.setPos(servermessage->player_data()->Get(self_id-1)->pos()->x(), servermessage->player_data()->Get(self_id-1)->pos()->y());
                        self.shape.setPosition(servermessage->player_data()->Get(self_id-1)->pos()->x(), servermessage->player_data()->Get(self_id-1)->pos()->y());
                        self.shape.setRadius(50.f);
                        if(self_id==1){
                            self.shape.setFillColor(sf::Color::Green);
                        }
                        else if(self_id==2){
                            self.shape.setFillColor(sf::Color::Blue);
                        }
                        else if(self_id==3){
                            self.shape.setFillColor(sf::Color::Red);
                        }
                        else if(self_id==4){
                            self.shape.setFillColor(sf::Color::Magenta);
                        }
                    }
                }

            }
            if(servermessage->player_data()->size()>other_players.size()+1){
                cout<<"psizeserver:"<<servermessage->player_data()->size()<<endl;
                cout<<"psizeclient"<<other_players.size()<<endl;
                for(int i=other_players.size()+2;i<=servermessage->player_data()->size();i++){
                    Player p(servermessage->player_data()->Get(i-1)->pos()->x(), servermessage->player_data()->Get(i-1)->pos()->y(), servermessage->player_data()->Get(i-1)->player_id());
                    other_players.push_back(p);
                    cout<<p.getx()<<" "<<p.gety()<<endl;
                    cout<<"now"<<other_players.size()<<endl;
                }
            }
            continue;
        }
        else if(servermessage->message_code()==1 && gameStart==0){
            gameStart=1;
            cout<<"Game started"<<endl;
            

            continue;
        }

        // if(servermessage->message_code()==2){
        //     cout<<"code 2"<<endl;
        //     continue;
        // }
        if(servermessage->player_data()){
            updates_buffer.push_back(servermessage);
            auto states = servermessage->player_data();
            cout<<states->size()<<endl;
            cout<<"hi"<<endl;
            for(int j=0;j<states->size();j++){
                cout<<"j:"<<j<<endl; 
                auto player_state = states->Get(j);
                    if(player_state->player_id()!=self_id){
                        cout<<"nahi"<<endl;
                        struct InterpolationData data;
                        data.pos.x = player_state->pos()->x();
                        data.pos.y = player_state->pos()->y();
                        // Get the current time from the system clock
                        auto now = chrono::system_clock::now();

                        // Convert the current time to time since epoch
                        auto duration = now.time_since_epoch();

                        // Convert duration to milliseconds
                        auto milliseconds= chrono::duration_cast<chrono::milliseconds>(duration).count();
                        data.timestamp = milliseconds;
                        interpolation_buffer[player_state->player_id()].push_back(data);
                    }
            }
            cout<<"no probles here"<<endl;
        }
        else{
            cout<<"empty"<<endl;
        }
    }
}


int main()
{
    
    clientSocket.open(udp::v4());
    clientSocket.bind(udp::endpoint(udp::v4(), 8887));  
    
    // if (clientSocket.bind(8887) != sf::Socket::Done)
    // {
    //     cout << "Cannot bind" << endl;
    //     return 1;
    // }
    flatbuffers::FlatBufferBuilder builder(1024);

    // Creating PlayerData

    auto player_id = -1;
    auto position = Game::Vec2(2.f, 2.f);
    auto timestamp = setCurrentTimestamp();
    auto seq_number = 1;

    auto selfData = CreatePlayerData(builder, player_id, &position, timestamp, seq_number);

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

    auto message = Game::GetGameMessage(builder.GetBufferPointer());
    const Game::ClientMessage* servermessage = static_cast<const Game::ClientMessage *>(message->data_type()); 
    boost::system::error_code ec;
    clientSocket.send_to(boost::asio::buffer(buf, size), serverEndpoint, 0, ec);
     if (ec) {
            cerr << "Could not send initial message " << ec.message() << endl;
        }

    // if(clientSocket.send(buf, size, serverAddress, serverPort) != sf::Socket::Done){
    //     cerr << "Connection lost\n";
    // }

    cout<<"ok"<<endl;

    std::thread update(updateClient);
    std::thread receive(receiveFromSender); 
    update.join();
    receive.join();
    // while (window.isOpen())
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed) {
    //             window.close();
    //             clientSocket.close();
    //         }

    //     }
    // }
    return 0;
}
