#include "camera.h"

void Camera::recomputeViewMatrix() {

  view_mat = translate(pos);
  view_projection_mat =
      view_mat *
      projection_mat; // NOTE:(vadim). Row major !!! Vertices * View matrix * Projection. Col major other way around
}

void Camera::setPosition(v3 new_pos) {
  pos = new_pos;
  recomputeViewMatrix();
}

void Camera::setRotation(f32 new_rot) {
  rot = new_rot;
  recomputeViewMatrix();
}

struct CameraController {
  CameraController() = default;
  CameraController(f32 aspect_ratio)
      : aspect_ratio{aspect_ratio},
        camera(-aspect_ratio * zoom_level, aspect_ratio * zoom_level, -zoom_level, zoom_level){};
  f32 aspect_ratio;
  f32 zoom_level = 1.0f;
  Camera camera;
  v3 camera_pos = {0.0f, 0.0f, 0.0f};
  f32 camera_move_speed = 5.0f;
  void update(GameControllerInput *controller_input, f32 dt_for_frame);
};


void CameraController::update(GameControllerInput *controller_input, f32 dt_for_frame) {

  if (controller_input->move_up.ended_down) {
    camera_pos.y -= camera_move_speed * dt_for_frame;
  }

  if (controller_input->move_down.ended_down) {
    camera_pos.y += camera_move_speed * dt_for_frame;
  }

  if (controller_input->move_right.ended_down) {
    camera_pos.x -= camera_move_speed * dt_for_frame;
  }

  if (controller_input->move_left.ended_down) {
    camera_pos.x += camera_move_speed * dt_for_frame;
  }

  camera.setPosition(camera_pos);
  camera_move_speed = zoom_level;
}
