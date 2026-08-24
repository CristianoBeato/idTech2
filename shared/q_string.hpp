
#ifndef __STRING_HPP__
#define __STRING_HPP__

class qString
{
public:
    qString( void );
    qString( const qString &in_ref );
    qString( const char* &in_ref );
    ~qString( void );

    void    Write( qFile* in_file );
    void    Read( qFile* in_file );

    inline int Cmp( const qString &in_str ) const { return std::strcmp( m_string, in_str.m_string ); }

    inline int Cmp( const char* &in_str ) const { return std::strcmp( m_string, in_str ); }

    inline const char* c_str( void ) const { return m_string; }

    inline const size_t Length( void ) const { return m_length; }

    inline operator const char*( void ) const { return m_string; }

    inline  qString& operator=( qString other ) 
    { 
        std::swap( *this, other );
        return *this;
    }


private:
    size_t  m_length;
    char*   m_string;

    void    Alloc( const size_t in_length );
};



#endif //!__STRING_HPP__