import Game.ClientMessage;
import java.util.concurrent.BlockingQueue;


public class Calculate implements Runnable {
    private BlockingQueue<ClientMessage> messageQueue;

    public Calculate(BlockingQueue<ClientMessage> messageQueue) {
        this.messageQueue = messageQueue;
    }
    private int mvnum = 0;

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
                    // for (boolean input : inputs) {
                    //     System.out.print(input + " ");
                    // }
                    Vec2 currentCoords = player.getCoordinates();
                    // System.out.println("Current coordinates: " + currentCoords.getX() + ", " + currentCoords.getY());
                    // float newX, newY;
                    // newY = currentCoords.getY() + (inputs[0] ? -2 : 0);  // Example for input 0
                    // newX = currentCoords.getX() + (inputs[1] ? -2 : 0);  // Example for input 0
                    // newY = currentCoords.getY() + (inputs[2] ? 2 : 0);  // Example for input 1
                    // newX = currentCoords.getX() + (inputs[3] ? 2 : 0);  // Example for input 1
                    float newX = currentCoords.getX();
                    float newY = currentCoords.getY();

                    newY += (inputs[0] ? -2 : 0);  // Move up if inputs[0] is true
                    newX += (inputs[1] ? -2 : 0);  // Move left if inputs[1] is true
                    newY += (inputs[2] ? 2 : 0);   // Move down if inputs[2] is true
                    newX += (inputs[3] ? 2 : 0);   // Move right if inputs[3] is true

                    // Update player coordinates
                    player.setCoordinates(new Vec2(newX, newY));
                    receive.printMessage(clientMessage);
                    System.out.println(player.getCoordinates().getX() + " " + player.getCoordinates().getY());
                    System.out.println("pid " + clientMessage.selfData().pos().x() + " " + clientMessage.selfData().pos().y());
                    System.out.println("the last move in calculate is: " + clientMessage.sequenceNumber() + " " + ++mvnum);
                    player.setLastProcessedSeqNum(clientMessage.sequenceNumber());
                    player.setTimestamp(clientMessage.selfData().timestamp());

                    // System.out.println("Updated player " + player.getPlayerId() + " to new coordinates: " + newX + ", " + newY);
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
