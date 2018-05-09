#pragma once
#include "glm/glm.hpp"

class Primitive
{
public:
	virtual float compute_distance_function_at_point(const glm::vec3 &p) = 0;
	glm::vec3 compute_normal_at_point(const glm::vec3 &p, const float &eps, const float &scale);
	virtual ~Primitive(){}
};

class Sphere: public Primitive 
{
public:
	float radius;
	Sphere(const float &r = 1.0f): radius(r){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Sphere(){}
};

class Box: public Primitive 
{
public:
	glm::vec3 sizes;
	Box(const glm::vec3 &s = glm::vec3(1.0f,1.0f,1.0f)): sizes(s){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Box(){}
};

class Plane: public Primitive 
{
public:
	glm::vec4 n;
	Plane(const glm::vec4 &n1 = glm::vec4(1.0f)): n(glm::normalize(n1)){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Plane(){}
};

class Torus: public Primitive 
{
public:
	glm::vec2 t;
	Torus(const glm::vec2 &t1 = glm::vec2(1.0f)): t(glm::normalize(t1)){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Torus(){}
};

class Cylinder: public Primitive
{
public:
	glm::vec2 h;
	Cylinder(const glm::vec2 &h1 = glm::vec2(1.0f)):h(h1){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Cylinder(){}
};

class Prism: public Primitive
{
public:
	glm::vec2 h;
	Prism(const glm::vec2 &h1 = glm::vec2(1.0f)):h(h1){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Prism(){}
};

class Union:public Primitive
{
public:
	Primitive &pr1, &pr2;

	Union(Primitive &p1, Primitive &p2):pr1(p1),pr2(p2){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Union(){}
};

class Substraction:public Primitive
{
public:
	Primitive &pr1, &pr2;

	Substraction(Primitive &p1, Primitive &p2):pr1(p1),pr2(p2){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Substraction(){}
};

class Intersection:public Primitive
{
public:
	Primitive &pr1, &pr2;

	Intersection(Primitive &p1, Primitive &p2):pr1(p1),pr2(p2){}
	float compute_distance_function_at_point(const glm::vec3 &p);
	~Intersection(){}
};