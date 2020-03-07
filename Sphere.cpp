#include "Sphere.h"

using namespace rt;

Intersection Sphere::getIntersection(const Line& line, float minDist, float maxDist) {
	Intersection in;

	Vector ray_origin = line.x0();
	Vector ray_direction = line.dx();
	Vector sphere_center = this->center();

	const float a = ray_direction * ray_direction;
	const float b = (ray_direction + ray_direction) * (ray_origin - sphere_center);
	const float c = (ray_origin - sphere_center) * (ray_origin - sphere_center) - this->radius() * this->radius();

	const float delta = b * b - 4 * a * c;


	if (delta < 0)  //the line doesn't intersect a sphere
	{
		return Intersection{};
	}
	else
	{
		float x1 = (-b - sqrt(delta)) / (2 * a);
		float x2 = (-b + sqrt(delta)) / (2 * a);

		if (x1 == x2) //the line intersects a sphere in one place
		{ 
			if (minDist<x1 &&  maxDist>x1)
				return Intersection{ true, this, &line, x1 };
		}
		if (x1 < x2)//x1 is closer to the source
		{
			if (minDist<x1 && maxDist>x1)
			{
				return Intersection{ true, this, &line, x1 };
			}
			else
				return Intersection{};
		}
		else //x2 is closer to the source
		{
			if (minDist<x2 && maxDist>x2)
			{
				return Intersection{ true, this, &line, x2 };
			}
			else
				return Intersection{}; //the solutions are <minDist or >maxDist
		}
	}
	


}


const Vector Sphere::normal(const Vector& vec) const {
    Vector n = vec - _center;
    n.normalize();
    return n;
}
