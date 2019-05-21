#include <cstdio>
#include <iostream>
#include <cmath>
#include <utility>
#include <functional>
#include "experiment.h"


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

inline void Experiment::process_wall_coll(Particle_3d *p)
{
	//Vector_3d v_ch = ((-2) * scalar_product(p->v, n)) * n;
	//p->v = p->v + v_ch;
		
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

	for (auto p : p_htable) {
		set_buckets(p.first, buckets);
		for (size_t i = 0; i < n_cubes; ++i) {
			for (auto it = p_htable.begin(buckets[i]); it != p_htable.end(buckets[i]); it++) {
				if (it->second > p.second)
					process_part_coll(it->second, p.second);
			}
		}
	}

	for (auto p : p_htable)
		move_particle(p.second);
}



