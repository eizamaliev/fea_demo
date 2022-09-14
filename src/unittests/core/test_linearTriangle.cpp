#include <gtest/gtest.h>

#include "linearTriangle.hpp"

TEST(LinearElement, Construction)
{
    Node node_1(0.0, 0.0, 1);
    Node node_2(0.0, 1.0, 2);
    Node node_3(1.0, 0.0, 3);

    LinearTriangleElement element(node_1, node_2, node_3);

    EXPECT_DOUBLE_EQ(element.getNode(0).x, node_1.x);
    EXPECT_DOUBLE_EQ(element.getNode(0).y, node_1.y);

    EXPECT_DOUBLE_EQ(element.getNode(1).x, node_2.x);
    EXPECT_DOUBLE_EQ(element.getNode(1).y, node_2.y);

    EXPECT_DOUBLE_EQ(element.getNode(2).x, node_3.x);
    EXPECT_DOUBLE_EQ(element.getNode(2).y, node_3.y);
}

TEST(LinearElement, Changing)
{
    Node node_1(0.0, 0.0, 1);
    Node node_2(0.0, 1.0, 2);
    Node node_3(1.0, 0.0, 3);

    LinearTriangleElement element(node_1, node_2, node_3);

    node_1.x += 0.1;
    node_1.y += 0.1;

    node_2.x += 0.1;
    node_2.y += 0.1;

    node_3.x += 0.1;
    node_3.y += 0.1;



    EXPECT_DOUBLE_EQ(element.getNode(0).x, node_1.x);
    EXPECT_DOUBLE_EQ(element.getNode(0).y, node_1.y);

    EXPECT_DOUBLE_EQ(element.getNode(1).x, node_2.x);
    EXPECT_DOUBLE_EQ(element.getNode(1).y, node_2.y);

    EXPECT_DOUBLE_EQ(element.getNode(2).x, node_3.x);
    EXPECT_DOUBLE_EQ(element.getNode(2).y, node_3.y);
}

TEST(LinearElement, Square)
{
    Node node_1(0.0, 0.0, 1);
    Node node_2(0.0, 1.0, 2);
    Node node_3(1.0, 0.0, 3);

    LinearTriangleElement element(node_1, node_2, node_3);

    EXPECT_DOUBLE_EQ(element.getSquare(), 0.5);
}

TEST(LinearElement, SquareLargerElement)
{
    Node node_1(0.0, 0.0, 1);
    Node node_2(0.0, 1.0, 2);
    Node node_3(2.0, 0.0, 3);

    LinearTriangleElement element(node_1, node_2, node_3);

    EXPECT_DOUBLE_EQ(element.getSquare(), 1.0);
}

TEST(LinearElement, StiffnessMatrix)
{
    Node node_1(0.0, 0.0, 0);
    Node node_2(0.0, 1.0, 1);
    Node node_3(1.0, 0.0, 2);

    LinearTriangleElement element(node_1, node_2, node_3);

    Eigen::Matrix3d D;
    D << 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0,
         0.0, 0.0, 0.0;

    

    auto result = element.calculateStiffnessMatrix(D);

    EXPECT_EQ(result.size(), 36);

    EXPECT_EQ(result[0].row(), 0);
    EXPECT_EQ(result[1].row(), 0);
    EXPECT_EQ(result[2].row(), 1);
    EXPECT_EQ(result[3].row(), 1);

    EXPECT_EQ(result[4].row(), 0);
    EXPECT_EQ(result[5].row(), 0);
    EXPECT_EQ(result[6].row(), 1);
    EXPECT_EQ(result[7].row(), 1);

    EXPECT_EQ(result[0].col(), 0);
    EXPECT_EQ(result[1].col(), 1);
    EXPECT_EQ(result[2].col(), 0);
    EXPECT_EQ(result[3].col(), 1);

    EXPECT_EQ(result[4].col(), 2);
    EXPECT_EQ(result[5].col(), 3);
    EXPECT_EQ(result[6].col(), 2);
    EXPECT_EQ(result[7].col(), 3);

    for (int i = 0; i < result.size(); ++i)
        EXPECT_DOUBLE_EQ(result[i].value(), 0);
}

TEST(LinearElement, Stress)
{
    Node node_1(0.0, 0.0, 0);
    Node node_2(0.0, 1.0, 1);
    Node node_3(1.0, 0.0, 2);

    LinearTriangleElement element(node_1, node_2, node_3);

    Eigen::Matrix3d D;
    D << 1.0, 0.0, 0.0,
         0.0, 1.0, 0.0,
         0.0, 0.0, 1.0;

    Eigen::Vector<double, 6> displacements;
    displacements << 0.0, 0.0, 
                     0.0, 0.1, 
                     0.0, 0.0;


    auto result = element.calculateStress(displacements, D);

    ASSERT_EQ(result.size(), 3);

    EXPECT_DOUBLE_EQ(result[0], 0.0);
    EXPECT_DOUBLE_EQ(result[1], 0.1);
    EXPECT_DOUBLE_EQ(result[2], 0.0);
}