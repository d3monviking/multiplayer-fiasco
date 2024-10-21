import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;

import javax.xml.crypto.Data;

import com.google.flatbuffers.FlatBufferBuilder;
import Game.ClientMessage;

public class receive implements Runnable {
    private DatagramPacket packet;
    

    public receive(DatagramPacket packet) {
        this.packet = packet;
    }

    public ClientMessage receiveMessage() {
        byte[] buffer = packet.getData();
        ByteBuffer buff = ByteBuffer.wrap(buffer);
        ClientMessage message = ClientMessage.getRootAsClientMessage(buff);
        return message;
    }

    @Override
    public void run() {
        while(true) {
            try {
                DatagramPacket packet = new DatagramPacket(new byte[1024], 1024);
                Server.udpSocket.receive(packet);
                this.packet = packet;
                receiveMessage();

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
}