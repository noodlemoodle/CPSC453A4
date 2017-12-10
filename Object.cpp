#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

#define _USE_MATH_DEFINES
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include "Object.h"

void Object::setMaterial(Material * mat){
    m = mat;
}

Triangle::Triangle(Point v0,Point v1, Point v2, Point no){
    p0 = v0;
    p1 = v1;
    p2 = v2;

    n = no;
    isLight = false;
}

Sphere::Sphere(Point p, double r){
    center = p;
    radius = r;

    isLight = false;
}
#define EPS 1E-6

Point Triangle::getIntersection(Ray r){
	// YOUR INTERSECTION CODE HERE.
	// RETURN THE POINT OF INTERSECTION FOR THIS TRIANGLE.
     double detm = glm::determinant(glm::mat3(glm::vec3((p1 - p0).x, (p1 - p0).y, (p1 - p0).z), glm::vec3((p2 - p0).x, (p2 - p0).y, (p2 - p0).z), glm::vec3(-(r.v).x, -(r.v).y, -(r.v).z)));
     double s_a1 = (1/(detm))*(glm::determinant(glm::mat3(glm::vec3((r.p - p0).x, (r.p - p0).y, (r.p - p0).z), glm::vec3((p2 - p0).x, (p2 - p0).y, (p2 - p0).z), glm::vec3(-(r.v).x, -(r.v).y, -(r.v).z))));
     double s_a2 = (1/(detm))*(glm::determinant(glm::mat3(glm::vec3((p1 - p0).x, (p1 - p0).y, (p1 - p0).z), glm::vec3((r.p - p0).x, (r.p - p0).y, (r.p - p0).z), glm::vec3(-(r.v).x, -(r.v).y, -(r.v).z))));
     double s_t = (1/(detm))*(glm::determinant(glm::mat3(glm::vec3((p1 - p0).x, (p1 - p0).y, (p1 - p0).z), glm::vec3((p2 - p0).x, (p2 - p0).y, (p2 - p0).z), glm::vec3((r.p - p0).x, (r.p - p0).y, (r.p - p0).z))));

     return (s_a1 > 0 && s_a2 > 0 && s_a1 + s_a2 < 1) ? Point((r.p + r.v*s_t).x, (r.p + r.v*s_t).y, (r.p + r.v*s_t).z) : Point::Infinite();
}

Point Triangle::getNormal(Point p){
    Point one = p0-p1;
    Point two = p0-p2;
    Point ret = one.cross(two);
    ret.normalize();
    return ret;
}

Point Sphere::getNormal(Point p){
    Point ret = (p-center);

    ret.normalize();
    return ret;
}

Point Sphere::getIntersection(Ray r){

     double s_b = glm::dot(glm::vec3((r.v*2).x, (r.v*2).y, (r.v*2).z), glm::vec3((r.p-center).x, (r.p-center).y, (r.p-center).z));
     double s_c = ((double)glm::length2(glm::vec3((r.p-center).x, (r.p-center).y, (r.p-center).z))) - ((double)pow(radius, 2.0));

     double b24c = pow(s_b, 2.0) - 4*s_c;
     double s_t1 = b24c < 0 ? NULL : (-s_b - sqrt(pow(s_b, 2.0) - 4*s_c))/2.0;
     double s_t2 = b24c < 0 ? NULL : (-s_b + sqrt(pow(s_b, 2.0) - 4*s_c))/2.0;
     double s_t = b24c != 0 ? NULL : -s_b / 2.0;

     // does not intersect
     if(b24c < 0) return Point::Infinite();
     // tangent to sphere
     else if(b24c == 0) return Point((r.p + r.v*s_t).x, (r.p + r.v*s_t).y, (r.p + r.v*s_t).z);
     // intersect 2 points
     else return (s_t1 < s_t2 && s_t1 > 0) ?  Point((r.p + r.v*s_t1).x, (r.p + r.v*s_t1).y, (r.p + r.v*s_t1).z) : Point((r.p + r.v*s_t2).x, (r.p + r.v*s_t2).y, (r.p + r.v*s_t2).z);

}
