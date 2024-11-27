public class PowerUp extends Tile {
    private char type = 'P';
    private float speedBoost = 4.0f;
    public long boostClock;

    public PowerUp (Vec2 coords, Vec2 size) {
        super(coords, size);
    }

    public void applyBoost(Player player) {
        player.vel.x *= speedBoost;
        player.maxSpeed *= speedBoost;
        boostClock = System.currentTimeMillis();
    }

    public char getType() {
        return type;
    }
}