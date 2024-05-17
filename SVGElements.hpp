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
     * @class SVGElement
     * @brief The base class for all SVG elements.
     * 
     * This class serves as the superclass for various SVG elements. 
     * It provides a common interface for operations like drawing, translating, rotating, and scaling.
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

    /**
     * @brief Reads an SVG file and populates the provided dimensions and elements vector.
     * @param svg_file The path to the SVG file.
     * @param dimensions The dimensions of the SVG canvas.
     * @param svg_elements A vector to store the SVG elements.
     */
    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    
    /**
     * @brief Converts an SVG file to a PNG file.
     * @param svg_file The path to the SVG file.
     * @param png_file The path to the output PNG file.
     */
    void convert(const std::string &svg_file,
                 const std::string &png_file);



    /**
     * @class Group
     * @brief A class representing a group of SVG elements.
     * 
     * This class allows grouping multiple SVG elements together, 
     * enabling collective transformations and drawing operations.
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



    /**
     * @class Ellipse
     * @brief A class representing an ellipse element.
     */
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


    /**
     * @class Circle
     * @brief A class representing a circle element.
     * 
     * This class is a specialized version of the Ellipse class.
     */
    class Circle : public Ellipse
    {
    public:
        Circle(const Color &fill, const Point &center, const int &radius);   //constructor
        void draw(PNGImage &img) const override;                //destructor
        Circle* clone() const override;     //function that creates a copy of the element
    };

    /**
     * @class Polyline
     * @brief A class representing a polyline element.
     */
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


    /**
     * @class Line
     * @brief A class representing a line element.
     * 
     * This class is a specialized version of the Polyline class.
     */
    class Line : public Polyline 
    {
    public:
        Line(int _x1, int _y1, int _x2, int _y2, Color _stroke);
        ~Line() {}
        Line* clone() const override;
    };


    /**
     * @class Polygon
     * @brief A class representing a polygon element.
     */
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


    /**
     * @class Rect
     * @brief A class representing a rectangle element.
     * 
     * This class is a specialized version of the Polygon class.
     */
    class Rect : public Polygon 
    {
    public:
        Rect(int _x, int _y, Color _fill, int _width, int _height); //constructor
        ~Rect() {}
        Rect* clone() const override;       //function that creates a copy of the element
    };
}

#endif