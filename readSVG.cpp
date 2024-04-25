#include <iostream>
#include <sstream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{

    void applyTransformation(SVGElement* element, const string& transform_attr, Point& transform_origin)
    {
            size_t open_p = transform_attr.find('(');
            size_t close_p = transform_attr.find(')');

            string f_type = transform_attr.substr(0, open_p);
            string p = transform_attr.substr(open_p + 1, close_p - open_p - 1);

            if (f_type == "rotate")
            {
                element->rotate(stoi(p), transform_origin);
            }
            else if (f_type == "scale")
            {
                element->scale(stoi(p), transform_origin);
            }
            else if (f_type == "translate")
            {
                size_t space_ = p.find(' ');
                int i1 = stoi(p.substr(0, space_));
                int i2 = stoi(p.substr(space_ + 1));
                Point t_point{i1, i2};
                element->translate(t_point);
            } 
    }


    void readGroup(XMLElement *child, vector<SVGElement*> &shapes) {
            const char* element_name = child->Name();

            string transform_attr;
            string transform_origin_attr;
            Point transform_origin{0, 0}; 
            bool istransform=false;

            // read transform attribute
            for (const XMLAttribute* attr = child->FirstAttribute(); attr != nullptr; attr = attr->Next())
            {
                if (strcmp(attr->Name(), "transform") == 0) {
                    transform_attr = attr->Value();
                    istransform=true;

                }
                if (strcmp(attr->Name(), "transform-origin") == 0)
                {
                    transform_origin_attr = attr->Value();
                    size_t pos = transform_origin_attr.find_first_of(" ");

                    if (pos != string::npos)
                    {
            
                        int x = stoi(transform_origin_attr.substr(0, pos));
                        int y = stoi(transform_origin_attr.substr(pos + 1));
            
                        transform_origin.x = x;
                        transform_origin.y = y;
                    }
                }
            }
        


            if (strcmp(element_name, "g") == 0) {

                vector<SVGElement*> group_e;

                for (XMLElement *group_child = child->FirstChildElement(); group_child != nullptr; group_child = group_child->NextSiblingElement()) {
                        readGroup(group_child, group_e);
                }

               
                Group* g = new Group(group_e);

                if (istransform) {
                    applyTransformation(g, transform_attr, transform_origin);
                }

        
                shapes.push_back(g);

                for (auto element : group_e) {
                    delete element;
                }

            }    
            
            else if (strcmp(element_name, "ellipse") == 0)
            {
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int rx = child->IntAttribute("rx");
                int ry = child->IntAttribute("ry");
                string fill_color = child->Attribute("fill");

                Ellipse* e= new Ellipse(parse_color(fill_color), {cx, cy}, {rx, ry});
                
                if (istransform) {
                    applyTransformation(e, transform_attr, transform_origin);
                }

                shapes.push_back(e);
            }

            else if (strcmp(element_name, "circle") == 0)
            {
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int r = child->IntAttribute("r");
                string fill_color = child->Attribute("fill");
                
                Circle* e= new Circle(parse_color(fill_color), {cx, cy}, r);

                if (istransform) {
                    applyTransformation(e, transform_attr, transform_origin);
                }

                shapes.push_back(e);
            }

            else if (strcmp(element_name, "polyline") == 0)
            {
                string points = child->Attribute("points");
                string stroke_color = child->Attribute("stroke");
                
                vector<Point> points_vec;
                stringstream ss(points);
                string point;
                while (getline(ss, point, ' '))
                {
                    size_t pos = point.find(',');
                    if (pos != string::npos)
                    {
                        int x = stoi(point.substr(0, pos));
                        int y = stoi(point.substr(pos + 1));
                        points_vec.push_back({x,y});
                    }
                }

                Polyline* e= new Polyline(points_vec, parse_color(stroke_color));

                if (istransform) {
                    applyTransformation(e, transform_attr, transform_origin);
                }

                shapes.push_back(e);
            }

            else if (strcmp(element_name, "line") == 0)
            {
                int x1 = child->IntAttribute("x1");
                int y1 = child->IntAttribute("y1");
                int x2 = child->IntAttribute("x2");
                int y2 = child->IntAttribute("y2");
                string stroke_color = child->Attribute("stroke");
                
                Line* e= new Line(x1,y1,x2,y2,parse_color(stroke_color));

                if (istransform) {
                    applyTransformation(e, transform_attr, transform_origin);
                }

                shapes.push_back(e);
            }

            else if (strcmp(element_name, "polygon") == 0)
            {
                string points = child->Attribute("points");
                string fill_color = child->Attribute("fill");
                
                vector<Point> points_vec;
                stringstream ss(points);
                string point;
                while (getline(ss, point, ' '))
                {
                    size_t pos = point.find(',');
                    if (pos != string::npos)
                    {
                        int x = stoi(point.substr(0, pos));
                        int y = stoi(point.substr(pos + 1));
                        points_vec.push_back({x,y});
                    }
                }

                Polygon* e = new Polygon(points_vec,parse_color(fill_color));

                if (istransform) {
                    applyTransformation(e, transform_attr, transform_origin);
                }

                shapes.push_back(e);
            }

            else if (strcmp(element_name, "rect") == 0)
            {
                int x = child->IntAttribute("x");
                int y = child->IntAttribute("y");
                string fill_color = child->Attribute("fill");
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                
                Rect* e= new Rect(x, y, parse_color(fill_color), width, height );

                if (istransform) {
                    applyTransformation(e, transform_attr, transform_origin);
                }

                shapes.push_back(e);
            }
    }



    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        

        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            readGroup(child, svg_elements);
            
        }
    
    }

}