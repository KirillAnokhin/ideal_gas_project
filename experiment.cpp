#include <cstdio>
#include <iostream>
#include <utility>
#include <functional>
#include "container.h"


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

void get_line_x_buckets(Vector_3d &vec, size_t *vector, int &i)
{
	vec.x -= Experiment::hash_step;

	for(int k = 0; k < 3; k++) {
		vector[i] = get_hash(vec);
		vec.x += Experiment::hash_step;
		i++;
	}

	vec.x -= Experiment::hash_step;		
}

void get_plane_xy_buckets(Vector_3d &vec, size_t *vector, int &i)
{
	vec.y -= Experiment::hash_step;

	for(int k = 0; k < 3; k++) {
		get_line_x_hashes(vec, vector, i);
		vec.y += Experiment::hash_step;;	
	}

	vec.y -= Experiment::hash_step;;
}

void Experiment::get_near_buckets(Particle &p, size_t *vector)
{
	auto cube = p.cube;
	cube.x;
	// buck size_type
	// buck[i] = htable.bucket(cube);

	int i = 0;
	double step_size = Experiment::hash_step;

	vec.z -= step_size;

	for(int k = 0; k < 3; k++) {
		get_plane_xy_hashes(vec, vector, i);
		vec.z += step_size;	
	}

	vec.y -= step_size;
}

void Experiment::upd_hash_table()
{
	htable.clear();
	for(auto & p : particles) {
		p.set_cube();	
		htable.insert(std::make_pair(p.cube, p));
	}
}

void Experiment::simulation()
{
	upd_hash_table();
	for(auto & p: particles) {
		size_t buck[27];
		get_near_buckets(p, buck);
		for(int k = 0; k < 27; k++) {
			//for(auto iter: htable[vector[k]]);
		}		
	}	
}


