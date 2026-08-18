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

#ifndef __COMAND_LINE_HPP__
#define __COMAND_LINE_HPP__

class crCMD
{
public:
    crCMD( void );
    ~crCMD( void );

    /// @brief allocates an initial text buffer that will grow as needed
    void Init (void);

    /// @brief as new commands are generated from the console or keybindings,
    /// the text is added to the end of the command buffer.
    void AddText ( const char *text);

    /// @brief when a command wants to issue other commands immediately, the text is
    /// inserted at the beginning of the buffer, before any remaining unexecuted
    /// commands.
    void InsertText ( const char *text);

    /// @brief this can be used in place of either Cbuf_AddText or Cbuf_InsertText
    void ExecuteText ( int exec_when, const char *text );

    /// @brief adds all the +set commands from the command line
    void AddEarlyCommands (bool clear);

    /// @brief adds all the remaining + commands from the command line
    /// Returns true if any late commands were added, which
    /// will keep the demoloop from immediately starting
    bool AddLateCommands (void);

    /// @brief Pulls off \\n terminated lines of text from the command buffer and sends
    /// them through Cmd_ExecuteString.  Stops when the buffer is empty.
    /// Normally called once per frame, but may be explicitly invoked.
    /// Do not call inside a command function!
    void Execute (void);

    /// These two functions are used to defer any pending commands while a map
    /// is being loaded
    void Cbuf_CopyToDefer (void);
    void Cbuf_InsertFromDefer (void);

private:

};



#endif //!__COMAND_LINE_HPP__