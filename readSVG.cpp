#include <iostream>
#include <sstream>
#include <algorithm>
#include "SVGElements.hpp"
#include <unordered_map>
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{

/**
 * @brief Applies a transformation to an SVG element based on the provided transformation attribute and origin.
 *
 * @param element Pointer to the SVGElement to which the transformation will be applied.
 * @param transform_attr The transformation attribute in string format (e.g., "rotate(45)").
 * @param transform_origin The origin point around which the transformation is applied.
 */
void applyTransformation(SVGElement* element, const string& transform_attr, Point& transform_origin)
{
    // Find the positions of the parentheses to extract the transformation type and parameters
    size_t open_p = transform_attr.find('(');
    size_t close_p = transform_attr.find(')');

    // Extract the transformation type (e.g., "rotate", "scale", "translate")
    string f_type = transform_attr.substr(0, open_p);

    // Extract the parameters between the parentheses
    string p = transform_attr.substr(open_p + 1, close_p - open_p - 1);

    // Apply the appropriate transformation based on the type
    if (f_type == "rotate")
    {
        // Convert the parameter to an integer and apply the rotate transformation
        element->rotate(stoi(p), transform_origin);
    }
    else if (f_type == "scale")
    {
        // Convert the parameter to an integer and apply the scale transformation
        element->scale(stoi(p), transform_origin);
    }
    else if (f_type == "translate")
    {
        // Replace commas with spaces in the parameters
        replace(p.begin(), p.end(), ',', ' ');

        // Count the number of spaces in the parameters to handle multiple spaces
        int space_counter = 0;
        for (char c : p) {
            if (c == ' ') space_counter++;
        }
        
        // If there are more than one space, remove the extra spaces
        bool first_space = true;
        if (space_counter > 1) {
            for (size_t i = 0; i < p.size(); i++) {
                if (p[i] == ' ') {
                    if (first_space)
                        first_space = false;
                    else p.erase(i, 1);
                }
            }
        }

        // Split the parameters into two integers (x and y translation values)
        size_t space_ = p.find(' ');
        int i1 = stoi(p.substr(0, space_));
        int i2 = stoi(p.substr(space_ + 1));

        // Create a translation point and apply the translate transformation
        Point t_point{i1, i2};
        element->translate(t_point);
    } 
}

/**
 * @brief Reads an SVG group element and processes its child elements, applying transformations if necessary.
 *
 * @param child Pointer to the XML element representing the group.
 * @param shapes Vector of pointers to SVGElement objects where the parsed elements will be stored.
 * @param id_map Unordered map of string to SVGElement pointers, used for resolving references.
 */
void readGroup(XMLElement *child, vector<SVGElement*> &shapes, unordered_map<string, SVGElement*> &id_map) {
    const char* element_name = child->Name(); // Get the name of the current XML element

    string transform_attr; // To store the transformation attribute value
    string transform_origin_attr; // To store the transform-origin attribute value
    Point transform_origin{0, 0}; // Initialize the transformation origin to (0, 0)
    bool istransform = false; // Flag to indicate if a transformation attribute is present

    // Read attributes of the current element
    for (const XMLAttribute* attr = child->FirstAttribute(); attr != nullptr; attr = attr->Next())
    {
        if (strcmp(attr->Name(), "transform") == 0) {
            // If a transform attribute is found, store its value and set the flag
            transform_attr = attr->Value();
            istransform = true;
        }
        if (strcmp(attr->Name(), "transform-origin") == 0)
        {
            // If a transform-origin attribute is found, parse its value
            transform_origin_attr = attr->Value();
            size_t pos = transform_origin_attr.find_first_of(" ");

            if (pos != string::npos)
            {
                int x = stoi(transform_origin_attr.substr(0, pos)); // Extract x-coordinate
                int y = stoi(transform_origin_attr.substr(pos + 1)); // Extract y-coordinate

                transform_origin.x = x; // Set the x-coordinate of the transform origin
                transform_origin.y = y; // Set the y-coordinate of the transform origin
            }
        }
    }

    if (strcmp(element_name, "use") == 0) {
        // Handle <use> elements (which reference other elements by ID)
        const char* href_attr = child->Attribute("href");
        if (href_attr && href_attr[0] == '#') {
            string element_id = href_attr + 1; // Extract the referenced element's ID
            if (id_map.find(element_id) != id_map.end()) {
                // If the referenced element is found in the id_map, clone it
                SVGElement* referenced_element = id_map[element_id];
                SVGElement* cloned_element = referenced_element->clone();

                // Apply transformation if the attribute is present
                if (istransform) {
                    applyTransformation(cloned_element, transform_attr, transform_origin);
                }

                shapes.push_back(cloned_element); // Add the cloned element to the shapes vector
            }
        }
    }

    cout << "SHAPES SIZE --->" << shapes.size() << '\n'; 

    if (strcmp(element_name, "g") == 0) {
        // Handle <g> elements (groups of SVG elements)
        vector<SVGElement*> group_e; // Vector to store the group's child elements

        // Recursively read the group's child elements
        for (XMLElement *group_child = child->FirstChildElement(); group_child != nullptr; group_child = group_child->NextSiblingElement()) {
            readGroup(group_child, group_e, id_map);
        }

        Group* g = new Group(group_e); // Create a new Group from the child elements

        // Apply transformation if the attribute is present
        if (istransform) {
            applyTransformation(g, transform_attr, transform_origin);
        }

        shapes.push_back(g); // Add the group to the shapes vector
    } 

    else if (strcmp(element_name, "ellipse") == 0)
    {
        // Handle <ellipse> elements
        int cx = child->IntAttribute("cx"); // Get the x-coordinate of the center
        int cy = child->IntAttribute("cy"); // Get the y-coordinate of the center
        int rx = child->IntAttribute("rx"); // Get the x-radius
        int ry = child->IntAttribute("ry"); // Get the y-radius
        string fill_color = child->Attribute("fill"); // Get the fill color

        Ellipse* e = new Ellipse(parse_color(fill_color), {cx, cy}, {rx, ry}); // Create a new Ellipse

        // Apply transformation if the attribute is present
        if (istransform) {
            applyTransformation(e, transform_attr, transform_origin);
        }

        shapes.push_back(e); // Add the ellipse to the shapes vector
    }

    else if (strcmp(element_name, "circle") == 0)
    {
        // Handle <circle> elements
        int cx = child->IntAttribute("cx"); // Get the x-coordinate of the center
        int cy = child->IntAttribute("cy"); // Get the y-coordinate of the center
        int r = child->IntAttribute("r"); // Get the radius
        string fill_color = child->Attribute("fill"); // Get the fill color

        Circle* e = new Circle(parse_color(fill_color), {cx, cy}, r); // Create a new Circle

        // Apply transformation if the attribute is present
        if (istransform) {
            applyTransformation(e, transform_attr, transform_origin);
        }

        shapes.push_back(e); // Add the circle to the shapes vector
    }

    else if (strcmp(element_name, "polyline") == 0)
    {
        // Handle <polyline> elements
        string points = child->Attribute("points"); // Get the points attribute
        string stroke_color = child->Attribute("stroke"); // Get the stroke color

        vector<Point> points_vec; // Vector to store the points
        stringstream ss(points); // Create a stringstream from the points string
        string point;

        // Parse the points attribute into individual points
        while (getline(ss, point, ' '))
        {
            size_t pos = point.find(',');
            if (pos != string::npos)
            {
                int x = stoi(point.substr(0, pos)); // Extract x-coordinate
                int y = stoi(point.substr(pos + 1)); // Extract y-coordinate
                points_vec.push_back({x, y}); // Add the point to the vector
            }
        }

        Polyline* e = new Polyline(points_vec, parse_color(stroke_color)); // Create a new Polyline

        // Apply transformation if the attribute is present
        if (istransform) {
            applyTransformation(e, transform_attr, transform_origin);
        }

        shapes.push_back(e); // Add the polyline to the shapes vector
    }

    else if (strcmp(element_name, "line") == 0)
    {
        // Handle <line> elements
        int x1 = child->IntAttribute("x1"); // Get the x-coordinate of the start point
        int y1 = child->IntAttribute("y1"); // Get the y-coordinate of the start point
        int x2 = child->IntAttribute("x2"); // Get the x-coordinate of the end point
        int y2 = child->IntAttribute("y2"); // Get the y-coordinate of the end point
        string stroke_color = child->Attribute("stroke"); // Get the stroke color

        Line* e = new Line(x1, y1, x2, y2, parse_color(stroke_color)); // Create a new Line

        // Apply transformation if the attribute is present
        if (istransform) {
            applyTransformation(e, transform_attr, transform_origin);
        }

        shapes.push_back(e); // Add the line to the shapes vector
    }

    else if (strcmp(element_name, "polygon") == 0)
    {
        // Handle <polygon> elements
        string points = child->Attribute("points"); // Get the points attribute
        string fill_color = child->Attribute("fill"); // Get the fill color

        vector<Point> points_vec; // Vector to store the points
        stringstream ss(points); // Create a stringstream from the points string
        string point;

        // Parse the points attribute into individual points
        while (getline(ss, point, ' '))
        {
            size_t pos = point.find(',');
            if (pos != string::npos)
            {
                int x = stoi(point.substr(0, pos)); // Extract x-coordinate
                int y = stoi(point.substr(pos + 1)); // Extract y-coordinate
                points_vec.push_back({x, y}); // Add the point to the vector
            }
        }

        Polygon* e = new Polygon(points_vec, parse_color(fill_color)); // Create a new Polygon

        // Apply transformation if the attribute is present
        if (istransform) {
            applyTransformation(e, transform_attr, transform_origin);
        }

        shapes.push_back(e); // Add the polygon to the shapes vector
    }

    else if (strcmp(element_name, "rect") == 0)
    {
        // Handle <rect> elements
        int x = child->IntAttribute("x"); // Get the x-coordinate of the rectangle
        int y = child->IntAttribute("y"); // Get the y-coordinate of the rectangle
        string fill_color = child->Attribute("fill"); // Get the fill color
        int width = child->IntAttribute("width"); // Get the width of the rectangle
        int height = child->IntAttribute("height"); // Get the height of the rectangle

        Rect* e = new Rect(x, y, parse_color(fill_color), width, height); // Create a new Rect

        // Apply transformation if the attribute is present
        if (istransform) {
            applyTransformation(e, transform_attr, transform_origin);
        }

        shapes.push_back(e); // Add the rectangle to the shapes vector
    }
}


/**
 * @brief Reads an SVG file and extracts its dimensions and elements.
 *
 * This function loads an SVG file, reads its dimensions (width and height),
 * and extracts its elements into a vector. It also keeps track of elements by their IDs.
 *
 * @param svg_file The path to the SVG file to be read.
 * @param dimensions A Point object to store the dimensions (width and height) of the SVG.
 * @param svg_elements A vector to store pointers to the extracted SVGElement objects.
 *
 */
void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
{
    XMLDocument doc;
    XMLError r = doc.LoadFile(svg_file.c_str());

    if (r != XML_SUCCESS)
    {
        throw runtime_error("Unable to load " + svg_file);
    }

    XMLElement *xml_elem = doc.RootElement();

    // Read the dimensions of the SVG from the "width" and "height" attributes of the root element
    dimensions.x = xml_elem->IntAttribute("width");
    dimensions.y = xml_elem->IntAttribute("height");

    // Create a map to store SVG elements by their IDs
    unordered_map<string, SVGElement*> id_map; 

    // Iterate over all child elements of the root element
    for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        // Read the group of SVG elements
        readGroup(child, svg_elements, id_map);
        
        // Get the "id" attribute of the child element
        const char* element_id = child->Attribute("id");
        
        // If the "id" attribute is present
        if (element_id) {
            // Add the element to the ID map using the ID as the key and the last element added to the svg_elements list as the value
            id_map[string(element_id)] = svg_elements.back();
        }
    }
}



}