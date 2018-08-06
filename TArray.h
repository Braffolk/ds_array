#ifndef _DS_ARRAY_TARRAY
#define _DS_ARRAY_TARRAY

#include <windows.h>
#include <list>

typedef unsigned char ubyte;
typedef signed char sbyte;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned int uint32;
typedef signed int sint32;
typedef unsigned long long int uint64;
typedef signed long long int sint64;
typedef float float32;
typedef double float64;
typedef unsigned int uint;

enum {
	e_error,
	e_bool,
	e_ubyte,
	e_sbyte,
	e_uint16,
	e_sint16,
	e_uint32,
	e_sint32,
	e_uint64,
	e_sint64,
	e_float32,
	e_float64
};

using namespace std;

class TArray {
public: 
	unsigned int size;

	inline virtual double get( int x ) { return 0.0; }
	inline virtual void set( int x, double val ) {}
	inline virtual void set_range( int x, int range, double val ) {}
	inline virtual void clear( double val ) {}
	inline virtual size_t bytesize() { return 0;}
	inline virtual unsigned int write( char* buffer, unsigned int offset ) { return 0; }
	inline virtual unsigned int read( char* buffer, unsigned int offset ) { return 0; }
	inline virtual void copy_range( int xoffset, char* src, int xsrc, int range ) {}
	inline virtual void* get_address() { return nullptr; }
	inline virtual unsigned int write_compressed( char* buffer, unsigned int offset ) { return 0; }
	inline virtual unsigned int read_compressed( char* buffer, unsigned int offset ) { return 0; }
	inline virtual int find_index( double val, int offset ) { return -1; }
};


template <typename T>
class TArray_type : public TArray {
	public:
	T* data;

	TArray_type( int size ) {
		this->data = new T[ size ]();
		this->size = size;
	}

	~TArray_type() {
		delete[] data;
	}

	inline double get( int x ) {
		return ( double ) data[ x ];
	}

	inline void set( int x, double val ) {
		data[ x ] = ( T ) val;
	}

	inline void set_range( int x, int range, double val ) {
		std::fill( this->data + x, this->data + x + range, ( T ) val );
	}
	inline void copy_range( int xoffset, char* src, int xsrc, int range ) {
		TArray_type<T>* src_data = ( TArray_type<T>* ) src;
		int usize = sizeof( T );
		memcpy( this->data + xoffset * usize, src_data->data + xsrc * usize, range * usize );
	}

	inline size_t bytesize() {
		return this->size * sizeof( T );
	}

	inline unsigned int write( char* buffer, unsigned int offset ) {
		memcpy( &buffer[ offset ], this->data, this->bytesize() );
		return offset + this->bytesize();
	}

	inline unsigned int read( char* buffer, unsigned int offset ) {
		memcpy( this->data, &buffer[ offset ], this->bytesize() );
		return offset + this->bytesize();
	}

	inline int find_index( double val, int offset ) {
		int index = -1;
		T find = ( T ) val;
		for( int n = offset; n < this->size; n++ ) {
			if( this->data[ n ] == find ) {
				index = n;
				break;
			}
		}
		return index;
	}

	inline unsigned int write_compressed( char* buffer, unsigned int offset ) {
		int pos = offset + sizeof( uint32 );
		ubyte count = 0;
		T current = this->data[ 0 ];
		memcpy( &buffer[ 0 ] + pos, &current, sizeof( T ) );
		pos += sizeof( T );

		uint32 datacell_count = 1;

		for( int n = 1; n < ( int ) this->size; n++ ) {
			if( current != this->data[ n ] || count >= 255 ) {
				memcpy( &buffer[ 0 ] + pos, &count, sizeof( ubyte ) );
				pos += sizeof( ubyte );
				current = this->data[ n ];
				memcpy( &buffer[ 0 ] + pos, &current, sizeof( T ) );
				pos += sizeof( T );
				count = 0;
				datacell_count++;
			} else {
				count++;
			}
		}
		memcpy( &buffer[ 0 ] + pos, &count, sizeof( ubyte ) );
		pos += sizeof( ubyte );

		// Write the amount of val:count cells we've written
		memcpy( &buffer[ 0 ] + offset, &datacell_count, sizeof( uint32 ) );

		return pos;
	}

	inline unsigned int read_compressed( char* buffer, unsigned int offset ) {
		uint32 datacell_count = 0;
		memcpy( &datacell_count, &buffer[ 0 ] + offset, sizeof( uint32 ) );

		unsigned int pos = offset + sizeof( uint32 );

		ubyte c; T v;
		uint count = 0;

		for( uint32 n = 0; n < datacell_count; n++ ) {
			memcpy( &v, &buffer[ 0 ] + pos, sizeof( T ) );
			pos += sizeof( T );
			memcpy( &c, &buffer[ 0 ] + pos, sizeof( ubyte ) );
			pos += sizeof( ubyte );
			std::fill( this->data + count, this->data + count + c + 1, v );
			count += c + 1;
		}

		return pos;
	}

	inline void* get_address() {
		return ( void* ) data;
	}
};

#endif
