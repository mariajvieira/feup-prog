#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius) {}

    void Ellipse::draw(PNGImage &img) const {
        img.draw_ellipse(center, radius, fill);
    }
    void Ellipse::translate(const Point &t) {
        center = center.translate(t);
    }

    void Ellipse::rotate(int degrees,Point &t){
        center = center.rotate(t, degrees);
    }

    void Ellipse::scale(int v,Point &t) {
        radius = radius.scale(t, v);
    }


    //  Circle
    Circle::Circle(const Color &fill, const Point &center, const int &radius) 
        : Ellipse(fill, center, {radius,radius}) {}
        
    void Circle::draw(PNGImage &img) const {
        Ellipse::draw(img);
    }

    // Polyline
    Polyline::Polyline(const std::vector<Point>& _points, Color _stroke)
        : points(_points), stroke(_stroke) {}
    void Polyline::draw(PNGImage& img) const {
        for (size_t i=0; i<points.size()-1; i++) {
            img.draw_line(points[i], points[i+1], stroke);
        }
    }
    void Polyline::translate(const Point &t) {
        for (auto &point : points) {
            point = point.translate(t);
        }
    }

    void Polyline::rotate(int degrees,Point &t) {
        for (auto &point : points) {
            point = point.rotate(t, degrees);
        }
    }

    void Polyline::scale(int v,Point &t) {
        for (auto &point : points) {
            point = point.scale(t, v);
        }
    }

    // Line
    Line::Line(int _x1, int _y1, int _x2, int _y2, Color _stroke) 
        : Polyline({{_x1,_y1},{_x2,_y2}},_stroke) {}


    // Polygon
    Polygon::Polygon(const std::vector<Point>& _points, Color _fill) 
        : points(_points), fill(_fill) {}
        
    void Polygon::draw(PNGImage& img) const {
        img.draw_polygon(points, fill);
    }

    void Polygon::translate(const Point &t) {
        for (auto &point : points) {
            point = point.translate(t);
        }
    }

    void Polygon::rotate(int degrees,Point &t) {
        for (auto &point : points) {
            point = point.rotate(t, degrees);
        }
    }

    void Polygon::scale(int v,Point &t) {
        for (auto &point : points) {
            point = point.scale(t, v);
        }
    }


    
    // Rect
    Rect::Rect(int _x, int _y, Color _fill, int _width, int _height) 
       : Polygon({{_x,_y}, {_x+_width-1, _y}, {_x+_width-1, _y+_height-1}, {_x, _y+_height-1}},_fill) {}
}