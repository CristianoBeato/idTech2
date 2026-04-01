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
// models.cpp -- model loading and caching

#include "gl_local.hpp"
#include "gl_model.hpp"

glModelManager Mod;

int		modfilelen;
byte	mod_novis[MAX_MAP_LEAFS/8];

glModel::glModel( void ) :
	type( MOD_BAD ),
	clipbox( false ),
	radius( 0.0f ),
	registration_sequence( 0 ),
	flags( 0 ),
	firstmodelsurface( 0 ),
	lightmap( 0 ),
	numframes( 0 ),
	nummodelsurfaces( 0 ),
	numsubmodels( 0 ),
	numplanes( 0 ),
	numleafs( 0 ),
	numvertexes( 0 ),
	numedges( 0 ),
	numnodes( 0 ),
	numtexinfo( 0 ),
	numsurfedges( 0 ),
	numsurfaces( 0 ),
	nummarksurfaces( 0 ),
	firstnode( 0 ),
	extradatasize( 0 ),
	submodels( nullptr ),
	planes( nullptr ),
	leafs( nullptr ),
	vertexes( nullptr ),
	edges( nullptr ),
	nodes( nullptr ),
	texinfo( nullptr ),
	surfaces( nullptr ),
	surfedges( nullptr ),
	marksurfaces( nullptr ),
	vis( nullptr ),
	lightdata( nullptr )
{
}

glModel::~glModel( void )
{
	Clear();
}

void glModel::LoadBrush(dheader_t *header)
{
	LoadVertexes (&header->lumps[LUMP_VERTEXES]);
	LoadEdges (&header->lumps[LUMP_EDGES]);
	LoadSurfedges (&header->lumps[LUMP_SURFEDGES]);
	LoadLighting (&header->lumps[LUMP_LIGHTING]);
	LoadPlanes (&header->lumps[LUMP_PLANES]);
	LoadTexinfo (&header->lumps[LUMP_TEXINFO]);
	LoadFaces (&header->lumps[LUMP_FACES]);
	LoadMarksurfaces (&header->lumps[LUMP_LEAFFACES]);
	LoadVisibility (&header->lumps[LUMP_VISIBILITY]);
	LoadLeafs (&header->lumps[LUMP_LEAFS]);
	LoadNodes (&header->lumps[LUMP_NODES]);
	LoadSubmodels (&header->lumps[LUMP_MODELS]);
	numframes = 2;		// regular and alternate animation
}

/*
===============
glModel::PointInLeaf
===============
*/
mleaf_t *glModel::PointInLeaf ( vec3_t p )
{
	float		d = 0.0f;
	mnode_t		*node = nullptr;
	cplane_t	*plane = nullptr;
	
	if ( !nodes )
		ri.Sys_Error (ERR_DROP, "Mod_PointInLeaf: bad model");

	node = nodes;
	while (1)
	{
		if (node->contents != -1)
			return (mleaf_t *)node;

		plane = node->plane;
		d = DotProduct (p,plane->normal) - plane->dist;
		if (d > 0)
			node = node->children[0];
		else
			node = node->children[1];
	}
	
	return nullptr;	// never reached
}


/*
===================
Mod_DecompressVis
===================
*/
byte* glModel::DecompressVis ( byte *in )
{
	static byte	decompressed[MAX_MAP_LEAFS/8];
	int		c;
	byte	*out;
	int		row;

	row = ( vis->numclusters+7 ) >>3;	
	out = decompressed;

	if (!in)
	{	// no vis info, so make all visible
		while (row)
		{
			*out++ = 0xff;
			row--;
		}
		return decompressed;		
	}

	do
	{
		if (*in)
		{
			*out++ = *in++;
			continue;
		}
	
		c = in[1];
		in += 2;
		while (c)
		{
			*out++ = 0;
			c--;
		}
	} while (out - decompressed < row);
	
	return decompressed;
}

/*
==============
Mod_ClusterPVS
==============
*/
byte *glModel::ClusterPVS ( int cluster )
{
	if ( cluster == -1 || !vis)
		return mod_novis;

	return DecompressVis ( (byte *)vis + vis->bitofs[cluster][DVIS_PVS] );
}


//===============================================================================
void Mod_Modellist_f (void)
{
	Mod.Modellist();
}


/*
================
glModelManager::Modellist
================
*/
void glModelManager::Modellist( void )
{
	int		i = 0;
	int		total = 0;
	glModel	*mod = nullptr;

	total = 0;
	ri.Con_Printf (PRINT_ALL,"Loaded models:\n");
	for ( i = 0, mod = mod_known ; i < mod_numknown ; i++, mod++)
	{
		auto name = mod->Name();
		if ( !name[0] )
			continue;
		ri.Con_Printf (PRINT_ALL, "%8i : %s\n",mod->ExtraDataSize(), mod->Name() );
		total += mod->ExtraDataSize();
	}
	ri.Con_Printf (PRINT_ALL, "Total resident: %i\n", total);
}

/*
===============
glModelManager::Init
===============
*/
void glModelManager::Init (void)
{
	memset (mod_novis, 0xff, sizeof(mod_novis));
}

/*
==================
Mod_ForName

Loads in a model for the given name
==================
*/
glModel *glModelManager::ForName ( const char *name, const bool crash )
{
	int		i = 0;
	unsigned int *buf = nullptr;
	glModel	*mod = nullptr;
	
	if (!name[0])
		ri.Sys_Error (ERR_DROP, "Mod_ForName: NULL name");
		
	//
	// inline models are grabbed only from worldmodel
	//
	if (name[0] == '*')
	{
		i = atoi(name+1);
		if (i < 1 || !r_worldmodel || i >= r_worldmodel->NumSubModels() )
			ri.Sys_Error (ERR_DROP, "bad inline model number");
		return &mod_inline[i];
	}

	//
	// search the currently loaded models
	//
	for (i=0 , mod=mod_known ; i<mod_numknown ; i++, mod++)
	{
		if (!mod->Name()[0])
			continue;

		if (!strcmp( mod->Name(), name ) )
			return mod;
	}
	
	//
	// find a free model slot spot
	//
	for (i=0 , mod = mod_known ; i<mod_numknown ; i++, mod++)
	{
		if (!mod->Name()[0])
			break;	// free spot
	}

	if (i == mod_numknown)
	{
		if (mod_numknown == MAX_MOD_KNOWN)
			ri.Sys_Error (ERR_DROP, "mod_numknown == MAX_MOD_KNOWN");
		mod_numknown++;
	}

	//strcpy (mod->name, name);
	mod->SetName( name );
	
	//
	// load the file
	//
	modfilelen = ri.FS_LoadFile( mod->Name(), (void**)&buf );
	if (!buf)
	{
		if (crash)
			ri.Sys_Error (ERR_DROP, "Mod_NumForName: %s not found", mod->Name() );
		//memset (mod->name, 0, sizeof(mod->name));
		mod->ClearName();
		return nullptr;
	}
	
	loadmodel = mod;

	//
	// fill it in
	//


	// call the apropriate loader
	switch (LittleLong(*(unsigned *)buf))
	{
	case IDALIASHEADER:
		loadmodel->AllocExtraData( 0x200000 );// loadmodel->extradata = Hunk_Begin (0x200000);
		LoadAliasModel (mod, buf);
		break;
		
	case IDSPRITEHEADER:
		loadmodel->AllocExtraData( 0x10000 ); // loadmodel->extradata = Hunk_Begin (0x10000);
		LoadSpriteModel (mod, buf);
		break;
	
	case IDBSPHEADER:
		loadmodel->AllocExtraData( 0x1000000 );// loadmodel->extradata = Hunk_Begin (0x1000000);
		LoadBrushModel (mod, buf);
		break;

	default:
		ri.Sys_Error (ERR_DROP,"Mod_NumForName: unknown fileid for %s", mod->Name() );
		break;
	}

	// loadmodel->extradatasize = Hunk_End ();

	ri.FS_FreeFile (buf);

	return mod;
}

/*
===============================================================================

					BRUSHMODEL LOADING

===============================================================================
*/

byte	*mod_base;


/*
=================
glModel::LoadLighting
=================
*/
void glModel::LoadLighting ( lump_t *l )
{
	if (!l->filelen)
	{
		lightdata = nullptr;
		return;
	}

	lightdata = static_cast<byte*>(Hunk_Alloc ( l->filelen) );	
	memcpy ( lightdata, mod_base + l->fileofs, l->filelen );
}


/*
=================
glModel::LoadVisibility
=================
*/
void glModel::LoadVisibility (lump_t *l)
{
	int		i = 0;

	if (!l->filelen)
	{
		vis = nullptr;
		return;
	}

	vis = static_cast<dvis_t*>( Hunk_Alloc ( l->filelen) );	
	memcpy ( vis, mod_base + l->fileofs, l->filelen);

	vis->numclusters = LittleLong ( vis->numclusters );
	for (i=0 ; i < vis->numclusters ; i++)
	{
		vis->bitofs[i][0] = LittleLong ( vis->bitofs[i][0]);
		vis->bitofs[i][1] = LittleLong ( vis->bitofs[i][1]);
	}
}


/*
=================
glModel::LoadVertexes
=================
*/
void glModel::LoadVertexes ( lump_t *l )
{
	int i = 0, count = 0;
	dvertex_t	*in = nullptr;
	mvertex_t	*out = nullptr;

	in = reinterpret_cast<dvertex_t*>( mod_base + l->fileofs );
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	out = static_cast<mvertex_t*>( Hunk_Alloc ( count*sizeof(*out)) );	

	vertexes = out;
	numvertexes = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		out->position[0] = LittleFloat (in->point[0]);
		out->position[1] = LittleFloat (in->point[1]);
		out->position[2] = LittleFloat (in->point[2]);
	}
}

/*
=================
RadiusFromBounds
=================
*/
float RadiusFromBounds (vec3_t mins, vec3_t maxs)
{
	int		i;
	vec3_t	corner;

	for (i=0 ; i<3 ; i++)
	{
		corner[i] = fabs(mins[i]) > fabs(maxs[i]) ? fabs(mins[i]) : fabs(maxs[i]);
	}

	return VectorLength (corner);
}


/*
=================
glModel::LoadSubmodels
=================
*/
void glModel::LoadSubmodels ( lump_t *l )
{
	int			i = 0, j= 0, count= 0;
	dmodel_t	*in = nullptr;
	mmodel_t	*out = nullptr;

	in =  reinterpret_cast<dmodel_t*>( mod_base + l->fileofs );
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	out = static_cast<mmodel_t*>( Hunk_Alloc ( count * sizeof( *out) ) );	

	submodels = out;
	numsubmodels = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{	// spread the mins / maxs by a pixel
			out->mins[j] = LittleFloat (in->mins[j]) - 1;
			out->maxs[j] = LittleFloat (in->maxs[j]) + 1;
			out->origin[j] = LittleFloat (in->origin[j]);
		}
		out->radius = RadiusFromBounds (out->mins, out->maxs);
		out->headnode = LittleLong (in->headnode);
		out->firstface = LittleLong (in->firstface);
		out->numfaces = LittleLong (in->numfaces);
	}
}

/*
=================
glModel::LoadEdges
=================
*/
void glModel::LoadEdges ( lump_t *l )
{
	int 	i = 0, count = 0;
	dedge_t *in = nullptr;
	medge_t *out = nullptr;

	in = reinterpret_cast<dedge_t*>( mod_base + l->fileofs );
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	out = static_cast<medge_t*>( Hunk_Alloc ( (count + 1) * sizeof(*out) ) );	

	edges = out;
	numedges = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		out->v[0] = ( unsigned short )LittleShort(in->v[0]);
		out->v[1] = ( unsigned short )LittleShort(in->v[1]);
	}
}

/*
=================
glModel::LoadTexinfo
=================
*/
void glModel::LoadTexinfo ( lump_t *l )
{
	texinfo_t *in;
	mtexinfo_t *out, *step;
	int 	i, j, count;
	char	name[MAX_QPATH];
	int		next;

	in = reinterpret_cast<texinfo_t*>(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	out = static_cast<mtexinfo_t*>( Hunk_Alloc ( count*sizeof(*out) ) );	

	texinfo = out;
	numtexinfo = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<8 ; j++)
			out->vecs[0][j] = LittleFloat (in->vecs[0][j]);

		out->flags = LittleLong (in->flags);
		next = LittleLong (in->nexttexinfo);
		if (next > 0)
			out->next = texinfo + next;
		else
		    out->next = NULL;
		Com_sprintf (name, sizeof(name), "textures/%s.wal", in->texture);

		out->image = GL_FindImage (name, it_wall);
		if (!out->image)
		{
			ri.Con_Printf (PRINT_ALL, "Couldn't load %s\n", name);
			out->image = r_notexture;
		}
	}

	// count animation frames
	for (i=0 ; i<count ; i++)
	{
		out = &texinfo[i];
		out->numframes = 1;
		for (step = out->next ; step && step != out ; step=step->next)
			out->numframes++;
	}
}

/*
================
glModel::CalcSurfaceExtents

Fills in s->texturemins[] and s->extents[]
================
*/
void glModel::CalcSurfaceExtents ( msurface_t *s )
{
	float	mins[2], maxs[2], val;
	int		i,j, e;
	mvertex_t	*v;
	mtexinfo_t	*tex;
	int		bmins[2], bmaxs[2];

	mins[0] = mins[1] = 999999;
	maxs[0] = maxs[1] = -99999;

	tex = s->texinfo;
	
	for (i=0 ; i<s->numedges ; i++)
	{
		e = surfedges[s->firstedge+i];
		if (e >= 0)
			v = &vertexes[edges[e].v[0]];
		else
			v = &vertexes[edges[-e].v[1]];
		
		for (j=0 ; j<2 ; j++)
		{
			val = v->position[0] * tex->vecs[j][0] + 
				v->position[1] * tex->vecs[j][1] +
				v->position[2] * tex->vecs[j][2] +
				tex->vecs[j][3];
			if (val < mins[j])
				mins[j] = val;
			if (val > maxs[j])
				maxs[j] = val;
		}
	}

	for (i=0 ; i<2 ; i++)
	{	
		bmins[i] = floor(mins[i]/16);
		bmaxs[i] = ceil(maxs[i]/16);

		s->texturemins[i] = bmins[i] * 16;
		s->extents[i] = (bmaxs[i] - bmins[i]) * 16;

//		if ( !(tex->flags & TEX_SPECIAL) && s->extents[i] > 512 /* 256 */ )
//			ri.Sys_Error (ERR_DROP, "Bad surface extents");
	}
}

extern void GL_BuildPolygonFromSurface(msurface_t *fa);
extern void GL_CreateSurfaceLightmap (msurface_t *surf);
extern void GL_EndBuildingLightmaps (void);
extern void GL_BeginBuildingLightmaps ( glModel *m);
extern void GL_SubdivideSurface (msurface_t *fa);

/*
=================
glModel::LoadFaces
=================
*/
void glModel::LoadFaces (lump_t *l)
{
	dface_t		*in;
	msurface_t 	*out;
	int			i, count, surfnum;
	int			planenum, side;
	int			ti;

	in = reinterpret_cast<dface_t*>( mod_base + l->fileofs );
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	out = static_cast<msurface_t*>( Hunk_Alloc ( count*sizeof(*out) ) );	

	surfaces = out;
	numsurfaces = count;

	currentmodel = this;

	GL_BeginBuildingLightmaps ( this );

	for ( surfnum=0 ; surfnum<count ; surfnum++, in++, out++)
	{
		out->firstedge = LittleLong(in->firstedge);
		out->numedges = LittleShort(in->numedges);		
		out->flags = 0;
		out->polys = NULL;

		planenum = LittleShort(in->planenum);
		side = LittleShort(in->side);
		if (side)
			out->flags |= SURF_PLANEBACK;			

		out->plane = planes + planenum;

		ti = LittleShort (in->texinfo);
		if (ti < 0 || ti >= numtexinfo )
			ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: bad texinfo number");
		out->texinfo = texinfo + ti;

		CalcSurfaceExtents (out);
				
	// lighting info

		for (i=0 ; i<MAXLIGHTMAPS ; i++)
			out->styles[i] = in->styles[i];
		i = LittleLong(in->lightofs);
		if (i == -1)
			out->samples = NULL;
		else
			out->samples = lightdata + i;
		
	// set the drawing flags
		
		if (out->texinfo->flags & SURF_WARP)
		{
			out->flags |= SURF_DRAWTURB;
			for (i=0 ; i<2 ; i++)
			{
				out->extents[i] = 16384;
				out->texturemins[i] = -8192;
			}
			GL_SubdivideSurface (out);	// cut up polygon for warps
		}

		// create lightmaps and polygons
		if ( !(out->texinfo->flags & (SURF_SKY|SURF_TRANS33|SURF_TRANS66|SURF_WARP) ) )
			GL_CreateSurfaceLightmap (out);

		if (! (out->texinfo->flags & SURF_WARP) ) 
			GL_BuildPolygonFromSurface(out);

	}

	GL_EndBuildingLightmaps ();
}


/*
=================
glModel::SetParent
=================
*/
void glModel::SetParent ( mnode_t *node, mnode_t *parent )
{
	node->parent = parent;
	if (node->contents != -1)
		return;

	SetParent (node->children[0], node);
	SetParent (node->children[1], node);
}

/*
=================
glModel::LoadNodes
=================
*/
void glModel::LoadNodes ( lump_t *l )
{
	int			i = 0, j = 0, count = 0, p = 0;
	dnode_t		*in = nullptr;
	mnode_t 	*out = nullptr;

	in = reinterpret_cast<dnode_t*>(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name);
	count = l->filelen / sizeof(*in);
	out = static_cast<mnode_t*>(Hunk_Alloc ( count*sizeof(*out)));	

	nodes = out;
	numnodes = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{
			out->minmaxs[j] = LittleShort (in->mins[j]);
			out->minmaxs[3+j] = LittleShort (in->maxs[j]);
		}
	
		p = LittleLong(in->planenum);
		out->plane = planes + p;

		out->firstsurface = LittleShort (in->firstface);
		out->numsurfaces = LittleShort (in->numfaces);
		out->contents = -1;	// differentiate from leafs

		for (j=0 ; j<2 ; j++)
		{
			p = LittleLong (in->children[j]);
			if (p >= 0)
				out->children[j] = nodes + p;
			else
				out->children[j] = (mnode_t *)( leafs + (-1 - p ) );
		}
	}
	
	SetParent ( nodes, nullptr );	// sets nodes and leafs
}

/*
=================
glModel::LoadLeafs
=================
*/
void glModel::LoadLeafs(lump_t *l)
{
	dleaf_t 	*in;
	mleaf_t 	*out;
	int			i, j, count, p;
//	glpoly_t	*poly;

	in = reinterpret_cast<dleaf_t*>(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name);
	count = l->filelen / sizeof(*in);
	out = static_cast<mleaf_t*>(Hunk_Alloc ( count*sizeof(*out)));	

	leafs = out;
	numleafs = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{
			out->minmaxs[j] = LittleShort (in->mins[j]);
			out->minmaxs[3+j] = LittleShort (in->maxs[j]);
		}

		p = LittleLong(in->contents);
		out->contents = p;

		out->cluster = LittleShort(in->cluster);
		out->area = LittleShort(in->area);

		out->firstmarksurface = marksurfaces +
			LittleShort(in->firstleafface);
		out->nummarksurfaces = LittleShort(in->numleaffaces);
		
		// gl underwater warp
#if 0
		if (out->contents & (CONTENTS_WATER|CONTENTS_SLIME|CONTENTS_LAVA|CONTENTS_THINWATER) )
		{
			for (j=0 ; j<out->nummarksurfaces ; j++)
			{
				out->firstmarksurface[j]->flags |= SURF_UNDERWATER;
				for (poly = out->firstmarksurface[j]->polys ; poly ; poly=poly->next)
					poly->flags |= SURF_UNDERWATER;
			}
		}
#endif
	}	
}

/*
=================
glModel::LoadMarksurfaces
=================
*/
void glModel::LoadMarksurfaces ( lump_t *l )
{	
	int		i, j, count;
	short		*in;
	msurface_t **out;
	
	in = reinterpret_cast<short*>(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	out = static_cast<msurface_t**>( Hunk_Alloc ( count*sizeof(*out)) );	

	marksurfaces = out;
	nummarksurfaces = count;

	for ( i=0 ; i<count ; i++)
	{
		j = LittleShort(in[i]);
		if ( j < 0 ||  j >= numsurfaces )
			ri.Sys_Error (ERR_DROP, "Mod_ParseMarksurfaces: bad surface number");
		out[i] = surfaces + j;
	}
}

/*
=================
glModel::LoadSurfedges
=================
*/
void glModel::LoadSurfedges ( lump_t *l )
{	
	int		i = 0, count = 0;
	int		*in = nullptr, *out = nullptr;
	
	in = reinterpret_cast<int*>( mod_base + l->fileofs );
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	if (count < 1 || count >= MAX_MAP_SURFEDGES)
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: bad surfedges count in %s: %i", name, count);

	out = static_cast<int*>( Hunk_Alloc ( count * sizeof(*out) ) );	

	surfedges = out;
	numsurfedges = count;

	for ( i=0 ; i<count ; i++)
		out[i] = LittleLong (in[i]);
}


/*
=================
glModel::LoadPlanes
=================
*/
void glModel::LoadPlanes ( lump_t *l )
{
	int			i = 0, j  = 0;
	int			count  = 0;
	int			bits  = 0;
	cplane_t	*out = nullptr;
	dplane_t 	*in = nullptr;
	
	in = reinterpret_cast<dplane_t*>( mod_base + l->fileofs );
	if (l->filelen % sizeof(*in))
		ri.Sys_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size in %s", name );
	count = l->filelen / sizeof(*in);
	out = static_cast<cplane_t*> ( Hunk_Alloc ( count*2*sizeof(*out) ) );	
	
	planes = out;
	numplanes = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		bits = 0;
		for (j=0 ; j<3 ; j++)
		{
			out->normal[j] = LittleFloat (in->normal[j]);
			if (out->normal[j] < 0)
				bits |= 1<<j;
		}

		out->dist = LittleFloat (in->dist);
		out->type = LittleLong (in->type);
		out->signbits = bits;
	}
}

/*
=================
glModelManager::LoadBrushModel
=================
*/
void glModelManager::LoadBrushModel ( glModel *mod, void *buffer )
{
	int			i = 0;
	dheader_t	*header = nullptr;
	mmodel_t 	*bm = nullptr;
	
	loadmodel->SetType( MOD_BRUSH ); //loadmodel->type = MOD_BRUSH;
	if (loadmodel != mod_known)
		ri.Sys_Error (ERR_DROP, "Loaded a brush model after the world");

	header = (dheader_t *)buffer;

	i = LittleLong (header->version);
	if (i != BSPVERSION)
		ri.Sys_Error (ERR_DROP, "Mod_LoadBrushModel: %s has wrong version number (%i should be %i)", mod->Name(), i, BSPVERSION);

// swap all the lumps
	mod_base = (byte *)header;

	for (i=0 ; i<sizeof(dheader_t)/4 ; i++)
		((int *)header)[i] = LittleLong ( ((int *)header)[i]);

// load into heap
	mod->LoadBrush( header );
//
// set up the submodels
//
	for (i=0 ; i< mod->NumSubModels() ; i++)
	{
		glModel	*starmod;

		bm = &mod->SubModels()[i];
		starmod = &mod_inline[i];

		*starmod = *loadmodel;
		
		starmod->SetFirstModelSurface( bm->firstface );
		starmod->SetNumModelSurfaces( bm->numfaces );
		starmod->SetFirstNod
		starmod->FirstNode() = bm->headnode;
		if (starmod->FirstNode() >= loadmodel->NumNodes() )
			ri.Sys_Error (ERR_DROP, "Inline model %i has bad firstnode", i);

		VectorCopy ( bm->maxs, starmod->maxs );
		VectorCopy ( bm->mins, starmod->mins );
		starmod->SetRadius( bm->radius );
	
		if (i == 0)
			*loadmodel = *starmod;

		starmod->numleafs = bm->visleafs;
	}
}

/*
==============================================================================

ALIAS MODELS

==============================================================================
*/

/*
=================
glModelManager::LoadAliasModel
=================
*/
void glModelManager::LoadAliasModel ( glModel *mod, void *buffer )
{
	int					i, j;
	dmdl_t				*pinmodel, *pheader;
	dstvert_t			*pinst, *poutst;
	dtriangle_t			*pintri, *pouttri;
	daliasframe_t		*pinframe, *poutframe;
	int					*pincmd, *poutcmd;
	int					version;

	pinmodel = (dmdl_t *)buffer;

	version = LittleLong (pinmodel->version);
	if (version != ALIAS_VERSION)
		ri.Sys_Error (ERR_DROP, "%s has wrong version number (%i should be %i)", mod->Name(), version, ALIAS_VERSION);

	pheader = static_cast<dmdl_t*>(Hunk_Alloc (LittleLong(pinmodel->ofs_end)));
	
	// byte swap the header fields and sanity check
	for (i=0 ; i<sizeof(dmdl_t)/4 ; i++)
		((int *)pheader)[i] = LittleLong (((int *)buffer)[i]);

	if (pheader->skinheight > MAX_LBM_HEIGHT)
		ri.Sys_Error (ERR_DROP, "model %s has a skin taller than %d", mod->Name(), MAX_LBM_HEIGHT);

	if (pheader->num_xyz <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no vertices", mod->Name() );

	if (pheader->num_xyz > MAX_VERTS)
		ri.Sys_Error (ERR_DROP, "model %s has too many vertices", mod->Name() );

	if (pheader->num_st <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no st vertices", mod->Name() );

	if (pheader->num_tris <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no triangles", mod->Name() );

	if (pheader->num_frames <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no frames", mod->Name() );

//
// load base s and t vertices (not used in gl version)
//
	pinst = (dstvert_t *) ((byte *)pinmodel + pheader->ofs_st);
	poutst = (dstvert_t *) ((byte *)pheader + pheader->ofs_st);

	for (i=0 ; i<pheader->num_st ; i++)
	{
		poutst[i].s = LittleShort (pinst[i].s);
		poutst[i].t = LittleShort (pinst[i].t);
	}

//
// load triangle lists
//
	pintri = (dtriangle_t *) ((byte *)pinmodel + pheader->ofs_tris);
	pouttri = (dtriangle_t *) ((byte *)pheader + pheader->ofs_tris);

	for (i=0 ; i<pheader->num_tris ; i++)
	{
		for (j=0 ; j<3 ; j++)
		{
			pouttri[i].index_xyz[j] = LittleShort (pintri[i].index_xyz[j]);
			pouttri[i].index_st[j] = LittleShort (pintri[i].index_st[j]);
		}
	}

//
// load the frames
//
	for (i=0 ; i<pheader->num_frames ; i++)
	{
		pinframe = (daliasframe_t *) ((byte *)pinmodel 
			+ pheader->ofs_frames + i * pheader->framesize);
		poutframe = (daliasframe_t *) ((byte *)pheader 
			+ pheader->ofs_frames + i * pheader->framesize);

		memcpy (poutframe->name, pinframe->name, sizeof(poutframe->name));
		for (j=0 ; j<3 ; j++)
		{
			poutframe->scale[j] = LittleFloat (pinframe->scale[j]);
			poutframe->translate[j] = LittleFloat (pinframe->translate[j]);
		}
		// verts are all 8 bit, so no swapping needed
		memcpy (poutframe->verts, pinframe->verts, 
			pheader->num_xyz*sizeof(dtrivertx_t));

	}

	//mod->type = MOD_ALIAS;
	mod->SetType( MOD_ALIAS );

	//
	// load the glcmds
	//
	pincmd = (int *) ((byte *)pinmodel + pheader->ofs_glcmds);
	poutcmd = (int *) ((byte *)pheader + pheader->ofs_glcmds);
	for (i=0 ; i<pheader->num_glcmds ; i++)
		poutcmd[i] = LittleLong (pincmd[i]);


	// register all skins
	memcpy ((char *)pheader + pheader->ofs_skins, (char *)pinmodel + pheader->ofs_skins, pheader->num_skins*MAX_SKINNAME);

	for (i=0 ; i<pheader->num_skins ; i++)
	{
		mod->skins[i] = GL_FindImage ((char *)pheader + pheader->ofs_skins + i*MAX_SKINNAME, it_skin);
	}

	mod->mins[0] = -32;
	mod->mins[1] = -32;
	mod->mins[2] = -32;
	mod->maxs[0] = 32;
	mod->maxs[1] = 32;
	mod->maxs[2] = 32;
}

/*
==============================================================================

SPRITE MODELS

==============================================================================
*/

/*
=================
glModelManager::LoadSpriteModel
=================
*/
void glModelManager::LoadSpriteModel ( glModel *mod, void *buffer )
{
	int			i = 0;
	dsprite_t	*sprin = nullptr, *sprout = nullptr;

	sprin = (dsprite_t *)buffer;
	sprout = static_cast<dsprite_t*>( Hunk_Alloc (modfilelen) );

	sprout->ident = LittleLong (sprin->ident);
	sprout->version = LittleLong (sprin->version);
	sprout->numframes = LittleLong (sprin->numframes);

	if (sprout->version != SPRITE_VERSION)
		ri.Sys_Error (ERR_DROP, "%s has wrong version number (%i should be %i)", mod->Name(), sprout->version, SPRITE_VERSION);

	if (sprout->numframes > MAX_MD2SKINS)
		ri.Sys_Error (ERR_DROP, "%s has too many frames (%i > %i)", mod->Name(), sprout->numframes, MAX_MD2SKINS);

	// byte swap everything
	for ( i = 0 ; i < sprout->numframes ; i++ )
	{
		sprout->frames[i].width = LittleLong (sprin->frames[i].width);
		sprout->frames[i].height = LittleLong (sprin->frames[i].height);
		sprout->frames[i].origin_x = LittleLong (sprin->frames[i].origin_x);
		sprout->frames[i].origin_y = LittleLong (sprin->frames[i].origin_y);
		memcpy (sprout->frames[i].name, sprin->frames[i].name, MAX_SKINNAME);
		mod->skins[i] = GL_FindImage (sprout->frames[i].name, it_sprite );
	}

	//mod->type = mod_sprite;
	mod->SetType( MOD_SPRITE );
}

//=============================================================================

/*
@@@@@@@@@@@@@@@@@@@@@
R_BeginRegistration

Specifies the model that will be used as the world
@@@@@@@@@@@@@@@@@@@@@
*/
void glModelManager::BeginRegistration ( const char *model )
{
	char	fullname[MAX_QPATH];
	cvar_t	*flushmap;

	registration_sequence++;
	r_oldviewcluster = -1;		// force markleafs

	Com_sprintf (fullname, sizeof(fullname), "maps/%s.bsp", model);

	// explicitly free the old map if different
	// this guarantees that mod_known[0] is the world map
	flushmap = ri.Cvar_Get ("flushmap", "0", 0);
	if ( strcmp(mod_known[0].Name(), fullname) || flushmap->value )
		Free (&mod_known[0]);
	r_worldmodel = ForName(fullname, true);

	r_viewcluster = -1;
}

/*
@@@@@@@@@@@@@@@@@@@@@
R_RegisterModel

@@@@@@@@@@@@@@@@@@@@@
*/
glModel* glModelManager::RegisterModel ( const char *name )
{
	int		i = 0;
	glModel*	mod = nullptr;
	dsprite_t*	sprout = nullptr;
	dmdl_t*		pheader = nullptr;

	mod = ForName ( name, false );
	if (mod)
	{
		mod->SetRegistrationSequence(  registration_sequence );

		// register any images used by the models
		if (mod->Type() == MOD_SPRITE)
		{
			sprout = (dsprite_t *)mod->ExtraData();
			for (i=0 ; i<sprout->numframes ; i++)
				mod->skins[i] = GL_FindImage (sprout->frames[i].name, it_sprite);
		}
		else if (mod->Type() == MOD_ALIAS)
		{
			pheader = (dmdl_t *)mod->ExtraData();
			for (i=0 ; i<pheader->num_skins ; i++)
				mod->skins[i] = GL_FindImage ((char *)pheader + pheader->ofs_skins + i*MAX_SKINNAME, it_skin);
//PGM
			//mod->numframes = pheader->num_frames;
			mod->SetFrames( pheader->num_frames );
//PGM
		}
		else if (mod->Type() == MOD_BRUSH)
		{
			for (i=0 ; i<mod->NumTexInfo() ; i++)
				mod->texinfo[i].image->registration_sequence = registration_sequence;
		}
	}
	return mod;
}

/*
@@@@@@@@@@@@@@@@@@@@@
glModelManager::EndRegistration
@@@@@@@@@@@@@@@@@@@@@
*/
void glModelManager::EndRegistration (void)
{
	uint32_t	i = 0;
	glModel*	mod = nullptr ;

	for (i=0, mod=mod_known ; i< mod_numknown ; i++, mod++)
	{
		if (!mod->Name()[0])
			continue;

		if (mod->RegistrationSequence() != registration_sequence )
			Free ( mod ); // don't need this model
	}

	GL_FreeUnusedImages ();
}


//=============================================================================


/*
================
glModelManager::Free
================
*/
void glModelManager::Free ( glModel *mod )
{
	// Hunk_Free ( mod->extradata );
	// memset ( mod, 0, sizeof(*mod) );
	mod->Clear();
}

/*
================
glModelManager::FreeAll
================
*/
void glModelManager::FreeAll ( void )
{
	for ( uint32_t i = 0; i< mod_numknown ; i++ )
	{
		if (mod_known[i].ExtraDataSize() )
			Free (&mod_known[i]);
	}
}
