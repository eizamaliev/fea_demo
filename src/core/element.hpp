#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <vector>

#include <Eigen/Dense>
#include <Eigen/Sparse>

struct Node
{
    Node(double _x, double _y, int _id) : x(_x), y(_y), id(_id) {}

    double x;
    double y;
    int id;
};


class Element
{
public:
    virtual ~Element() {};

    virtual const Node& getNode(int local_id) const = 0;
    virtual Node& getNode(int local_id) = 0;

    virtual double getSquare() const = 0;

    virtual std::vector<Eigen::Triplet<double>> calculateStiffnessMatrix(const Eigen::Matrix3d & D) const = 0;

    virtual std::vector<double> calculateStress(const Eigen::VectorX<double> & displacements, const Eigen::Matrix3d& D) const = 0;

    virtual void updateB() = 0;
};

#endif /* ELEMENT_HPP */