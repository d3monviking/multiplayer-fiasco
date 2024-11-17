import Game.ClientMessage;
import Game.GameData;
import Game.GameMessage;
import Game.PlayerData;

import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;
import java.util.concurrent.BlockingQueue;

import com.google.flatbuffers.FlatBufferBuilder;

public class receive implements Runnable {
    // private DatagramPacket packet;
    public BlockingQueue<ClientMessage> messageQueue;
    // public static int mvnum = 0;

    public receive(BlockingQueue<ClientMessage> messageQueue) {
        this.messageQueue = messageQueue;
    }

    public static ClientMessage receiveMessage(DatagramPacket packet) {
        byte[] buffer = packet.getData();
        // System.out.println(packet.getSocketAddress());
        ByteBuffer buff = ByteBuffer.wrap(buffer);
        GameMessage gameMessage = GameMessage.getRootAsGameMessage(buff);
        ClientMessage message = (ClientMessage) gameMessage.dataType(new ClientMessage());
        // System.out.println(message.selfData().playerId());
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
        // Game.Vec2 position = selfData.pos();
        // if (position != null) {
        //     // System.out.println("Player position: " + position.x() + ", " + position.y());
        //     System.out.println("Sequence number from receive: " + clientMessage.sequenceNumber());
        // } else {
        //     System.out.println("Player position is null");
        // }

        // Uncomment if needed
        // System.out.println("Player timestamp: " + selfData.timestamp());
    }

    @Override
    public void run() {
        // System.out.println("hello0");
        while (true) {
            // System.out.println("hello11");
            DatagramPacket packet = new DatagramPacket(new byte[112], 112);
            try {
                // System.out.println("hello12");
                Server.udpSocket.receive(packet);
                // System.out.println("hello13");
            } catch (NullPointerException | IOException e) {
                e.printStackTrace();
                // System.out.println("hello1");
//                    ClientMessage receivedMessage = receiveMessage(packet);
            }
            ClientMessage receivedMessage = receiveMessage(packet);

            if (messageQueue != null && receivedMessage != null) {
                try {
                    messageQueue.put(receivedMessage); // Safely put message into queue
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            InetAddress clientAddress = packet.getAddress();
            int clientPort = packet.getPort();
            try {
                Server.addNewPlayer(clientAddress, clientPort, receivedMessage.sequenceNumber());  // Check and add player if needed
            } catch (UnknownHostException e) {
                throw new RuntimeException(e);
            }
        }
    }
}