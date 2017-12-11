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
using std::ofstream;
using std::ifstream;

// store width and height of the render
int width = 512;
int height = 512;

// Our ray tracer
RayTracer * rt;

string p6 = "P6";

int main(){
    // Test scene with max depth of 4 and sampling of 1
    rt = new RayTracer(Scene::initTestScene(width),12,4);
    float pixels[width][height][4];
    for(int ctr = 0; ctr < height*width; ctr++){
    	int i = ctr % width;
    	int j = ctr / width;
    	Color rad = rt->calculate(i,j);
    	pixels[i][j][0] = glm::clamp(rad.r, 0.f, 1.f); //Red
    	pixels[i][j][1] = glm::clamp(rad.g, 0.f, 1.f); //Green
    	pixels[i][j][2] = glm::clamp(rad.b, 0.f, 1.f); //Blue
    	pixels[i][j][3] = 1.0; //Alpha
    }
    // once we are done calculating, we will write to file.
    ofstream testimage;
    ofstream ppm;
	testimage.open("TestScene.rgb",ios::binary | ios::out);
     ppm.open("File.ppm", ios::binary | ios::out);
     ppm << "P3\n" << "# File.pmm\n" << "512 512\n" << "255\n";

	for(int j = height-1; j >=0 ; j-- ) {
	    for(int i = 0; i< width; i++){
	        for(int k = 0; k < 3; k++){
	             // normalize color value to 0-255.
	        	 // This assumes that the color values are in the
	        	 // range [0,1].
	        	char c = int(pixels[i][j][k]*255);
	             testimage << c;
                  ppm << int(pixels[i][j][k]*255) << " ";
                  if(k==2) ppm << "\n";
	        }
	    }
	}
	testimage.close();
     ppm.close();



    return 0;
}
