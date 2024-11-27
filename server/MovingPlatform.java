import java.util.List;

public class MovingPlatform extends Tile {

    private Vec2 initialPosition;
    private boolean movingLeft = false;
    public static boolean moveLeft = true;

    public MovingPlatform(Vec2 position, Vec2 size) {
        super(position, size);
        this.initialPosition = new Vec2(position.x, position.y);
        super.vel = new Vec2(1, 0);
    }

    public synchronized void movePlatform() {

        if(movingLeft){
            if (this.coordinates.x <= this.initialPosition.x - 100) {
                this.coordinates.x = this.initialPosition.x - 100;
                movingLeft = false;
                return;
            } 
            this.coordinates.x -= this.vel.x;
            moveLeft = true;
        } else {
            if (this.coordinates.x >= this.initialPosition.x + 100) {
                this.coordinates.x = this.initialPosition.x + 100;
                movingLeft = true;
                return;
            } 
            this.coordinates.x += this.vel.x;
            moveLeft = false;
        }
    }

    public static synchronized void updatePlatforms(List<MovingPlatform> platforms) {
        for (MovingPlatform platform : platforms) {
            platform.movePlatform();
        }
        
    }

}
