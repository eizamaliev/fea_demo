#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "element.hpp"

struct BoundaryNode
{
    enum Type
    {
        UX,
        UY,
        UXY,
        F
    };

    BoundaryNode(Type _type, int _node) : type(_type), node(_node) {}

    Type type;
    int node;
};

struct Boundary
{
    std::vector<BoundaryNode> nodes;
};

class Geometry
{
public:
    Geometry();

    void loadFromFile(const std::string &filename);

    /// @name Getters
    /// @{ 
    std::vector<Element*>& getElements() { return elements; }
    std::vector<Node>& getNodes() { return nodes; }
    std::vector<Boundary>& getBoundaries() { return boundaries; }
    int getShift() { return shift; }
    /// @}

    Node& getNode(int id);
protected:
    /// @brief Create boundaries
    /// @details BICYCLE
    void createBoundaries();

    /// @details Fixes meshes, which enumerate nodes not from zero (Hello, my FortRan friend)
    void applyNodesShift();
private:
    std::vector<Node> nodes;
    std::vector<Element*> elements;
    std::vector<Boundary> boundaries;

    int shift;
};

#endif /* GEOMETRY_HPP */
