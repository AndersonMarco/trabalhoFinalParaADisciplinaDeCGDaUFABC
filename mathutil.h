#ifndef MATHUTIL_H
#define MATHUTIL_H
#include <QVector3D>

class MathUtil
{
public:
    MathUtil();
    static QVector3D normal(QVector3D vectorA,QVector3D vectorB);
};

#endif // MATHUTIL_H
