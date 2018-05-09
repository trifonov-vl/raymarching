#pragma once
#include "primitives.h"
#include "material.h"
#include <vector>
#include <cmath> 
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#define EPSILON 10e-4f
#define MAX_MARCHING_STEPS 4000
#define BOX_SIZE 100
#define MAX_REC_DEPTH 5
#define FOG_ON 0

class Ray
{
public:
	glm::vec3 pos;
	glm::vec3 dir;
	Ray(const glm::vec3 &p, const glm::vec3 &d):pos(p), dir(glm::normalize(d)){}
	~Ray(){}
};

class Object
{
public:
	std::shared_ptr<Primitive> prim;
	Material mat;
	glm::mat4 model_to_world;
	glm::mat4 world_to_model;
	float scale;

	Object(Primitive *pr, const Material &m): prim(pr), mat(m), model_to_world(glm::mat4(1.0f)), world_to_model(glm::mat4(1.0f)),scale(1.0f) {}
	~Object(){}
	glm::vec3 get_normal_at_point(const glm::vec3 &point)
	{
		return (*prim).compute_normal_at_point(glm::vec3(world_to_model*glm::vec4(point,1.0f)),3*EPSILON,scale);
	}

	void set_transformation_matrix(const glm::mat4 &matrix)
	{
		model_to_world = matrix;
		world_to_model = glm::inverse(matrix);
	}	

	void set_scale(const float &f)
	{
		scale = f;
	}

	bool ray_hits(const Ray &r, float &t) const;

	bool soft_shadow_march(const Ray &r, float &t, float &k_shadow) const;
};


class PointLight
{
public:
	glm::vec3 pos;
	glm::vec3 color;
	float intensity;

	PointLight(const glm::vec3 &p, const glm::vec3 &c = glm::vec3(1.0f,1.0f,1.0f), const float &in = 10000):
		pos(p), color(c), intensity(in){}
	~PointLight(){}

	glm::vec3 get_intensity_at_point(const glm::vec3 &point);
};


class Scene
{
public:
	std::vector<Object> object_list;
	std::vector<PointLight> lights_list;
	glm::vec3 view_pos;
	glm::vec3 cam_up_vec;
	glm::vec3 cam_right_vec;
	glm::vec3 cam_dir_vec;

	Scene();
	~Scene(){}

	void render(int width, int height, const char* const path);
	glm::vec3 find_color_from_ray(const Ray &r, float &distance, int depth);
	bool find_intersection_with_objects(const Ray &r, int &object_num, float &t_min);
	float find_min_distance(const glm::vec3 &point) const;
	float soft_shadow_march(const Ray &r) const;
};