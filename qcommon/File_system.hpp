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

#ifndef __FILE_SYSTEM_HPP__
#define __FILE_SYSTEM_HPP__

class crFileSystem
{
public:
    crFileSystem( void );
    ~crFileSystem( void );

    void		Init( void );
    void		SetGamedir ( const char *dir);
    const char* Gamedir (void);
    const char* NextPath ( const char *prevpath);
    void		ExecAutoexec (void);

    int         FOpenFile ( const char *filename, FILE **file);
    void	    FCloseFile (FILE *f);
    
    // note: this can't be called from another DLL, due to MS libc issues
    int		    LoadFile ( const char *path, void **buffer );
    // a null buffer will just return the file length without loading
    // a -1 length is not present

    void        Read (void *buffer, int len, FILE *f);
    // properly handles partial reads

    void        FreeFile ( void *buffer );

    void        CreatePath ( const char *path );

private:

};

#endif //!__FILE_SYSTEM_HPP__