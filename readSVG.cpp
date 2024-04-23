
#include <iostream>
#include <sstream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();
        vector<SVGElement *> shapes;

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        // TODO complete code -->

        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            const char* element_name = child->Name();
            

            string transform_attr;
            string transform_origin_attr;
            Point transform_origin{0, 0}; // Default transformation origin
            
            for (const XMLAttribute* attr = child->FirstAttribute(); attr != nullptr; attr = attr->Next())
            {
                if (strcmp(attr->Name(), "transform") == 0) {
                    transform_attr = attr->Value();
                }
                else if (strcmp(attr->Name(), "transform-origin") == 0)
                {
                    transform_origin_attr = attr->Value();
                    istringstream iss(transform_origin_attr);
                    iss >> transform_origin.x >> transform_origin.y;
                }
            }


            if (strcmp(element_name, "ellipse") == 0)
            {
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int rx = child->IntAttribute("rx");
                int ry = child->IntAttribute("ry");
                string fill_color = child->Attribute("fill");
                
                shapes.push_back(new Ellipse(parse_color(fill_color), {cx, cy}, {rx, ry}));
            }

            else if (strcmp(element_name, "circle") == 0)
            {
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int r = child->IntAttribute("r");
                string fill_color = child->Attribute("fill");
                
                shapes.push_back(new Circle(parse_color(fill_color), {cx, cy}, r));
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
                
                shapes.push_back(new Polyline(points_vec, parse_color(stroke_color)));
            }

            else if (strcmp(element_name, "line") == 0)
            {
                int x1 = child->IntAttribute("x1");
                int y1 = child->IntAttribute("y1");
                int x2 = child->IntAttribute("x2");
                int y2 = child->IntAttribute("y2");
                string stroke_color = child->Attribute("stroke");
                
                shapes.push_back(new Line(x1,y1,x2,y2,parse_color(stroke_color)));
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

                shapes.push_back(new Polygon(points_vec,parse_color(fill_color)));
            }

            else if (strcmp(element_name, "rect") == 0)
            {
                int x = child->IntAttribute("x");
                int y = child->IntAttribute("y");
                string fill_color = child->Attribute("fill");
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                
                
                shapes.push_back(new Rect(x, y, parse_color(fill_color), width, height ));
            } 
            
        }
        svg_elements=shapes;
    }
}