#include <unordered_map>

struct vector_3d 
{
	double x, y, z;
};

struct particle
{
	struct vector_3d *r
	struct vector_3d *v;
};

struct wall
{
	struct vector_3d bot_left;
	struct vector_3d up_right;
};

class experiment
{
public:
	unordered_map<vector_3d, particle> htable;
	//std::vector<wall> walls;	
};

//пока разбираюсь с stl, коммитить буду на след неделе
