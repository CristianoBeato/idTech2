// in_SDL3.c -- for systems without a mouse

#include "client/client.h"
#include "SDL3_shared.h"

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_events.h>

static int mouse_x;
static int mouse_y;
cvar_t	*in_mouse;
cvar_t	*in_joystick;

static int  MapSDLToQuakeKey( const SDL_Keycode code );
static void HandleKeyboardEvent( const SDL_KeyboardEvent key );
static void HandleTextEvent( const SDL_TextInputEvent text );
static void HandleMouseMotionEvent( const SDL_MouseMotionEvent motion );
static void HandleMouseButtonEvent( const SDL_MouseButtonEvent button );

/*
===========
IN_Init
===========
*/
void IN_Init( void )
{
    in_mouse = Cvar_Get ("in_mouse", "1", CVAR_ARCHIVE);
    in_joystick	= Cvar_Get ( "in_joystick", "0", CVAR_ARCHIVE );

   // SDL_SetRelativeMouseMode( true );
}

/*
===========
IN_Shutdown
===========
*/
void IN_Shutdown( void )
{
}

/*
===========
IN_Commands
===========
*/
void IN_Commands( void )
{
}

/*
===========
IN_Frame
===========
*/
void IN_Frame( void )
{
}

/*
===========
IN_MouseMove
===========
*/
void IN_MouseMove ( usercmd_t *cmd )
{
    /// Only free look, no strafe, or mouse moviment
    cl.viewangles[PITCH] += m_pitch->value * mouse_y;
    cl.viewangles[YAW] -= m_yaw->value * mouse_x;
}


/*
===========
IN_JoyMove
===========
*/
void IN_JoyMove (usercmd_t *cmd)
{
	
}

/*
===========
IN_Move
===========
*/
void IN_Move ( usercmd_t *cmd )
{
	IN_MouseMove ( cmd );

	if ( sys.ActiveApp )
		IN_JoyMove ( cmd );
}

/*
===========
IN_Activate
===========
*/
void IN_Activate( bool active )
{
}

/*
===========
IN_ActivateMouse
===========
*/
void IN_ActivateMouse( void )
{
}

/*
===========
IN_DeactivateMouse
===========
*/
void IN_DeactivateMouse( void )
{
}

/*
===================
IN_ClearStates
===================
*/
void IN_ClearStates (void)
{
	mouse_x = 0;
	mouse_y = 0;
}

void IN_TextInput( const bool textinput )
{
    if( textinput )
        SDL_StartTextInput( video.window );
    else
        SDL_StopTextInput( video.window );
}

extern unsigned int sys_frame_time;
void Sys_SendKeyEvents (void)
{
	SDL_Event ev;
	while ( SDL_PollEvent( &ev ) )
	{
        switch ( ev.type ) 
        {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            HandleKeyboardEvent( ev.key );
            break;

        case SDL_EVENT_MOUSE_MOTION:
            HandleMouseMotionEvent( ev.motion );
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            HandleMouseButtonEvent( ev.button );
            break;

        case SDL_EVENT_TEXT_INPUT:
            HandleTextEvent( ev.text );
            break;

        case SDL_EVENT_QUIT:
            // Comand para fechar o jogo
            Cbuf_AddText("quit\n");
            break;
        }
	}

	sys_frame_time = SDL_GetTicks();	
}

void HandleKeyboardEvent( const SDL_KeyboardEvent key ) 
{
    int qkey = MapSDLToQuakeKey( key.key ); /// SDL - > quake conversion table
    Key_Event( qkey, key.type == SDL_EVENT_KEY_DOWN, Sys_Milliseconds());
}

void HandleTextEvent( const SDL_TextInputEvent text ) 
{
    // O SDL3 entrega uma string UTF-8 em ev.text.text
    for ( const char *p = text.text; *p; p++ ) 
    {
        // O Quake 2 espera caracteres ASCII simples no Key_Event
        // Passamos o caractere e 'true' para simular o pressionamento
        Key_Event( (unsigned char)*p, true, Sys_Milliseconds() );
        // E 'false' logo em seguida para liberar a tecla no buffer interno
        Key_Event( (unsigned char)*p, false, Sys_Milliseconds() );
    }
}

void HandleMouseMotionEvent( const SDL_MouseMotionEvent motion ) 
{
    // Adicione o deslocamento relativo às variáveis de input do Quake
    mouse_x += motion.xrel;
    mouse_y += motion.yrel;
}

void HandleMouseButtonEvent( const SDL_MouseButtonEvent button ) 
{
    // Mapeie botões para as "teclas" de mouse do Quake (K_MOUSE1, K_MOUSE2, etc)
    int qkey = K_MOUSE1 + (button.button - 1);
    bool down = ( button.type == SDL_EVENT_MOUSE_BUTTON_DOWN );
    Key_Event(qkey, down, Sys_Milliseconds() );
}

int MapSDLToQuakeKey(const SDL_Keycode code)
{
    switch ( code )
    {
        case SDLK_TAB:          return K_TAB;
        case SDLK_RETURN:       return K_ENTER;
        case SDLK_ESCAPE:       return K_ESCAPE;
        case SDLK_SPACE:        return K_SPACE;
        case SDLK_BACKSPACE:    return K_BACKSPACE;
        case SDLK_UP:           return K_UPARROW;
        case SDLK_DOWN:         return K_DOWNARROW;
        case SDLK_LEFT:         return K_LEFTARROW;
        case SDLK_RIGHT:        return K_RIGHTARROW;
        case SDLK_LALT:
        case SDLK_RALT:         return K_ALT;
        case SDLK_LCTRL:
        case SDLK_RCTRL:        return K_CTRL;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:       return K_SHIFT;
        case SDLK_F1:           return K_F1;
        case SDLK_F2:           return K_F2;
        case SDLK_F3:           return K_F3;
        case SDLK_F4:           return K_F4;
        case SDLK_F5:           return K_F5;
        case SDLK_F6:           return K_F6;
        case SDLK_F7:           return K_F7;
        case SDLK_F8:           return K_F8;
        case SDLK_F9:           return K_F9;
        case SDLK_F10:          return K_F10;
        case SDLK_F11:          return K_F11;
        case SDLK_F12:          return K_F12;
        case SDLK_INSERT:       return K_INS;
        case SDLK_DELETE:       return K_DEL;
        case SDLK_PAGEDOWN:     return K_PGDN;
        case SDLK_PAGEUP:       return K_PGUP;
        case SDLK_HOME:         return K_HOME;
        case SDLK_END:          return K_END;
        case SDLK_KP_7:         return K_KP_HOME;
        case SDLK_KP_8:         return K_KP_UPARROW;
        case SDLK_KP_9:         return K_KP_PGUP;
        case SDLK_KP_4:         return K_KP_LEFTARROW;
        case SDLK_KP_5:         return K_KP_5;
        case SDLK_KP_6:         return K_KP_RIGHTARROW;
        case SDLK_KP_1:         return K_KP_END;
        case SDLK_KP_2:         return K_KP_DOWNARROW;
        case SDLK_KP_3:         return K_KP_PGDN;
        case SDLK_KP_ENTER:     return K_KP_ENTER;
        case SDLK_KP_0:         return K_KP_INS;
        case SDLK_KP_COMMA:     return K_KP_DEL;
        case SDLK_KP_DIVIDE:    return K_KP_SLASH;
        case SDLK_KP_MINUS:     return K_KP_MINUS;
        case SDLK_KP_PLUS:      return K_KP_PLUS;
        case SDLK_PAUSE:        return K_PAUSE;
        case SDLK_0:            return '0';
        case SDLK_1:            return '1';
        case SDLK_2:            return '2';
        case SDLK_3:            return '3';
        case SDLK_4:            return '4';
        case SDLK_5:            return '5';
        case SDLK_6:            return '6';
        case SDLK_7:            return '7';
        case SDLK_8:            return '8';
        case SDLK_9:            return '9';
        case SDLK_A:            return 'a';
        case SDLK_B:            return 'b';
        case SDLK_C:            return 'c';
        case SDLK_D:            return 'd';
        case SDLK_E:            return 'e';
        case SDLK_F:            return 'f';
        case SDLK_G:            return 'g';
        case SDLK_H:            return 'h';
        case SDLK_I:            return 'i';
        case SDLK_J:            return 'j';
        case SDLK_K:            return 'k';
        case SDLK_L:            return 'l';
        case SDLK_M:            return 'm';
        case SDLK_N:            return 'n';
        case SDLK_O:            return 'o';
        case SDLK_P:            return 'p';
        case SDLK_Q:            return 'q';
        case SDLK_R:            return 'r';
        case SDLK_S:            return 's';
        case SDLK_T:            return 't';
        case SDLK_U:            return 'u';
        case SDLK_V:            return 'v';
        case SDLK_W:            return 'w';
        case SDLK_X:            return 'x';
        case SDLK_Y:            return 'y';
        case SDLK_Z:            return 'z';
        case SDLK_COLON:        return ':';
        case SDLK_SEMICOLON:    return ';';
        case SDLK_LESS:         return '<';
        case SDLK_EQUALS:       return '=';
        case SDLK_GREATER:      return '>';
        case SDLK_GRAVE:        return '`';
        case SDLK_LEFTBRACKET:  return '[';
        case SDLK_BACKSLASH:    return '\\';
        case SDLK_RIGHTBRACKET: return ']';
        case SDLK_CARET:        return '^';
        case SDLK_TILDE:        return '~';
        case SDLK_APOSTROPHE:   return '\'';
        case SDLK_UNDERSCORE:   return '_';
        case SDLK_DBLAPOSTROPHE: return '"';
    default:
        break;
    }
    return 0;
}