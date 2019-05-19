#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_

#include <unordered_map>
#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>
#include <functional>
#include <utility>

class Experiment;

struct Vector_3d 
{
	struct Cube
	{
		long x, y, z;

		bool operator==(const Vector_3d::Cube & other) const
		{
			assert(0);
			return ((x == other.x) && (y == other.y) && (z == other.z));
		}
	};
	//static double cube_size
	double x, y, z;

	bool operator==(const Vector_3d & other) const
	{
		assert(0);
		return ((x == other.x) && (y == other.y) && (z == other.z));
	}

	Cube get_cube() const
	{
		Cube c;
		c.x = x / Experiment::cube_size;
		c.y = y / Experiment::cube_size;
		c.z = z / Experiment::cube_size;

		return c;
	}
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

size_t get_hash(const Vector_3d::Cube cube)
{
	return  std::hash<long>{}((long)cube.x) ^
		std::hash<long>{}((long)cube.y) ^
		std::hash<long>{}((long)cube.z);

}

namespace std {
	template<> struct hash<Vector_3d::Cube> 
	{
		std::size_t operator()(const Vector_3d::Cube cube) const
		{
			return get_hash(cube);	
		}
	};
}


struct Particle
{
	Vector_3d r;
	Vector_3d::Cube cube;

	Vector_3d v;
	
	void set_cube() { cube = r.get_cube(); };
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

class Experiment
{
	std::vector<Wall> walls;
	std::vector<Particle> particles;
	std::unordered_multimap<Vector_3d::Cube, Particle &> htable;
	void upd_hash_table();
	void get_near_buckets(Particle &p, size_t *buck, int &i);

	double time_step;
public:
	Experiment(double cube_size_);
	~Experiment();

	static double cube_size;


	void simulation();

	void add_wall(Wall &wall);

	void add_particle(Particle &particle);

	void set_cube_size(double cube_size);

	void set_time_step(double time_step);
	
	void simulate(size_t n_steps);
};

#endif //EXPERIMENT_H_
