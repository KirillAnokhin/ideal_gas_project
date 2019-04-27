#include <cstdio>
#include <iostream>
#include <utility>
#include <functional>
#include "container.h"

void experiment::upd_hash_table()
{
	htable.clear();
	size_t vector_size = particles.size();
	for(size_t i = 0; i < vector_size; i++) {		
		htable.insert(std::pair<Vector_3d &, Particle &>(*(particles[i].r), particles[i]));
	}
}

void experiment::simulation()
{
	upd_hash_table();
	//for(auto elem: )	
}


