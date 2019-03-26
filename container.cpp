#include <unordered_map>
#include <math.h>


inline struct vector_3d vector_norm(vector_3d vec);
inline struct vector_3d vector_product(vector_3d vec_a, vector_3d vec_b);

struct vector_3d 
{
	double x, y, z;
};

struct particle
{
	struct vector_3d *r;
	struct vector_3d *v;
};

struct wall
{
	struct vector_3d vec_r;
	struct vector_3d vec_a;
	struct vector_3d vec_b;
	struct vector_3d vec_n;
	
	wall(vector_3d vec_r_, vector_3d vec_a_, vector_3d vec_b_)
	{
		vec_r = vec_r_;
		vec_a = vec_a_;
		vec_b = vec_b_;
		vec_n = vector_norm(vector_product(vec_a_, vec_b_));
	}
};

/*
class experiment
{
public:
	unordered_map<vector_3d, particle> htable;
	//std::vector<wall> walls;	
};
*/

inline double scalar_product(vector_3d vec_a, vector_3d vec_b)
{
}

inline struct vector_3d vector_product(vector_3d vec_a, vector_3d vec_b)
{				
	struct vector_3d *vec = new vector_3d;
	vec->x = vec_a.y * vec_b.z - vec_a.z * vec_b.y;
	vec->y = vec_a.z * vec_b.x - vec_a.x * vec_b.z;
	vec->z = vec_a.x * vec_b.y - vec_a.y * vec_b.x;
	return *vec;
}

inline struct vector_3d vector_norm(vector_3d vec)
{
	struct vector_3d *vec_n = new vector_3d;
	double norm = sqrt(vec.x*vec.x + vec.y*vec.y +vec.z*vec.z);
	vec_n->x = vec.x/norm;
	vec_n->y = vec.y/norm;
	vec_n->z = vec.z/norm; 
	return *vec_n;	
}

