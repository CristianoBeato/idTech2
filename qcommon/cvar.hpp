/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2026 Cristiano B. Santos.

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

#ifndef __CVAR_HPP__
#define __CVAR_HPP__


/*
==============================================================

CVAR

==============================================================
*/

/*

cvar_t variables are used to hold scalar or string variables that can be changed or displayed at the console or prog code as well as accessed directly
in C code.

The user can access cvars from the console in three ways:
r_draworder			prints the current value
r_draworder 0		sets the current value to 0
set r_draworder 0	as above, but creates the cvar if not present
Cvars are restricted from having the same names as commands to keep this
interface from being ambiguous.
*/
class crCVARLocal : public crCVAR
{
public:
    crCVARLocal( void );
    ~crCVARLocal( void );

    virtual void	Init (void);

    /// @brief creates the variable if it doesn't exist, or returns the existing one
    /// if it exists, the value will not be changed, but flags will be ORed in
    /// that allows variables to be unarchived without needing bitflags
    virtual cvar_t *Get ( const char *var_name, const char *value, const uint32_t flags );
    
    /// @brief will create the variable if it doesn't exist
    virtual cvar_t 	*Set ( const char *var_name, const char *value );

    /// @brief will set the variable even if NOSET or LATCH
    virtual cvar_t *ForceSet ( const char *var_name, const char *value );

    virtual cvar_t 	*FullSet ( const char *var_name, const char *value, const uint32_t flags );

    /// @brief expands value to a string and calls Cvar_Set
    void	SetValue ( const char *var_name, float value );

    /// @brief returns 0 if not defined or non numeric
    float   VariableValue ( const char *var_name ) const;

    virtual void WriteSave( qFile* save_file ) const;
    virtual void ReadSave( qFile* save_file );

    /// @brief returns an empty string if not defined
    virtual const char* VariableString ( const char *var_name ) const;

    /// @brief attempts to match a partial variable name for command line completion
    /// returns nullptr if nothing fits
    const char* CompleteVariable ( const char *partial ) const;

    /// @brief any CVAR_LATCHED variables that have been set will now take effect
    void	GetLatchedVars (void);

    /// @brief appends lines containing "set variable value" for all variables
    /// with the archive flag set to true.
    void    WriteVariables ( const char *path );

    /// @brief returns an info string containing all the CVAR_USERINFO cvars
    const char	*Userinfo ( void ) const;

    /// @brief returns an info string containing all the CVAR_SERVERINFO cvars
    const char	*Serverinfo ( void ) const;

    /// @brief called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
    /// command.  Returns true if the command was a variable reference that
    /// was handled. (print or change)
    bool Command (void);

private:
    // this is set each time a CVAR_USERINFO variable is changed
    // so that the client knows to send it to the server
    bool	m_userInfoModified;
    
    cvar_t	*m_vars;

    cvar_t *FindVar ( const char *var_name ) const;
    cvar_t*	Set2( const char *var_name, const char *value, bool force );

    const uint32_t  Count( void ) const;
    const char* BitInfo ( const uint32_t bit ) const;
    
    static void Set_f (void);
    static void List_f (void);
};

extern crCVAR* gCvar;

#endif //!__CVAR_HPP__