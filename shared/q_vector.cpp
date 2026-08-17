
#include "q_vector.hpp"

#define DEG2RAD( a ) ( a * M_PI ) / 180.0F

/*
=============
TempVector

This is just a convenience function
for making temporary vectors for function calls
=============
*/
vec3_t tv ( const float x, const float y, const float z )
{
	static	int		index;
	static	vec3_t	vecs[8];
	vec3_t	v;

	// use an array so that multiple tempvectors won't collide
	// for a while
	v = vecs[index];
	index = (index + 1)&7;

	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

vec_t _DotProduct (vec3_t v1, vec3_t v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

void _VectorSubtract (vec3_t veca, vec3_t vecb, vec3_t out)
{
	out.x = veca.x-vecb.x;
	out.y = veca.y-vecb.y;
	out.z = veca.z-vecb.z;
}

void _VectorAdd (vec3_t veca, vec3_t vecb, vec3_t out)
{
	out.x = veca.x+vecb.x;
	out.y = veca.y+vecb.y;
	out.z = veca.z+vecb.z;
}

void _VectorCopy (vec3_t in, vec3_t out)
{
	out.x = in.x;
	out.y = in.y;
	out.z = in.z;
}

void ClearBounds (vec3_t mins, vec3_t maxs)
{
	mins.x = mins.y = mins.z = 99999;
	maxs.x = maxs.y = maxs.z = -99999;
}

void AddPointToBounds (vec3_t v, vec3_t mins, vec3_t maxs)
{
	int		i;
	vec_t	val;

	for (i=0 ; i<3 ; i++)
	{
		val = v[i];
		if (val < mins[i])
			mins[i] = val;
		if (val > maxs[i])
			maxs[i] = val;
	}
}

int VectorCompare (vec3_t v1, vec3_t v2)
{
	if (v1.x != v2.x || v1.y != v2.y || v1.z != v2.z)
			return 0;
			
	return 1;
}

float VectorLength( const vec3_t v)
{
	int		i;
	float	length;
	
	length = 0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = std::sqrt (length);		// FIXME

	return length;
}

void CrossProduct ( const vec3_t v1, const vec3_t v2, vec3_t &cross)
{
	cross.x = v1.y * v2.z - v1.z * v2.y;
	cross.y = v1.z * v2.x - v1.x * v2.z;
	cross.z = v1.x * v2.y - v1.y * v2.x;
}

float VectorNormalize ( vec3_t &v )
{
	float	length, ilength;

	length = v.x *v.x + v.y*v.y + v.z*v.z;
	length = std::sqrt ( length );		// FIXME

	if ( length )
	{
		ilength = 1/length;
		v *= ilength;
	}
		
	return length;

}

float VectorNormalize2 ( const vec3_t v, vec3_t &out )
{
	float	length, ilength;

	length = v.x*v.x + v.y*v.y + v.z*v.z;
	length = std::sqrt (length);		// FIXME

	if (length)
	{
		ilength = 1 / length;
		out = v * ilength;
	}
		
	return length;

}

void VectorInverse ( vec3_t &v )
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
}

void AngleVectors ( const vec3_t angles, vec3_t* forward, vec3_t* right, vec3_t* up )
{
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * (M_PI*2 / 360);
	sy = std::sin(angle);
	cy = std::cos(angle);
	angle = angles[PITCH] * (M_PI*2 / 360);
	sp = std::sin(angle);
	cp = std::cos(angle);
	angle = angles[ROLL] * (M_PI*2 / 360);
	sr = std::sin(angle);
	cr = std::cos(angle);

	if ( forward )
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	
	if ( right )
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * (-sy) );
		right->y = (-1 * sr * sp * sy + -1 * cr *cy );
		right->z = -1*sr*cp;
	}

	if (up)
	{
		up->x = ( cr* sp *cy + ( -sr ) * (-sy ) );
		up->y = ( cr * sp * sy+ ( -sr ) * cy);
		up->z = cr * cp;
	}
}

void ProjectPointOnPlane( vec3_t &dst, const vec3_t p, const vec3_t normal )
{
	float d;
	vec3_t n;
	float inv_denom;

	inv_denom = 1.0F / DotProduct( normal, normal );

	d = DotProduct( normal, p ) * inv_denom;

	n.x = normal.x * inv_denom;
	n.y = normal.y * inv_denom;
	n.z = normal.z * inv_denom;

	dst.x = p.x - d * n.x;
	dst.y = p.y - d * n.y;
	dst.z = p.z - d * n.z;
}


//
// assumes "src" is normalized
//
void PerpendicularVector( vec3_t &dst, const vec3_t src )
{
	int	pos;
	int i;
	float minelem = 1.0F;
	vec3_t tempvec;

	/*
	** find the smallest magnitude axially aligned vector
	*/
	for ( pos = 0, i = 0; i < 3; i++ )
	{
		if ( std::fabs( src[i] ) < minelem )
		{
			pos = i;
			minelem = std::fabs( src[i] );
		}
	}
	tempvec.x = tempvec.y = tempvec.z = 0.0F;
	tempvec[pos] = 1.0F;

	// project the point onto the plane defined by src
	ProjectPointOnPlane( dst, tempvec, src );

	// normalize the result
	VectorNormalize( dst );
}

void RotatePointAroundVector( vec3_t &dst, const vec3_t dir, const vec3_t point, const float degrees )
{
	int	i;
	alignas( 16 ) float	m[3][3];
	alignas( 16 ) float	im[3][3];
	alignas( 16 ) float	zrot[3][3];
	alignas( 16 ) float	tmpmat[3][3];
	alignas( 16 ) float	rot[3][3];
	vec3_t vr, vup, vf;

	vf = dir;

	PerpendicularVector( vr, dir );
	CrossProduct( vr, vf, vup );

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	memcpy( im, m, sizeof( im ) );

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

	memset( zrot, 0, sizeof( zrot ) );
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	zrot[0][0] = cos( DEG2RAD( degrees ) );
	zrot[0][1] = sin( DEG2RAD( degrees ) );
	zrot[1][0] = -sin( DEG2RAD( degrees ) );
	zrot[1][1] = cos( DEG2RAD( degrees ) );

	R_ConcatRotations( m, zrot, tmpmat );
	R_ConcatRotations( tmpmat, im, rot );

	for ( i = 0; i < 3; i++ )
	{
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
	}
}