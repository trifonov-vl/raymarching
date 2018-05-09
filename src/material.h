#pragma once
#include "glm/glm.hpp"

class Material 
{
public:
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	float k_diffuse;
	float k_specular;
	float k_reflection;
	Material(const glm::vec3 &d_c = glm::vec3(1.0f, 0.0f, 0.0f), const glm::vec3 &s_c = glm::vec3(1.0f, 1.0f, 1.0f), const float &k_d = 0.6, const float &k_s = 0.2, const float &k_refl = 0.1f):
		diffuse_color(d_c), specular_color(s_c), k_diffuse(k_d), k_specular(k_s), k_reflection(k_refl){}
	~Material(){}
};