#include "camera.h"

void Camera::recomputeViewMatrix() {
    view_mat = translate(pos) * rotZ(rot);
    view_projection_mat = projection_mat * view_mat;
}

void Camera::setPosition(v3 new_pos) {
    pos = new_pos;
    recomputeViewMatrix();
}

void Camera::setRotation(f32 new_rot) {
    rot = new_rot;
    recomputeViewMatrix();
}
