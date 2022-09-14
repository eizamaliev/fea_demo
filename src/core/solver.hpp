#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <Eigen/Sparse>

#include "geometry.hpp"

class Solver
{
public:
    Solver();
    Solver(double _poissonRatio, double youngModulus);
    Solver(const std::string & filename);
    Solver(const std::string & filename, double _poissonRatio, double youngModulus);

    /// @brief Loads geometry
    /// @details Loads geometry from file and prepares matrix and vector, e.g. resizes, initialises
    /// @param filename file with mesh
    void loadGeometry(const std::string & filename);

    /// @brief Solves the equations
    void solve();

    /// @brief Calculates striffness matrix
    void calcuateStiffnessMatrix();
    /// @brief Applies loads
    /// @details Applies external forces and boundary contitions to vector and matrix.
    ///          External force values goes to load vector, boundary condition goes both to
    ///          vector, nullifying it, and matrix, nullifying all element in row and column except diagonal.
    /// @todo should split into two methods
    void applyLoad();

    /// @brief Save results
    /// @{
    void save(const std::string &filename);
    void saveSigma(const std::string & filename);
    /// @}

    /// @brief Calculates stress
    /// @return stresses for each element
    std::vector<std::vector<double>> calculateStress();

    /// @brief Getter for global matrix
    /// @return global sparse matrix
    const Eigen::SparseMatrix<double>& getMatrix() { return globalK; };
    /// @brief Getter for load vector
    /// @return global load vector
    const Eigen::VectorX<double>& getLoadVector() { return F; };
    const Eigen::VectorX<double>& getDisplacements() { return displacements; };
protected:
    // void calculateStress();
    
private:
    Geometry geometry;
    Eigen::SparseMatrix<double> globalK; ///< stiffness matrix
    Eigen::VectorX<double> F; ///< load vector
    Eigen::VectorX<double> displacements; ///< results, available only after succesful Solver::solve call

    double poissonRatio; ///< Poisson ratio (should be element-specific in common case)
    double youngModulus; ///< Young modulus (should be element-specific in common case)
};

#endif /* SOLVER_HPP */
