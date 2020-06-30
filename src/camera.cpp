#include "camera.h"

void Camera::recomputeViewMatrix() {
    inv_mat = translate(pos).matrix;
    Transform transform = Transform(inv_mat) * rotate(rot, v3(0, 0, 1));
    view_mat = transform.matrix;  // undoes the last transformation
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
