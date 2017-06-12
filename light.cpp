#include "light.h"

Light::Light(){
   position = QVector4D(0,20,0, 1.0);
   ambient = QVector4D(0.2,0.2, 0.2, 1.0);
   diffuse = QVector4D(0.4, 0.4, 0.4, 1.0);
   specular = QVector4D(0.4, 0.4, 0.4, 1.0);

}
