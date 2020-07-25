#include "tile_map.h"


inline void TileMap::recanonicalizeCoord(u32 &tile, f32 &tile_local) {
  i32 offset = static_cast<i32>(roundf(tile_local / tile_side_in_meters));
  tile += offset;
  tile_local -= offset * tile_side_in_meters;

  assert(tile_local >= -0.5001f * tile_side_in_meters);
  assert(tile_local <= 0.5001f * tile_side_in_meters);
}


TileMapPosition TileMap::recanonicalizePosition(TileMapPosition pos) {
  TileMapPosition result = pos;
  recanonicalizeCoord(result.abs_x, result.offset.x);
  recanonicalizeCoord(result.abs_y, result.offset.y);

  return result;
}

inline TileChunk *TileMap::getTileChunk(u32 chunk_x, u32 chunk_y, u32 chunk_z) {
  TileChunk *tile_chunk = nullptr;
  if ((chunk_x >= 0) && (chunk_x < chunk_count_x) && (chunk_y >= 0) && (chunk_y < chunk_count_y) && (chunk_z >= 0) &&
      (chunk_z < chunk_count_z)) {
    tile_chunk = &tile_chunks[chunk_z * chunk_count_y * chunk_count_x + chunk_y * chunk_count_x + chunk_count_x];
  }

  return tile_chunk;
}


inline u32 TileMap::getTileValueUnchecked(TileChunk *tile_chunk, u32 tile_x, u32 tile_y) {
  assert(tile_chunk);
  assert(tile_x < chunk_dim);
  assert(tile_y < chunk_dim);

  u32 tile_chunk_value = tile_chunk->tiles[tile_y * chunk_dim + tile_x];

  return tile_chunk_value;
}

inline void TileMap::setTileValueUnchecked(TileChunk *tile_chunk, u32 tile_x, u32 tile_y, TileValue tile_value) {
  assert(tile_chunk);
  assert(tile_x < chunk_dim);
  assert(tile_y < chunk_dim);

  tile_chunk->tiles[tile_y * chunk_dim + tile_x] = static_cast<u32>(tile_value);
}

inline u32 TileMap::getTileValue(TileChunk *tile_chunk, u32 test_tile_x, u32 test_tile_y) {
  u32 tile_chunk_value = 0;
  if (tile_chunk && tile_chunk->tiles) {
    tile_chunk_value = getTileValueUnchecked(tile_chunk, test_tile_x, test_tile_y);
  }

  return tile_chunk_value;
}

inline void TileMap::setTileValue(TileChunk *tile_chunk, u32 test_tile_x, u32 test_tile_y, TileValue tile_value) {
  u32 tile_chunk_value = 0;
  if (tile_chunk && tile_chunk->tiles) {
    setTileValueUnchecked(tile_chunk, test_tile_x, test_tile_y, tile_value);
  }
}


TileChunkPosition TileMap::getChunkPositionFor(u32 abs_tile_x, u32 abs_tile_y, u32 abs_tile_z) {
  TileChunkPosition result;
  result.chunk_x = abs_tile_x >> chunk_shift;
  result.chunk_y = abs_tile_y >> chunk_shift;
  result.chunk_z = abs_tile_z >> chunk_shift;

  result.local_x = abs_tile_x & chunk_mask;
  result.local_y = abs_tile_y & chunk_mask;

  return result;
}

inline b32 onSameTile(const TileMapPosition &a, const TileMapPosition &b) {
  return (a.abs_x == b.abs_x && a.abs_y == b.abs_y && a.abs_z == b.abs_z);
}

internal u32 getTileValue(TileMap *tile_map, u32 abs_tile_x, u32 abs_tile_y, u32 abs_tile_z) {
  b32 empty = false;
  TileChunkPosition chunk_pos = tile_map->getChunkPositionFor(abs_tile_x, abs_tile_y, abs_tile_z);
  TileChunk *tile_chunk = tile_map->getTileChunk(chunk_pos.chunk_x, chunk_pos.chunk_y, chunk_pos.chunk_z);
  u32 tile_chunk_value = tile_map->getTileValue(tile_chunk, chunk_pos.local_x, chunk_pos.local_y);

  return tile_chunk_value;
}

internal void setTileValue(
    TileMap *tile_map, const MemoryStorage &memory, u32 abs_tile_x, u32 abs_tile_y, u32 abs_tile_z, TileValue value) {
  TileChunkPosition chunk_pos = tile_map->getChunkPositionFor(abs_tile_x, abs_tile_y, abs_tile_z);
  TileChunk *tile_chunk = tile_map->getTileChunk(chunk_pos.chunk_x, chunk_pos.chunk_y, chunk_pos.chunk_z);

  assert(tile_chunk);

  if (!tile_chunk->tiles) {
    constexpr u32 tile_count = chunk_dim * chunk_dim;
    tile_chunk->tiles = alloc_array<u32, tile_count>(memory.game_partition);
    for (u32 i = 0; i < tile_count; ++i) {
      tile_chunk->tiles[i] = static_cast<u32>(TileValue::ACTIVE);
    }

    tile_map->setTileValue(tile_chunk, chunk_pos.local_x, chunk_pos.local_y, value);
  }
}


inline TileMapPosition centerTilePoint(u32 abs_tile_x, u32 abs_tile_y, u32 abs_tile_z) {
  TileMapPosition result = {};
  result.abs_x = abs_tile_x;
  result.abs_y = abs_tile_y;
  result.abs_z = abs_tile_z;

  return result;
}

inline TileMapDifference TileMap::subtractPoints(TileMapPosition &a, TileMapPosition &b) {
  TileMapDifference result;
  v3 d_tile_xyz = {static_cast<f32>(a.abs_x) - static_cast<f32>(b.abs_x),
      static_cast<f32>(a.abs_y) - static_cast<f32>(b.abs_y), static_cast<f32>(a.abs_z) - static_cast<f32>(b.abs_z)};

  result = tile_side_in_meters * d_tile_xyz + (a.offset - b.offset);

  return result;
}


TileMapPosition TileMap::offsetMap(TileMapPosition pos, v3 offset) {
  pos.offset += offset;
  pos = recanonicalizePosition(pos);

  return pos;
}
