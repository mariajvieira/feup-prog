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
        virtual ~SVGElement();  // destructor
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

        Group(vector<SVGElement*> elements); //constructor
        ~Group();                            //destructor
        void addElement(SVGElement* e);     //function to add element to the vector elements
        vector<SVGElement*> getElements();          //getter
        void draw(PNGImage &img) const override;
        void translate(const Point &t) override;

        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;
        Group* clone() const override;          //function that creates a copy of the group

    private:
        vector<SVGElement*> elements;
    };




    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);  //constructor
        ~Ellipse() {}                               //destructor
        void draw(PNGImage &img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;
        Ellipse* clone() const override;    //function that creates a copy of the element

    private:
        Color fill;
        Point center;
        Point radius;
    };


    //this class is a subclass of Ellipse
    //therefore, it overrides some functions that don't need to be defined
    class Circle : public Ellipse
    {
    public:
        Circle(const Color &fill, const Point &center, const int &radius);   //constructor
        void draw(PNGImage &img) const override;                //destructor
        Circle* clone() const override;     //function that creates a copy of the element
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
        Polyline* clone() const override;     //function that creates a copy of the element

    private:
        std::vector<Point> points; 
        Color stroke;                             
    };


    //this class is a subclass of Polyline
    //therefore, it overrides some functions that don't need to be defined
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
        Polygon(const std::vector<Point>& _points, Color _fill);  //constructor
        ~Polygon() {}                               //destructor
        void draw(PNGImage& img) const override;
        void translate(const Point &t) override;
        void rotate(int degrees,Point &t) override;
        void scale(int v,Point &t) override;
        Polygon* clone() const override;      //function that creates a copy of the element


    private:
        std::vector<Point> points;
        Color fill; 
    };


    //this class is a subclass of Polygon
    //therefore, it overrides some functions that don't need to be defined
    class Rect : public Polygon 
    {
    public:
        Rect(int _x, int _y, Color _fill, int _width, int _height); //constructor
        ~Rect() {}
        Rect* clone() const override;       //function that creates a copy of the element
    };
}

#endif