#include "objects.h"
#include <string>
#include <fstream>
#include <iostream>

void apply_fog(glm::vec3 &rgb, const float &distance)
{
	float fogDensity = 0.15f;
	float fogAmount = 1.0f - exp( -distance*fogDensity );
    glm::vec3  fogColor(0.2f,0.3f,0.4f);
    rgb = glm::mix(rgb, fogColor, fogAmount);
}

float clamp(const float &v, const float &lo, const float &hi)
{
	if( v < lo )
		return lo;
	if( v > hi )
		return hi;
	return v;
}

bool Object::ray_hits(const Ray &r, float &t) const
{
	float depth = 3*EPSILON;

	for(int i = 0; i < MAX_MARCHING_STEPS; i++)
	{
		float distance = (*prim).compute_distance_function_at_point(glm::vec3(world_to_model*glm::vec4(r.pos+depth*r.dir,1.0f))/scale)*scale;

		if( distance < EPSILON )
		{
			t = depth;
			return true;
		}

		depth += distance;

		if( depth >= BOX_SIZE) 
			return false;
	}

	return false;
}

float Scene::find_min_distance(const glm::vec3 &point) const
{
	float min_distance = 0;
	for(unsigned int i = 0; i < object_list.size(); i++ )
	{
		const Object &obj = object_list[i];
		float distance = (*obj.prim).compute_distance_function_at_point(glm::vec3(obj.world_to_model*glm::vec4(point,1.0f))/obj.scale)*obj.scale;
		if( i == 0 || distance < min_distance)
			min_distance = distance;
	}

	return min_distance;
}


float Scene::soft_shadow_march(const Ray &r) const
{
	float depth = EPSILON;
	float res = 1.0f;
	float k = 15;


	for(int i = 0; i < MAX_MARCHING_STEPS; i++)
	{
		float distance = find_min_distance(r.pos+depth*r.dir);

		if( distance < EPSILON )
		{
			return 0.0f;
		}

		res = std::min( res, k*distance/clamp(depth,0.0f,20.0f) );	
		depth += distance;

		if( depth >= BOX_SIZE )
			break;
	}

    return clamp(res,0.0f,1.0f);
}

glm::vec3 PointLight::get_intensity_at_point(const glm::vec3 &point)
{
	float dist = glm::distance(point, pos);

	return color*float(intensity / (4*M_PI*dist*dist));
}

Scene::Scene()
{

	//create scene objects and materials for ambient ocllusion and normal scene

	#if AO_SCENE

	Material white(glm::vec3(1.0f),glm::vec3(1.0f),0.7f,0.0f,0.0f);

	glm::mat4 r;

	Plane *pl = new Plane(glm::vec4(0.0f,1.0f,0.0f,0.0f));

	Object plane(pl, white);
	
	object_list.push_back(plane);

	Sphere *s1 = new Sphere(1.2f);

	Object sphere(s1,white);

	r = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f,0.8f,-3.5f));
	sphere.set_transformation_matrix(r);
	sphere.set_scale(0.7f);

	object_list.push_back(sphere);

	Box *b = new Box();

	r = glm::translate(glm::mat4(1.0f), glm::vec3(4.2f,1.25f,-4.5f))*glm::rotate(glm::mat4(1.0f),float(M_PI/4), glm::vec3(0.0f,1.0f,0.0f));
	Object box(b,white);
	box.set_transformation_matrix(r);
	
	object_list.push_back(box);

	Sphere *s2 = new Sphere(0.3f);

	Object sphere1(s2,white);
	r = glm::translate(glm::mat4(1.0f), glm::vec3(4.2f,2.55f,-4.5f));
	sphere1.set_transformation_matrix(r);

	object_list.push_back(sphere1);

	Sphere *s3 = new Sphere(0.8f);

	Object sphere2(s3,white);
	r = glm::translate(glm::mat4(1.0f), glm::vec3(5.7f,0.8f,-3.3f));
	sphere2.set_transformation_matrix(r);

	object_list.push_back(sphere2);

	#else

	Material red;
	Material gray(glm::vec3(0.1f,0.1f,0.1f),glm::vec3(0.3f,0.3f,0.3f),0.4f,0.4f);
	Material yellow(glm::vec3(1.0f,1.0f,0.0f),glm::vec3(1.0f, 1.0f, 1.0f), 0.6f, 0.1f, 0.5f);
	Material green(glm::vec3(0.0f,1.0f,0.0f));
	Material blue(glm::vec3(0.1f,0.1f,0.8f),glm::vec3(0.8f,0.1f,0.1f),0.6f, 0.3f);
	Material violet(glm::vec3(1.0f,0.0f,1.0f),glm::vec3(0.8f,0.8f,0.8f),0.7f,0.2f);
	Material metallic(glm::vec3(0.8f,0.8f,0.8f),glm::vec3(1.0f,1.0f,1.0f),0.2f,0.9f,0.0f);
	

	Torus *t1 = new Torus(glm::vec2(1.0f,0.5f));
	Object tor_1(t1, red);

	glm::mat4 r = glm::translate(glm::mat4(1.0f),glm::vec3(-1.5f,2.0f,-6.0f))*glm::rotate(glm::mat4(1.0f),float(M_PI/4), glm::vec3(1.0f,0.0f,0.0f));
	tor_1.set_transformation_matrix(r);

	object_list.push_back(tor_1);

	Sphere *s1 = new Sphere(1.2f);

	Object sphere(s1,yellow);

	r = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f,0.8f,-3.5f))*glm::rotate(glm::mat4(1.0f),float(M_PI/4), glm::vec3(1.0f,0.0f,0.0f));
	sphere.set_transformation_matrix(r);
	sphere.set_scale(0.7f);

	object_list.push_back(sphere);

	Torus *t3 = new Torus(glm::vec2(1.0f,0.5f));

	Object tor_3(t3, green);

	r = glm::translate(glm::mat4(1.0f), glm::vec3(7.5f,2.0f,-6.0f))*glm::rotate(glm::mat4(1.0f),float(M_PI/4), glm::vec3(1.0f,0.0f,0.0f));
	tor_3.set_transformation_matrix(r);

	object_list.push_back(tor_3);

	Box *b = new Box();
	Sphere *s = new Sphere(1.3f);
	Intersection *box_sphere = new Intersection(*b,*s);

	r = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f,1.25f,-8.0f))*glm::rotate(glm::mat4(1.0f),float(M_PI/4), glm::vec3(0.0f,1.0f,0.0f));
	Object box(box_sphere,blue);
	box.set_transformation_matrix(r);
	
	object_list.push_back(box);

	Cylinder *c_1 = new Cylinder(glm::vec2(0.8f,1.3f));
	Box *b_1 = new Box();
	Substraction *substr = new Substraction(*c_1, *b_1);
	r = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f,1.0f,-1.0f))*glm::rotate(glm::mat4(1.0f),float(M_PI/2), glm::vec3(1.0f,0.0f,0.0f));
	Object cyl(substr,violet);
	cyl.set_transformation_matrix(r);
	cyl.set_scale(0.5f);
	
	object_list.push_back(cyl);

	Prism *prism_1 = new Prism(glm::vec2(1.3f,1.3f));
	Box *b_2 = new Box(glm::vec3(0.9f));
	Union *un = new Union(*b_2,*prism_1);

	r = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,1.0f,-1.0f))*glm::rotate(glm::mat4(1.0f),float(M_PI/2), glm::vec3(1.0f,0.0f,0.0f));
	Object prism(un, metallic);
	prism.set_transformation_matrix(r);
	prism.set_scale(0.5f);

	object_list.push_back(prism);

	Plane *pl = new Plane(glm::vec4(0.0f,1.0f,0.0f,0.0f));

	Object plane(pl, gray);

	object_list.push_back(plane);

	PointLight l(glm::vec3(0.0f, 16.0f, 5.0f),glm::vec3(1.0f,1.0f,1.0f),4000);

	lights_list.push_back(l);

	PointLight l1(glm::vec3(9.0f,10.0f,-8.0f),glm::vec3(0.3f,0.3f,0.8f),4000);

	lights_list.push_back(l1);

	#endif

	//create cam

	view_pos = glm::vec3(3.0f, 4.0f, 5.0f);

	cam_dir_vec = glm::vec3(0.0f, -0.3f,-1.0f);
	cam_up_vec = glm::vec3(0.0f,1.0f,0.0f);
	cam_right_vec = glm::cross(cam_dir_vec,cam_up_vec);
}

void Scene::render(int width, int height, const char* const path)//https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing
{
	std::vector<glm::vec3> img(width*height);

	#pragma omp parallel for
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
            float distance;
            glm::vec3 direction = cam_dir_vec + ((width*1.0f)/(height*1.0f))*((x*1.0f+0.5f)/width-0.5f)*cam_right_vec-((y*1.0f+0.5f)/height-0.5f)*cam_up_vec;

            Ray pixray(view_pos, direction);

            img[width*y+x] = find_color_from_ray(pixray,distance,0);
            if( FOG_ON)
            	apply_fog(img[width*y+x],distance);
		}
	}

	//saving to file

	std::ofstream ofs(std::string(path)+"scene.ppm", std::ios::out | std::ios::binary); 
    ofs << "P6\n" << width << " " << height << "\n255\n"; 
    for (int i = 0; i < width * height; i++) { 
        ofs << char(std::min(1.0f, std::max(0.0f,img[i].x)) * 255) << 
               char(std::min(1.0f, std::max(0.0f,img[i].y))*255) << 
               char(std::min(1.0f, std::max(0.0f,img[i].z)) * 255); 
    } 
    ofs.close(); 
}

glm::vec3 Scene::find_color_from_ray(const Ray &r, float &d, int depth)
{
	bool intersection_flag;
	float t_min;
	int intersection_object_index;
	
	intersection_flag = find_intersection_with_objects(r,intersection_object_index,t_min);

	if(!intersection_flag)
	{
		d = INFINITY;
		return glm::vec3(0.0f,0.0f,0.0f);//background color
	}

	glm::vec3 hit_color(0.0f,0.0f,0.0f);

	#if AO_SCENE
	glm::vec3 background_color(0.45f);
	#else
	glm::vec3 background_color(0.06f);
	#endif

	glm::vec3 hit_position = r.pos+r.dir*t_min;
	d = t_min;
	Object *hit_object = &(object_list[intersection_object_index]);
	glm::vec3 hit_normal = (*hit_object).get_normal_at_point(hit_position);

	hit_position += hit_normal*1000.0f*EPSILON;//to avoid shadow acne

	//light ambient oclusion for ray marching
	float normal_len = 0;
	float ao_len = 0;

	for( int j = 0; j < 5; j++)
	{
		normal_len += 0.3*j;
		glm::vec3 ao_point = hit_position + normal_len*hit_normal;
		
		float min_distance = find_min_distance(ao_point);

		if(min_distance < 0 )
			min_distance = 0;

		ao_len += min_distance;
	}

	float avg_normal_len = normal_len/5;
	float avg_ao_len = ao_len/5;

	float k_ao = avg_ao_len/avg_normal_len;

	hit_color += k_ao*background_color;

	glm::vec3 to_cam_vec = glm::normalize(view_pos - hit_position);

	for(unsigned int i = 0; i < lights_list.size(); i++)
	{
		glm::vec3 shadow_vec = glm::normalize(lights_list[i].pos - hit_position);
		Ray from_point_to_light(hit_position, shadow_vec);

		float k_shadow = soft_shadow_march(from_point_to_light);//find shadow part with ray marhing method

		glm::vec3 light_color = lights_list[i].get_intensity_at_point(hit_position);

		//phong model

		//diffuse
		glm::vec3 diffuse_color(0.0f);

		float diffuse_dot_product = glm::dot(hit_normal, shadow_vec);

		if( diffuse_dot_product >=0 )
			diffuse_color = (*hit_object).mat.diffuse_color*diffuse_dot_product*(*hit_object).mat.k_diffuse;
		
		glm::vec3 reflect_vec = glm::normalize(glm::reflect(-shadow_vec,hit_normal));


		//specular
		glm::vec3 specular_color(0.0f);

		float alpha = 16;

		float specular_dot_product = glm::dot(reflect_vec, to_cam_vec);

		if( specular_dot_product >= 0)
			specular_color = float(pow(specular_dot_product, alpha))*(*hit_object).mat.specular_color*(*hit_object).mat.k_specular;

		hit_color += ((specular_color + diffuse_color)*k_shadow)*light_color;
	}

	//reflection
	
	float reflection_value = (*hit_object).mat.k_reflection;
	if(reflection_value > 0)
	{
		if(depth < MAX_REC_DEPTH)
		{
			glm::vec3 reflection_vec = glm::normalize(glm::reflect(r.dir,hit_normal));
			Ray reflection_ray(hit_position,reflection_vec);
			float d_tmp;
			glm::vec3 reflection_color = find_color_from_ray(reflection_ray,d_tmp,depth + 1);
			hit_color += reflection_value * reflection_color;
		}
	}

	return hit_color;
}

bool Scene::find_intersection_with_objects(const Ray &r, int &object_num, float &t_intersection)
{
	bool intersection_flag = false;
	float t_min, t;
	int intersection_object_index;
	for(unsigned int i = 0; i < object_list.size(); i++ )
	{
		if(object_list[i].ray_hits(r,t))
		{
			if(!intersection_flag)
			{
				intersection_flag = true;
				t_min = t;
				intersection_object_index = i;
			}
			else
			{
				if(t < t_min)
				{
					t_min = t;
					intersection_object_index = i;
				}
			}
		}
	}

	if( intersection_flag )
	{
		object_num = intersection_object_index;
		t_intersection = t_min;
	}

	return intersection_flag;
}