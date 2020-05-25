#ifndef MATH_H
#define MATH_H

#include <math.h>

const f32 Pi = 3.14159265358979323846;

namespace math {
    inline f32 radians(f32 deg) 
    {
        return (Pi / 180) * deg;
    }

    inline f32 degress(f32 rad)
    {
        return (180 / Pi) * rad;
    }
    
    template <typename T>
    constexpr T square(T value)
    {
        return value * value;
    } 
}


template <typename T>
struct Vector2
{
    T x,y;

    Vector2() = default;

    Vector2(T a, T b)
    {
        x = a;
        y = b;
    }
    
    Vector2<T> &operator+=(const Vector2<T> &v)
    {
        x += v.x;
        y += v.y;
        
        return (*this);
    }
    
    Vector2<T> &operator-=(const Vector2<T> &v)
    {
        x -= v.x;
        y -= v.y;
        
        return (*this);
    }
    
    Vector2<T> &operator*=(T val)
    {
        x *= val;
        y *= val;
        
        return (*this);
    }

    Vector2<T> &operator/=(T val)
    {
        val = 1.0f / val;
        x *= val;
        y *= val;
        
        return (*this);
    }
};

template <typename T>
inline Vector2<T> operator+(const Vector2<T> &a, const Vector2<T> &b)
{
    return (Vector2<T>(a.x + b.x, a.y + b.y));
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T> &a, const Vector2<T> &b)
{
    return (Vector2<T>(a.x - b.x, a.y - b.y));
}

template <typename T>
inline Vector2<T> operator *(const Vector2<T> &v, T val)
{
    return (Vector2<T>(v.x * val, v.y * val));
}

template <typename T>
inline Vector2<T> operator *(T val, const Vector2<T> &v)
{
    return (Vector2<T>(v.x * val, v.y * val));
}

template <typename T>
inline Vector2<T> operator /(const Vector2<T> &v, T val)
{
    val = 1.0f / val;
    return (Vector2<T>(v.x * val, v.y * val));
}

template <typename T>
inline T magnitude(const Vector2<T> &v)
{
    return (sqrt(v.x * v.x + v.y * v.y));
}

template <typename T>
inline Vector2<T> normalize(const Vector2<T> &v)
{
    return (v / magnitude(v));
}

template <typename T>
inline T dot(const Vector2<T> &a, const Vector2<T> &b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
struct Vector3
{
    T x,y,z;

    Vector3() = default;

    Vector3(T a, T b, T c)
    {
        x = a;
        y = b;
        z = c;
    }
    
    Vector3<T> &operator+=(const Vector3<T> &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        
        return (*this);
    }
    
    Vector3<T> &operator-=(const Vector3<T> &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        
        return (*this);
    }
    
    Vector3<T> &operator*=(T val)
    {
        x *= val;
        y *= val;
        z *= val;
        
        return (*this);
    }

    Vector3<T> &operator/=(T val)
    {
        val = 1.0f / val;
        x *= val;
        y *= val;
        z *= val;
        
        return (*this);
    }
};

template <typename T>
inline Vector3<T> operator+(const Vector3<T> &a, const Vector3<T> &b)
{
    return (Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z));
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T> &a, const Vector3<T> &b)
{
    return (Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z));
}

template <typename T>
inline Vector3<T> operator *(const Vector3<T> &v, T val)
{
    return (Vector3<T>(v.x * val, v.y * val, v.z * val));
}

template <typename T>
inline Vector3<T> operator /(const Vector3<T> &v, T val)
{
    val = 1.0f / val;
    return (Vector3<T>(v.x * val, v.y * val, v.z * val));
}

template <typename T>
inline T magnitude(const Vector3<T> &v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

template <typename T>
inline Vector3<T> normalize(const Vector3<T> &v)
{
    return (v / magnitude(v));
}

template <typename T>
inline T dot(const Vector3<T> &a, const Vector3<T> &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
inline Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b)
{
    return Vector3<T>(a.y * b.z - a.z * b.y,
                      a.z * b.x - a.x * b.z,
                      a.x * b.y - a.y * b.x);
}

template <typename T>
inline Vector3<T> project(const Vector3<T> &a, const Vector3<T> &b)
{
    return b * (dot(a, b) / dot(b, b));
}
template <typename T>
inline Vector3<T> reject(const Vector3<T> &a, const Vector3<T> &b)
{
    return  a - b * (dot(a, b) / dot(b,b));
}

template <typename T>
struct Vector4
{
    T x,y,z,w;
};

typedef Vector2<float> v2;
typedef Vector2<int> v2i;
typedef Vector3<int> v3i;
typedef Vector3<float> v3;
typedef Vector4<float> v4;

template <typename T>
struct Mat3x3
{
    T n[3][3];

    Mat3x3() = default;

    Mat3x3(T n00, T n01, T n02,
           T n10, T n11, T n12,
           T n20, T n21, T n22) {
        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;       
    }

    Mat3x3(const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c)
    {
        n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z;
        n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z;
        n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z;       
    }
    
    T &operator()(int i, int j)
    {
        return n[i][j];
    }

    Vector3<T> &operator[](int j)
    {
        return *reinterpret_cast<Vector3<T> *>(n[j]);
    }
};

template <typename T>
Mat3x3<T> operator *(const Mat3x3<T> &a, const Mat3x3<T> &b)
{
    return Mat3x3<T>( 
        a(0, 0) * b(0,0) + a(0,1) * b(1,0) + a(0,2) * b(2,0),
        a(0,0) * b (0, 1) + a(0, 1) * b (1, 1) + a(0,2) * b(2, 1),
        a(0,0) * b(0,2) + a(0, 1) * b (1, 2) + a(0,2) * b(2,2),
        a(1,0) * b(0,0) + a(1, 1) * b (1, 0) + a(1,2) * b(2, 0),
        a(1, 0) * b(0,1) + a(1,1) * b (1, 1) + a(1,2) * b(2, 1),
        a(1, 0) * b(0,2) + a(1, 1) * b (1,2) + a(1,2) * b (2, 2),
        a(2,0) * b(0,0) + a(2, 1) * b (1, 0) + a(2,2) * b (2, 0),
        a(2,0) * b(0,1) + a(2, 1) * b (1, 1) + a(2,2) * b(2, 1),
        a(2, 0) * b(0,2) + a(2, 1) * b (1, 2) + a(2,2) * b(2, 2));
}    

template <typename T>
Vector3<T> operator *(const Mat3x3<T> &m, const Vector3<T> &v)
{
    return Vector3<T>(m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z,
                      m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z,
                      m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z);
}


template <typename T>
T det(const Mat3x3<T> &m)
{
    return m(0,0) * (m(1,1) * m(2,2) - m(1,2) * m(2,1))
         + m(0,1) * (m(1,2) * m(2,0) - m(1,0) * m(2,2))
         + m(0,2) * (m(1,0) * m(2,1) - m(1,1) * m(2,0));
}

template <typename T>
Mat3x3<T> inverse(const Mat3x3<T> &m)
{
    const Vector3<T> &a = m[0];
    const Vector3<T> &b = m[1];
    const Vector3<T> &c = m[2];

    Vector3<T> r0 = cross(b,c);
    Vector3<T> r1 = cross(c,a);
    Vector3<T> r2 = cross(a,b);

    float inv_det = 1.0f / dot(r2, c);

    return Mat3x3<T>(r0.x * inv_det, r0.y * inv_det, r0.z * inv_det,
                r1.x * inv_det, r1.y * inv_det, r1.z * inv_det,
                r2.x * inv_det, r2.y * inv_det, r2.z * inv_det);
}


struct Mat4x4
{
    f32 n[4][4];

    Mat4x4() {
        n[0][0] = n[1][1] = n[2][2] = n[3][3] = 1.f;
        n[0][1] = n[0][2] = n[0][3] = n[1][0] =
            n[1][2] = n[1][3] = n[2][0] = n[2][1] = n[2][3] =
            n[3][0] = n[3][1] = n[3][2] = 0.f;
    }

    Mat4x4(f32 n00, f32 n01, f32 n02, f32 n03,
           f32 n10, f32 n11, f32 n12, f32 n13,
           f32 n20, f32 n21, f32 n22, f32 n23,
           f32 n30, f32 n31, f32 n32, f32 n33
           ) {
        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
        n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
    }
    
    f32 &operator()(int i, int j)
    {
        return n[i][j];
    }
    
    Mat4x4 transpose(const Mat4x4 &m);
};

Mat4x4 operator *(const Mat4x4 &m1, const Mat4x4 &m2) {
    Mat4x4 r;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            r.n[i][j] = m1.n[i][0] * m2.n[0][j] +
                        m1.n[i][1] * m2.n[1][j] +
                        m1.n[i][2] * m2.n[2][j] +
                        m1.n[i][3] * m2.n[3][j];
        }
    }

    return r;
}

Mat4x4 transpose(const Mat4x4 &m) {
   return Mat4x4(m.n[0][0], m.n[1][0], m.n[2][0], m.n[3][0],
                    m.n[0][1], m.n[1][1], m.n[2][1], m.n[3][1],
                    m.n[0][2], m.n[1][2], m.n[2][2], m.n[3][2],
                    m.n[0][3], m.n[1][3], m.n[2][3], m.n[3][3]);
}


struct Transform
{
    Mat4x4 matrix;
    Transform() = default;
    Transform(const Mat4x4 &m): matrix{m} {}
    Transform(const f32 mat[4][4]) {
        matrix = Mat4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                         mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                         mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                         mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    }
    
    Transform operator*(const Transform &t);
};


Transform Transform::operator*(const Transform &t)
{
    return Transform(matrix * t.matrix);
}

Transform transpose(const Transform &t)
{
    return Transform(transpose(t.matrix));
}

Transform translate(const v3 &delta)
{
    Mat4x4 m(1,0,0, delta.x,
             0,1,0, delta.y,
             0,0,1, delta.z,
             0,0,0,1);

    return Transform(m);
}

Transform scale(f32 x, f32 y, f32 z) 
{
    Mat4x4 m(x,0,0,0,
            0,y,0,0,
            0,0,z,0,
            0,0,0,1);

    return Transform(m);
}

Transform rotX(f32 t)
{
    f32 sin_t = sin(math::radians(t));
    f32 cos_t = cos(math::radians(t));

    Mat4x4 m(1,0,0,0,
           0,cos_t,-sin_t,0,
           0,sin_t,cos_t, 0,
           0,0,0,1); 

    return Transform(m);   
}

Transform rotY(f32 t)
{
    f32 sin_t = sin(math::radians(t));
    f32 cos_t = cos(math::radians(t));

    Mat4x4 m(cos_t,0,sin_t,0,
           0,1,0,0,
           -sin_t,0,cos_t, 0,
           0,0,0,1); 

    return Transform(m);   
}

Transform rotZ(f32 t)
{
    f32 sin_t = sin(math::radians(t));
    f32 cos_t = cos(math::radians(t));

    Mat4x4 m(cos_t,-sin_t,0,0,
           sin_t,cos_t,0,0,
           0,0,1,0,
           0,0,0,1); 

    return Transform(m);   
}

Transform rotate(f32 t, const v3 &axis) 
{
    v3 a = normalize(axis);
    f32 sin_t = sin(math::radians(t));
    f32 cos_t = cos(math::radians(t));
    
    Mat4x4 m;
    m.n[0][0] = a.x * a.x + (1 - a.x * a.x) * cos_t;
    m.n[0][1] = a.x * a.y * (1 - cos_t) - a.z * sin_t;
    m.n[0][2] = a.x * a.z * (1 - cos_t) + a.y * sin_t;
    m.n[0][3] = 0;

    m.n[1][0] = a.x * a.y * (1 - cos_t) + a.z * sin_t;
    m.n[1][1] = a.y * a.y + (1 - a.y * a.y) * cos_t;
    m.n[1][2] = a.y * a.z * (1 - cos_t) - a.x * sin_t;
    m.n[1][3] = 0;

    m.n[2][0] = a.x * a.z * (1 - cos_t) - a.y * sin_t;
    m.n[2][1] = a.y * a.z * (1 - cos_t) + a.x *  sin_t;
    m.n[2][2] = a.z * a.z + (1 - a.z * a.z) * cos_t;
    m.n[2][3] = 0;

    return Transform(m);
}

v3 operator *(const Transform &t, const v3 &v)
{
    v3 result;
    Mat4x4 m = t.matrix;
    return v3(m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z,
                      m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z,
                      m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z);
}

v3 operator*(const Transform &t, const v4 &v)
{
    v3 result;
    Mat4x4 m = t.matrix;
    return v3(m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z,
                      m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z,
                      m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z);
}

template <typename T>
struct Point3 : Vector3<T>
{
    Point3() = default;

    Point3(T a, T b, T c): Vector3<T>(a,b,c) {}
};

template <typename T>
inline Point3<T> operator+(const Point3<T> &p, const Vector3<T> &v) {
    return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
}

template <typename T>
inline Vector3<T> operator-(const Point3<T> &p1, const Point3<T> &p2) {
    return Vector3<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}


struct Vertex 
{
    v3 position;
    v4 color;
};

#endif
