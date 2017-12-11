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
#include "RayTracer.h"

RayTracer::RayTracer(Scene * s, int maxd, int sm){
  scene = s;
  maxdepth = maxd;
  samples = sm;
}

// This function determines the closest object that a ray intersects.
// The getIntersection() method is polymorphic. Please provide implementations
// for all the objects in your scene (see Object.h and Object.cpp).
Object * RayTracer::intersect(Ray r){
  scene->startIteration();
  Object * current = NULL;
  Object * minObject = NULL;
  Point inter;
  //Initialize min to infinite point;
  Point min = Point::Infinite();
  while((current = scene->getNextObject()) != NULL){
    inter = current->getIntersection(r);
    if((inter - r.p).length() > 1E-6){
      if((inter-r.p).length()  < (min-r.p).length()){
        min = inter;
        minObject = current;
      }
    }
  }
  return minObject;
}

// Trace a ray recursively
Color RayTracer::trace(Ray r, int depth){
  Color rad=Color(0.0,0.0,0.0,0.0);

  // YOUR CODE FOR RECURSIVE RAY TRACING GOES HERE
  Object* o = intersect(r);
  if(o!=NULL) {
       rad = rad + Phong(o->getNormal(o->getIntersection(r)), o->getIntersection(r), r, o->getMaterial(), o);
     //   if(((o->getMaterial())->type == REFLECTIVE) && (depth < maxdepth)) {
     //        Point n = o->getNormal(o->getIntersection(r));
     //        Point normal = Point(glm::normalize(glm::vec3(n.x, n.y, n.z)).x, glm::normalize(glm::vec3(n.x, n.y, n.z)).y, glm::normalize(glm::vec3(n.x, n.y, n.z)).z);
     //        return (rad + trace(r.reflect(normal, o->getIntersection(r)), 1));
     //   }
     //   else {
     //        return rad;
     //   }
  }


  return rad;
}

// Local Phong illumination at a point.
Color RayTracer::Phong(Point normal,Point p, Ray r, Material * m, Object * o){

     Color ret = Color(0.0, 0.0, 0.0, 0.0);

     Color a = m->getAmbient(p);

     // YOUR CODE HERE.
     // There is ambient lighting irrespective of shadow.
     // Specular-diffuse lighting only if the point is not in shadow
     Point l1 = Point(185.0, 500.0, 169.0);
     Point l2 = Point(400.0, 500.0, 320.0);

     Ray r1 = Ray(l1, Point(p.x - l1.x, p.y - l1.y, p.z - l1.z));
     Ray r2 = Ray(l2, Point(p.x - l2.x, p.y - l2.y, p.z - l2.z));

     Object* o1 = intersect(r1);
     Object* o2 = intersect(r2);
     Point p1 = o1==NULL ? Point::Infinite() : o1->getIntersection(r1);
     Point p2 = o2==NULL ? Point::Infinite() : o2->getIntersection(r2);
     if(p1.x != 1E20 || p2.x != 1E20) {
     // if((p1.x == p.x && p1.y == p.y && p1.z == p.z) || (p2.x == p.x && p2.y == p.y && p2.z == p.z)) {
          Point np = o->getNormal(p);
          glm::vec3 n = glm::normalize(glm::vec3(np.x, np.y, np.z));
          glm::vec3 l11 = glm::normalize(glm::vec3(l1.x, l1.y, l1.z));
          glm::vec3 l21 = glm::normalize(glm::vec3(l2.x, l2.y, l2.z));
          float diffuse_intensity = glm::clamp(glm::dot(n, l11 + l21), 0.f, 1.f);
          glm::vec3 half_angle = glm::normalize(l11+l21+glm::vec3(278.f,273.f,-500.f));
          float spec_weight = glm::clamp(glm::dot(half_angle, n), 0.f, 1.f);

          Color d = (m->getDiffuse(p) * diffuse_intensity);
          Color s = (m->getSpecular(p) * pow(spec_weight, 100.f));
          // return ret + m->getAmbient(p) + m->getDiffuse(p);
          if(m ->type == REFLECTIVE) {
                 //Point normal = Point(glm::normalize(glm::vec3(normal.x, normal.y, normal.z)).x, glm::normalize(glm::vec3(normal.x, normal.y, normal.z)).y, glm::normalize(glm::vec3(normal.x, normal.y, normal.z)).z);
                 Ray a = r.reflect(normal, p);
                 ret = ret + trace(a,1);
            }
          return ret + a + d + s;
     }



      return ret + a;
}


// This function generates point on the image plane and starts a trace
// through them.
// Grid supersampling is also implemented.
Color RayTracer::calculate(int x, int y){
  Color c = Color(0.0,0.0,0.0,0.0);
  for(int i = 1; i <= samples; i++){
    for(int j = 1; j <= samples; j++){
      double one, two;
      // We subtract 0.5 so that the (u,v)-origin is at the
      //center of the image plane.
      one = double(x)-0.5+double(i)/double(samples);
      two = double(y)-0.5+double(j)/double(samples);
      Ray r = scene->makeRay(one,two);
      c = c+trace(r,0);
    }
  }
  c = c*(1.0/double(samples*samples));
  return c;
}
