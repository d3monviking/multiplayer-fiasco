class Tile {
    public Vec2 coordinates;  // Coordinates in tile grid
    private Vec2 acc;     // Acceleration
    private Vec2 size;    // Tile dimensions (width/height)

    public Vec2 vel;     // Velocity
    
    public Tile(Vec2 coords, Vec2 tileSize) {
        this.size = tileSize;
        this.vel = new Vec2(0, 0);
        this.acc = new Vec2(0, 0);
        this.coordinates = coords;
        //if player add gravity and add to player list in level
    }
    public Vec2 getCoordinates() {
        return coordinates;
    }
    public Vec2 getVel() {
        return vel;
    }

    public Vec2 getAcc() {
        return acc;
    }

    public Vec2 getSize() {
        return size;
    }
    public void update(float x_shift, float y_shift) {
        // Your update logic here
    }
}