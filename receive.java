import Game.ClientMessage;
import Game.GameData;
import Game.GameMessage;
import Game.PlayerData;

import java.net.*;
import java.nio.ByteBuffer;
import java.util.concurrent.BlockingQueue;

import com.google.flatbuffers.FlatBufferBuilder;

public class receive implements Runnable {
    // private DatagramPacket packet;
    public BlockingQueue<ClientMessage> messageQueue;


    public receive(BlockingQueue<ClientMessage> messageQueue) {
        this.messageQueue = messageQueue;
    }

    public static ClientMessage receiveMessage(DatagramPacket packet) { 
        byte[] buffer = packet.getData();
        ByteBuffer buff = ByteBuffer.wrap(buffer);
        GameMessage gameMessage = GameMessage.getRootAsGameMessage(buff);
        ClientMessage message = (ClientMessage) gameMessage.dataType(new ClientMessage());
        // ClientMessage message = ClientMessage.getRootAsClientMessage(buff);
        return message;
    }

    // public void printMessage(ClientMessage clientMessage) {
    //     if(clientMessage == null)return;
    //     System.out.println("received message with lol number");
    //     System.out.println("received message with sequence number: " + clientMessage.sequenceNumber());
    //     System.out.println(" and player ID: " + clientMessage.selfData().playerId());
    //     System.out.println("Player inputs: " + clientMessage.playerInput(0) + ", " + clientMessage.playerInput(1) + ", " + clientMessage.playerInput(2) + ", " + clientMessage.playerInput(3));
    //     System.out.println("Player position: " + clientMessage.selfData().pos().x() + ", " + clientMessage.selfData().pos().y());
    //     // System.out.println("Player timestamp: " + clientMessage.selfData().timestamp());
    // }
    public static void printMessage(ClientMessage clientMessage) {
        if (clientMessage == null) {
            System.out.println("ClientMessage is null");
            return;
        }
    
        System.out.println("received message with sequence number: " + clientMessage.sequenceNumber());
    
        PlayerData selfData = clientMessage.selfData();
        if (selfData == null) {
            System.out.println("PlayerData is null");
        }
    
        // System.out.println("Player ID: " + selfData.playerId());
    
        // Checking if playerInput exists
        if (clientMessage.playerInputLength() >= 4) {
            System.out.println("Player inputs: " + clientMessage.playerInput(0) + ", " 
                + clientMessage.playerInput(1) + ", " + clientMessage.playerInput(2) + ", " 
                + clientMessage.playerInput(3));
        } else {
            System.out.println("Player inputs are missing or not enough inputs provided.");
        }
    
        // Checking if position exists
        Game.Vec2 position = selfData.pos();
        if (position != null) {
            System.out.println("Player position: " + position.x() + ", " + position.y());
        } else {
            System.out.println("Player position is null");
        }
    
        // Uncomment if needed
        // System.out.println("Player timestamp: " + selfData.timestamp());
    }

    @Override
    public void run() {
        
            try {
                while(true){
                    DatagramPacket packet = new DatagramPacket(new byte[104], 104);
                    try{
                    Server.udpSocket.receive(packet);
                }
                catch(NullPointerException e){
                    e.printStackTrace();
                    ClientMessage receivedMessage = receiveMessage(packet);
                    printMessage(receivedMessage);
                }
                    ClientMessage receivedMessage = receiveMessage(packet);
                    printMessage(receivedMessage);

                    if (messageQueue != null && receivedMessage != null) {
                        try {
                            messageQueue.put(receivedMessage); // Safely put message into queue
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                    // messageQueue.put(receivedMessage);
                    // Hand IP and port to server for player creation if needed
                    InetAddress clientAddress = packet.getAddress();
                    int clientPort = packet.getPort();
                    Server.addNewPlayer(clientAddress, clientPort, receivedMessage.sequenceNumber());  // Check and add player if needed
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    
}
// import Game.ClientMessage;
// import Game.GameData;
// import Game.GameMessage;
// import Game.PlayerData;

// import java.net.*;
// import java.nio.ByteBuffer;
// import java.util.concurrent.BlockingQueue;
// import java.io.IOException;

// public class receive implements Runnable {
//     private DatagramPacket packet;
//     public BlockingQueue<ClientMessage> messageQueue;
//     public receive(BlockingQueue<ClientMessage> messageQueue) {
//         this.messageQueue = messageQueue;
//     }

//     public ClientMessage receiveMessage(DatagramPacket packet) {
//         byte[] buffer = packet.getData();
//         ByteBuffer buff = ByteBuffer.wrap(buffer);
//         // GameMessage gameMessage = GameMessage.getRootAsGameMessage(buff);
//         // ClientMessage message = (ClientMessage) gameMessage.dataType(new ClientMessage());
//         ClientMessage message = ClientMessage.getRootAsClientMessage(buff);
//         return message;
//     }

//     public void printMessage(ClientMessage clientMessage) {
//         if (clientMessage == null) {
//             System.out.println("ClientMessage is null");
//             return;
//         }

//         System.out.println("received message with sequence number: " + clientMessage.sequenceNumber());

//         PlayerData selfData = clientMessage.selfData();
//         if (selfData == null) {
//             System.out.println("PlayerData is null");
//             return; // Add return here to prevent NullPointerException
//         }

//         System.out.println("Player ID: " + selfData.playerId());

//         // Checking if playerInput exists
//         if (clientMessage.playerInputLength() >= 4) {
//             System.out.println("Player inputs: " + clientMessage.playerInput(0) + ", "
//                     + clientMessage.playerInput(1) + ", " + clientMessage.playerInput(2) + ", "
//                     + clientMessage.playerInput(3));
//         } else {
//             System.out.println("Player inputs are missing or not enough inputs provided.");
//         }

//         // Checking if position exists
//         Game.Vec2 position = selfData.pos();
//         if (position != null) {
//             System.out.println("Player position: " + position.x() + ", " + position.y());
//         } else {
//             System.out.println("Player position is null");
//         }

//         // Uncomment if needed
//         // System.out.println("Player timestamp: " + selfData.timestamp());
//     }

//     @Override
//     public void run() {
//         try {
//             while (true) {
//                 packet = new DatagramPacket(new byte[1024], 1024);
//                 try {
//                     Server.udpSocket.receive(packet);
//                 } catch (IOException e) {
//                     e.printStackTrace();
//                     continue; // Skip to the next iteration if an exception occurs
//                 }

//                 ClientMessage receivedMessage = receiveMessage(packet);
//                 printMessage(receivedMessage);

//                 if (messageQueue != null && receivedMessage != null) {
//                     try {
//                         messageQueue.put(receivedMessage); // Safely put message into queue
//                     } catch (InterruptedException e) {
//                         e.printStackTrace();
//                         Thread.currentThread().interrupt(); // Restore the interrupted status
//                     }
//                 }

//                 // Hand IP and port to server for player creation if needed
//                 InetAddress clientAddress = packet.getAddress();
//                 int clientPort = packet.getPort();
//                 Server.addNewPlayer(clientAddress, clientPort, receivedMessage.sequenceNumber()); // Check and add player if needed
//             }
//         } catch (Exception e) {
//             e.printStackTrace();
//         }
//     }
// }