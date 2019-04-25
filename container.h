#ifndef CONTAINER_H_
#define CONTAINER_H_

#include <unordered_map>
#include <math.h>
#include <iostream>
#include <vector> 

inline struct Vector_3d vector_norm(Vector_3d vec);
inline struct Vector_3d vector_product(Vector_3d vec_a, Vector_3d vec_b);
inline double scalar_product(Vector_3d vec_a, Vector_3d vec_b);

//struct Vector3dHash;

double hash_step; //убрать из h файла

struct Vector_3d 
{
	double x, y, z;
};

struct Particle
{
	struct Vector_3d *r;
	struct Vector_3d *v;
	size_t hash;
};

struct Wall
{
	struct Vector_3d vec_r;
	struct Vector_3d vec_a;
	struct Vector_3d vec_b;
	struct Vector_3d vec_n;
	
	Wall(Vector_3d vec_r_, Vector_3d vec_a_, Vector_3d vec_b_)
	{
		vec_r = vec_r_;
		vec_a = vec_a_;
		vec_b = vec_b_;
		vec_n = vector_norm(vector_product(vec_a_, vec_b_));
	}
};

struct Vector3dHash 
{
	inline std::size_t operator()(Vector_3d &p) const
	{
		std::size_t x_h, y_h, z_h;
		x_h = p.x / hash_step;
		y_h = p.y / hash_step;
		z_h = p.z / hash_step;

		x_h = std::hash<std::size_t>{}(x_h);
		y_h = std::hash<std::size_t>{}(y_h);
		z_h = std::hash<std::size_t>{}(z_h);

		return x_h ^ y_h ^ z_h;
	}
};

class experiment
{
	//вектор стен, вектор частиц
	std::vector<Wall> walls;
	std::vector<Particle> particles;
	std::unordered_multimap<Vector_3d &, Particle &, Vector3dHash> htable;
	void upd_hash_table();
public:
	void add_wall(Wall &wall);
	void add_particles(Particle &particles);
	void simulation();
	//std::vector<wall> walls;	
};



inline double scalar_product(Vector_3d vec_a, Vector_3d vec_b)
{
	double sc_pr = vec_a.x * vec_b.x +
	       	vec_a.y * vec_b.y + vec_a.z * vec_b.z;
	return sc_pr;
}

inline struct Vector_3d vector_product(Vector_3d vec_a, Vector_3d vec_b)
{				
	struct Vector_3d vec;
	vec.x = vec_a.y * vec_b.z - vec_a.z * vec_b.y;
	vec.y = vec_a.z * vec_b.x - vec_a.x * vec_b.z;
	vec.z = vec_a.x * vec_b.y - vec_a.y * vec_b.x;
	return vec;
}

inline struct Vector_3d vector_norm(Vector_3d vec)
{
	struct Vector_3d vec_n;
	double norm = sqrt(vec.x*vec.x + vec.y*vec.y +vec.z*vec.z);
	vec_n.x = vec.x/norm;
	vec_n.y = vec.y/norm;
	vec_n.z = vec.z/norm; 
	return vec_n;	
}

#endif //CONTAINER_H_
