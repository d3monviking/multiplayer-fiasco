import java.net.InetSocketAddress;
import java.time.Instant;
import java.util.ArrayList;
// Vec2 class to represent 2D coordinates (x, y)
class Vec2 {
    public float x;
    public float y;

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

    public Vec2 vel;
    public Vec2 acc;
    public Vec2 size;

    public boolean onGround;
    public float prevXVel;
    public float runAcc;
    public float maxSpeed;
    public boolean[] inputs = new boolean[5];

    public ArrayList<PowerUp> powerUps = new ArrayList<PowerUp>();
    public ArrayList<Shell> shells = new ArrayList<Shell>();

    public boolean isBoostActive = false;
    public long boostStart;
    public boolean onPlatform = false;

    

    public Player(InetSocketAddress address, Vec2 coordinates, int playerId, int lastProcessedSeqNum, long timestamp) {
        this.address = address;
        this.coordinates = coordinates;
        this.playerId = playerId;
        this.lastProcessedSeqNum = lastProcessedSeqNum;
        this.timestamp = Instant.ofEpochMilli(timestamp); // Store the timestamp
        this.vel = new Vec2(0, 0);
        this.acc = new Vec2(0, 0);
        // this.size = new Vec2(50, 50);
        if(this.playerId == 1){
            this.size = new Vec2(55, 81);
        } else if(this.playerId == 2){
            this.size = new Vec2(69, 80);
        } else if(this.playerId == 3){
            this.size = new Vec2(56, 94);
        } else if(this.playerId == 4){
            this.size = new Vec2(57, 80);
        }
        this.onGround = true;
        this.prevXVel = 0;
        this.runAcc = 4f;
        this.maxSpeed = 12f;
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
    public synchronized Vec2 getVelocity() {
        return vel;
    }

    public synchronized void setCoordinates(Vec2 coordinates) {
        this.coordinates = coordinates;
    }
    public synchronized void setVel(Vec2 vel) {
        this.vel = vel;
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

    public synchronized void setTimestamp(long timestamp) {
        this.timestamp = Instant.ofEpochMilli(timestamp);
    }

    public synchronized void applyPowerUp(long time){
        if(powerUps.size() != 0){
            float originalSpeed = this.vel.x;
            powerUps.get(0).applyBoost(this);
            isBoostActive = true;
            boostStart = time;
            powerUps.remove(0);
        } 
    }

    @Override
    public synchronized String toString() {
        return "Player{" +
                "address=" + address +
                ", coordinates=" + coordinates.getX() + ", " + coordinates.getY() +
                ", playerId=" + playerId +
                ", lastProcessedSeqNum=" + lastProcessedSeqNum +
                ", timestamp=" + timestamp +
                '}';
    }

    // public static void main(String[] args) {
    //     // Example of how to create a Player
    //     InetSocketAddress address = new InetSocketAddress("127.0.0.1", 8080);
    //     Vec2 coords = new Vec2(100.0f, 200.0f);
    //     Player player = new Player(address, coords, 1, 42, System.currentTimeMillis());
    //     System.out.println(player);
    // }
}
