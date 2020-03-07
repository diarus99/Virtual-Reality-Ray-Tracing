#include <cmath>
#include <iostream>
#include <string>

#include "Vector.h"
#include "Line.h"
#include "Geometry.h"
#include "Sphere.h"
#include "Image.h"
#include "Color.h"
#include "Intersection.h"
#include "Material.h"

#include "Scene.h"

using namespace std;
using namespace rt;

float imageToViewPlane(int n, int imgSize, float viewPlaneSize) {
    float u = (float)n*viewPlaneSize / (float)imgSize;
    u -= viewPlaneSize / 2;
    return u;
}

const Intersection findFirstIntersection(const Line& ray,
    float minDist, float maxDist) {
    Intersection intersection;

    for (int i = 0; i < geometryCount; i++) {
        Intersection in = scene[i]->getIntersection(ray, minDist, maxDist);
        if (in.valid()) {
            if (!intersection.valid()) {
                intersection = in;
            }
            else if (in.t() < intersection.t()) {
                intersection = in;
            }
        }
    }

    return intersection;
}

int main() {
    Vector viewPoint(-50, 10, 0);
    Vector viewDirection(50, -10, 100);
    Vector viewUp(0, -1, 0);

	viewDirection.normalize();
	viewUp.normalize();

    float frontPlaneDist = 0;
    float backPlaneDist = 1000;

    float viewPlaneDist = 65;
    float viewPlaneWidth = 160;
    float viewPlaneHeight = 120;

    int imageWidth = 1024;
    int imageHeight = 768;

    Vector viewParallel = viewUp^viewDirection;
    viewParallel.normalize();

    Image image(imageWidth, imageHeight);

///change backgroung color in green
	/*
	for(int i=0;i< imageWidth;i++)
		for (int j = 0; j < imageHeight; j++)
		{
			Color c{ 0,255,0 };
			image.setPixel(i, j, c);
		}
	*/	
	
	for (int i = 0; i < imageWidth; i++)
	{
		for (int j = 0; j < imageHeight; j++)
		{
			Vector onPlaneHeightVector = viewUp * imageToViewPlane(j, imageHeight, viewPlaneHeight);
			Vector onPlaneWidthVector = viewParallel * imageToViewPlane(i, imageWidth, viewPlaneWidth);
			Vector viewVector = viewDirection * viewPlaneDist;
			Vector pointVector = viewPoint + viewVector + onPlaneHeightVector + onPlaneWidthVector;
			Line line = Line(viewPoint, pointVector, false);
			Intersection in = findFirstIntersection(line, frontPlaneDist, backPlaneDist);
															//minDist			//maxDist
			if (in.valid())
			{
				Color color = in.geometry()->color();
				for (Light* light : lights) 
				{
					Vector N = in.geometry()->normal(in.vec());// normal to the surface at the intersection point
					//N.normalize();
					Vector E = viewPoint- in.vec(); // vector from the intersection point to the camera
					E.normalize();
					Vector T = light->position() -in.vec(); //// vector from the intersection point to the light
					T.normalize();
					Vector R = N * (N * T) * 2 - T; //reflection vector
					R.normalize();

					//Color color = in.geometry()->material().ambient() * light->ambient();
					color += in.geometry()->material().ambient() * light->ambient();
					if (N * T > 0)
						color += in.geometry()->material().diffuse() * light->diffuse() * (N * T);
					if (E * R > 0)
						color += in.geometry()->material().specular() * light->specular() * pow(E * R, in.geometry()->material().shininess());
					color *= light->intensity();
					image.setPixel(i, j, color);
				}
			}
			else
			{
				image.setPixel(i, j, Color{ 0,0,0 }); //set image to black
			}
		}


	}
	
    image.store("scene.png");

    for (int i = 0; i < geometryCount; i++) {
        delete scene[i];
    }

	for (int i = 0; i < lightCount; i++) {
		delete lights[i];
	}
	
    return 0;
}
