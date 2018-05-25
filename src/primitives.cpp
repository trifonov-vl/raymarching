#include "primitives.h"
#include <algorithm>
#include <fstream>
#include <iostream>

glm::vec3 Primitive::compute_normal_at_point(const glm::vec3 &p, const float &eps, const float &scale)
{
	glm::vec3 z1 = p + glm::vec3(eps, 0, 0);
	glm::vec3 z2 = p - glm::vec3(eps, 0, 0);
	glm::vec3 z3 = p + glm::vec3(0, eps, 0);
	glm::vec3 z4 = p - glm::vec3(0, eps, 0);
	glm::vec3 z5 = p + glm::vec3(0, 0, eps);
	glm::vec3 z6 = p - glm::vec3(0, 0, eps);

	float dx = compute_distance_function_at_point(z1/scale) - compute_distance_function_at_point(z2/scale);
	float dy = compute_distance_function_at_point(z3/scale) - compute_distance_function_at_point(z4/scale);
	float dz = compute_distance_function_at_point(z5/scale) - compute_distance_function_at_point(z6/scale);
	return glm::normalize(glm::vec3(scale*dx, scale*dy, scale*dz) / (2.0f*eps));
}

float Sphere::compute_distance_function_at_point(const glm::vec3 &p)
{
	return glm::length(p)-radius;
}

float Box::compute_distance_function_at_point(const glm::vec3 &p)
{
	glm::vec3 d = glm::abs(p) - sizes;
	return std::min(std::max(d.x,std::max(d.y,d.z)),0.0f) + glm::length(glm::max(d,glm::vec3(0.0f)));
}

float Plane::compute_distance_function_at_point(const glm::vec3 &p)
{
	return glm::dot(p,glm::vec3(n.x,n.y,n.z)) + n.w;
}

float Torus::compute_distance_function_at_point(const glm::vec3 &p)
{
	glm::vec2 q = glm::vec2(length(glm::vec2(p.x,p.z))-t.x,p.y);
  	return length(q)-t.y;
}

float Cylinder::compute_distance_function_at_point(const glm::vec3 &p)
{
	glm::vec2 d = glm::abs(glm::vec2(glm::length(glm::vec2(p.x,p.z)),p.y)) - h;
  	return std::min(std::max(d.x,d.y),0.0f) + glm::length(glm::max(d,0.0f));
}

float Prism::compute_distance_function_at_point(const glm::vec3 &p)
{
	glm::vec3 q = glm::abs(p);
    return std::max(q.z-h.y,std::max(q.x*0.866025f+p.y*0.5f,-p.y)-h.x*0.5f);
}

float Union::compute_distance_function_at_point(const glm::vec3 &p)
{
	return std::min(pr1.compute_distance_function_at_point(p),pr2.compute_distance_function_at_point(p));
}

float Substraction::compute_distance_function_at_point(const glm::vec3 &p)
{
	return std::max(-pr1.compute_distance_function_at_point(p),pr2.compute_distance_function_at_point(p)); 
}

float Intersection::compute_distance_function_at_point(const glm::vec3 &p)
{
	return std::max(pr1.compute_distance_function_at_point(p),pr2.compute_distance_function_at_point(p)); 
}