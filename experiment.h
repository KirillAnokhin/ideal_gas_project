#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_

#include <unordered_map>
#include <cmath>
#include <iostream>
#include <vector> 

struct Vector_3d 
{
	struct Cube
	{
		size_t x, y, z;
	};

	static double cube_size;
	double x, y, z;

	bool operator==(const Vector_3d & other) const
	{
		assert(0);
		return ((x == other.x) && (y == other.y) && (z == other.z));
	}

	Cube get_cube() const
	{
		return (Cube) {
			.x = x / cube_size;
			.y = y / cube_size;
			.z = z / cube_size;
		};
	}
};

namespace std {
	struct hash<Vector_3d::Cube>
	{
		std::size_t operator()(const Vector_3d::Cube& cube) const
		{
			return  std::hash()(cube.x) ^
				std::hash()(cube.y) ^
				std::hash()(cube.z);
		}
	};
}


struct Particle
{
	Vector_3d r;
	Vector_3d::Cube cube;

	Vector_3d v;
	
	void set_cube() { cube = r.get_cube() };
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
	std::unordered_multimap<Vector_3d::Cube &, Particle &> htable;
	void upd_hash_table();
	void get_near_hashes(Vector_3d &vec, size_t *vector);

	double time_step;
public:
	Experiment();
	~Experiment();

	void add_wall(Wall &wall);
	void add_particle(Particle &particle);
	void set_cube_size(double cube_size);
	void set_time_step(double time_step);
	
	void simulate(size_t n_steps);
};

#endif //EXPERIMENT_H_
