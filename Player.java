import java.net.InetSocketAddress;
import java.time.Instant;

// Vec2 class to represent 2D coordinates (x, y)
class Vec2 {
    private float x;
    private float y;

    public Vec2(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public float getX() {
        return x;
    }

    public float getY() {
        return y;
    }

//    @Override
//    public String toString() {
//        return "Vec2{" + "x=" + x + ", y=" + y + '}';
//    }
}

public class Player {
    private InetSocketAddress address;
    private Vec2 coordinates;
    private int playerId;
    private int lastProcessedSeqNum;
    private Instant timestamp;
    private boolean canRead = false; // 1 if being read

    public Player(InetSocketAddress address, Vec2 coordinates, int playerId, int lastProcessedSeqNum, long timestamp) {
        this.address = address;
        this.coordinates = coordinates;
        this.playerId = playerId;
        this.lastProcessedSeqNum = lastProcessedSeqNum;
        this.timestamp = Instant.ofEpochMilli(timestamp); // Store the timestamp
    }

    private synchronized void threadWriteNotify(){
        canRead = true;
        notifyAll();
    }

    private synchronized void threadWriteWait(){
        while(canRead){
            try{
                wait();
            }
            catch(InterruptedException e){
                System.out.println("Set Thread Interrupted, Wait failed!");
                e.printStackTrace();
            }
        }
        canRead = false;
    }

    private synchronized void threadReadNotify(){
        notifyAll();
    }

    private synchronized void threadReadWait(){
        while(!canRead){
            try{
                wait();
            }
            catch(InterruptedException e){
                System.out.println("Set Thread Interrupted, Wait failed!");
                e.printStackTrace();
            }
        }
        canRead = true;
    }

    // Getters and Setters with synchronized keyword
    public synchronized InetSocketAddress getAddress() {
        return address;
    }

    public synchronized void setAddress(InetSocketAddress address) {
        this.address = address;
    }

    public synchronized Vec2 getCoordinates() {
        return coordinates;
    }

    public synchronized void setCoordinates(Vec2 coordinates) {
        this.coordinates = coordinates;
    }

    public synchronized int getPlayerId() {
        return playerId;
    }

    public synchronized void setPlayerId(int playerId) {
        this.playerId = playerId;
    }

    public synchronized int getLastProcessedSeqNum() {
        return lastProcessedSeqNum;
    }

    public synchronized void setLastProcessedSeqNum(int lastProcessedSeqNum) {
        this.lastProcessedSeqNum = lastProcessedSeqNum;
    }

    public synchronized long getTimestampMilli() {
        return timestamp.toEpochMilli();
    }

    public synchronized void updateTimestamp() {
        this.timestamp = Instant.now(); // Update the timestamp to the current time
    }

    @Override
    public synchronized String toString() {
        return "Player{" +
                "address=" + address +
                ", coordinates=" + coordinates +
                ", playerId=" + playerId +
                ", lastProcessedSeqNum=" + lastProcessedSeqNum +
                ", timestamp=" + timestamp +
                '}';
    }

    public static void main(String[] args) {
        // Example of how to create a Player
        InetSocketAddress address = new InetSocketAddress("127.0.0.1", 8080);
        Vec2 coords = new Vec2(100.0f, 200.0f);
        Player player = new Player(address, coords, 1, 42, System.currentTimeMillis());
        System.out.println(player);
    }
}
