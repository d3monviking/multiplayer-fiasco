import Game.ClientMessage;
import Game.GameMessage;
import Game.GameData;

import java.net.*;
import java.nio.ByteBuffer;
import java.util.concurrent.BlockingQueue;

public class receive implements Runnable {
    private DatagramPacket packet;
    public BlockingQueue<ClientMessage> messageQueue;


    public receive(BlockingQueue<ClientMessage> messageQueue) {
        this.messageQueue = messageQueue;
    }

    public ClientMessage receiveMessage() {
        byte[] buffer = packet.getData();
        ByteBuffer buff = ByteBuffer.wrap(buffer);
        //ClientMessage message = ClientMessage.getRootAsClientMessage(buff);
        GameMessage gameMessage = GameMessage.getRootAsGameMessage(buff);
        int dataType = gameMessage.dataTypeType();
        if(dataType == GameData.ClientMessage){
            ClientMessage message = (ClientMessage) gameMessage.dataType(new ClientMessage());
            return message;
        } else {
            return null;
        }
    }

    @Override
    public void run() {
        
            try {
                while(true){
                    DatagramPacket packet = new DatagramPacket(new byte[1024], 1024);
                    Server.udpSocket.receive(packet);
                    this.packet = packet;
                    ClientMessage receivedMessage = receiveMessage();
                    messageQueue.put(receivedMessage);
                    // Hand IP and port to server for player creation if needed
                    InetAddress clientAddress = packet.getAddress();
                    int clientPort = packet.getPort();
                    Server.addNewPlayer(clientAddress, clientPort);  // Check and add player if needed
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    
}