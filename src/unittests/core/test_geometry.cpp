#include <gtest/gtest.h>

#include "geometry.hpp"


TEST(Geometry, LoadAbsentFile)
{
    Geometry geometry;

    EXPECT_ANY_THROW(geometry.loadFromFile("data/mesh_not_exist.k"));
}

TEST(Geometry, LoadCorrectFile)
{
    Geometry geometry;

    EXPECT_NO_THROW(geometry.loadFromFile("data/mesh_simple.k"));
}

TEST(Geometry, LoadNodes)
{
    Geometry geometry;
    geometry.loadFromFile("data/mesh_simple.k");

    auto nodes = geometry.getNodes();

    ASSERT_EQ(nodes.size(), 4);

    EXPECT_EQ(nodes[0].id, 0);
    EXPECT_EQ(nodes[1].id, 1);
    EXPECT_EQ(nodes[2].id, 3);
    EXPECT_EQ(nodes[3].id, 2);
}


TEST(Geometry, LoadElements)
{
    Geometry geometry;
    geometry.loadFromFile("data/mesh_simple.k");

    auto elements = geometry.getElements();

    ASSERT_EQ(elements.size(), 2);

    EXPECT_DOUBLE_EQ(elements[0]->getSquare(), 0.045);
    EXPECT_DOUBLE_EQ(elements[1]->getSquare(), 0.045);
}

TEST(Geometry, Boundaries)
{
    Geometry geometry;
    geometry.loadFromFile("data/mesh_simple.k");

    auto boundaries = geometry.getBoundaries();

    ASSERT_EQ(boundaries.size(), 3);

    auto leftBoundary = boundaries[0];

    EXPECT_EQ(leftBoundary.nodes.size(), 2);
}



TEST(GeometryCoarseMesh, LoadMesh)
{
    EXPECT_NO_THROW(Geometry().loadFromFile("data/mesh_coarse.k"));
}

TEST(GeometryCoarseMesh, CheckNodesCount)
{
    Geometry geometry;
    geometry.loadFromFile("data/mesh_coarse.k");

    auto nodes = geometry.getNodes();

    EXPECT_EQ(nodes.size(), 28);
}

TEST(GeometryCoarseMesh, CheckElementsCount)
{
    Geometry geometry;
    geometry.loadFromFile("data/mesh_coarse.k");

    auto elements = geometry.getElements();

    EXPECT_EQ(elements.size(), 38);
}

TEST(GeometryCoarseMesh, CheckSortedBoundaries)
{
    Geometry geometry;
    geometry.loadFromFile("data/mesh_coarse.k");

    auto boundary = geometry.getBoundaries()[2].nodes;

    EXPECT_EQ(boundary[0].node + geometry.getShift(), 1);
    EXPECT_EQ(boundary[1].node + geometry.getShift(), 10);
    EXPECT_EQ(boundary[2].node + geometry.getShift(), 11);
    EXPECT_EQ(boundary[3].node + geometry.getShift(), 12);
    EXPECT_EQ(boundary[4].node + geometry.getShift(), 3);
}

TEST(GeometryHomoMesh, LoadMesh)
{
    Geometry geometry;
    ASSERT_NO_THROW(geometry.loadFromFile("data/mesh_homo.k")); 
}

TEST(GeometryHomoMesh, CheckBoundaries)
{
    Geometry geometry;
    geometry.loadFromFile("data/mesh_homo.k");

    auto boundaries = geometry.getBoundaries();

    EXPECT_EQ(boundaries[0].nodes.size(), 12);
    EXPECT_EQ(boundaries[1].nodes.size(), 9);
    EXPECT_EQ(boundaries[2].nodes.size(), 13);
}





    //    1            0.15               0               0       0       0
    //   10            0.15       0.0601873               0       0       0
    //   11            0.15        0.123094               0       0       0
    //   12            0.15        0.186547               0       0       0
    //    3            0.15            0.25               0       0       0