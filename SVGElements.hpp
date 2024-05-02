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
    /**
     * SVGElement Class
     * This is the superclass of all type of elements.
    */
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        SVGElement(string id_);
        virtual void draw(PNGImage &img) const = 0;
        virtual void translate(const Point &t) = 0;
        virtual void rotate(int degrees,Point &t) = 0;
        virtual void scale(int v, Point &t) = 0;
        virtual SVGElement* clone() const = 0;
        string get_id();
    private:
        string id;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);



    /**
     * Group Class
     * 
     * This is the class for the Group Elements.
    */
    class Group : public SVGElement {
    public:
        Group(vector<SVGElement*> elements);    //constructor
        ~Group();                               //destructor
        void addElement(SVGElement* e);
        vector<SVGElement*> getElements();
        void draw(PNGImage &img) const override;    
        void translate(const Point &t) override; 
        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;
        Group* clone() const override;

    private:
        vector<SVGElement*> elements;
    };




    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        ~Ellipse() {}
        void draw(PNGImage &img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;
        Ellipse* clone() const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };



    class Circle : public Ellipse
    {
    public:
        Circle(const Color &fill, const Point &center, const int &radius);
        ~Circle() {}
        void draw(PNGImage &img) const override;
        Circle* clone() const override;
    };


    class Polyline : public SVGElement 
    {
    public:
        Polyline(const std::vector<Point>& _points, Color _stroke);
        ~Polyline() {}
        void draw(PNGImage& img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees, Point &t) override;
        void scale(int v,Point &t) override;
        Polyline* clone() const override;

    private:
        std::vector<Point> points; 
        Color stroke;                             
    };

    class Line : public Polyline 
    {
    public:
        Line(int _x1, int _y1, int _x2, int _y2, Color _stroke);
        ~Line() {}
        Line* clone() const override;
    };

    class Polygon : public SVGElement 
    {

    public:
        Polygon(const std::vector<Point>& _points, Color _fill);
        ~Polygon() {}
        void draw(PNGImage& img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;
        Polygon* clone() const override;


    private:
        std::vector<Point> points;
        Color fill; 
    };

    class Rect : public Polygon 
    {
    public:
        Rect(int _x, int _y, Color _fill, int _width, int _height);
        ~Rect() {}
        Rect* clone() const override;
    };
}

#endif