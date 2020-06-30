#ifndef SDL_PLATFORM_H
#define SDL_PLATFORM_H

#include <string.h>
#include <limits.h>

#define SDL_PATH_MAX PATH_MAX

struct SDLx_LoadedCode {
	b32 is_valid;
	char *dll_fullname_path;	
	void *dll;
	time_t dll_last_write_time;

	u32 function_count;
	const char **function_names;
	void **functions;

	void loadCode();
	void reloadCode();
	void unloadCode();
	b32 isCodeChanged();
};


struct SDLx_State {
	u64 total_size;
	void *game_memory_block;
	char *exepath; // @Implement string
	char *past_last_exepath_slash;
	
	//Functions
	void setEXEPath();
	void buildEXEFileName(const char *name, size_t dest_count, char *dest);
	void freeMemoryBlock();
};

void SDLx_State::setEXEPath()
{
	exepath = SDL_GetBasePath();
	past_last_exepath_slash = exepath;
	for (char *token = exepath;*token; ++token) {
		if (*token == '/') {
			past_last_exepath_slash = token + 1;
		}
	}
}

void SDLx_State::buildEXEFileName(const char *name, size_t dest_count, char *dest)
{
	concatStr(past_last_exepath_slash - exepath, 
			exepath, strlen(name),
			reinterpret_cast<char *>(const_cast<char *>(name)), dest_count, dest);
}

void SDLx_State::freeMemoryBlock()
{
	int result = munmap(game_memory_block, total_size);
	assert(result != -1);
}

#endif
