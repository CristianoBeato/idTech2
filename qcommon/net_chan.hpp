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

#ifndef __NET_CHANEL_HPP__
#define __NET_CHANEL_HPP__


inline constexpr float OLD_AVG = 0.99f;		// total = oldtotal*OLD_AVG + new*(1-OLD_AVG)

inline constexpr uint32_t	MAX_LATENT = 32;

typedef struct
{
	bool	fatal_error;

	netsrc_t	sock;

	int			dropped;			// between last packet and previous

	int			last_received;		// for timeouts
	int			last_sent;			// for retransmits

	netadr_t	remote_address;
	int			qport;				// qport value to write when transmitting

// sequencing variables
	int			incoming_sequence;
	int			incoming_acknowledged;
	int			incoming_reliable_acknowledged;	// single bit

	int			incoming_reliable_sequence;		// single bit, maintained local

	int			outgoing_sequence;
	int			reliable_sequence;			// single bit
	int			last_reliable_sequence;		// sequence number of last send

// reliable staging and holding areas
	sizebuf_t	message;		// writing buffer to send to server
	byte		message_buf[MAX_MSGLEN-16];		// leave space for header

// message is copied to this buffer when it is first transfered
	int			reliable_length;
	byte		reliable_buf[MAX_MSGLEN-16];	// unacked reliable message
} netchan_t;

class crNetChan
{
public:
    crNetChan( void );
    ~crNetChan( void );
    
    void Init (void);
    void Setup (netsrc_t sock, netchan_t *chan, netadr_t adr, int qport);

    bool NeedReliable ( netchan_t *chan);
    void Transmit ( netchan_t *chan, int length, byte *data);
    void OutOfBand ( const uint32_t net_socket, netadr_t adr, const size_t length, byte *data);
    void OutOfBandPrint ( const uint32_t net_socket, netadr_t adr, const char *format, ...);
    bool Process (netchan_t *chan, sizebuf_t *msg);

    bool 	CanReliable (netchan_t *chan) const;
	byte*	GetMessageBuffer( void ) const { return const_cast<byte*>( &net_message_buffer[0] ); }

private:
    netadr_t	net_from;
    sizebuf_t	net_message;
    byte		net_message_buffer[MAX_MSGLEN];
};

extern crNetChan* gNetChan;

#endif //!__NET_CHANEL_HPP__