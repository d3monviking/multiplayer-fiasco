import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

class Level {
    private List<Tile> tiles;
    private Vec2 tileSize;
    private float gravity;
    private static ArrayList<String> levelMap = new ArrayList<>(Arrays.asList(
                "                   ",
                "                   ",
                "                   ",
                "                   ",
                "                   ",
                "XXXXXXXXXXXX  XXXXX",
                "                   ",
                "                   ",
                "XXXX  XXXXXXXXXXXXX",
                "                   ",
                "                   ",
                "                   ",
                "                   ",
                "                   ",
                "XXXXXXXXXXXX  XXXXX",
                "                   ",
                "                   ",
                "                   ",
                "                   ",
                "                   ",
                "XXXXXXXXXXXXXXXXXXX"
        ));
    
    // Constants
    private final float FRICTION = 0.9f;
    private final float JUMP_FORCE = 15f;
    private final float VELOCITY_THRESHOLD = 0.1f;

    Level(){
        tileSize = new Vec2(50, 50);
        gravity = 0.7f;
        tiles = new ArrayList<Tile>();
        for(int i = 0; i < levelMap.size(); i++){
            for(int j = 0; j < levelMap.get(i).length(); j++){
                if(levelMap.get(i).charAt(j) == 'X'){
                    tiles.add(new Tile(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
                }
            }
        }
    }

    public void applyInput(Player self, boolean[] inputs) {
        self.prevXVel = self.vel.getX();
        // Handle movement
        if (inputs[1]) {  // Left
            self.acc.x = -self.runAcc;
        } else if (inputs[3]) {  // Right
            self.acc.x = self.runAcc;
        } else {
            self.acc.x = 0;
            self.vel.x *= FRICTION;
            if (Math.abs(self.vel.x) < VELOCITY_THRESHOLD) {
                self.vel.x = 0;
            }
        }
        
        // Handle jump
        if (inputs[4] && self.onGround) {
            self.vel.y = -JUMP_FORCE;
            self.onGround = false;
        }
        
        xCollisions(self);
        yCollisions(self);
        // detectInterPlayerCollisions();
    }
    
    private void xCollisions(Player self) {
        // Apply horizontal acceleration and speed limit
        self.vel.x += self.acc.x;
        self.vel.x = Math.min(Math.max(self.vel.x, -self.maxSpeed), self.maxSpeed);
        
        // Update position
        self.getCoordinates().x += self.vel.x;
        
        // Check collisions with tiles
        for (Tile t : tiles) {
            if (checkCollision(t, self)) {
                float relVel = self.vel.x - t.getVel().x;
                if (relVel < 0) {
                    self.getCoordinates().x = t.getCoordinates().x + tileSize.x;
                    self.vel.x = 0;
                } else if (relVel > 0) {
                    self.getCoordinates().x = t.getCoordinates().x - self.size.x;
                    self.vel.x = 0;
                }
            }
        }
    }
    
    private void yCollisions(Player self) {
        // Apply gravity and vertical movement
        self.acc.y = gravity;
        self.vel.y += self.acc.y;
        self.getCoordinates().y += self.vel.y;
        
        // Check collisions
        for (Tile t : tiles) {
            if (checkCollision(t, self)) {
                float relVel = self.vel.y - t.getVel().y;
                if (relVel < 0) {
                    self.getCoordinates().y = t.getCoordinates().y + tileSize.y;
                    self.vel.y = 0;
                } else if (relVel > 0) {
                    self.getCoordinates().y = t.getCoordinates().y - self.size.y;
                    self.vel.y = 0;
                    self.onGround = true;
                }
            }
        }
    }

    private boolean checkCollision(Tile t, Player p) {
        Vec2 tPos = t.getCoordinates();
        Vec2 tSize = t.getSize();
        Vec2 pPos = p.getCoordinates();
        Vec2 pSize = p.size;
        
        return pPos.x < tPos.x + tSize.x &&
               pPos.x + pSize.x > tPos.x &&
               pPos.y < tPos.y + tSize.y &&
               pPos.y + pSize.y > tPos.y;
    }
}
