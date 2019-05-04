#include <cstdio>
#include <iostream>
#include <utility>
#include <functional>
#include "container.h"


inline size_t get_hash(Vector_3d &vec)
{
		std::size_t x_h, y_h, z_h;
		x_h = vec.x / Experiment::hash_step; 
		y_h = vec.y / Experiment::hash_step;
		z_h = vec.z / Experiment::hash_step;

		x_h = std::hash<std::size_t>{}(x_h);
		y_h = std::hash<std::size_t>{}(y_h);
		z_h = std::hash<std::size_t>{}(z_h);

		return x_h ^ y_h ^ z_h;
}


void get_line_x_hashes(Vector_3d &vec, size_t *vector, int &i)
{
	vec.x -= Experiment::hash_step;

	for(int k = 0; k < 3; k++) {
		vector[i] = get_hash(vec);
		vec.x += Experiment::hash_step;
		i++;
	}

	vec.x -= Experiment::hash_step;		
}

void get_plane_xy_hashes(Vector_3d &vec, size_t *vector, int &i)
{
	vec.y -= Experiment::hash_step;

	for(int k = 0; k < 3; k++) {
		get_line_x_hashes(vec, vector, i);
		vec.y += Experiment::hash_step;;	
	}

	vec.y -= Experiment::hash_step;;
}



void Experiment::get_near_hashes(Vector_3d &vec, size_t *vector)
{
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
	size_t vector_size = particles.size();
	for(size_t i = 0; i < vector_size; i++) {		
		htable.insert(std::pair<Vector_3d &,
			      Particle &>(*(particles[i].r), particles[i]));
	}
}

void Experiment::simulation()
{
	upd_hash_table();
	for(auto elem: particles) {
		size_t vector[27];
		get_near_hashes(*(elem.r), vector);		
	}	
}


