#ifndef LINEAR_TRIANGLE_HPP
#define LINEAR_TRIANGLE_HPP

#include "element.hpp"

class LinearTriangleElement: public Element
{
public:
    LinearTriangleElement(Node &_node_1, Node &_node_2, Node &_node_3);
    virtual ~LinearTriangleElement();

    virtual const Node& getNode(int local_id) const;
    virtual Node& getNode(int local_id);

    virtual double getSquare() const;

    virtual std::vector<Eigen::Triplet<double>> calculateStiffnessMatrix(const Eigen::Matrix3d& D) const;

    virtual std::vector<double> calculateStress(const Eigen::VectorX<double> & displacements, const Eigen::Matrix3d& D) const;

    /// @brief Updates B matrix
    /// @details The B matrix is updated right after element construction. Call this method if grid was deformed
    void updateB();

private:
    Node &node_1;
    Node &node_2;
    Node &node_3;
    std::vector<int> nodes;
    Eigen::Matrix<double, 3, 6> B; 
};

#endif /* LINEAR_TRIANGLE_HPP */
