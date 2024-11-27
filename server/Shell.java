public class Shell extends Tile {
    private char type = 'S';
    private boolean held;
    private boolean kicked;

    public Shell (Vec2 coords, Vec2 size) {
        super(coords, size);
    }
    public char getType() {
        return type;
    }
    public boolean isHeld() {
        return held;
    }
    public boolean isKicked() {
        return kicked;
    }
}