#include "matrix.h"

#include <gtest/gtest.h>

using namespace std;

TEST(Matrix1, Size_EmptyMatrix) {
    Matrix<int, 1, -1> m;

    EXPECT_EQ(m.size(), 0);
}

TEST(Matrix1, GetElement_EmptyMatrix) {
    Matrix<int, 1, -1> m;

    EXPECT_EQ(m[0], -1);
    EXPECT_EQ(m[100], -1);
}

TEST(Matrix1, SetElement_NewElement) {
    Matrix<int, 1, -1> m;

    m[100] = 7;

    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[100], 7);
}

TEST(Matrix1, SetElement_NewDefaultElement) {
    Matrix<int, 1, -1> m;

    m[100] = -1;

    EXPECT_EQ(m.size(), 0);
    EXPECT_EQ(m[100], -1);
}

TEST(Matrix1, SetElement_ResetDefaultElement) {
    Matrix<int, 1, -1> m;
    m[100] = 7;

    m[100] = -1;

    EXPECT_EQ(m.size(), 0);
    EXPECT_EQ(m[100], -1);
}

TEST(Matrix1, SetElement_Chain) {
    Matrix<int, 1, -1> m;

    ((m[100] = 314) = 0) = 217;

    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[100], 217);
}

TEST(Matrix1, Iterate) {
    Matrix<int, 1, -1> m;

    m[0] = 0;
    m[1] = 1;

    for (const auto& c : m) {
        unsigned int x = c.first[0];
        int v = c.second;

        EXPECT_EQ(x, v);
    }
}

TEST(Matrix2, Size_EmptyMatrix) {
    Matrix<int, 2, -1> m;

    EXPECT_EQ(m.size(), 0);
}

TEST(Matrix2, GetElement_EmptyMatrix) {
    Matrix<int, 2, -1> m;

    EXPECT_EQ(m[0][0], -1);
    EXPECT_EQ(m[100][500], -1);
}

TEST(Matrix2, SetElement_NewElement) {
    Matrix<int, 2, -1> m;

    m[100][500] = 7;

    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[100][500], 7);
}

TEST(Matrix2, SetElement_NewDefaultElement) {
    Matrix<int, 2, -1> m;

    m[100][500] = -1;

    EXPECT_EQ(m.size(), 0);
    EXPECT_EQ(m[100][500], -1);
}

TEST(Matrix2, SetElement_ResetDefaultElement) {
    Matrix<int, 2, -1> m;
    m[100][500] = 7;

    m[100][500] = -1;

    EXPECT_EQ(m.size(), 0);
    EXPECT_EQ(m[100][500], -1);
}

TEST(Matrix2, SetElement_Chain) {
    Matrix<int, 2, -1> m;

    ((m[100][500] = 314) = 0) = 217;

    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[100][500], 217);
}

TEST(Matrix2, Iterate) {
    Matrix<int, 2, -1> m;

    m[0][0] = 0;
    m[0][1] = 1;
    m[1][0] = 1;
    m[1][1] = 2;

    for (const auto& c : m) {
        unsigned int x = c.first[0];
        unsigned int y = c.first[1];
        int v = c.second;

        EXPECT_EQ(x + y, v);
    }
}

TEST(Matrix3, Size_EmptyMatrix) {
    Matrix<int, 3, -1> m;

    EXPECT_EQ(m.size(), 0);
}

TEST(Matrix3, GetElement_EmptyMatrix) {
    Matrix<int, 3, -1> m;

    EXPECT_EQ(m[0][0][0], -1);
    EXPECT_EQ(m[100][500][100], -1);
}

TEST(Matrix3, SetElement_NewElement) {
    Matrix<int, 3, -1> m;

    m[100][500][100] = 7;

    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[100][500][100], 7);
}

TEST(Matrix3, SetElement_NewDefaultElement) {
    Matrix<int, 3, -1> m;

    m[100][500][100] = -1;

    EXPECT_EQ(m.size(), 0);
    EXPECT_EQ(m[100][500][100], -1);
}

TEST(Matrix3, SetElement_ResetDefaultElement) {
    Matrix<int, 3, -1> m;
    m[100][500][100] = 7;

    m[100][500][100] = -1;

    EXPECT_EQ(m.size(), 0);
    EXPECT_EQ(m[100][500][100], -1);
}

TEST(Matrix3, SetElement_Chain) {
    Matrix<int, 3, -1> m;

    ((m[100][500][100] = 314) = 0) = 217;

    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[100][500][100], 217);
}

TEST(Matrix3, Iterate) {
    Matrix<int, 3, -1> m;

    m[0][0][0] = 0;
    m[0][1][0] = 1;
    m[1][0][0] = 1;
    m[1][1][0] = 2;

    for (const auto& c : m) {
        unsigned int x = c.first[0];
        unsigned int y = c.first[1];
        unsigned int z = c.first[2];
        int v = c.second;

        EXPECT_EQ(x + y, v);
    }
}
