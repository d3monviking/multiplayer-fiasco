import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.util.concurrent.BlockingQueue;

import javax.xml.crypto.Data;

import com.google.flatbuffers.FlatBufferBuilder;
import Game.ClientMessage;

public class receive implements Runnable {
    private DatagramPacket packet;
    public BlockingQueue<ClientMessage> messageQueue;


    public receive(DatagramPacket packet, BlockingQueue<ClientMessage> messageQueue) {
        this.packet = packet;
        this.messageQueue = messageQueue;
    }

    public ClientMessage receiveMessage() {
        byte[] buffer = packet.getData();
        ByteBuffer buff = ByteBuffer.wrap(buffer);
        ClientMessage message = ClientMessage.getRootAsClientMessage(buff);
        return message;
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
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    
}