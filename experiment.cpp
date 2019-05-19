#include <cstdio>
#include <iostream>
#include <utility>
#include <functional>
#include "experiment.h"

Experiment::Experiment(double cube_size_)
{
	cube_size = cube_size_;
}

void get_line_x_buckets(Vector_3d::Cube &cube, size_t *buck, int &i)
{
	cube.x -= Experiment::cube_size;

	for(int k = 0; k < 3; k++) {
		buck[i] = get_hash(cube);
		cube.x += Experiment::cube_size;
		i++;
	}

	cube.x -= Experiment::cube_size;		
}

void get_plane_xy_buckets(Vector_3d::Cube &cube, size_t *buck, int &i)
{
	cube.y -= Experiment::cube_size;

	for(int k = 0; k < 3; k++) {
		get_line_x_buckets(cube, buck, i);
		cube.y += Experiment::cube_size;;	
	}

	cube.y -= Experiment::cube_size;;
}

void Experiment::get_near_buckets(Particle &p, size_t *buck, int &i)
{
	auto cube = p.cube;

	double size = Experiment::cube_size;

	cube.z -= size;

	for(int k = 0; k < 3; k++) {
		get_plane_xy_buckets(cube, buck, i);
		cube.z += size;	
	}

	cube.z -= size;
}

void Experiment::upd_hash_table()
{
	htable.clear();
	for(auto &p : particles) {
		p.set_cube();	
		//htable.insert(std::make_pair(p.cube, p));
		htable.insert(std::pair<Vector_3d::Cube, Particle &>(p.cube, p));
	}
}

void Experiment::simulation()
{
	int ptr = 0;
	int &i = ptr;
	upd_hash_table();
	for(auto & p: particles) {
		size_t buck[27];
		i = 0;
		get_near_buckets(p, buck, i);

	
	}	
}

void collision handler(size_t *buck)
{
	for(int k = 0; k < 27; k++) {
		auto iter = htable.equal_range(buck[k]); //здесь вместо buck[k]
								//нужен ключ типа cube, 
								//а не size_t
		for(auto it1 = iter.first; it1 != iter.second; it1++) {
			for(auto it2 = it1 + 1; it < iter.second; it2++) {
						
			}	
		}
	}	
}

