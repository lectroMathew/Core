#ifndef CORE_MATRIX_H
#define CORE_MATRIX_H

#include "Essential.h"

namespace core {


template<int cols, int rows, typename T> class Matrix {
public:
    static Matrix3 Translate(const Vector3 &v);
    static Matrix4 Translate(const Vector4 &v);

protected:
    T mtx[cols][rows];
};

template<> class Matrix<3, 3, float> {
public:
    Matrix(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22 )
    {
        mtx[0][0] = m00; mtx[0][1] = m01; mtx[0][2] = m02;
        mtx[1][0] = m10; mtx[1][1] = m11; mtx[1][2] = m12;
        mtx[2][0] = m20; mtx[2][1] = m21; mtx[2][2] = m22;
    }

    static const Matrix3 zero;
    static const Matrix3 identity;
protected:
    float mtx[3][3];
};

template<> class Matrix<4, 4, float> {
public:
    Matrix(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33 )
    {
        mtx[0][0] = m00; mtx[0][1] = m01; mtx[0][2] = m02; mtx[0][3] = m03;
        mtx[1][0] = m10; mtx[1][1] = m11; mtx[1][2] = m12; mtx[1][3] = m13;
        mtx[2][0] = m20; mtx[2][1] = m21; mtx[2][2] = m22; mtx[2][3] = m23;
        mtx[3][0] = m30; mtx[3][1] = m31; mtx[3][2] = m32; mtx[3][3] = m33;
    }

    static const Matrix4 zero;
    static const Matrix4 identity;
protected:
    float mtx[4][4];
};


} // namespace core

#endif //CORE_MATRIX_H
