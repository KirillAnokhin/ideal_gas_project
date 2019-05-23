#include <cstdio>
#include <iostream>
#include <cmath>
#include <utility>
#include <functional>
#include "experiment.h"

inline double fast_abs(double val)
{
	if(val < 0)
		return -val;
	return val;
}

inline double fast_sign(double val)
{
	if(val >= 0)
		return 1;
	return -1;
}

void Experiment::p_htable_update()
{
	p_htable.clear();
	for (auto p : p_vector)
		p_htable.insert({ p.r.get_cube(cube_size), &p});
}


void Experiment::simulate(size_t n_steps)
{
	while (n_steps--)
		simulate_step();
}

inline void Experiment::set_buckets(const Vector_3d::Cube &c, size_t arr[27])
{
	size_t counter = 0;
	Vector_3d::Cube d;
	for 			(d.x = -1; d.x < 2; d.x++) {
		for 		(d.y = -1; d.y < 2; d.y++) {
			for 	(d.z = -1; d.z < 2; d.z++)
				arr[counter++] = p_htable.bucket(c + d);
		}
	}
}


inline void Experiment::process_part_coll(Particle_3d *p1, Particle_3d *p2)
{
	Vector_3d r = p1->r - p2->r;
	double r2 = scalar_product(r, r);
	if (r2 > min_sc)
		return;
	
	Vector_3d v_delta = p1->v - p2->v;
	double v_delta2 = scalar_product(v_delta, v_delta);

	Vector_3d v_ch = (scalar_product(r, v_delta) / r2) * r;

	double tmp = scalar_product(v_delta, r);
	tmp = tmp * tmp + (min_sc - r2) * v_delta2;
	double coll_time = (sqrt(tmp) - scalar_product(v_delta, r)) / v_delta2;

	p1->v = p1->v + v_ch;
	p2->v = p2->v - v_ch;

	p1->r = p1->r + coll_time * v_ch;
	p2->r = p2->r - coll_time * v_ch;
}

inline void Experiment::process_wall_coll(Wall_3d &w)
{
	
	double sc_ab = scalar_product(w.a, w.b);
	double sc_aa = scalar_product(w.a, w.a);
	double sc_bb = scalar_product(w.b, w.b);
	double det = sc_aa * sc_bb - sc_ab * sc_ab;
	double c_bb = sc_bb/det;
	double c_ab = sc_ab/det;
	double c_aa = sc_aa/det;

	for (auto &p : p_vector) {
		Vector_3d r = p.r - w.r;
		double sc_rn = scalar_product(r, w.n);
		if(fast_abs(sc_rn) > wall_wdth) 
			continue;
		double sc_ra = scalar_product(r, w.a);
		double sc_rb = scalar_product(r, w.b);
		double r_a = sc_ra*c_bb - sc_rb*c_ab;
		double r_b = sc_rb*c_aa - sc_ra*c_ab;
		if(r_a < 0 || r_b < 0 || r_a > 1 || r_b > 1)
			continue;
		
		double sc_vn = scalar_product(p.v - w.v, w.n); 

		Vector_3d v_ch = ((-2) * sc_vn) * w.n;		
		Vector_3d r_ch = ((-2)*(fast_sign(sc_vn * sc_rn) * wall_wdth * fast_sign(sc_rn) + sc_rn))*w.n; 
		
		p.v = p.v + v_ch;
		p.r = p.r + r_ch;

		/* sum_p_imp += fast_abs(v_ch); */
	}
		
}

inline void Experiment::move_particle(Particle_3d *p)
{
	p->r = p->r + time_step * p->v;
}

void Experiment::simulate_step()
{
	p_htable_update();
	const size_t n_cubes = 27;
	size_t buckets[n_cubes];

	for (auto &p : p_htable) {
		set_buckets(p.first, buckets);
		for (size_t i = 0; i < n_cubes; ++i) {
			for (auto it = p_htable.begin(buckets[i]); it != p_htable.end(buckets[i]); it++) {
				if (it->second > p.second)
					process_part_coll(it->second, p.second);
			}
		}
	}

	for (auto &w : walls)
		process_wall_coll(w);

	for (auto &p : p_htable)
		move_particle(p.second);

	/* timer += time_step; */
}



