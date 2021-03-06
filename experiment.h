#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_

#include <unordered_map>
#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>
#include <functional>
#include <utility>
#include <cmath>


struct Vector_3d 
{
	double x, y, z;

	Vector_3d(double x_ = 0, double y_ = 0, double z_ = 0)  
	{
		x = x_;
		y = y_;
		z = z_;
	}

	inline double get_mod()
	{
		return sqrt(scalar_product(*this, *this));
	}
	
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
friend class Experiment;

private:
	struct Vector_3d v;
	struct Vector_3d n; //|n| = 1
	struct Vector_3d r;
	struct Vector_3d a;
	struct Vector_3d b;
public:
	
	Wall_3d(Vector_3d r_, Vector_3d a_, Vector_3d b_, Vector_3d v_)
	{
		v = v_;
		r = r_;
		a = a_;
		b = b_;
		n = vector_norm(vector_product(a_, b_));
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
	double wall_wdth;

	double timer;
	double sum_p_imp;

	inline void move_particle(Particle_3d *p);
	inline void process_part_coll(Particle_3d *p1, Particle_3d *p2);
	inline void process_wall_coll(Wall_3d &w);
	inline void set_buckets(const Vector_3d::Cube &c, size_t arr[27]);
public:
	Experiment() { ; }
	~Experiment();
	
	void add_wall(Wall_3d &&wall)
       	{
	       	walls.push_back(wall);
       	}

	void add_particle(Particle_3d &&p)
       	{
	       	p_vector.push_back(p);
       	}

	void create_piston(double len, double wid, double heig)
	{		
		add_wall(Wall_3d(Vector_3d (-len/2, -wid/2, heig/2), Vector_3d (len, 0, 0), 
				 Vector_3d (0, wid, 0), Vector_3d (0, 0, 0)));
		add_wall(Wall_3d(Vector_3d (-len/2, -wid/2, -heig/2), Vector_3d(len, 0, 0), 
				 Vector_3d (0, wid, 0), Vector_3d (0, 0, 0)));

		add_wall(Wall_3d(Vector_3d (-len/2, -wid/2, heig/2), Vector_3d (0, wid, 0), 
				 Vector_3d (0, 0, -heig), Vector_3d (0, 0, 0)));
		add_wall(Wall_3d(Vector_3d (len/2, -wid/2, heig/2), Vector_3d (0, wid, 0), 
				 Vector_3d (0, 0, -heig), Vector_3d (0, 0, 0)));
	
		add_wall(Wall_3d(Vector_3d (-len/2, -wid/2, heig/2), Vector_3d (len, 0, 0), 
				 Vector_3d (0, 0, -heig), Vector_3d (0, 0, 0)));
		add_wall(Wall_3d(Vector_3d (-len/2, wid/2, heig/2), Vector_3d (len, 0, 0), 
				 Vector_3d (0, 0, -heig), Vector_3d (0, 0, 0)));

	}
	
	void set_time_step(double t)
       	{
	       	time_step = t;
       	}

	void set_particle_r(double r)
       	{
	       	particle_r = r; 
		min_sc = 4 * r * r; 
		wall_wdth = wall_wdth_mult * r; 
	}

	void set_cube_size(double s)
       	{
	       	cube_size = s;
       	}
	
	void simulate(size_t n_steps);
	void simulate_step();

	void reset_timer()
	{
		timer = 0;
	}

	double meas_p()
	{
		reset_meas_p();
		double s = 0;
		for(auto &w : walls) {
			s += (vector_product(w.a, w.b)).get_mod();
		}
		return sum_p_imp/(timer*s);
	}

	void reset_meas_p()
	{
		reset_timer();
		sum_p_imp = 0;
	}
};

#endif //EXPERIMENT_H_
