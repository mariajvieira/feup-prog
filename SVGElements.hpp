//! @file shape.hpp
#ifndef _svg_SVGElements_hpp_
#define _svg_SVGElements_hpp_

#include <vector>
using namespace std;
#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual void translate(const Point &t) = 0;
        virtual void rotate(int degrees,Point &t) = 0;
        virtual void scale(int v, Point &t) = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);



    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;

    private:
        Color fill;
        Point center;
        Point radius;
    };



    class Circle : public Ellipse
    {
    public:
        Circle(const Color &fill, const Point &center, const int &radius);
        void draw(PNGImage &img) const override;
    };


    class Polyline : public SVGElement 
    {
    public:
        Polyline(const std::vector<Point>& _points, Color _stroke);
        void draw(PNGImage& img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees, Point &t) override;
        void scale(int v,Point &t) override;

    private:
        std::vector<Point> points; 
        Color stroke;                             
    };

    class Line : public Polyline 
    {
    public:
        Line(int _x1, int _y1, int _x2, int _y2, Color _stroke);

    };

    class Polygon : public SVGElement 
    {

    public:
        Polygon(const std::vector<Point>& _points, Color _fill);
        void draw(PNGImage& img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;


    private:
        std::vector<Point> points;
        Color fill; 
    };

    class Rect : public Polygon 
    {
    public:
        Rect(int _x, int _y, Color _fill, int _width, int _height);
    };

}
#endif