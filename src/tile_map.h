#ifndef TILE_MAP_H
#define TILE_MAP_H

typedef v3 TileMapDifference;

namespace {
constexpr u32 chunk_shift = 4;
constexpr u32 chunk_dim = (1 << chunk_shift);
constexpr u32 chunk_mask = (1 << chunk_shift) - 1;
}; // namespace

struct TileMapPosition {
  u32 abs_x;
  u32 abs_y;
  u32 abs_z;

  v3 offset;
};

struct TileChunkPosition {
  u32 chunk_x;
  u32 chunk_y;
  u32 chunk_z;

  u32 local_x;
  u32 local_y;
};

struct TileChunk {
  u32 *tiles;
};

enum class TileValue { ACTIVE };

struct TileMap {
  f32 tile_side_in_meters;

  i32 chunk_count_x;
  i32 chunk_count_y;
  i32 chunk_count_z;

  TileChunk *tile_chunks;

  void recanonicalizeCoord(u32 &tile, f32 &tile_local);
  TileMapPosition recanonicalizePosition(TileMapPosition pos);

  TileChunk *getTileChunk(u32 chunk_x, u32 chunk_y, u32 chunk_z);

  u32 getTileValueUnchecked(TileChunk *tile_chunk, u32 tile_x, u32 tile_y);
  void setTileValueUnchecked(TileChunk *tile_chunk, u32 tile_x, u32 tile_y, TileValue tile_value);

  u32 getTileValue(TileChunk *tile_chunk, u32 test_tile_x, u32 test_tile_y);
  void setTileValue(TileChunk *tile_chunk, u32 test_tile_x, u32 test_tile_y, TileValue tile_value);

  TileMapDifference subtractPoints(TileMapPosition &a, TileMapPosition &b);
  TileChunkPosition getChunkPositionFor(u32 abs_tile_x, u32 abs_tile_y, u32 abs_tile_z);
  TileMapPosition offsetMap(TileMapPosition pos, v3 offset);
};

#endif
