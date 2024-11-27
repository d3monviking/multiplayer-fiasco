
import Game.ClientMessage;
import java.util.concurrent.BlockingQueue;

public class Calculate implements Runnable {
    private BlockingQueue<ClientMessage> messageQueue;

    // Define width and height constants for players
    private static final float WIDTH = 50.0f;  // Example width
    private static final float HEIGHT = 50.0f; // Example height

    public Calculate(BlockingQueue<ClientMessage> messageQueue) {
        this.messageQueue = messageQueue;
    }

    Level level; // Create an instance of the Level class

    @Override
    public void run() {
        try {
            level = new Level();
            while (true) {

                MovingPlatform.updatePlatforms(level.movingPlatformTiles);


                // Get the next client message from the queue
                ClientMessage clientMessage = messageQueue.take();

                // Find the corresponding player
                Player player = findPlayer(clientMessage.selfData().playerId());
                if (player != null) {
                    // Get inputs to determine movement direction
                    boolean[] inputs = getInputsFromClientMessage(clientMessage);
                    // System.out.println("Processing Player ID: " + player.getPlayerId());
                    // System.out.println("Player inputs: " + inputs[0] + ", " + inputs[1] + ", " + inputs[2] + ", " + inputs[3]);

                    float currX = player.getCoordinates().getX();
                    float currY = player.getCoordinates().getY();
                    // System.out.println("Player current position: x=" + currX + ", y=" + currY);

                    // Apply level input logic for tile collision
                    level.applyInput(player, inputs); // Updates the new position of the player by applying inputs

                    // Get the player's new position after input
                    float newX = player.getCoordinates().getX();
                    float newY = player.getCoordinates().getY();
                    // System.out.println("Player after input applied position: x=" + newX + ", y=" + newY);

                    // Check for collision with other players
                    for (Player otherPlayer : Server.getPlayerList()) {
                        // Skip checking collision with itself
                        if (otherPlayer.getPlayerId() == player.getPlayerId()) continue;

                        // System.out.println("Checking collision with Player ID: " + otherPlayer.getPlayerId());

                        // Get other player's position
                        float objectX = otherPlayer.getCoordinates().getX();
                        float objectY = otherPlayer.getCoordinates().getY();

                        // Debug other player's position
                        // System.out.println("Other Player position: x=" + objectX + ", y=" + objectY);

                        // Check for collision
                        if (checkCollision(newX, newY, WIDTH, HEIGHT, objectX, objectY, WIDTH, HEIGHT)) {
                            // System.out.println("Collision detected with Player ID: " + otherPlayer.getPlayerId());

                            // Adjust the player's position to resolve collision
                            float[] adjustedPosition = adjustToCollision(newX, newY, WIDTH, HEIGHT,
                                                                         objectX, objectY, WIDTH, HEIGHT,player);
                            newX = adjustedPosition[0];
                            newY = adjustedPosition[1];

                            // System.out.println("Adjusted position: x=" + newX + ", y=" + newY);
                        }
                    }

                    // Update player coordinates
                    player.setCoordinates(new Vec2(newX, newY));

                    // Log final coordinates
                    // System.out.println("Final position: x=" + player.getCoordinates().getX() +", y=" + player.getCoordinates().getY());

                    // Update player state
                    player.setLastProcessedSeqNum(clientMessage.sequenceNumber());
                    player.setTimestamp(clientMessage.selfData().timestamp());
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // AABB collision detection method
    private boolean checkCollision(float x1, float y1, float width1, float height1,
                                   float x2, float y2, float width2, float height2) {
        // System.out.println("Entered collision check");
        boolean collision = (x1 < x2 + width2 && x1 + width1 > x2 &&
                             y1 < y2 + height2 && y1 + height1 > y2);
        // System.out.println("Collision result: " + collision);
        return collision;
    }

    // Adjust position to resolve collision using Minimum Translation Vector (MTV)
    // private float[] adjustToCollision(float x1, float y1, float width1, float height1,
    //                                   float x2, float y2, float width2, float height2) {
    //     System.out.println("Entered adjust collision");

    //     // Compute the centers of the bounding boxes
    //     float centerX1 = x1 + width1 / 2.0f;
    //     float centerY1 = y1 + height1 / 2.0f;
    //     float centerX2 = x2 + width2 / 2.0f;
    //     float centerY2 = y2 + height2 / 2.0f;

    //     // Compute delta between centers
    //     float deltaX = centerX1 - centerX2;
    //     float deltaY = centerY1 - centerY2;

    //     // Compute overlap on x and y axes
    //     float overlapX = (width1 / 2.0f + width2 / 2.0f) - Math.abs(deltaX);
    //     float overlapY = (height1 / 2.0f + height2 / 2.0f) - Math.abs(deltaY);

    //     // Initialize new position
    //     float newX = x1;
    //     float newY = y1;

    //     if (overlapX > 0 && overlapY > 0) {
    //         // Collision detected, resolve it by moving along the axis of minimal penetration
    //         if (overlapX < overlapY) {
    //             // Resolve along x axis
    //             if (deltaX > 0) {
    //                 // Move player to the right
                    
    //                 newX +=(overlapX+50);
    //                 System.out.println("Adjusting position along X axis to the right by " + overlapX);
    //             } else {
    //                 // Move player to the left
    //                 newX -= (overlapX+50);
    //                 System.out.println("Adjusting position along X axis to the left by " + overlapX);
    //             }
    //         } else {
    //             // Resolve along y axis
    //             if (deltaY > 0) {
    //                 // Move player down
    //                 newY += (overlapY+100);
    //                 System.out.println("Adjusting position along Y axis down by " + overlapY);
    //             } else {
    //                 // Move player up
    //                 newY -= (overlapY+100);
    //                 System.out.println("Adjusting position along Y axis up by " + overlapY);
    //             }
    //         }
    //     }

    //     return new float[] { newX, newY };
    // }
    private float[] adjustToCollision(float x1, float y1, float width1, float height1,
                                  float x2, float y2, float width2, float height2, Player player) {
    // System.out.println("Entered adjust collision");

    // Compute the centers of the bounding boxes
    float centerX1 = x1 + width1 / 2.0f;
    float centerY1 = y1 + height1 / 2.0f;
    float centerX2 = x2 + width2 / 2.0f;
    float centerY2 = y2 + height2 / 2.0f;

    // Compute delta between centers
    float deltaX = centerX1 - centerX2;
    float deltaY = centerY1 - centerY2;

    // Compute overlap on x and y axes
    float overlapX = (width1 / 2.0f + width2 / 2.0f) - Math.abs(deltaX);
    float overlapY = (height1 / 2.0f + height2 / 2.0f) - Math.abs(deltaY);

    // Initialize new position
    float newX = x1;
    float newY = y1;

    if (overlapX > 0 && overlapY > 0) {
        // Collision detected, resolve it by moving along the axis of minimal penetration
        if (overlapX < overlapY) {
            // Resolve along x-axis
            if (deltaX > 0) {
                // Move player to the right
                newX += (overlapX);
                // System.out.println("Adjusting position along X axis to the right by " + overlapX);
            } else {
                // Move player to the left
                newX -= (overlapX);
                // System.out.println("Adjusting position along X axis to the left by " + overlapX);
            }
        } else {
            // Resolve along y-axis
            if (deltaY > 0) {
                // Move player down
                newY += (overlapY);
                // System.out.println("Adjusting position along Y axis down by " + overlapY);
            } else {
                // Move player up (bouncing)
                newY -= (overlapY);
                // System.out.println("Adjusting position along Y axis up by " + overlapY);

                // Apply bounce effect by setting velocity to -15
                // System.out.println("Bouncing upwards, reducing Y velocity to -15.");
                player.getVelocity().y = -15; // Update the velocity for rebound effect
            }
        }
    }

    return new float[] { newX, newY };
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