import Game.ClientMessage;
import java.io.IOException;
import java.net.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;


public class Server {
    public static DatagramSocket udpSocket;
    public static int serverPort = 8888;
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

    public static int ipv4ToInt(String ip) {
        String[] parts = ip.split("\\.");
        int result = 0;
        for (int i = 0; i < parts.length; i++) {
            result |= (Integer.parseInt(parts[i]) << (24 - (8 * i)));
        }
        return result;
    }


    public static void main(String[] args) throws SocketException {
        udpSocket = new DatagramSocket(serverPort);
        try {
            Server.udpSocket.setBroadcast(true);
        } catch (SocketException e) {
            throw new RuntimeException(e);
        }
        System.out.println("Server has started waiting for clients.....");
        // Start receive thread
        new Thread(new receive(messageQueue)).start();
        int x = 0;
        try {
            x = System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println(x);
        gameState = 1;

        // Start calculate thread
        new Thread(new Calculate(messageQueue)).start();
        new Thread(new SendServerMessage(50)).start();
        // Placeholder for the Send thread (if you need it)
        // new Thread(new Send()).start();
    }
    // Add player if not exists, based on their IP and port
    public static synchronized void addNewPlayer(InetAddress clientAddress, int clientPort, int lastProcessedSeqNum) throws UnknownHostException {
//        if (gameState == 1) return;
        InetSocketAddress clientSocketAddress = new InetSocketAddress(clientAddress, clientPort);
        for (Player player : playerList) {
            if (player.getAddress().equals(clientSocketAddress)) {
                return;  // Player already exists
            }
        }
        Player newPlayer = new Player(clientSocketAddress, new Vec2(50, 350*16), ++playerCount, lastProcessedSeqNum, System.currentTimeMillis());
        playerList.add(newPlayer);
        DatagramPacket playerIDPacket = SendServerMessage.makeServerMessage(0, newPlayer.getPlayerId());
        try {
            udpSocket.send(playerIDPacket);
            udpSocket.send(playerIDPacket);
            playerIDPacket.setAddress(newPlayer.getAddress().getAddress());
            //dummy commment to test git
            playerIDPacket.setPort(newPlayer.getAddress().getPort());
            udpSocket.send(playerIDPacket);
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("New player added: " + newPlayer);
    }
}
