#include "geometry.hpp"

#include <fstream>
#include <string>
#include <sstream>

#include <iostream>

#include "element.hpp"
#include "linearTriangle.hpp"

using namespace std;

Geometry::Geometry() : shift(INT_MAX) {};

void Geometry::loadFromFile(const string &filename)
{
    ifstream input;
    input.open(filename);

    if (!input.is_open())
        throw "File not found";
    
    bool isParsingNodes=false;
    bool isParsingElements=false;
    string line;
    while (getline(input, line))
    {
        if (line.find("*NODE") == 0)
        {
            isParsingNodes = true;
            isParsingElements = false;
            continue;
        }
        
        if (line.find("*ELEMENT_SHELL") == 0)
        {
            
            isParsingElements = true;
            isParsingNodes = false;
            applyNodesShift();
            continue;
        }
        if (line[0] == '$')
            continue;

        istringstream input_line(move(line));

        if (isParsingNodes)
        {
            int id;
            double x,y;

            if (! (input_line >> id >> x >> y))
            {
                isParsingNodes = false;
                applyNodesShift();
                continue;
            }
            if (shift > id)
                shift = id;
            nodes.push_back(Node(x, y, id));
            continue;
        }

        if (isParsingElements)
        {
            int id;
            int pid;
            if (!(input_line >> id >> pid))
            {
                isParsingElements = false;
                continue;
            }

            // pid file should represent element type and used for element factory. Not supported by *.k format
            if (pid == pid)
            {
                
                int i, j, k;
                input_line >> i >> j >> k;
                elements.push_back(new LinearTriangleElement(getNode(i), getNode(j), getNode(k)));
            }

        }
    }

    createBoundaries();
};


Node& Geometry::getNode(int id)
{
    for (int i=0; i<nodes.size(); ++i)
        if (nodes[i].id == id - shift)
            return nodes[i];
    throw "No found found";
}




void Geometry::createBoundaries()
{
    boundaries.resize(3);
    for (int i=0; i<nodes.size(); ++i)
    {
        if (nodes[i].x < 1.e-10)
        {
            boundaries[0].nodes.push_back(BoundaryNode(BoundaryNode::UX, nodes[i].id));
        }

        if (nodes[i].y < 1.e-10)
        {
            boundaries[1].nodes.push_back(BoundaryNode(BoundaryNode::UY, nodes[i].id));
        }

        if (nodes[i].x > 0.15 - 1.e-10)
        {
            boundaries[2].nodes.push_back(BoundaryNode(BoundaryNode::F, nodes[i].id));
        }
    }
}

void Geometry::applyNodesShift() 
{
    for (auto &node: nodes)
        node.id -= shift;
}
