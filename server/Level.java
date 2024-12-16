import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.time.Instant;
import java.net.UnknownHostException;

public class Level {
    private List<Tile> tiles; 
    private List<PowerUp> powerUpTiles;
    private List<Tile> collectibleTiles;
    private List<Tile> deathTiles;
    public List<MovingPlatform> movingPlatformTiles;
    private List<Tile> finishLineTiles;
    private Vec2 tileSize;
    private Vec2 movingPlatformSize;
    private float gravity;
    private boolean[] inputs = new boolean[5];
    private static ArrayList<ArrayList<Integer>> levelMap = new ArrayList<>();
    private static ArrayList<ArrayList<Integer>> powerUps = new ArrayList<>();
    private static ArrayList<ArrayList<Integer>> movingPlatforms = new ArrayList<>();
    private static ArrayList<ArrayList<Integer>> water = new ArrayList<>();
    private static ArrayList<ArrayList<Integer>> trees = new ArrayList<>();
    private static ArrayList<ArrayList<Integer>> spikes = new ArrayList<>();
    private static ArrayList<ArrayList<Integer>> finishLine = new ArrayList<>();
    public ArrayList<Integer> winners = new ArrayList<>();
    public ArrayList<Vec2> winnerPos = new ArrayList<>();
    private static String src = "../Client/TileMapFiles";
    static {
        loadLevelMapFromFile(src + "/level_terrain.csv");
        loadPowerUpsFromFile(src + "/level_powerups.csv");
        loadMovingPlatformFromFile(src + "/level_moving_platform.csv");
        loadWaterTiles(src + "/level_water.csv");
        loadTrees(src + "/level_tree.csv");
        loadSpikes(src + "/level_spikes.csv");
        loadFinishLine(src + "/level_finishLine.csv");
    }

    // private static void loadLevelMapFromFile(String fileName) {
    //     try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
    //         String line;
    //         while ((line = br.readLine()) != null) {
    //             levelMap.add(line);
    //         }
    //     } catch (IOException e) {
    //         e.printStackTrace();
    //     }
    // }

    private static void loadLevelMapFromFile(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(","); 
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    try {
                        row.add(Integer.parseInt(value));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                levelMap.add(row); 
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private static void loadPowerUpsFromFile(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(","); 
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    try {
                        row.add(Integer.parseInt(value));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                powerUps.add(row); 
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void loadMovingPlatformFromFile(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(","); 
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    try {
                        row.add(Integer.parseInt(value));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                movingPlatforms.add(row); 
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void loadWaterTiles(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(","); 
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    try {
                        row.add(Integer.parseInt(value));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                water.add(row); 
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void loadTrees(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(","); 
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    try {
                        row.add(Integer.parseInt(value));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                trees.add(row); 
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void loadSpikes(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(","); 
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    try {
                        row.add(Integer.parseInt(value));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                spikes.add(row); 
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void loadFinishLine(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(","); 
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    try {
                        row.add(Integer.parseInt(value));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                finishLine.add(row); 
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static ArrayList<ArrayList<Integer>> getLevelMap() {
        return levelMap;
    }

    // Constants
    private final float FRICTION = 0.9f;
    private final float JUMP_FORCE = 25f;
    private final float VELOCITY_THRESHOLD = 0.1f;

    Level(){
        tileSize = new Vec2(16, 16);
        movingPlatformSize = new Vec2(48, 16);
        int q = 2;
        for(int i = 0; i<4; i++){
            winnerPos.add(new Vec2(36, q*16));
            q += 11;
        }
        gravity = 1.4f;
        tiles = new ArrayList<Tile>();
        powerUpTiles = new ArrayList<PowerUp>();
        collectibleTiles = new ArrayList<Tile>();
        movingPlatformTiles = new ArrayList<MovingPlatform>();
        deathTiles = new ArrayList<Tile>();
        finishLineTiles = new ArrayList<Tile>();
        for(int i = 0; i < levelMap.size(); i++){
            for(int j = 0; j < levelMap.get(i).size(); j++){
                if(levelMap.get(i).get(j) == -1){
                    continue;
                }
                tiles.add(new Tile(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
            }
        }
        for(int i = 0; i < powerUps.size(); i++){
            for(int j = 0; j < powerUps.get(i).size(); j++){
                if(powerUps.get(i).get(j) == -1){
                    continue;
                }
                powerUpTiles.add(new PowerUp(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
                collectibleTiles.add(new Tile(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
            }
        }
        for(int i = 0; i < movingPlatforms.size(); i++){
            for(int j = 0; j < movingPlatforms.get(i).size(); j++){
                if(movingPlatforms.get(i).get(j) == -1){
                    continue;
                }
                movingPlatformTiles.add(new MovingPlatform(new Vec2(j * tileSize.x, i * tileSize.y), new Vec2(tileSize.x*6, tileSize.y*1.2f)));
                // movingPlatformTiles.add(new MovingPlatform(new Vec2(j * movingPlatformSize.x, i * movingPlatformSize.y), movingPlatformSize));
            }
        }
        for(int i = 0; i < water.size(); i++){
            for(int j = 0; j < water.get(i).size(); j++){
                if(water.get(i).get(j) == -1){
                    continue;
                }
                deathTiles.add(new Tile(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
            }
        }
        for(int i = 0; i < trees.size(); i++){
            for(int j = 0; j < trees.get(i).size(); j++){
                if(trees.get(i).get(j) == -1){
                    continue;
                }
                tiles.add(new Tile(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
            }
        }
        for(int i = 0; i < spikes.size(); i++){
            for(int j = 0; j < spikes.get(i).size(); j++){
                if(spikes.get(i).get(j) == -1){
                    continue;
                }
                deathTiles.add(new Tile(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
            }
        }
        for(int i = 0; i < finishLine.size(); i++){
            for(int j = 0; j < finishLine.get(i).size(); j++){
                if(finishLine.get(i).get(j) == -1){
                    continue;
                }
                finishLineTiles.add(new Tile(new Vec2(j * tileSize.x, i * tileSize.y), tileSize));
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

        if(inputs[0]){
            if(!self.isBoostActive){
                Instant instant = Instant.now();
                long currentTime = instant.toEpochMilli();
                self.applyPowerUp(currentTime);
            }
        }
        
        xCollisions(self);
        yCollisions(self);
        System.out.println(self.getCoordinates().getX() + " " + self.getCoordinates().getY());
//        if(self.getCoordinates().y > 6400){
//            self.getCoordinates().y = 5710+(177*4);
//            self.getCoordinates().x = 20;
//        }
        // detectInterPlayerCollisions();

        Instant currInstant = Instant.now();
        long curr = currInstant.toEpochMilli();

        if(curr - self.boostStart > 30){
            self.isBoostActive = false;
            self.maxSpeed = 12f;
        }
    }

//    public void applyPhysics(Player self) {
//        while(self.vel.x != 0){
//            this.applyInput(self, inputs);
//        }
//    }
    
    private void xCollisions(Player self) {
        // Apply horizontal acceleration and speed limit
        self.vel.x += self.acc.x;
        self.vel.x = Math.min(Math.max(self.vel.x, -self.maxSpeed), self.maxSpeed);

        if(self.onPlatform){
            if(MovingPlatform.moveLeft){
                self.getCoordinates().x -= 1.0f;
            } else {
                self.getCoordinates().x += 1.0f;
            }
        }
        
        // Update position
//        System.out.println("Get x " + self.getCoordinates().x);
        self.getCoordinates().x += self.vel.x;
//        System.out.println("Set x " + self.getCoordinates().x);
        // Check collisions with tiles
        for (Iterator<Tile> it = tiles.iterator(); it.hasNext();) {
            Tile t = it.next();
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
        for(Iterator<PowerUp> it = powerUpTiles.iterator(); it.hasNext();){
            PowerUp p = it.next();
            if(checkCollision(p, self)){
                self.powerUps.add(p);
                it.remove();
                collectibleTiles.removeIf(tile -> tile == p);
                try {
                    SendServerMessage.serverCollectible(3, collectibleTiles);
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                }
            }
        }
        for(Tile f : finishLineTiles){
            if(checkCollision(f, self)){
                self.vel.x = 0;
                self.vel.y = 0;
                // self.getCoordinates().x = 20;
                // self.getCoordinates().y = 5710+(177*self.getPlayerId());
                winners.add(self.getPlayerId());
                self.setCoordinates(winnerPos.get(winners.size()-1));
                break;
            }
        }
    }
    
    private void yCollisions(Player self) {
        // Apply gravity and vertical movement
        self.acc.y = gravity;
        self.vel.y += self.acc.y;
        self.getCoordinates().y += self.vel.y;
        
//         Check collisions
        for (Iterator<Tile> it = tiles.iterator(); it.hasNext();) {
            Tile t = it.next();
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

        int flag = 0;
        for(MovingPlatform m : movingPlatformTiles){
            float relVel = self.vel.y;
            if(checkCollision(m, self)){
                if(relVel < 0){
                    self.getCoordinates().y = m.getCoordinates().y + tileSize.y;
                    self.vel.y = 0;
                } else if(relVel > 0){
                    self.getCoordinates().y = m.getCoordinates().y - self.size.y;
                    self.vel.y = 0;
                    self.onGround = true;
                    self.onPlatform = true;
                    flag = 1;
                }
                // System.out.println(self.getCoordinates().x + " " + self.getCoordinates().y);
            }
        }
        if(self.onPlatform == true && flag == 0){
            self.onPlatform = false;
        }

        for(Iterator<PowerUp> it = powerUpTiles.iterator(); it.hasNext();){
            PowerUp p = it.next();
            if(checkCollision(p, self)){
                self.powerUps.add(p);
                it.remove();
                collectibleTiles.removeIf(tile -> tile == p);
                try {
                    SendServerMessage.serverCollectible(3, collectibleTiles);
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                }
            }
        }
        for(Tile d : deathTiles){
            if(checkCollision(d, self)){
                self.vel.x = 0;
                self.vel.y = 0;
                self.getCoordinates().x = 20;
                self.getCoordinates().y = 5710+(177*self.getPlayerId());
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

    public List<PowerUp> getPowerUpTiles() {
        return powerUpTiles;
    }

    
}