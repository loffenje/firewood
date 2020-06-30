#ifndef INPUT_H
#define INPUT_H

#include <assert.h>
struct GameButtonState {
	int half_transition_count;
	b32 ended_down;
};

struct GameControllerInput {
	b32 is_connected;
	b32 is_analog;
	f32 stick_average_x;
	f32 stick_average_y;
	f32 clutch_max;

	union {
		GameButtonState buttons[12];
		struct {
			GameButtonState move_up;
			GameButtonState move_down;
			GameButtonState move_left;
			GameButtonState move_right;

			GameButtonState action_up;
			GameButtonState action_down;
			GameButtonState action_left;
			GameButtonState action_right;

			GameButtonState left_shoulder;
			GameButtonState right_shoulder;
		
			GameButtonState back;
			GameButtonState start;

			GameButtonState terminator;
		};
	};
};

#define MAX_CONTROLLER_COUNT 5

enum GameInputMouseButton {
	MouseButton_Left,
	MouseButton_Middle,
	MouseButton_Right,
	MouseButton_Extended0,
	MouseButton_Extended1,
	MouseButton_Count
};

struct GameInput {
	f32 dt_for_frame;
	GameControllerInput controllers[MAX_CONTROLLER_COUNT];
	b32 quit_requested;
	GameButtonState mouse_buttons[MouseButton_Count];
	v3 clip_space_mouse_p;
	b32 shift_down;
};

inline GameControllerInput *getController(GameInput *input, unsigned int controller_index)
{
	assert(controller_index < ARRAY_LEN(input->controllers));	
	GameControllerInput *result = &input->controllers[controller_index];

	return result; 
}

inline void swapInput(GameInput **input0, GameInput **input1)
{
	GameInput *temp = *input0;
	*input0 = *input1;
	*input1 = temp;
}

#endif
