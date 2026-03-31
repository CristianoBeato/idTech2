
#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

typedef float vec_t;

typedef struct vec3_s
{
	float x;
	float y;
	float z;

	vec3_s( void )
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vec3_s( const vec3_s &v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	vec3_s( const float v )
	{
		x = v;
		y = v;
		z = v;
	}

	vec3_s( const float _x, const float _y, const float _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}

	const vec3_s operator = ( const vec3_s& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	const float operator[]( int i ) const
	{
		/// TODO:implement an assertion
		return  (&x)[i];
	}

	float &operator[]( int i )
	{
		/// TODO:implement an assertion
		return  (&x)[i];
	}

} vec3_t;

typedef struct vec5_s
{
	float x;
	float y;
	float z;
	float w;
	float k;

	const float operator[]( int i ) const
	{
		/// TODO:implement an assertion
		return  (&x)[i];
	}

	float &operator[]( int i )
	{
		/// TODO:implement an assertion
		return  (&x)[i];
	}

} vec5_t;

static inline float DotProduct( vec3_t x, vec3_t y )
{
	return x.x * y.x + x.y * y.y + x.z * y.z;
}

static inline void VectorSubtract( const vec3_t &a, const vec3_t &b, vec3_t &c )
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

static inline void VectorAdd( const vec3_t &a, const vec3_t &b, vec3_t &c )
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

static inline void VectorCopy( const vec3_t &a , vec3_t &b )
{
	b.x = a.x;
	b.y = a.y;
	b.z = a.z;
}

static inline void VectorClear( vec3_t &a )
{
	a.x = 0.0f;
	a.y = 0.0f;
	a.z = 0.0f;
}			

static inline void VectorNegate( const vec3_t &a, vec3_t &b )		
{
	b.x = -a.x;
	b.y = -a.y;
	b.z = -a.z;
}

static inline void VectorSet( vec3_t &v, const float x, const float y, const float z )
{
	v = vec3_t( x, y, z );
}

static inline void VectorMA (vec3_t veca, float scale, vec3_t vecb, vec3_t vecc)
{
	vecc.x = veca.x + scale * vecb.x;
	vecc.y = veca.y + scale * vecb.y;
	vecc.z = veca.z + scale * vecb.z;
}

// just in case you do't want to use the macros
extern vec_t	_DotProduct (vec3_t v1, vec3_t v2);
extern void		_VectorSubtract (vec3_t veca, vec3_t vecb, vec3_t out);
extern void		_VectorAdd (vec3_t veca, vec3_t vecb, vec3_t out);
extern void		_VectorCopy (vec3_t in, vec3_t out);
extern void		ClearBounds (vec3_t mins, vec3_t maxs);
extern void		AddPointToBounds (vec3_t v, vec3_t mins, vec3_t maxs);
extern int		VectorCompare (vec3_t v1, vec3_t v2);
extern vec_t	VectorLength (vec3_t v);
extern void		CrossProduct (vec3_t v1, vec3_t v2, vec3_t cross);
extern vec_t	VectorNormalize (vec3_t v);		// returns vector length
extern vec_t	VectorNormalize2 (vec3_t v, vec3_t out);
extern void		VectorInverse (vec3_t v);
extern void		VectorScale (vec3_t in, vec_t scale, vec3_t out);
extern void		AngleVectors (vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);

#endif //!__VECTOR_HPP__