/**
 * @file    Point.h
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-03-06 15:18:28
 *
 * Created on Mon Mar  6 15:18:28 2017.
 */

#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point 
{
    double x;
    double y;

    Point(void);
    Point(double nx, double ny); 
    Point(const Point& src);
    ~Point(void);

    Point& operator=(const Point& src);
    Point operator*(const Point& alt);
    Point operator/(const Point& alt);
    Point operator-(const Point& alt);
    Point operator+(const Point& alt);
    Point operator*(double num);
    Point operator/(double num);
    Point operator-(double num);
    Point operator+(double num);
    Point operator*=(const Point& alt);
    Point operator/=(const Point& alt);
    Point operator-=(const Point& alt);
    Point operator+=(const Point& alt);
    Point operator*=(double num);
    Point operator/=(double num);
    Point operator-=(double num);
    Point operator+=(double num);

    friend std::ostream& operator<<(std::ostream& out, const Point& pt);
};

inline std::ostream& operator<<(std::ostream& out, const Point& pt){
    out << "Point(" << pt.x << "," << pt.y << ")";
    return out;
}

#endif
