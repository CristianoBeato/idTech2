
#include "q_string.hpp"


qString::qString( void ) :
    m_length( 0 ),
    m_string( nullptr )
{
}

qString::qString( const qString &in_ref )
{
    // alloc string memory
    Alloc( in_ref.Length() );

    // Copy the source string
    std::strcpy( m_string, in_ref );
    m_length = in_ref.Length();
}

qString::qString( const char *&in_ref )
{
    auto source_len = std::strlen( in_ref );
    // alloc string memory
    Alloc( source_len );

    // Copy the source string 
    std::strncpy( m_string, in_ref, source_len );
    m_length = source_len;
}

qString::~qString( void )
{
    if( m_string != nullptr )
    {
        std::free( m_string );
        m_string = nullptr;
    }
}

void qString::Write( qFile* in_file )
{
    uint32_t len = m_length;
    in_file->Write( &len, 1 );
    in_file->Write( m_string, len );
}

void qString::Read( qFile* in_file )
{
    // Get sting legenth
    uint32_t len = 0;
    in_file->Write( &len, 1 );
    if( len >= m_length )
    {
        // clear string
        std::memset( m_string, '\0', m_length + 1 );
        in_file->Read( m_string, len );
    }
    else
    {
        Alloc( len );
        in_file->Read( m_string, len );
    }
}

void qString::Alloc(const size_t in_length)
{
    m_string = static_cast<char*>( std::malloc( in_length + 1 ) );
    if( !m_string )
        throw std::bad_alloc();
        
    m_length = in_length;
}
