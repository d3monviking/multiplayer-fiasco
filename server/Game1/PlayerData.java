// automatically generated by the FlatBuffers compiler, do not modify

package Game;

import com.google.flatbuffers.BaseVector;
import com.google.flatbuffers.BooleanVector;
import com.google.flatbuffers.ByteVector;
import com.google.flatbuffers.Constants;
import com.google.flatbuffers.DoubleVector;
import com.google.flatbuffers.FlatBufferBuilder;
import com.google.flatbuffers.FloatVector;
import com.google.flatbuffers.IntVector;
import com.google.flatbuffers.LongVector;
import com.google.flatbuffers.ShortVector;
import com.google.flatbuffers.StringVector;
import com.google.flatbuffers.Struct;
import com.google.flatbuffers.Table;
import com.google.flatbuffers.UnionVector;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

@SuppressWarnings("unused")
public final class PlayerData extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static PlayerData getRootAsPlayerData(ByteBuffer _bb) { return getRootAsPlayerData(_bb, new PlayerData()); }
  public static PlayerData getRootAsPlayerData(ByteBuffer _bb, PlayerData obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public PlayerData __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int playerId() { int o = __offset(4); return o != 0 ? bb.getInt(o + bb_pos) : 0; }
  public Game.Vec2 pos() { return pos(new Game.Vec2()); }
  public Game.Vec2 pos(Game.Vec2 obj) { int o = __offset(6); return o != 0 ? obj.__assign(o + bb_pos, bb) : null; }
  public Game.Vec2 vel() { return vel(new Game.Vec2()); }
  public Game.Vec2 vel(Game.Vec2 obj) { int o = __offset(8); return o != 0 ? obj.__assign(o + bb_pos, bb) : null; }
  public long timestamp() { int o = __offset(10); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long lastProcessedSeqNumber() { int o = __offset(12); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }

  public static void startPlayerData(FlatBufferBuilder builder) { builder.startTable(5); }
  public static void addPlayerId(FlatBufferBuilder builder, int playerId) { builder.addInt(0, playerId, 0); }
  public static void addPos(FlatBufferBuilder builder, int posOffset) { builder.addStruct(1, posOffset, 0); }
  public static void addVel(FlatBufferBuilder builder, int velOffset) { builder.addStruct(2, velOffset, 0); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(3, timestamp, 0L); }
  public static void addLastProcessedSeqNumber(FlatBufferBuilder builder, long lastProcessedSeqNumber) { builder.addLong(4, lastProcessedSeqNumber, 0L); }
  public static int endPlayerData(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public PlayerData get(int j) { return get(new PlayerData(), j); }
    public PlayerData get(PlayerData obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

