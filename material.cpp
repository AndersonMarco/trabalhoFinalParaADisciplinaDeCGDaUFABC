#include "material.h"

Material::Material()
{
    ambient = QVector4D(1, 0.0, 1, 1.0);
    diffuse = QVector4D(0.6, 0.0, 0.6, 1.0);
    specular = QVector4D(1, 1,1, 1.0);
    shininess = 50.0;
}

