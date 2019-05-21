#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_

#include <unordered_map>
#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>
#include <functional>
#include <utility>


struct Vector_3d 
{
	double x, y, z;

	struct Cube
	{
		long x, y, z;

		inline std::size_t get_hash()
		{
			return  std::hash<long>{}(x) ^
				std::hash<long>{}(y) ^
				std::hash<long>{}(z);
		}

		inline bool operator ==(const Vector_3d::Cube &other) const
		{
			return ((x == other.x) && (y == other.y) && (z == other.z));
		}

		inline friend Cube operator+(const Cube &l, const Cube &r)
		{
			Cube c;
			c.x = l.x + r.x;
			c.y = l.y + r.y;
			c.z = l.z + r.z;
			return c;
		}
	};

	inline Cube get_cube(double size) const
	{
		Vector_3d::Cube c;
		c.x = x/size;
		c.y = y/size;
		c.z = z/size;
		return c;
	}

	inline friend Vector_3d operator+(const Vector_3d &l, const Vector_3d &r)
	{
		Vector_3d c;
		c.x = l.x + r.x;
		c.y = l.y + r.y;
		c.z = l.z + r.z;
		return c;
	}

	inline friend Vector_3d operator-(const Vector_3d &l, const Vector_3d &r)
	{
		Vector_3d c;
		c.x = l.x - r.x;
		c.y = l.y - r.y;
		c.z = l.z - r.z;
		return c;
	}

	inline friend Vector_3d operator*(const Vector_3d &v, const double n)
	{
		Vector_3d c;
		c.x = n * v.x;
		c.y = n * v.y;
		c.z = n * v.z;
		return c;
	}

	inline friend Vector_3d operator*(const double n, const Vector_3d &v)
	{
		Vector_3d c;
		c.x = n * v.x;
		c.y = n * v.y;
		c.z = n * v.z;
		return c;
	}

	inline friend double scalar_product(const Vector_3d &vec_a, const Vector_3d &vec_b)
	{
		double sc_pr = vec_a.x * vec_b.x +
		       	vec_a.y * vec_b.y + vec_a.z * vec_b.z;
		return sc_pr;
	}

	inline friend Vector_3d vector_product(const Vector_3d &vec_a, const Vector_3d &vec_b)
	{				
		Vector_3d vec;
		vec.x = vec_a.y * vec_b.z - vec_a.z * vec_b.y;
		vec.y = vec_a.z * vec_b.x - vec_a.x * vec_b.z;
		vec.z = vec_a.x * vec_b.y - vec_a.y * vec_b.x;
		return vec;
	}

	inline friend Vector_3d vector_norm(const Vector_3d &vec)
	{
		Vector_3d vec_n;
		double norm = sqrt(vec.x*vec.x + vec.y*vec.y +vec.z*vec.z);
		vec_n.x = vec.x/norm;
		vec_n.y = vec.y/norm;
		vec_n.z = vec.z/norm; 
		return vec_n;	
	}
};

namespace std {
	template<> struct hash<Vector_3d::Cube>
	{
		std::size_t operator()(const Vector_3d::Cube cube) const
		{
			return  std::hash<long>{}(cube.x) ^
				std::hash<long>{}(cube.y) ^
				std::hash<long>{}(cube.z);
		}
	};
}

struct Particle_3d
{
	Vector_3d r;
	Vector_3d v;
	Particle_3d(Vector_3d r_, Vector_3d v_)
	{
		r = r_;
		v = v_;
	}
};


struct Wall_3d
{
private:
	struct Vector_3d vec_n; //|n| = 1
public:
	struct Vector_3d vec_r;
	struct Vector_3d vec_a;
	struct Vector_3d vec_b;

	
	Wall_3d(Vector_3d vec_r_, Vector_3d vec_a_, Vector_3d vec_b_)
	{
		vec_r = vec_r_;
		vec_a = vec_a_;
		vec_b = vec_b_;
		vec_n = vector_norm(vector_product(vec_a_, vec_b_));
	}
};


class Experiment
{
	std::vector<Wall_3d> walls;
	std::vector<Particle_3d> p_vector;
	std::unordered_multimap<Vector_3d::Cube, Particle_3d *> p_htable;
	void p_htable_update();

	double time_step;
	double particle_r;
	double min_sc;
	double cube_size;
	const double wall_wdth_mult = 30;

	inline void move_particle(Particle_3d *p);
	inline void process_part_coll(Particle_3d *p1, Particle_3d *p2);
	inline void process_wall_coll(Particle_3d *p);
	inline void set_buckets(const Vector_3d::Cube &c, size_t arr[27]);
public:
	Experiment() { ; }
	~Experiment();
	
	void add_wall(Wall_3d &wall) { walls.push_back(wall); }
	void add_particle(Particle_3d &p) { p_vector.push_back(p); }
	void set_time_step(double t) { time_step = t; }
	void set_particle_r(double r) { particle_r = r; min_sc = 4 * r * r; }
	void set_cube_size(double s) { cube_size = s; }
	
	void simulate(size_t n_steps);
	void simulate_step();
};

#endif //EXPERIMENT_H_
