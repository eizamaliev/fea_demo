#include <gtest/gtest.h>

#include "solver.hpp"


TEST(Solver, LoadFile)
{
    ASSERT_NO_THROW(Solver("data/mesh_simple.k"));
};

TEST(Solver, TestInitialMatrix)
{
    Solver solver("data/mesh_simple.k");

    Eigen::MatrixX<double> matrix(solver.getMatrix()); //making dense matrix for element access

    ASSERT_EQ(matrix.cols(), 8);
    ASSERT_EQ(matrix.rows(), 8);

    for (int i=0; i<8; ++i)
        for (int j=0; j<8; ++j)
            EXPECT_DOUBLE_EQ(matrix(i, j), 0.0);
}


TEST(Solver, TestInitialVector)
{
    Solver solver("data/mesh_simple.k");

    Eigen::VectorX<double> vector = solver.getLoadVector();

    ASSERT_EQ(vector.cols(), 1);
    ASSERT_EQ(vector.rows(), 8);

    for (int i=0; i<8; ++i)
        EXPECT_NEAR(vector(i), 0.0, 1.e-15);           
}

TEST(Solver, FillMatrix)
{
    Solver solver("data/mesh_simple.k");

    solver.calcuateStiffnessMatrix();

}

TEST(Solver, CheckFAfterLoad)
{
    Solver solver("data/mesh_simple.k");

    solver.applyLoad();

    Eigen::VectorX<double> vector = solver.getLoadVector();

    EXPECT_DOUBLE_EQ(vector(0), 0);
    EXPECT_DOUBLE_EQ(vector(1), 0);

    EXPECT_DOUBLE_EQ(vector(2), 300.0);
    EXPECT_DOUBLE_EQ(vector(3), 0);

    EXPECT_DOUBLE_EQ(vector(4), 0);
    EXPECT_DOUBLE_EQ(vector(5), 0);

    EXPECT_DOUBLE_EQ(vector(6), 300.0);
    EXPECT_DOUBLE_EQ(vector(7), 0);
}

TEST(Solver, CheckMatrixAfterLoad)
{
    Solver solver("data/mesh_simple.k");


    solver.calcuateStiffnessMatrix();
    solver.applyLoad();

    Eigen::MatrixX<double> matrix(solver.getMatrix()); //making dense matrix for element access

    EXPECT_DOUBLE_EQ(matrix(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(matrix(1, 1), 1.0);

    EXPECT_DOUBLE_EQ(matrix(3, 3), 1.0);
    EXPECT_DOUBLE_EQ(matrix(4, 4), 1.0);

    // for (int i=0; i<8; ++i)
    //     for (int j=0; j<8; ++j)
    //         EXPECT_DOUBLE_EQ(matrix(i, j), i == j ? 1.0 : 0.0);
}

TEST(Solver, Solve)
{
    Solver solver("data/mesh_simple.k");


    solver.calcuateStiffnessMatrix();
    solver.applyLoad();

    solver.solve();
    auto result = solver.getDisplacements();

    EXPECT_DOUBLE_EQ(result(0), 0.0);
    EXPECT_DOUBLE_EQ(result(4), 0.0);
}

TEST(SolverCoarse, CheckFAfterLoad)
{
    Solver solver("data/mesh_coarse.k");

    solver.applyLoad();

    Eigen::VectorX<double> vector = solver.getLoadVector();

    EXPECT_DOUBLE_EQ(vector(0), 30093.65);
    EXPECT_DOUBLE_EQ(vector(18), 61547.0);
    EXPECT_DOUBLE_EQ(vector(20), 63179.849999999995);
    EXPECT_DOUBLE_EQ(vector(22), 63453.00000000001);
    EXPECT_DOUBLE_EQ(vector(4), 31726.500000000005);
}
