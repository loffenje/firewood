#ifndef MATH_H
#define MATH_H

#include <math.h>

constexpr f32 Pi = 3.14159265358979323846;

namespace math {
inline f32 radians(f32 deg) { return (Pi / 180) * deg; }

inline f32 degress(f32 rad) { return (180 / Pi) * rad; }

template <typename T>
constexpr T square(T value) {
    return value * value;
}
}  // namespace math

template <typename T>
struct Vector2 {
    T x, y;

    Vector2() = default;

    Vector2(T a, T b) {
	x = a;
	y = b;
    }

    Vector2<T> &operator+=(const Vector2<T> &v) {
	x += v.x;
	y += v.y;

	return (*this);
    }

    Vector2<T> &operator-=(const Vector2<T> &v) {
	x -= v.x;
	y -= v.y;

	return (*this);
    }

    Vector2<T> &operator*=(T val) {
	x *= val;
	y *= val;

	return (*this);
    }

    Vector2<T> &operator/=(T val) {
	val = 1.0f / val;
	x *= val;
	y *= val;

	return (*this);
    }
};

template <typename T>
inline Vector2<T> operator+(const Vector2<T> &a, const Vector2<T> &b) {
    return (Vector2<T>(a.x + b.x, a.y + b.y));
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T> &a, const Vector2<T> &b) {
    return (Vector2<T>(a.x - b.x, a.y - b.y));
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T> &v, T val) {
    return (Vector2<T>(v.x * val, v.y * val));
}

template <typename T>
inline Vector2<T> operator*(T val, const Vector2<T> &v) {
    return (Vector2<T>(v.x * val, v.y * val));
}

template <typename T>
inline Vector2<T> operator/(const Vector2<T> &v, T val) {
    val = 1.0f / val;
    return (Vector2<T>(v.x * val, v.y * val));
}

template <typename T>
inline T magnitude(const Vector2<T> &v) {
    return (sqrt(v.x * v.x + v.y * v.y));
}

template <typename T>
inline Vector2<T> normalize(const Vector2<T> &v) {
    return (v / magnitude(v));
}

template <typename T>
inline T dot(const Vector2<T> &a, const Vector2<T> &b) {
    return a.x * b.x + a.y * b.y;
}

template <typename T>
struct Vector4 {
    T x, y, z, w;
};

template <typename T>
struct Vector3 {
    T x, y, z;

    Vector3() = default;

    Vector3(T a, T b, T c) {
	x = a;
	y = b;
	z = c;
    }

    Vector3(const Vector4<T> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Vector3<T> &operator-() {
        x = -x;
        y = -y;
        z = -z;

        return *this;
    }

    Vector3<T> &operator+=(const Vector3<T> &v) {
	x += v.x;
	y += v.y;
	z += v.z;

	return (*this);
    }

    Vector3<T> &operator-=(const Vector3<T> &v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return (*this);
    }

    Vector3<T> &operator*=(T val) {
	x *= val;
	y *= val;
	z *= val;

	return (*this);
    }

    Vector3<T> &operator/=(T val) {
	val = 1.0f / val;
	x *= val;
	y *= val;
	z *= val;

	return (*this);
    }
};

template <typename T>
inline Vector3<T> operator+(const Vector3<T> &a, const Vector3<T> &b) {
    return (Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z));
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T> &a, const Vector3<T> &b) {
    return (Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z));
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T> &v, T val) {
    return (Vector3<T>(v.x * val, v.y * val, v.z * val));
}


template <typename T>
inline Vector3<T> operator*(T val, const Vector3<T> &v) {
    return (Vector3<T>(v.x * val, v.y * val, v.z * val));
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T> &v, T val) {
    val = 1.0f / val;
    return (Vector3<T>(v.x * val, v.y * val, v.z * val));
}

template <typename T>
inline T magnitude(const Vector3<T> &v) {
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

template <typename T>
inline Vector3<T> normalize(const Vector3<T> &v) {
    return (v / magnitude(v));
}

template <typename T>
inline T dot(const Vector3<T> &a, const Vector3<T> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
inline Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b) {
    return Vector3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
		      a.x * b.y - a.y * b.x);
}

template <typename T>
inline Vector3<T> project(const Vector3<T> &a, const Vector3<T> &b) {
    return b * (dot(a, b) / dot(b, b));
}
template <typename T>
inline Vector3<T> reject(const Vector3<T> &a, const Vector3<T> &b) {
    return a - b * (dot(a, b) / dot(b, b));
}


typedef Vector2<float> v2;
typedef Vector2<int> v2i;
typedef Vector3<int> v3i;
typedef Vector3<float> v3;
typedef Vector4<float> v4;

struct Mat4x4 {
    f32 e[4][4]; // ROW-MAJOR entries
};

Mat4x4 operator*(const Mat4x4 &a, const Mat4x4 &b) {
    Mat4x4 result = {};
    for (int row = 0; row <= 3; ++row) {
        for (int col = 0; col <= 3; ++col) {
            for (int i = 0; i <= 3; ++i) {
                result.e[row][col] += a.e[row][i]*b.e[i][col];
            }
        }
    }

    return result;
}

inline Mat4x4 identity()
{
    Mat4x4 result = {
        {{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}}
    };
    
    return result;
}

inline Mat4x4 transpose(const Mat4x4 &m) {
    Mat4x4 result;

    for (int j = 0; j <= 3; ++j) {
        for (int i = 0; i <= 3; ++i) {
            result.e[j][i] = m.e[i][j];
        }
    }

    return result;
}

Mat4x4 affineInverse(Mat4x4 &m) {
    const v3 &a = reinterpret_cast<const v3 &>(m.e[0]);
    const v3 &b = reinterpret_cast<const v3 &>(m.e[1]);
    const v3 &c = reinterpret_cast<const v3 &>(m.e[2]);
    const v3 &d = reinterpret_cast<const v3 &>(m.e[3]);

    f32 &x = m.e[3][0];
    f32 &y = m.e[3][1];
    f32 &z = m.e[3][2];
    f32 &w = m.e[3][3];

    v3 s = cross(a, b);
    v3 k = cross(c, d);
    v3 u = a * y - b * x;
    v3 v = c * w - d * z;

    f32 inv_det = 1.0f / dot(s, v) + dot(k, u);
    s *= inv_det;
    k *= inv_det;
    u *= inv_det;
    v *= inv_det;

    v3 r0 = cross(b, v) + k * y;
    v3 r1 = cross(v, a) - k * x;
    v3 r2 = cross(d, u) + s * w;
    v3 r3 = cross(u, c) - s * z;

    Mat4x4 result = {
        {{r0.x, r0.y, r0.z, -dot(b,k)},
            {r1.x, r1.y, r1.z, dot(a,k)},
            {r2.x, r2.y, r2.z, -dot(d,s)},
            {r3.x, r3.y, r3.z, dot(c,s)}}
    };

    
    return result;
}

Mat4x4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    Mat4x4 result = identity();
    result.e[0][0] = 2.0f / (right - left);
    result.e[1][1] = 2.0f / (top - bottom);
    result.e[2][2] = -2.0f / (far - near);
    result.e[3][0] = -(right + left) / (right - left);
    result.e[3][1] = -(top + bottom) / (top - bottom);
    result.e[3][2] = -(far + near) / (far - near);

    return result;
}

inline Mat4x4 translate(Mat4x4 m, const v3 &delta) {
    Mat4x4 result = m;

    result.e[0][3] += delta.x;
    result.e[1][3] += delta.y;
    result.e[2][3] += delta.z;

    return result;
}

inline Mat4x4 translate(const v3 &delta) {
    Mat4x4 result = {
            {{1, 0, 0, delta.x},
            {0, 1, 0, delta.y},
            {0, 0, 1, delta.z}, 
            {0, 0, 0, 1}}
    };

    return result;
}

inline Mat4x4 scale(const v3 &size) {
    Mat4x4 result = {
            {{size.x, 0, 0, 0}, 
            {0, size.y, 0, 0}, 
            {0, 0, size.z, 0}, 
            {0, 0, 0, 1}}
    };

    return result;
}

inline Mat4x4 rotX(f32 t) {
    f32 sin_t = sin(math::radians(t));
    f32 cos_t = cos(math::radians(t));

    Mat4x4 result = {
            {{1, 0, 0, 0}, 
            {0, cos_t, -sin_t, 0}, 
            {0, sin_t, cos_t, 0},
            {0, 0, 0, 1}},
    };

    return result;
}

inline Mat4x4 rotY(f32 t) {
    f32 sin_t = sin(math::radians(t));
    f32 cos_t = cos(math::radians(t));

    Mat4x4 result = {
            {{cos_t, 0, sin_t, 0},
            {0, 1, 0, 0}, 
            {-sin_t, 0, cos_t, 0}, 
            {0, 0, 0, 1}}
    };

    return result;
}

inline Mat4x4 rotZ(f32 t) {
    f32 sin_t = sin(math::radians(t));
    f32 cos_t = cos(math::radians(t));

    Mat4x4 result = {
        {{cos_t, -sin_t, 0, 0}, 
        {sin_t, cos_t, 0, 0},
        {0, 0, 1, 0}, 
        {0, 0, 0, 1}}
    };

    return result;
}

internal v4 transform(const Mat4x4 &m, v4 p) {
    v4 result;

    result.x = p.x*m.e[0][0] + p.y*m.e[0][1] + p.z*m.e[0][2] + p.w*m.e[0][3];
    result.y = p.x*m.e[1][0] + p.y*m.e[1][1] + p.z*m.e[1][2] + p.w*m.e[1][3];
    result.z = p.x*m.e[2][0] + p.y*m.e[2][1] + p.z*m.e[2][2] + p.w*m.e[2][3];
    result.w = p.x*m.e[3][0] + p.y*m.e[3][1] + p.z*m.e[3][2] + p.w*m.e[3][3];
    
    return result;
}

inline v3 operator*(const Mat4x4 &m, v3 p) {
    v4 result = transform(m, v4{p.x,p.y,p.z, 1.0f});

    return v3{result.x, result.y, result.z};
}


inline v4 operator*(const Mat4x4 &m, v4 p) {
    v4 result = transform(m, p);

    return result;
}

template <typename T>
struct Point3 : Vector3<T> {
    Point3() = default;

    Point3(T a, T b, T c) : Vector3<T>(a, b, c) {}
};

template <typename T>
inline Point3<T> operator+(const Point3<T> &p, const Vector3<T> &v) {
    return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
}

template <typename T>
inline Vector3<T> operator-(const Point3<T> &p1, const Point3<T> &p2) {
    return Vector3<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

struct Vertex {
    v3 position;
    v4 color;
};

#endif
