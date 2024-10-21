import Game.ClientMessage;
import java.util.concurrent.BlockingQueue;


public class Calculate implements Runnable {
    private BlockingQueue<ClientMessage> messageQueue;

    public Calculate(BlockingQueue<ClientMessage> messageQueue) {
        this.messageQueue = messageQueue;
    }

    @Override
    public void run() {
        try {
            while (true) {
                // Get the next client message from the queue
                ClientMessage clientMessage = messageQueue.take();

                // Find the corresponding player
                Player player = findPlayer(clientMessage.selfData().playerId());

                if (player != null) {
                    // Update player's coordinates based on input array
                    boolean[] inputs = getInputsFromClientMessage(clientMessage);

                    Vec2 currentCoords = player.getCoordinates();
                    float newX = currentCoords.getX() + (inputs[0] ? 2 : 0);  // Example for input 0
                    float newY = currentCoords.getY() + (inputs[1] ? 2 : 0);  // Example for input 1

                    // Update player coordinates
                    player.setCoordinates(new Vec2(newX, newY));
                    player.updateTimestamp();

                    System.out.println("Updated player " + player.getPlayerId() + " to new coordinates: " + newX + ", " + newY);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // Helper method to find player by ID
    private Player findPlayer(int playerId) {
        for (Player player : Server.getPlayerList()) {
            if (player.getPlayerId() == playerId) {
                return player;
            }
        }
        return null;
    }

    // Method to extract boolean array from ClientMessage
    private boolean[] getInputsFromClientMessage(ClientMessage clientMessage) {
        boolean[] inputs = new boolean[clientMessage.playerInputLength()];
        for (int i = 0; i < inputs.length; i++) {
            inputs[i] = clientMessage.playerInput(i);
        }
        return inputs;
    }
}
