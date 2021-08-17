#pragma once

#include"Config.h"

#include<cmath>
#include<cstdint>

#include<Eigen/Eigen>

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

const double pi = acos(-1.0);

const double eps = 1e-6;

typedef Eigen::Vector3d Direction;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;

const Color colorGray(105 / 255.999, 105 / 255.999, 105 / 255.999);
const Color colorGreen(32 / 255.999, 127 / 255.999, 76 / 255.999);