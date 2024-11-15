import Game.*;
import Game.Vec2;
import Game.ServerMessage;
import com.google.flatbuffers.*;

import javax.xml.crypto.Data;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.List;

public class SendServerMessage implements Runnable {
    private final int serverRefreshRate;

    public static DatagramPacket makeServerMessage(int messageCode, int playerID) throws UnknownHostException {
        FlatBufferBuilder fbb = new FlatBufferBuilder(0);
        List<Player> playerList = Server.getPlayerList();
        byte[] data;
        DatagramPacket message = null;
        int playerDataList[] = new int[playerList.size()];
        for(int i = 0; i < playerList.size(); i++) {
            Player player = playerList.get(i);
            PlayerData.startPlayerData(fbb);
            PlayerData.addPlayerId(fbb, player.getPlayerId());
            int pos = Vec2.createVec2(fbb, player.getCoordinates().getX(), player.getCoordinates().getY());
            int vel = Vec2.createVec2(fbb, player.vel.getX(), player.vel.getY());
            PlayerData.addVel(fbb, vel);
            // System.out.println("Player ID: " + player.getPlayerId() + " X: " + player.getCoordinates().getX() + " Y: " + player.getCoordinates().getY());
            PlayerData.addPos(fbb, pos);
            PlayerData.addTimestamp(fbb, player.getTimestampMilli());
            PlayerData.addLastProcessedSeqNumber(fbb, player.getLastProcessedSeqNum());
            playerDataList[i] = PlayerData.endPlayerData(fbb);
        }
        int playerDataVector = ServerMessage.createPlayerDataVector(fbb, playerDataList);
        if(messageCode == 2){
            ServerMessage.startServerMessage(fbb);
            ServerMessage.addMessageCode(fbb, messageCode);
            ServerMessage.addPlayerData(fbb, playerDataVector);
            ServerMessage.addPlayerId(fbb, 0);
            int serverMessage = ServerMessage.endServerMessage(fbb);
            data = makeGameMessage(fbb, serverMessage);
            message = new DatagramPacket(data, data.length);
        }
        else if(messageCode == 0 || messageCode == 1){
            ServerMessage.startServerMessage(fbb);
            ServerMessage.addMessageCode(fbb, messageCode);
            ServerMessage.addPlayerId(fbb, playerID);
            ServerMessage.addPlayerData(fbb, playerDataVector);
            int serverMessage = ServerMessage.endServerMessage(fbb);
            data = makeGameMessage(fbb, serverMessage);
            message = new DatagramPacket(data, data.length);
        }
        message.setPort(8887);
        message.setAddress(InetAddress.getByName("255.255.255.255"));
        return message;
    }

    private static byte[] makeGameMessage(FlatBufferBuilder fbb, int serverMessage) {
        byte[] data;
        GameMessage.startGameMessage(fbb);
        GameMessage.addDataTypeType(fbb, GameData.ServerMessage);
        GameMessage.addDataType(fbb, serverMessage);
        int gameMessage = GameMessage.endGameMessage(fbb);
        fbb.finish(gameMessage);
        data = fbb.sizedByteArray();
        return data;
    }

    public SendServerMessage(int serverRefreshRate) {
        this.serverRefreshRate = serverRefreshRate;

    }

    public void run() {
        try {
            DatagramPacket gameStart = makeServerMessage(1, 0);
            Server.udpSocket.send(gameStart);
            for(Player player : Server.getPlayerList()) {
                gameStart.setAddress(player.getAddress().getAddress());
                gameStart.setPort(player.getAddress().getPort());
                Server.udpSocket.send(gameStart);
            }
            System.out.println("Sent start Message!");
            while (true) {
                try {
//                    System.out.println("Sent new Message!");
                    DatagramPacket packet = makeServerMessage(2, -1);
                    Server.udpSocket.send(packet);
                    try {
                        //noinspection BusyWait
                        Thread.sleep(serverRefreshRate);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
