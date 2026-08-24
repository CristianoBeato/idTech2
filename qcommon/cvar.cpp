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
// cvar.c -- dynamic variable tracking

#include "qcommon.hpp"
#include "cvar.hpp"

/// @brief local cvar 
static crCVARLocal lCvar = crCVARLocal();
crCVAR* gCvar = &lCvar;

crCVARLocal::crCVARLocal( void )
{
}

crCVARLocal::~crCVARLocal( void )
{
}


/*
============
InfoValidate
============
*/
static bool InfoValidate ( const char *s)
{
	if ( std::strstr( s, "\\" ) )	
		return false;
	if ( std::strstr( s, "\"" ) )	
		return false;
	if ( std::strstr( s, ";" ) ) 	
		return false;
	return true;
}

/*
============
Cvar_FindVar
============
*/
cvar_t* crCVARLocal::FindVar ( const char *var_name ) const
{
	cvar_t	*var = nullptr;
	
	for ( var = m_vars ; var ; var=var->next )
	{
		if (!std::strcmp (var_name, var->name))
			return var;
	}
	
	return nullptr;
}

/*
============
 crCvar::VariableValue
============
*/
float crCVARLocal::VariableValue ( const char *var_name ) const
{
	cvar_t	*var = nullptr;
	
	var = FindVar (var_name);
	if (!var)
		return 0;
		
	return std::atof( var->string );
}

/*
============
crCVARLocal::WriteSave
============
*/
void crCVARLocal::WriteSave( qFile* save_file ) const
{
	count count = Count();

	save_file->Write( &count, 1 );

	// these will be things like coop, skill, deathmatch, etc
	for ( auto var = m_vars ; var ; var=var->next )
	{
		if (! ( var->flags & CVAR_LATCH ) )
			continue;
		
		// Write Cvars

		//std::memset ( string, 0, sizeof( string ) );
		//std::strcpy ( name, var->name);
		//std::strcpy ( string, var->string );
		//fwrite ( name, 1, sizeof( name ), save_file);
		//fwrite ( string, 1, sizeof( string ), save_file);

		var->name.Write( save_file );
		var->string.Write( save_file );
	}
}

void crCVARLocal::ReadSave(qFile *save_file)
{
	uint32_t count = 0;
	save_file->Read( &count, 1 );
}

/*
============
crCVARLocal::VariableString
============
*/
const char *crCVARLocal::VariableString ( const char *var_name ) const
{
	cvar_t *var = nullptr;
	
	var = FindVar (var_name);
	if (!var)
		return "";

	return var->string;
}


/*
============
CompleteVariable
============
*/
const char *crCVARLocal::CompleteVariable ( const char *partial ) const
{
	size_t	len = 0;
	cvar_t*	cvar = nullptr;
	
	len = std::strlen( partial );
	
	if (!len)
		return nullptr;
		
	// check exact match
	for ( cvar = m_vars; cvar; cvar=cvar->next )
	{
		if (!std::strcmp (partial,cvar->name))
			return cvar->name;
	}

	// check partial match
	for ( cvar = m_vars ; cvar; cvar=cvar->next )
		if (!std::strncmp (partial,cvar->name, len))
			return cvar->name;

	return nullptr;
}


/*
============
Cvar_Get

If the variable already exists, the value will not be set
The flags will be or'ed in if the variable exists.
============
*/
cvar_t *crCVARLocal::Get ( const char *var_name, const char *var_value, const uint32_t flags )
{
	cvar_t	*var = nullptr;
	
	if (flags & (CVAR_USERINFO | CVAR_SERVERINFO))
	{
		if ( !InfoValidate (var_name) )
		{
			Com_Printf("invalid info cvar name\n");
			return nullptr;
		}
	}

	var = FindVar (var_name);
	if (var)
	{
		var->flags |= flags;
		return var;
	}

	if (!var_value)
		return nullptr;

	if (flags & (CVAR_USERINFO | CVAR_SERVERINFO))
	{
		if (!InfoValidate (var_value))
		{
			Com_Printf("invalid info cvar value\n");
			return nullptr;
		}
	}

	var = static_cast<cvar_t*>(Z_Malloc (sizeof(cvar_t)));
	var->name = CopyString (var_name);
	var->string = CopyString (var_value);
	var->modified = true;
	var->value = std::atof ( var->string );

	// link the variable in
	var->next = m_vars;
	m_vars = var;

	var->flags = flags;

	return var;
}

/*
============
crCVARLocal::Set2
============
*/
cvar_t*	crCVARLocal::Set2( const char *var_name, const char *value, bool force)
{
	cvar_t	*var = nullptr;

	var = FindVar (var_name);
	if (!var) // create it
		return Get (var_name, value, 0);

	if (var->flags & (CVAR_USERINFO | CVAR_SERVERINFO))
	{
		if ( !InfoValidate (value) )
		{
			Com_Printf("invalid info cvar value\n");
			return var;
		}
	}

	if (!force)
	{
		if (var->flags & CVAR_NOSET)
		{
			Com_Printf ("%s is write protected.\n", var_name);
			return var;
		}

		if (var->flags & CVAR_LATCH)
		{
			if (var->latched_string)
			{
				if ( std::strcmp( value, var->latched_string ) == 0)
					return var;
				Z_Free (var->latched_string);
			}
			else
			{
				if ( std::strcmp(value, var->string) == 0)
					return var;
			}

			if (Com_ServerState())
			{
				Com_Printf ("%s will be changed for next game.\n", var_name);
				var->latched_string = CopyString(value);
			}
			else
			{
				var->string = CopyString(value);
				var->value = std::atof (var->string);
				if (!std::strcmp( var->name, "game") )
				{
					FS_SetGamedir (var->string);
					FS_ExecAutoexec ();
				}
			}
			return var;
		}
	}
	else
	{
		if (var->latched_string)
		{
			Z_Free (var->latched_string);
			var->latched_string = nullptr;
		}
	}

	if (!std::strcmp(value, var->string))
		return var;		// not changed

	var->modified = true;

	if (var->flags & CVAR_USERINFO)
		m_userInfoModified = true;	// transmit at next oportunity
	
	Z_Free (var->string);	// free the old value string
	
	var->string = CopyString(value);
	var->value = std::atof (var->string);

	return var;
}

/*
============
crCVARLocal::ForceSet
============
*/
cvar_t *crCVARLocal::ForceSet ( const char *var_name, const char *value )
{
	return Set2 (var_name, value, true);
}

/*
============
crCVARLocal::Set
============
*/
cvar_t *crCVARLocal::Set ( const char *var_name, const char *value )
{
	return Set2 (var_name, value, false);
}

/*
============
crCvar::FullSet
============
*/
cvar_t *crCVARLocal::FullSet ( const char *var_name, const char *value, const uint32_t flags )
{
	cvar_t	*var = nullptr;
	
	var = FindVar (var_name);
	if (!var) // create it
		return Get (var_name, value, flags);

	var->modified = true;

	if (var->flags & CVAR_USERINFO)
		m_userInfoModified = true;	// transmit at next oportunity
	
	Z_Free (var->string);	// free the old value string
	
	var->string = CopyString(value);
	var->value = std::atof (var->string);
	var->flags = flags;

	return var;
}

/*
============
crCvar::SetValue
============
*/
void crCVARLocal::SetValue ( const char *var_name, float value )
{
	char	val[32]{0};

	if (value == (int)value)
		Com_sprintf (val, sizeof(val), "%i",(int)value);
	else
		Com_sprintf (val, sizeof(val), "%f",value);

	Set (var_name, val);
}


/*
============
Cvar_GetLatchedVars

Any variables with latched values will now be updated
============
*/
void crCVARLocal::GetLatchedVars (void)
{
	cvar_t	*var = nullptr;

	for (var = m_vars ; var ; var = var->next)
	{
		if (!var->latched_string)
			continue;

		Z_Free (var->string);
		var->string = var->latched_string;
		var->latched_string = nullptr;
		var->value = std::atof(var->string);
		if ( !std::strcmp( var->name, "game" ) )
		{
			FS_SetGamedir (var->string);
			FS_ExecAutoexec ();
		}
	}
}

/*
============
Cvar_Command

Handles variable inspection and changing from the console
============
*/
bool crCVARLocal::Command (void)
{
	cvar_t			*v = nullptr;

// check variables
	v = FindVar (gCmd->Argv(0));
	if (!v)
		return false;
		
// perform a variable print or set
	if (gCmd->Argc() == 1)
	{
		Com_Printf ("\"%s\" is \"%s\"\n", v->name, v->string);
		return true;
	}

	Set (v->name, gCmd->Argv(1));
	return true;
}

/*
============
Cvar_Set_f

Allows setting and defining of arbitrary cvars from console
============
*/
void crCVARLocal::Set_f (void)
{
	int	c = 0;
	int	flags = 0;

	c = gCmd->Argc();
	if (c != 3 && c != 4)
	{
		Com_Printf ("usage: set <variable> <value> [u / s]\n");
		return;
	}

	if (c == 4)
	{
		if (!std::strcmp(gCmd->Argv(3), "u"))
			flags = CVAR_USERINFO;
		else if (!std::strcmp(gCmd->Argv(3), "s"))
			flags = CVAR_SERVERINFO;
		else
		{
			Com_Printf ("flags can only be 'u' or 's'\n");
			return;
		}
		lCvar.FullSet (gCmd->Argv(1), gCmd->Argv(2), flags);
	}
	else
		lCvar.Set (gCmd->Argv(1), gCmd->Argv(2));
}


/*
============
Cvar_WriteVariables

Appends lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void crCVARLocal::WriteVariables ( const char *path)
{
	cvar_t	*var = nullptr;
	FILE	*f = nullptr;
	char	buffer[1024];

	f = fopen (path, "a");
	for (var = m_vars ; var ; var = var->next)
	{
		if (var->flags & CVAR_ARCHIVE)
		{
			Com_sprintf (buffer, sizeof(buffer), "set %s \"%s\"\n", var->name, var->string);
			fprintf (f, "%s", buffer);
		}
	}
	fclose (f);
}

/*
============
Cvar_List_f

============
*/
void crCVARLocal::List_f (void)
{
	int		i = 0;
	cvar_t	*var = nullptr;

	for (var = lCvar.m_vars ; var ; var = var->next, i++)
	{
		if (var->flags & CVAR_ARCHIVE)
			Com_Printf ("*");
		else
			Com_Printf (" ");
		if (var->flags & CVAR_USERINFO)
			Com_Printf ("U");
		else
			Com_Printf (" ");
		if (var->flags & CVAR_SERVERINFO)
			Com_Printf ("S");
		else
			Com_Printf (" ");
		if (var->flags & CVAR_NOSET)
			Com_Printf ("-");
		else if (var->flags & CVAR_LATCH)
			Com_Printf ("L");
		else
			Com_Printf (" ");
		Com_Printf (" %s \"%s\"\n", var->name, var->string);
	}
	Com_Printf ("%i cvars\n", i);
}

const uint32_t crCVARLocal::Count(void) const
{
	uint32_t count = 0;
	for ( auto var = m_vars ; var; var = var->next)
	{
		count++;
	}
    return count;
}

const char *crCVARLocal::BitInfo(const uint32_t bit) const
{
	static char	info[MAX_INFO_STRING]{0};
	cvar_t	*var = nullptr;

	info[0] = 0;

	for (var = m_vars ; var ; var = var->next)
	{
		if (var->flags & bit)
			Info_SetValueForKey (info, var->name, var->string);
	}

	return info;
}

// returns an info string containing all the CVAR_USERINFO cvars
const char* crCVARLocal::Userinfo( void ) const
{
	return BitInfo (CVAR_USERINFO);
}

// returns an info string containing all the CVAR_SERVERINFO cvars
const char* crCVARLocal::Serverinfo( void ) const
{
	return BitInfo (CVAR_SERVERINFO);
}

/*
============
Cvar_Init

Reads in all archived cvars
============
*/
void crCVARLocal::Init (void)
{
	gCmd->AddCommand ("set", Set_f);
	gCmd->AddCommand ("cvarlist", List_f);
}
