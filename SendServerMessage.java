import Game.*;
import Game.GameData;
import Game.Vec2;
import com.google.flatbuffers.*;

import java.net.DatagramPacket;
import java.util.List;

public class SendServerMessage implements Runnable {
    private int serverRefreshRate;

    public SendServerMessage(int serverRefreshRate) {
        this.serverRefreshRate = serverRefreshRate;
    }

    public void run() {
        while (true) {
            FlatBufferBuilder fbb = new FlatBufferBuilder(0);
            List<Player> playerList = Server.getPlayerList();
            int playerDataList[] = new int[playerList.size()];
            for(int i = 0; i < playerList.size(); i++) {
                Player player = playerList.get(i);
                PlayerData.startPlayerData(fbb);
                PlayerData.addPlayerId(fbb, player.getPlayerId());
                int pos = Vec2.createVec2(fbb, player.getCoordinates().getX(), player.getCoordinates().getY());
                PlayerData.addPos(fbb, pos);
                PlayerData.addTimestamp(fbb, player.getTimestampMilli());
                PlayerData.addLastProcessedSeqNumber(fbb, player.getLastProcessedSeqNum());
                playerDataList[i] = PlayerData.endPlayerData(fbb);
            }
            int playerDataVector = ServerMessage.createPlayerDataVector(fbb, playerDataList);
            ServerMessage.startServerMessage(fbb);
            ServerMessage.addMessageCode(fbb, 1);
            ServerMessage.addPlayerData(fbb, playerDataVector);
            ServerMessage.addPlayerId(fbb, 0);
            int serverMessage = ServerMessage.endServerMessage(fbb);
            GameMessage.startGameMessage(fbb);
            GameMessage.addDataTypeType(fbb, GameData.ServerMessage);
            GameMessage.addDataType(fbb, serverMessage);
            int gameMessage = GameMessage.endGameMessage(fbb);
            fbb.finish(gameMessage);
            byte[] data = fbb.sizedByteArray();
            // broadcast to all clients
            DatagramPacket message = new DatagramPacket(data, data.length, playerList.get(0).getAddress());
            try {
                Thread.sleep(serverRefreshRate);
                // Add your server message sending logic here
            } catch (InterruptedException e) {
                // Handle exception, for example, break the loop to stop the thread
                Thread.currentThread().interrupt(); // Retain the interrupt status
                break;
            }
        }
    }
}
