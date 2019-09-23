#pragma once

#include <vector>
#include "Ray.h"
#include "Scene.h"


//Single object intersection detection
bool intersect(const Ray& ray, const Sphere& sphere, float& t);
bool intersect(const Ray& ray, const Triangle& triangle, float& t);


//Multiple objects intersection detection
bool intersectNearest(const Ray& ray, const std::vector<Sphere*>& spheres, float& t, Sphere& sphere);
bool intersectNearest(const Ray& ray, const std::vector<Triangle*>& triangles, float& t, Triangle& triangle);