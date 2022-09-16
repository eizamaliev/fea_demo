#include "linearTriangle.hpp"

#include <cmath>
#include <iostream>

#include <Eigen/Dense>
#include <Eigen/Sparse>

LinearTriangleElement::LinearTriangleElement(Node &_node_1, Node &_node_2, Node &_node_3)
    : node_1(_node_1), node_2(_node_2), node_3(_node_3)
{
    nodes.push_back(node_1.id);
    nodes.push_back(node_2.id);
    nodes.push_back(node_3.id);

    updateB();
}

LinearTriangleElement::~LinearTriangleElement(){};

Node &LinearTriangleElement::getNode(int local_id)
{
    switch (local_id)
    {
    case 0:
        return node_1;
    case 1:
        return node_2;
    case 2:
        return node_3;
    default:
        throw "No node with such id";
    }
}

const Node &LinearTriangleElement::getNode(int local_id) const
{
    switch (local_id)
    {
    case 0:
        return node_1;
    case 1:
        return node_2;
    case 2:
        return node_3;
    default:
        throw "No node with such id";
    }
}


double LinearTriangleElement::getSquare() const
{
    double det = node_2.x * node_3.y + node_3.x * node_1.y + node_1.x * node_2.y - node_2.x * node_1.y - node_3.x * node_2.y - node_1.x * node_3.y;
    return 0.5 * std::abs(det);
}

std::vector<Eigen::Triplet<double>> LinearTriangleElement::calculateStiffnessMatrix(const Eigen::Matrix3d &D) const
{
    Eigen::Matrix<double, 6, 6> K = B.transpose() * D * B * getSquare();

    std::vector<Eigen::Triplet<double>> triplets;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Eigen::Triplet<double> trplt11(2 * nodes[i] + 0, 2 * nodes[j] + 0, K(2 * i + 0, 2 * j + 0));
            Eigen::Triplet<double> trplt12(2 * nodes[i] + 0, 2 * nodes[j] + 1, K(2 * i + 0, 2 * j + 1));
            Eigen::Triplet<double> trplt21(2 * nodes[i] + 1, 2 * nodes[j] + 0, K(2 * i + 1, 2 * j + 0));
            Eigen::Triplet<double> trplt22(2 * nodes[i] + 1, 2 * nodes[j] + 1, K(2 * i + 1, 2 * j + 1));

            triplets.push_back(trplt11);
            triplets.push_back(trplt12);
            triplets.push_back(trplt21);
            triplets.push_back(trplt22);
        }
    }

    return triplets;
}

std::vector<double> LinearTriangleElement::calculateStress(const Eigen::VectorX<double> &displacements, const Eigen::Matrix3d &D) const
{
    Eigen::Matrix<double, 6, 1> delta;
    delta << displacements(2 * node_1.id), displacements(2 * node_1.id + 1),
        displacements(2 * node_2.id), displacements(2 * node_2.id + 1),
        displacements(2 * node_3.id), displacements(2 * node_3.id + 1);

    Eigen::Vector3<double> sigma = D * B * delta;

    std::vector<double> result = {sigma(0), sigma(1), sigma(2)};
    return result;
}

void LinearTriangleElement::updateB()
{
    Eigen::Matrix3d C;
    C << 1.0, node_1.x, node_1.y,
         1.0, node_2.x, node_2.y,
         1.0, node_3.x, node_3.y;

    Eigen::Matrix3d IC = C.inverse();

    B << IC(1, 0),  0.0,        IC(1, 1),   0.0,        IC(1, 2),       0.0,
         0.0,       IC(2, 0),   0.0,        IC(2, 1),   0.0,            IC(2, 2),
         IC(2, 0),  IC(1, 0),   IC(2, 1),   IC(1, 1),   IC(2, 2),       IC(1, 2);

}