#ifndef _VEC2D_
#define _VEC2D_

template <class T>
class Vec2d {
public:
    Vec2d() {}
    Vec2d(T x, T y)
        : x(x), y(y) {}
     
    double norm() const {
        return sqrt(x*x + y*y);
    }
     
    double norm_squared() const {
        return x*x + y*y;
    }

    Vec2d<T> unit() const {
        return *this * (1 / norm());
    }

    double dot(Vec2d<T> v) const {
        return x * v.x + y * v.y;
    }
    
    Vec2d<T> operator-() const {
        Vec2d<T> v(-x, -y);
        return v;
    }
    
    Vec2d<T>& operator+=(const Vec2d<T>& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    
    Vec2d<T>& operator-=(const Vec2d<T>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    
    Vec2d<T>& operator*=(const double rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    
    Vec2d<T>& operator/=(const double rhs) {
        x *= 1.0 / rhs;
        y *= 1.0 / rhs;
        return *this;
    }
    
    friend bool operator==(const Vec2d<T> lhs, const Vec2d<T>& rhs) {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }
    
    friend bool operator!=(const Vec2d<T> lhs, const Vec2d<T>& rhs) {
        return (lhs.x != rhs.x || lhs.y != rhs.y);
    }
    
    friend Vec2d<T> operator+(const Vec2d<T>& lhs, const Vec2d<T>& rhs) {
        return Vec2d<T>(lhs.x + rhs.x, lhs.y + rhs.y);
    }
    
    friend Vec2d<T> operator-(const Vec2d<T>& lhs, const Vec2d<T>& rhs) {
        return Vec2d<T>(lhs.x - rhs.x, lhs.y - rhs.y);
    }
    
    friend Vec2d<T> operator*(const Vec2d<T>& lhs, const double rhs) {
        return Vec2d<T>(rhs * lhs.x, rhs * lhs.y);
    }
    
    friend Vec2d<T> operator/(const Vec2d<T>& lhs, const double rhs) {
        return Vec2d<T>(lhs.x * (1.0 / rhs), lhs.y * (1.0 / rhs));
    }
    
    friend Vec2d<T> operator*(const double lhs, const Vec2d<T>& rhs) {
        return Vec2d<T>(lhs * rhs.x, lhs * rhs.y);
    }
    
    friend double operator|(const Vec2d<T>& lhs, const Vec2d<T>& rhs) {
        Vec2d<T> e = rhs.unit();
        return lhs.dot(e);
    }
    
    T x;
    T y;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Vec2d<T>& rhs)
{
  os << rhs.x << ", " << rhs.y;
  return os;
}

#endif
