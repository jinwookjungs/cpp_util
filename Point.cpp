/**
 * @file    Point.cpp
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-03-06 15:19:31
 *
 * Created on Mon Mar  6 15:19:31 2017.
 */

#include "Point.h" 

Point::Point(void) : x(0), y(0) {
    //
}

Point::Point(double nx, double ny) : x(nx), y(ny) {
    //
}

Point::Point(const Point& src) : x(src.x), y(src.y) {
    //
}

Point::~Point(void){
    //
}

Point& Point::operator=(const Point& src){
    x = src.x; y = src.y;
    return *this;
}

Point Point::operator*(const Point& alt){
    Point p(x*alt.x, y*alt.y);
    return p;
}

Point Point::operator/(const Point& alt){
    Point p(x/alt.x, y/alt.y);
    return p;
}

Point Point::operator-(const Point& alt){
    Point p(x-alt.x, y-alt.y);
    return p;
}

Point Point::operator+(const Point& alt){
    Point p(x+alt.x, y+alt.y);
    return p;
}

Point Point::operator*(double num){
    Point p(x*num, y*num);
    return p;
}

Point Point::operator/(double num){
    Point p(x/num, y/num);
    return p;
}

Point Point::operator-(double num){
    Point p(x-num, y-num);
    return p;
}

Point Point::operator+(double num){
    Point p(x+num, y+num);
    return p;
}

Point Point::operator*=(const Point& alt){
    x *= alt.x; y *= alt.y;
    return *this;
}

Point Point::operator/=(const Point& alt){
    x /= alt.x; y /= alt.y;
    return *this;
}

Point Point::operator-=(const Point& alt){
    x -= alt.x; y -= alt.y;
    return *this;
}

Point Point::operator+=(const Point& alt){
    x += alt.x; y += alt.y;
    return *this;
}

Point Point::operator*=(double num){
    x *= num; y *= num;
    return *this;
}

Point Point::operator/=(double num){
    x /= num; y /= num;
    return *this;
}

Point Point::operator-=(double num){
    x -= num; y -= num;
    return *this;
}

Point Point::operator+=(double num){
    x += num; y += num;
    return *this;
}

