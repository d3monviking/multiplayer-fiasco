import Game.ClientMessage;
import java.util.concurrent.BlockingQueue;

public class Calculate implements Runnable {
    private BlockingQueue<ClientMessage> messageQueue;
    private int mvnum = 0;

    // Define width and height constants for players
    private static final float WIDTH = 50.0f; // Example width
    private static final float HEIGHT = 50.0f; // Example height

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
                    // Get inputs to determine movement direction
                    boolean[] inputs = getInputsFromClientMessage(clientMessage);
                    //level.applyInput(player, inputs); level is instance of Level class
                    Vec2 currentCoords = player.getCoordinates();
                    
                    // Calculate new potential coordinates based on inputs
                    float newX = currentCoords.getX();
                    float newY = currentCoords.getY();
                    newY += (inputs[0] ? -2 : 0);  // Move up if inputs[0] is true
                    newX += (inputs[1] ? -2 : 0);  // Move left if inputs[1] is true
                    newY += (inputs[2] ? 2 : 0);   // Move down if inputs[2] is true
                    newX += (inputs[3] ? 2 : 0);   // Move right if inputs[3] is true

                    // Check for collision with other players
                    for (Player otherPlayer : Server.getPlayerList()) {
                        // Skip checking collision with the player itself
                        if (otherPlayer.getPlayerId() == player.getPlayerId()) {
                            continue;
                        }
                        System.out.println("other player id="+otherPlayer.getPlayerId());

                        // Get other player's position
                        float objectX = otherPlayer.getCoordinates().getX();
                        float objectY = otherPlayer.getCoordinates().getY();

                        // Check for collision with the new coordinates
                        if (checkCollision(newX, newY, WIDTH, HEIGHT, objectX, objectY, WIDTH, HEIGHT)) {
                            // Adjust the position to stop exactly at the point of collision
                            newX = adjustToCollision(newX, currentCoords.getX(), WIDTH, objectX, WIDTH, inputs[1], inputs[3]);
                            newY = adjustToCollision(newY, currentCoords.getY(), HEIGHT, objectY, HEIGHT, inputs[0], inputs[2]);
                        }
                    }

                    // Update player coordinates
                    player.setCoordinates(new Vec2(newX, newY));
                    receive.printMessage(clientMessage);
                    System.out.println(player.getCoordinates().getX() + " " + player.getCoordinates().getY());
                    System.out.println("pid " + clientMessage.selfData().pos().x() + " " + clientMessage.selfData().pos().y());
                    System.out.println("the last move in calculate is: " + clientMessage.sequenceNumber() + " " + ++mvnum);
                    player.setLastProcessedSeqNum(clientMessage.sequenceNumber());
                    player.setTimestamp(clientMessage.selfData().timestamp());
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // AABB collision detection method
    private boolean checkCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2) {
        System.out.println("entered collision check");
        return (x1 < x2 + width2 && x1 + width1 > x2 &&
                y1 < y2 + height2 && y1 + height1 > y2);
    }

    // Adjust position to the point of collision without overlapping
    private float adjustToCollision(float newCoord, float currentCoord, float size, float objectCoord, float objectSize, boolean isNegative, boolean isPositive) {
        System.out.println("entered adjust check");
        if (isNegative && newCoord < objectCoord + objectSize && currentCoord >= objectCoord + objectSize) {
            // Moving left/up and would overlap, so stop at the edge
            return objectCoord + objectSize +30;
        }
        if (isPositive && newCoord + size > objectCoord && currentCoord + size <= objectCoord) {
            // Moving right/down and would overlap, so stop at the edge
            return objectCoord - size -30;
        }
        return newCoord;
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
