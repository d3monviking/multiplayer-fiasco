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
public final class ServerMessage extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static ServerMessage getRootAsServerMessage(ByteBuffer _bb) { return getRootAsServerMessage(_bb, new ServerMessage()); }
  public static ServerMessage getRootAsServerMessage(ByteBuffer _bb, ServerMessage obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public ServerMessage __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int messageCode() { int o = __offset(4); return o != 0 ? bb.getInt(o + bb_pos) : 0; }
  public Game.PlayerData playerData(int j) { return playerData(new Game.PlayerData(), j); }
  public Game.PlayerData playerData(Game.PlayerData obj, int j) { int o = __offset(6); return o != 0 ? obj.__assign(__indirect(__vector(o) + j * 4), bb) : null; }
  public int playerDataLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public Game.PlayerData.Vector playerDataVector() { return playerDataVector(new Game.PlayerData.Vector()); }
  public Game.PlayerData.Vector playerDataVector(Game.PlayerData.Vector obj) { int o = __offset(6); return o != 0 ? obj.__assign(__vector(o), 4, bb) : null; }
  public int playerId() { int o = __offset(8); return o != 0 ? bb.getInt(o + bb_pos) : 0; }

  public static int createServerMessage(FlatBufferBuilder builder,
      int messageCode,
      int playerDataOffset,
      int playerId) {
    builder.startTable(3);
    ServerMessage.addPlayerId(builder, playerId);
    ServerMessage.addPlayerData(builder, playerDataOffset);
    ServerMessage.addMessageCode(builder, messageCode);
    return ServerMessage.endServerMessage(builder);
  }

  public static void startServerMessage(FlatBufferBuilder builder) { builder.startTable(3); }
  public static void addMessageCode(FlatBufferBuilder builder, int messageCode) { builder.addInt(0, messageCode, 0); }
  public static void addPlayerData(FlatBufferBuilder builder, int playerDataOffset) { builder.addOffset(1, playerDataOffset, 0); }
  public static int createPlayerDataVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startPlayerDataVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addPlayerId(FlatBufferBuilder builder, int playerId) { builder.addInt(2, playerId, 0); }
  public static int endServerMessage(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public ServerMessage get(int j) { return get(new ServerMessage(), j); }
    public ServerMessage get(ServerMessage obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

