#ifndef CAMERA_H
#define CAMERA_H

struct Camera {
    Camera(f32 left, f32 right, f32 bottom, f32 top)  {
        projection_mat = ortho(left, right, bottom, top, -1.0f, 1.0f);
        view_mat = identity();
        view_projection_mat = projection_mat * view_mat;
    }

    void recomputeViewMatrix();
    void setPosition(v3 new_pos);
    void setRotation(f32 new_rot);

    Mat4x4 projection_mat;
    Mat4x4 view_mat;
    Mat4x4 view_projection_mat;
    v3 pos;
    f32 rot = 0.0f;
};

#endif
