#ifndef DEBUG_SERVICE_H
#define DEBUG_SERVICE_H

enum class DebugType { BeginProfile, EndProfile, FrameMarker, MemoryUsage };


struct DebugEvent {
  u64 clock;
  const char *GUID;
  const char *name;
  DebugType type;
  // TODO: add thread id when we support threading
  union {
    f32 value_f32;
    u32 value_u32;
  };
};


typedef std::vector<DebugEvent> DebugTable;

#ifdef GAME_INTERNAL

extern DebugTable g_debug_table;

#define DEBUG_NAME__(x, y, z) x "|" #y "|" #z
#define DEBUG_NAME_(x, y, z) DEBUG_NAME__(x, y, z)
#define DEBUG_NAME(name) DEBUG_NAME_(__FILE__, __LINE__, __COUNTER__)

#define END_DEBUG() g_debug_table.clear()

#define recordDebugEvent(event_type, GUID_init, name_init)                                                             \
  DebugEvent event = {0};                                                                                              \
  event.clock = __rdtsc();                                                                                             \
  event.type = event_type;                                                                                             \
  event.GUID = GUID_init;                                                                                              \
  event.name = name_init

#define BEGIN_PROFILE_(GUID, name)                                                                                     \
  {                                                                                                                    \
    recordDebugEvent(DebugType::BeginProfile, GUID, name);                                                             \
    g_debug_table.push_back(event);                                                                                    \
  }
#define END_PROFILE_(GUID, name)                                                                                       \
  {                                                                                                                    \
    recordDebugEvent(DebugType::EndProfile, GUID, name);                                                               \
    g_debug_table.push_back(event);                                                                                    \
  }

#define BEGIN_PROFILE(name) BEGIN_PROFILE_(DEBUG_NAME(name), name)
#define END_PROFILE() END_PROFILE_(DEBUG_NAME("END_PROFILE_"), "END_PROFILE_")

#define TIMED_BLOCK__(GUID, name) TimedBlock timed_block_##number(GUID, name)
#define TIMED_BLOCK_(GUID, name) TIMED_BLOCK__(GUID, name)
#define TIMED_BLOCK(name) TIMED_BLOCK_(DEBUG_NAME(name), name)
#define TIMED_FUNC(...) TIMED_BLOCK_(DEBUG_NAME(__FUNCTION__), (const char *)__FUNCTION__)

#define FRAME_MARKER(elapsed_seconds)                                                                                  \
  {                                                                                                                    \
    recordDebugEvent(DebugType::FrameMarker, DEBUG_NAME("Frame Marker"), "Frame Marker");                              \
    event.value_f32 = elapsed_seconds;                                                                                 \
    g_debug_table.push_back(event);                                                                                    \
  }

#define MEMORY_USAGE(memory)                                                                                           \
  {                                                                                                                    \
    recordDebugEvent(DebugType::MemoryUsage, DEBUG_NAME("Memory Usage"), "Memory Usag");                               \
    event.value_u32 = memory.resource_partition->used_memory + memory.game_partition->used_memory;                     \
    g_debug_table.push_back(event);                                                                                    \
  }

struct TimedBlock {
  TimedBlock(const char *GUID, const char *name) { BEGIN_PROFILE_(GUID, name); }

  ~TimedBlock() { END_PROFILE(); }
};


internal void DEBUG_PlainConsolePrint(const DebugTable &debug_table) {

  for (const auto &debug_entry : g_debug_table) {
    if (debug_entry.type == DebugType::FrameMarker) {
      fprintf(stdout, "GUID:%s; Name:%s, Sec elapsed:%f.\n", debug_entry.GUID, debug_entry.name, debug_entry.value_f32);
    } else if (debug_entry.type == DebugType::MemoryUsage) {
      fprintf(stdout, "GUID:%s; Name:%s, Memory used: %d bytes.\n", debug_entry.GUID, debug_entry.name, debug_entry.value_u32);
    } else {
      u64 end_clock = __rdtsc();
      u64 duration = end_clock - debug_entry.clock;
      u32 current_kilocycles = static_cast<u32>(duration / 1000);
      fprintf(stdout, "GUID:%s; Name:%s, Clock:%dkcy.\n", debug_entry.GUID, debug_entry.name, current_kilocycles);
    }
  }

  fprintf(stdout, "\n\n");
  fflush(stdout);
}

#else

#define BEGIN_PROFILE(...)
#define END_PROFILE(...)
#define TIMED_BLOCK(...)
#define FRAME_MARKER(...)
#define END_DEBUG(...)

#endif

#endif
