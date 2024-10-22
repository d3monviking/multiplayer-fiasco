import Game.ClientMessage;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;


public class Server {
    public static DatagramSocket udpSocket;
    public static int serverPort = 8888;
    public static int bufferSize = 1024;
    private static List<Player> playerList = new ArrayList<>();
    private static BlockingQueue<ClientMessage> messageQueue = new LinkedBlockingQueue<>();
    private static int gameState = 0;

    public static List<Player> getPlayerList() {
        return playerList;
    }

    private static int playerCount = 0;

    public static int assignPlayerId() {
        return playerCount;
    }

    public static void main(String[] args) throws SocketException {
        udpSocket = new DatagramSocket(serverPort);
        // Start receive thread
        new Thread(new receive(messageQueue)).start();

        try {
            int x = System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
        gameState = 1;

        // Start calculate thread
        new Thread(new Calculate(messageQueue)).start();
        new Thread(new SendServerMessage(100)).start();
        // Placeholder for the Send thread (if you need it)
        // new Thread(new Send()).start();
    }

    // Add player if not exists, based on their IP and port
    public static synchronized void addNewPlayer(InetAddress clientAddress, int clientPort) {
        if (gameState == 1) return;
        InetSocketAddress clientSocketAddress = new InetSocketAddress(clientAddress, clientPort);
        for (Player player : playerList) {
            if (player.getAddress().equals(clientSocketAddress)) {
                return;  // Player already exists
            }
        }
        Player newPlayer = new Player(clientSocketAddress, new Vec2(0, playerList.size() + 2), playerList.size() + 1, 0, System.currentTimeMillis());
        playerList.add(newPlayer);
        DatagramPacket playerIDPacket = SendServerMessage.makeServerMessage(0, playerCount);
        playerCount++;
        try {
            udpSocket.send(playerIDPacket);
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("New player added: " + newPlayer);
    }
}