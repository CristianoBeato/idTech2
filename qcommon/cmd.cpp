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
// cmd.c -- Quake script command processing module

#include "qcommon.hpp"
#include "cmd.hpp"

extern void Cmd_ForwardToServer (void);

static const char*	cmd_null_string = "";
static bool	cmd_wait = false;
constexpr	uint32_t ALIAS_LOOP_COUNT = 16;


//=============================================================================


crCMD::crCMD( void )
{
}

crCMD::~crCMD( void )
{
}

/*
============
crCmd::Wait_f

Causes execution of the remainder of the command buffer to be delayed until
next frame.  This allows commands like:
bind g "impulse 5 ; +attack ; wait ; -attack ; impulse 2"
============
*/
void crCMD::Wait_f (void)
{
	cmd_wait = true;
}

/*
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

/*
============
crCMD::AddText

Adds command text at the end of the buffer
============
*/
void crCMD::AddText ( const char *text)
{	
	size_t l = std::strlen ( text );

	if ( m_text.cursize + l >= m_text.maxsize )
	{
		Com_Printf ("Cbuf_AddText: overflow\n");
		return;
	}

	SZ_Write ( &m_text, const_cast<char*>(text), std::strlen( text ) );
}

/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
FIXME: actually change the command buffer to do less copying
============
*/
void crCMD::InsertText ( const char *text)
{
	int		templen = 0;
	char	*temp = nullptr;

	// copy off any commands still remaining in the exec buffer
	templen = m_text.cursize;
	if (templen)
	{
		temp = static_cast<char*>( Z_Malloc (templen) );
		std::memcpy (temp, m_text.data, templen);
		SZ_Clear (&m_text);
	}
	else
		temp = nullptr;	// shut up compiler
		
	// add the entire text of the file
	AddText (text);
	
	// add the copied off data
	if (templen)
	{
		SZ_Write (&m_text, temp, templen);
		Z_Free (temp);
	}
}


/*
============
crCMD::CopyToDefer
============
*/
void crCMD::CopyToDefer (void)
{
	std::memcpy( m_deferTextBuf, m_textBuf, m_text.cursize );
	m_deferTextBuf[m_text.cursize] = 0;
	m_text.cursize = 0;
}

/*
============
crCMD::InsertFromDefer
============
*/
void crCMD::InsertFromDefer (void)
{
	InsertText ( (const char*)m_deferTextBuf );
	m_deferTextBuf[0] = 0;
}


/*
============
crCMD::ExecuteText
============
*/
void crCMD::ExecuteText ( const uint32_t exec_when, const char *text )
{
	switch (exec_when)
	{
	case EXEC_NOW:
		ExecuteString (text);
		break;
	case EXEC_INSERT:
		InsertText (text);
		break;
	case EXEC_APPEND:
		AddText (text);
		break;
	default:
		Com_Error (ERR_FATAL, "Cbuf_ExecuteText: bad exec_when");
	}
}

/*
============
Cbuf_Execute
============
*/
void crCMD::Execute (void)
{
	int		i = 0;
	int		quotes = 0;
	char	*text = nullptr;
	char	line[1024];

	m_aliasCount = 0;		// don't allow infinite alias loops

	while ( m_text.cursize )
	{
		// find a \n or ; line break
		text = (char *)m_text.data;

		quotes = 0;
		for (i=0 ; i< m_text.cursize ; i++)
		{
			if (text[i] == '"')
				quotes++;
			if ( !(quotes&1) &&  text[i] == ';')
				break;	// don't break if inside a quoted string
			if (text[i] == '\n')
				break;
		}
			
				
		std::memcpy (line, text, i);
		line[i] = 0;
		
	// delete the text from the command buffer and move remaining commands down
	// this is necessary because commands (exec, alias) can insert data at the
	// beginning of the text buffer

		if ( i == m_text.cursize )
			m_text.cursize = 0;
		else
		{
			i++;
			m_text.cursize -= i;
			std::memmove ( text, text + i, m_text.cursize);
		}

		// execute the command line
		ExecuteString (line);
		
		if (cmd_wait)
		{
			// skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait = false;
			break;
		}
	}
}


/*
===============
crCMD::AddEarlyCommands

Adds command line parameters as script statements
Commands lead with a +, and continue until another +

Set commands are added early, so they are guaranteed to be set before
the client and server initialize for the first time.

Other commands are added late, after all initialization is complete.
===============
*/
void crCMD::AddEarlyCommands ( const bool clear )
{
	int		i = 0;
	const char	*s = nullptr;

	for ( i = 0 ; i< COM_Argc() ; i++)
	{
		s = COM_Argv(i);
		if (std::strcmp (s, "+set"))
			continue;
		AddText (va("set %s %s\n", COM_Argv(i+1), COM_Argv(i+2)));
		if (clear)
		{
			COM_ClearArgv(i);
			COM_ClearArgv(i+1);
			COM_ClearArgv(i+2);
		}
		i+=2;
	}
}

/*
=================
crCMD::AddLateCommands

Adds command line parameters as script statements
Commands lead with a + and continue until another + or -
quake +vid_ref gl +map amlev1

Returns true if any late commands were added, which
will keep the demoloop from immediately starting
=================
*/
bool crCMD::AddLateCommands (void)
{
	bool	ret = false;
	int		i = 0, j = 0;
	int		s = 0;
	int		argc = 0;
	char	*text, *build, c;

// build the combined string to parse from
	s = 0;
	argc = COM_Argc();
	for (i=1 ; i<argc ; i++)
	{
		s += std::strlen (COM_Argv(i)) + 1;
	}

	if (!s)
		return false;
		
	text =  static_cast<char*>( Z_Malloc (s+1) );
	text[0] = 0;
	for (i=1 ; i<argc ; i++)
	{
		std::strcat (text,COM_Argv(i));
		if (i != argc-1)
			std::strcat (text, " ");
	}
	
	// pull out the commands
	build = static_cast<char*>( Z_Malloc (s+1) );
	build[0] = 0;
	
	for (i=0 ; i<s-1 ; i++)
	{
		if (text[i] == '+')
		{
			i++;

			for (j=i ; (text[j] != '+') && (text[j] != '-') && (text[j] != 0) ; j++)
				;

			c = text[j];
			text[j] = 0;
			
			std::strcat (build, text+i);
			std::strcat (build, "\n");
			text[j] = c;
			i = j-1;
		}
	}

	ret = (build[0] != 0);
	if (ret)
		AddText (build);
	
	Z_Free (text);
	Z_Free (build);

	return ret;
}


/*
==============================================================================

						SCRIPT COMMANDS

==============================================================================
*/


/*
===============
crCVAR::Exec_f
===============
*/
void crCMD::Exec_f (void)
{
	char	*f, *f2;
	int		len;

	if ( gCmd->Argc() != 2)
	{
		Com_Printf ("exec <filename> : execute a script file\n");
		return;
	}

	len = FS_LoadFile ( gCmd->Argv(1), reinterpret_cast<void **>(&f));
	if (!f)
	{
		Com_Printf ("couldn't exec %s\n", gCmd->Argv(1));
		return;
	}

	Com_Printf ("execing %s\n", gCmd->Argv(1));
	
	// the file doesn't have a trailing 0, so we need to copy it off
	f2 = static_cast<char*>( Z_Malloc( len + 1 ) );
	std::memcpy (f2, f, len);
	f2[len] = 0;

	gCmd->InsertText ( f2 );

	Z_Free (f2);
	FS_FreeFile (f);
}

/*
===============
Cmd_Echo_f

Just prints the rest of the line to the console
===============
*/
void crCMD::Echo_f (void)
{
	int		i;
	
	for (i=1 ; i< gCmd->Argc() ; i++)
		Com_Printf ("%s ", gCmd->Argv(i));
	Com_Printf ("\n");
}

/*
===============
Cmd_Alias_f

Creates a new command that executes a command string (possibly ; seperated)
===============
*/
void crCMD::Alias_f ( void )
{
	int			i = 0, c = 0;
	const char*	s = nullptr;
	cmdalias_t*	a = nullptr;
	char		cmd[1024]{ 0 };

	if ( gCmd->Argc() == 1 )
	{
		Com_Printf ("Current alias commands:\n");
		for (a = gCmd->Alias() ; a; a = a->next)
			Com_Printf ("%s : %s\n", a->name, a->value);
		return;
	}

	s = gCmd->Argv(1);
	if (std::strlen(s) >= MAX_ALIAS_NAME)
	{
		Com_Printf ("Alias name is too long\n");
		return;
	}

	// if the alias already exists, reuse it
	for (a = gCmd->Alias() ; a; a = a->next)
	{
		if (!std::strcmp(s, a->name))
		{
			Z_Free (a->value);
			break;
		}
	}

	if (!a)
	{
		a = static_cast<cmdalias_t*>( Z_Malloc (sizeof(cmdalias_t)) );
		a->next = gCmd->Alias();
		gCmd->SetAlias( a );
	}
	std::strcpy (a->name, s);	

// copy the rest of the command line
	cmd[0] = 0;		// start out with a null string
	c = gCmd->Argc();
	for (i=2 ; i< c ; i++)
	{
		std::strcat (cmd, gCmd->Argv(i));
		if (i != (c - 1))
			std::strcat (cmd, " ");
	}

	std::strcat (cmd, "\n");
	
	a->value = CopyString (cmd);
}

/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/


/*
============
crCMD::Argc
============
*/
int crCMD::Argc( void ) const
{
	return m_argc;
}

/*
============
crCMD::Argv
============
*/
const char*	crCMD::Argv( const int arg ) const
{
	if ( (unsigned)arg >= m_argc )
		return cmd_null_string;

	return m_argv[arg];	
}

/*
============
crCMD::Args

Returns a single string containing argv(1) to argv(argc()-1)
============
*/
const char* crCMD::Args ( void ) const
{
	return m_args;
}

/*
======================
crCMD::MacroExpandString
======================
*/
const char *crCMD::MacroExpandString( const char *text )
{
	bool	inquote = false;
	int		i = 0, j = 0, count = 0, len = 0;
	char	*scan = nullptr;
	const char	*token = nullptr, *start = nullptr;
	char	temporary[MAX_STRING_CHARS]{0};

	inquote = false;
	scan = const_cast<char*>( text );

	len = std::strlen (scan);
	if ( len >= MAX_STRING_CHARS )
	{
		Com_Printf ("Line exceeded %i chars, discarded.\n", MAX_STRING_CHARS);
		return nullptr;
	}

	count = 0;

	for (i=0 ; i<len ; i++)
	{
		if (scan[i] == '"')
			inquote ^= 1;

		if (inquote)
			continue;	// don't expand inside quotes

		if (scan[i] != '$')
			continue;

		// scan out the complete macro
		start = scan+i+1;
		token = COM_Parse ( (const char**)&start );

		if (!start)
			continue;
	
		token = gCvar->VariableString (token);

		j = std::strlen( token );
		len += j;
		if (len >= MAX_STRING_CHARS)
		{
			Com_Printf ("Expanded line exceeded %i chars, discarded.\n", MAX_STRING_CHARS);
			return nullptr;
		}

		std::strncpy (temporary, scan, i);
		std::strcpy (temporary+i, token);
		std::strcpy (temporary+i+j, start);

		std::strcpy ( m_expanded, temporary);
		scan = m_expanded;
		i--;

		if (++count == 100)
		{
			Com_Printf ("Macro expansion loop, discarded.\n");
			return nullptr;
		}
	}

	if (inquote)
	{
		Com_Printf ("Line has unmatched quote, discarded.\n");
		return nullptr;
	}

	return scan;
}

/*
============
crCMD::TokenizeString

Parses the given string into command line tokens.
$Cvars will be expanded unless they are in a quoted token
============
*/
void crCMD::TokenizeString ( const char *text, const bool macroExpand )
{
	int i = 0;
	const char	*com_token = nullptr;

// clear the args from the last string
	for ( i = 0; i < m_argc ; i++)
		Z_Free ( m_argv[i]);
		
	m_argc = 0;
	m_args[0] = 0;
	
	// macro expand the text
	if (macroExpand)
		text = MacroExpandString ( text );

	if (!text)
		return;

	while (1)
	{
// skip whitespace up to a /n
		while (*text && *text <= ' ' && *text != '\n')
		{
			text++;
		}
		
		if (*text == '\n')
		{	// a newline seperates commands in the buffer
			text++;
			break;
		}

		if (!*text)
			return;

		// set cmd_args to everything after the first arg
		if ( m_argc == 1)
		{
			int		l = 0;

			std::strcpy ( m_args, text );

			// strip off any trailing whitespace
			l = std::strlen( m_args ) - 1;
			for ( ; l >= 0 ; l--)
				if ( m_args[l] <= ' ')
					m_args[l] = 0;
				else
					break;
		}
			
		com_token = COM_Parse ( &text );
		if (!text)
			return;

		if ( m_argc < MAX_STRING_TOKENS)
		{
			m_argv[m_argc] = static_cast<char*>( Z_Malloc ( std::strlen( com_token ) + 1 ) );
			std::strcpy ( m_argv[m_argc], com_token);
			m_argc++;
		}
	}
}

/*
============
crCMD::AddCommand
============
*/
void crCMD::AddCommand ( const char *cmd_name, xcommand_t function )
{
	cmd_function_t	*cmd = nullptr;
	
	// fail if the command is a variable name
	if ( gCvar->VariableString(cmd_name)[0] )
	{
		Com_Printf ("Cmd_AddCommand: %s already defined as a var\n", cmd_name);
		return;
	}
	
// fail if the command already exists
	for ( cmd = m_functions ; cmd ; cmd=cmd->next)
	{
		if (!std::strcmp (cmd_name, cmd->name))
		{
			Com_Printf ("Cmd_AddCommand: %s already defined\n", cmd_name);
			return;
		}
	}

	cmd = static_cast<cmd_function_t*>( Z_Malloc (sizeof(cmd_function_t)) );
	cmd->name = cmd_name;
	cmd->function = function;
	cmd->next = m_functions;
	m_functions = cmd;
}

/*
============
crCMD::RemoveCommand
============
*/
void crCMD::RemoveCommand ( const char *cmd_name )
{
	cmd_function_t* cmd = nullptr, **back = nullptr;

	back = & m_functions;
	while (1)
	{
		cmd = *back;
		if (!cmd)
		{
			Com_Printf ("Cmd_RemoveCommand: %s not added\n", cmd_name);
			return;
		}

		if (!std::strcmp (cmd_name, cmd->name))
		{
			*back = cmd->next;
			Z_Free (cmd);
			return;
		}
		back = &cmd->next;
	}
}

/*
============
crCMD::Exists
============
*/
bool crCMD::Exists (const char *cmd_name) const
{
	cmd_function_t	*cmd = nullptr;

	for ( cmd = m_functions ; cmd; cmd=cmd->next )
	{
		if (!std::strcmp (cmd_name,cmd->name))
			return true;
	}

	return false;
}

/*
============
Cmd_CompleteCommand
============
*/
const char* crCMD::CompleteCommand ( const char *partial)
{
	size_t			len = 0;
	cmd_function_t	*cmd = nullptr;
	cmdalias_t		*a = nullptr;
	
	len = std::strlen(partial);
	
	if (!len)
		return nullptr;
		
// check for exact match
	for ( cmd = m_functions ; cmd ; cmd=cmd->next )
	{
		if (!std::strcmp (partial,cmd->name))
			return cmd->name;
	}

	for (a= m_alias ; a ; a=a->next)
	{
		if (!std::strcmp (partial, a->name))
			return a->name;
	}

// check for partial match
	for ( cmd = m_functions; cmd ; cmd=cmd->next )
	{
		if (!std::strncmp (partial,cmd->name, len))
			return cmd->name;
	}

	for (a = m_alias; a; a = a->next )
	{
		if (!std::strncmp (partial, a->name, len))
			return a->name;
	}

	return nullptr;
}


/*
============
crCMD::ExecuteString

A complete command line has been parsed, so try to execute it
FIXME: lookupnoadd the token to speed search?
============
*/
void crCMD::ExecuteString ( const char *text )
{	
	cmd_function_t	*cmd = nullptr;
	cmdalias_t		*a = nullptr;

	TokenizeString ( text, true );
			
	// execute the command line
	if ( !Argc() )
		return;		// no tokens

	// check functions
	for ( cmd = m_functions ; cmd ; cmd=cmd->next)
	{
		if (!Q_strcasecmp (m_argv[0],cmd->name))
		{
			if (!cmd->function)
				// forward to server command
				ExecuteString ( va("cmd %s", text) );
			else
				cmd->function ();

			return;
		}
	}

	// check alias
	for ( a = m_alias ; a ; a=a->next )
	{
		if (!Q_strcasecmp (m_argv[0], a->name))
		{
			if ( ++m_aliasCount == ALIAS_LOOP_COUNT )
			{
				Com_Printf ("ALIAS_LOOP_COUNT\n");
				return;
			}

			InsertText (a->value);
			return;
		}
	}
	
	// check cvars
	if (gCvar->Command ())
		return;

	// send it as a server command if we are connected
	Cmd_ForwardToServer();
}

/*
============
crCMD::List_f
============
*/
void crCMD::List_f (void)
{
	int i = 0;
	cmd_function_t	*cmd = nullptr;

	i = 0;
	for ( cmd= gCmd->Functions(); cmd ; cmd=cmd->next, i++ )
		Com_Printf ("%s\n", cmd->name);
	Com_Printf ("%i commands\n", i);
}

/*
============
Cmd_Init
============
*/
void crCMD::Init( void )
{
	SZ_Init (& m_text, m_textBuf, sizeof( m_textBuf));

	//
	// register our commands
	//
	AddCommand ("cmdlist", List_f );
	AddCommand ("exec",	Exec_f);
	AddCommand ("echo",	Echo_f);
	AddCommand ("alias", Alias_f);
	AddCommand ("wait", Wait_f);
}

