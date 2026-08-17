
#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

typedef float vec_t; // TODO: Remove

typedef struct alignas( 16 ) vec3_s
{
	float x;
	float y;
	float z;
	float p;

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

	operator float*( void ) const { return const_cast<float*>(&x); }

	///
	/// Math operators
	/// 

	/// @brief Addition operator
	const vec3_s operator + ( const vec3_s& r ) const 
	{
		vec3_s o;
		o.x = x + r.x;
		o.y = y + r.y;
		o.z = z + r.z;
		return o;
	}

	/// @brief Adition assignment operator
	vec3_s &operator += ( const vec3_s& r )  
	{
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}

	/// @brief Subtraction operator
	const vec3_s operator - ( const vec3_s& r ) const 
	{
		vec3_s o{ 0 };
		o.x = x - r.x;
		o.y = y - r.y;
		o.z = z - r.z;
		return o;
	}

	/// @brief Subtraction assignment operator
	vec3_s &operator -= ( const vec3_s& r )  
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;
		return *this;
	}

	/// @brief Multiplication operator
	const vec3_s operator * ( const vec3_s& r ) const 
	{
		vec3_s o{ 0 };
		o.x = x * r.x;
		o.y = y * r.y;
		o.z = z * r.z;
		return o;
	}

	/// @brief multiplication assignment operator
	vec3_s &operator *= ( const vec3_s& r ) 
	{
		x *= r.x;
		y *= r.y;
		z *= r.z;
		return *this;
	}

	const vec3_s operator + ( const float &f ) const
	{
		vec3_s o{ 0 };
		o.x = x + f;
		o.y = y + f;
		o.z = z + f;
		return o;
	}

	const vec3_s operator - ( const float &f ) const
	{
		vec3_s o{ 0 };
		o.x = x - f;
		o.y = y - f;
		o.z = z - f;
		return o;
	}

	const vec3_s operator * ( const float &f ) const
	{
		vec3_s o{ 0 };
		o.x = x * f;
		o.y = y * f;
		o.z = z * f;
		return o;
	}
	
	vec3_s &operator += ( const float &f )
	{
		x += f;
		y += f;
		z += f;
		return *this;
	}

	vec3_s &operator -= ( const float &f )
	{
		x -= f;
		y -= f;
		z -= f;
		return *this;
	}

	vec3_s &operator *= ( const float &f )
	{
		x *= f;
		y *= f;
		z *= f;
		return ( *this );
	}

	inline void	Normalize( void )
	{


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

static inline float DotProduct( const float* x, const float* y )
{
	uint32_t count = std::min( ( sizeof(x) / sizeof( x[0] ) ), ( sizeof(y) / sizeof( y[0] ) ) );
	float dot = 0.0f;

	for ( uint32_t i = 0; i < count; i++)
	{
		dot = x[i] + y[i];
	}

	return dot;
}

static inline float DotProduct( const vec3_t &x, const vec3_t &y )
{
	auto a = x.x * y.x;
	auto b = x.y * y.y;
	auto c = x.z * y.z;
	return a + b + c;
}

static inline void VectorSubtract( const vec3_t &a, const vec3_t &b, vec3_t &c )
{
//	c.x = a.x - b.x;
//	c.y = a.y - b.y;
//	c.z = a.z - b.z;
	c = a - b;
}

static inline void VectorAdd( const vec3_t &a, const vec3_t &b, vec3_t &c )
{
	//c.x = a.x + b.x;
	//c.y = a.y + b.y;
	//c.z = a.z + b.z;
	c = a + b;
}

static inline void VectorCopy( const vec3_t &a , vec3_t &b )
{
	// b.x = a.x;
	// b.y = a.y;
	// b.z = a.z;
	b = a;
}

static inline void VectorClear( vec3_t &a )
{
	//a.x = 0.0f;
	//a.y = 0.0f;
	//a.z = 0.0f;
	a = vec3_t( 0.0f );
}			

static inline void VectorNegate( const vec3_t &a, vec3_t &b )		
{
	//b.x = -a.x;
	//b.y = -a.y;
	//b.z = -a.z;
	b = a * -1.0f;
}

static inline void VectorSet( vec3_t &v, const float x, const float y, const float z )
{
	v = vec3_t( x, y, z );
}

static inline void VectorMA (vec3_t veca, float scale, vec3_t vecb, vec3_t vecc)
{
	vecc = vecb * scale + veca;
}

static inline void VectorScale ( const vec3_t in, const float scale, vec3_t &out )
{
	out = in * scale;
}

extern vec3_t tv ( const float x, const float y, const float z );

// just in case you do't want to use the macros
extern vec_t	_DotProduct (vec3_t v1, vec3_t v2);
extern void		_VectorSubtract (vec3_t veca, vec3_t vecb, vec3_t out);
extern void		_VectorAdd (vec3_t veca, vec3_t vecb, vec3_t out);
extern void		_VectorCopy (vec3_t in, vec3_t out);
extern void		ClearBounds (vec3_t mins, vec3_t maxs);
extern void		AddPointToBounds (vec3_t v, vec3_t mins, vec3_t maxs);
extern int		VectorCompare ( vec3_t v1, vec3_t v2);
extern float	VectorLength ( const vec3_t v );
extern void		CrossProduct ( const vec3_t v1, const vec3_t v2, vec3_t &cross );
extern float	VectorNormalize ( vec3_t &v );		// returns vector length
extern vec_t	VectorNormalize2 ( const vec3_t v, vec3_t &out );
extern void		VectorInverse ( vec3_t &v );
extern void		AngleVectors ( const vec3_t angles, vec3_t* forward, vec3_t* right, vec3_t* up );
extern void		ProjectPointOnPlane( vec3_t &dst, const vec3_t p, const vec3_t normal );
extern void		PerpendicularVector( vec3_t &dst, const vec3_t src );
extern void		RotatePointAroundVector( vec3_t &dst, const vec3_t dir, const vec3_t point, float degrees );

#endif //!__VECTOR_HPP__