#ifndef _DS_ARRAY
#define _DS_ARRAY

#include "TArray.h"

// x + this.width * y
// i % width
// i / width

class DSArray {
public:
	int width, height;
	int type;
	TArray* data;

	std::list<DSArray*>::iterator list_it;

	DSArray(int type, int width, int height);
	~DSArray();

	

	void set(int x, int y, double val);
	double get(int x, int y);
	void set_region( int x1, int y1, int x2, int y2, double val );
	
	unsigned int write( char* buffer, unsigned int offset );
	unsigned int read( char* buffer, unsigned int offset );
	unsigned int write_compressed( char* buffer, unsigned int offset );
	unsigned int read_compressed( char* buffer, unsigned int offset );
	void clear( double val );

	int find_index( double val, int offset );

	//void resize( int new_width, int new_height ); // - to implement later on

	void init_data();
};

inline DSArray::DSArray( int type, int width, int height )
{
	this->width = width;
	this->height = height;
	this->type = type;
	this->init_data();
}

inline DSArray::~DSArray()
{
	delete data;
}

inline void DSArray::set( int x, int y, double val )
{
	data->set( x + this->width * y, val );
}

inline double DSArray::get( int x, int y )
{
	return data->get( x + this->width * y );
}

inline void DSArray::set_region( int x1, int y1, int x2, int y2, double val )
{
	int range = x2 - x1 + 1;
	for( int y = y1; y <= y2; y++ ) {
		this->data->set_range( x1 + this->width * y, range, val );
	}
}

inline unsigned int DSArray::write( char* buffer, unsigned int offset )
{
	return this->data->write( buffer, offset );
}

inline unsigned int DSArray::read( char* buffer, unsigned int offset )
{
	return this->data->read( buffer, offset );
}

inline unsigned int DSArray::write_compressed( char * buffer, unsigned int offset )
{
	return this->data->write_compressed( buffer, offset );
}

inline unsigned int DSArray::read_compressed( char * buffer, unsigned int offset )
{
	return this->data->read_compressed( buffer, offset );
}

inline void DSArray::clear( double val )
{
	this->data->set_range( 0, this->data->size, val );
}

inline int DSArray::find_index( double val, int offset )
{
	return this->data->find_index( val, offset );
}

inline void DSArray::init_data()
{
	int size = this->width * this->height;

	switch( this->type ) {
		case e_sbyte:
			this->data = new TArray_type<sbyte>( size );
			break;
		case e_ubyte:
			this->data = new TArray_type<ubyte>( size );
			break;
		case e_uint16:
			this->data = new TArray_type<uint16>( size );
			break;
		case e_sint16:
			this->data = new TArray_type<sint16>( size );
			break;
		case e_uint32:
			this->data = new TArray_type<uint32>( size );
			break;
		case e_sint32:
			this->data = new TArray_type<sint32>( size );
			break;
		case e_uint64:
			this->data = new TArray_type<uint64>( size );
			break;
		case e_sint64:
			this->data = new TArray_type<sint64>( size );
			break;
		case e_float32:
			this->data = new TArray_type<float32>( size );
			break;
		case e_float64:
			this->data = new TArray_type<float64>( size );
			break;
		default:
			printf( "Unknown type in ds_array_create!" );
			break;
	}
}


#endif
