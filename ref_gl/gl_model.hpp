/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __GL_MODEL_HPP__
#define __GL_MODEL_HPP__

/*

d*_t structures are on-disk representations
m*_t structures are in-memory

*/

/*
==============================================================================

BRUSH MODELS

==============================================================================
*/


//
// in memory representation
//
// !!! if this is changed, it must be changed in asm_draw.h too !!!
typedef struct
{
	vec3_t		position;
} mvertex_t;

typedef struct
{
	vec3_t		mins, maxs;
	vec3_t		origin;		// for sounds or lights
	float		radius;
	int			headnode;
	int			visleafs;		// not including the solid leaf 0
	int			firstface, numfaces;
} mmodel_t;

inline constexpr uint32_t SIDE_FRONT	= 0u;
inline constexpr uint32_t SIDE_BACK 	= 1u;
inline constexpr uint32_t SIDE_ON		= 2u;

inline constexpr uint32_t	SURF_PLANEBACK		= 2u;
inline constexpr uint32_t	SURF_DRAWSKY		= 4u;
inline constexpr uint32_t	SURF_DRAWTURB		= 0x10;
inline constexpr uint32_t	SURF_DRAWBACKGROUND	= 0x40;
inline constexpr uint32_t	SURF_UNDERWATER		= 0x80;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
typedef struct
{
	unsigned short	v[2];
	unsigned int	cachededgeoffset;
} medge_t;

typedef struct mtexinfo_s
{
	float		vecs[2][4];
	int			flags;
	int			numframes;
	struct mtexinfo_s	*next;		// animation chain
	image_t		*image;
} mtexinfo_t;

#define	VERTEXSIZE	7

typedef struct glpoly_s
{
	struct	glpoly_s	*next;
	struct	glpoly_s	*chain;
	int		numverts;
	int		flags;			// for SURF_UNDERWATER (not needed anymore?)
	float	verts[4][VERTEXSIZE];	// variable sized (xyz s1t1 s2t2)
} glpoly_t;

typedef struct msurface_s
{
	int			visframe;		// should be drawn when node is crossed

	cplane_t	*plane;
	int			flags;

	int			firstedge;	// look up in model->surfedges[], negative numbers
	int			numedges;	// are backwards edges
	
	short		texturemins[2];
	short		extents[2];

	int			light_s, light_t;	// gl lightmap coordinates
	int			dlight_s, dlight_t; // gl lightmap coordinates for dynamic lightmaps

	glpoly_t	*polys;				// multiple if warped
	struct	msurface_s	*texturechain;
	struct  msurface_s	*lightmapchain;

	mtexinfo_t	*texinfo;
	
// lighting info
	int			dlightframe;
	int			dlightbits;

	int			lightmaptexturenum;
	byte		styles[MAXLIGHTMAPS];
	float		cached_light[MAXLIGHTMAPS];	// values currently used in lightmap
	byte		*samples;		// [numstyles*surfsize]
} msurface_t;

typedef struct mnode_s
{
// common with leaf
	int			contents;		// -1, to differentiate from leafs
	int			visframe;		// node needs to be traversed if current
	
	float		minmaxs[6];		// for bounding box culling

	struct mnode_s	*parent;

// node specific
	cplane_t	*plane;
	struct mnode_s	*children[2];	

	unsigned short		firstsurface;
	unsigned short		numsurfaces;
} mnode_t;

typedef struct mleaf_s
{
// common with node
	int			contents;		// wil be a negative contents number
	int			visframe;		// node needs to be traversed if current

	float		minmaxs[6];		// for bounding box culling

	struct mnode_s	*parent;

// leaf specific
	int			cluster;
	int			area;

	msurface_t	**firstmarksurface;
	int			nummarksurfaces;
} mleaf_t;


//===================================================================

//
// Whole model
//

typedef enum { MOD_BAD, MOD_BRUSH, MOD_SPRITE, MOD_ALIAS } modtype_t;

inline constexpr uint32_t MAX_MOD_KNOWN	= 512;


//============================================================================

class glModel
{
public:
	glModel( void );
	~glModel( void );

	void	Clear( void );

	inline void SetFirstModelSurface( const int first )
	{
		firstmodelsurface = first;
	}

	inline void	SetFrames( const uint32_t frames ) 
	{
		numframes = frames;
	}

	inline void SetRegistrationSequence( const uint32_t sequence )
	{
		registration_sequence = sequence; 
	}

	inline void SetType( const modtype_t modelType )
	{
		type = modelType;
	}

	inline void ClearName( void )
	{
		memset ( const_cast<char*>( name ), 0, sizeof( name ) );
	}

	inline void	SetName( const char* newName )
	{
		strncpy( const_cast<char*>( name ), newName, MAX_QPATH );
	}
	
	inline const char*	Name( void ) const { return name; }
	inline uint32_t		NumTexInfo( void ) const { return numtexinfo; }
	inline const int	ExtraDataSize( void ) const { return extradatasize; }
	inline void	*		ExtraData( void ) const { return extradata; };
	inline uint32_t		Numsubmodels( void ) const { return numsubmodels; }
	inline mmodel_t*	SubModels( void ) const { return submodels; }
	inline uint32_t 	RegistrationSequence( void ) const { return registration_sequence; }
	inline uint32_t		FirstModelSurface( void ) const { return firstmodelsurface; }
	inline modtype_t 	Type( void ) const { return type; }
	inline byte*		LightData( void ) const { return lightdata; }
	inline mnode_t*		Nodes( void ) const { return nodes; }
	inline msurface_t*	Surfaces( void ) const { return surfaces; }
	
	inline void	AllocExtraData( const size_t size )
	{
		extradatasize = size;
		extradata = malloc( extradatasize );
	}

	inline void FreeExtraData( void )
	{
		if( extradata != nullptr )
		{
			free( extradata );
			extradata = nullptr;
		}

		extradatasize = 0;
	}
	
	void LoadBrush( dheader_t	*header );

private:
	const char	name[MAX_QPATH];

	uint32_t	registration_sequence;

	modtype_t	type;
	int			numframes;
	
	int			flags;

//
// volume occupied by the model graphics
//		
	vec3_t		mins, maxs;
	float		radius;

//
// solid volume for clipping 
//
	bool		clipbox;
	vec3_t		clipmins, clipmaxs;

//
// brush model
//
	int			firstmodelsurface, nummodelsurfaces;
	int			lightmap;		// only for submodels

	uint32_t	numsubmodels;
	mmodel_t	*submodels;

	uint32_t	numplanes;
	cplane_t	*planes;

	uint32_t	numleafs;		// number of visible leafs, not counting 0
	mleaf_t		*leafs;

	uint32_t	numvertexes;
	mvertex_t	*vertexes;

	uint32_t	numedges;
	medge_t		*edges;

	uint32_t	numnodes;
	int			firstnode;
	mnode_t		*nodes;

	uint32_t	numtexinfo;
	mtexinfo_t	*texinfo;

	int			numsurfaces;
	msurface_t	*surfaces;

	int			numsurfedges;
	int			*surfedges;

	int			nummarksurfaces;
	msurface_t	**marksurfaces;

	dvis_t		*vis;

	byte		*lightdata;

	// for alias models and skins
	image_t		*skins[MAX_MD2SKINS];

	size_t		extradatasize;
	void		*extradata;

	mleaf_t*	PointInLeaf ( vec3_t p );
	byte*		DecompressVis ( byte *in );
	byte*		ClusterPVS ( int cluster );
	void 		CalcSurfaceExtents ( msurface_t *s );
	void		LoadLighting ( lump_t *l );	//
	void		LoadVisibility ( lump_t *l );
	void		LoadVertexes ( lump_t *l ); //
	void		LoadSubmodels ( lump_t *l );
	void		LoadEdges ( lump_t *l ); //
	void		LoadTexinfo ( lump_t *l );	///
	void		LoadFaces ( lump_t *l );	//
	void		SetParent ( mnode_t *node, mnode_t *parent );
	void		LoadNodes ( lump_t *l );
	void		LoadLeafs( lump_t *l );
	void		LoadMarksurfaces ( lump_t *l);
	void		LoadSurfedges ( lump_t *l );	//
	void		LoadPlanes ( lump_t *l );		//
};

class glModelManager
{
public:
	glModelManager( void );
	~glModelManager( void );

	void		Init (void);
	void		ClearAll (void);
	void		BeginRegistration ( const char *model );
	glModel*	RegisterModel ( const char *name);
	void		EndRegistration (void);
	void		Modellist( void );
	glModel*	ForName ( const char *name, bool crash );
	glModel*	LoadModel ( glModel *mod, bool crash);
	glModel*	PointInLeaf (float *p, glModel *model);
	byte*		ClusterPVS (int cluster, glModel *model);
	void		FreeAll (void);
	void		Free ( glModel *mod );

private:
	uint32_t	registration_sequence;
	uint32_t	mod_numknown;
	glModel*	loadmodel;

	glModel		mod_known[MAX_MOD_KNOWN];

	// the inline * models from the current map are kept seperate
	glModel		mod_inline[MAX_MOD_KNOWN];

	void LoadSpriteModel ( glModel *mod, void *buffer );
	void LoadBrushModel ( glModel *mod, void *buffer );
	void LoadAliasModel ( glModel *mod, void *buffer );
};

extern glModelManager Mod;

#endif //!__GL_MODEL_HPP__