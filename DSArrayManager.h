#ifndef _DS_ARRAY_MANAGER
#define _DS_ARRAY_MANAGER

#include "DSArray.h"
#include <vector>

class DSArrayManager {
	private:
	

	public:
	//list<DSArray*>* ds_arrays;
	vector<DSArray*> ds_arrays;
	DSArray* ds_array_active = nullptr;
	int ds_array_active_index = -1;
	int array_count = 0;

	DSArrayManager();
	~DSArrayManager();

	DSArray* get_ptr( int index );

	int ds_array_create( int d_type, int width, int height );
	int ds_array_create_fast( int d_type, int width, int height );
	bool ds_array_exists( int index );
	bool ds_array_destroy( int index );
	int ds_array_copy( int index );

	/// Handling
	void ds_array_set( int index, int x, int y, double val );
	double ds_array_get( int index, int x, int y );
	void ds_array_set_region( int index, int x1, int y1, int x2, int y2, double val );
	void ds_array_clear( int index, double val );

	int ds_array_find_index( int index, double val, int offset );

	/// Target handling
	void ds_array_target_set( int x, int y, double val );
	double ds_array_target_get( int x, int y );
	void ds_array_target_set_region( int x1, int y1, int x2, int y2, double val );
	void ds_array_target_clear( double val );

	int ds_array_target_find_index( double val, int offset );

	/// Buffers
	int ds_array_poke( int index, char* address_buffer, size_t offset );
	int ds_array_peek( int index, char* address_buffer, size_t offset );
	int ds_array_poke_compressed( int index, char* address_buffer, size_t offset );
	int ds_array_peek_compressed( int index, char* address_buffer, size_t offset );

	/// Information & Utility
	int ds_array_size( int index );
	int ds_array_type( int index );
	int ds_array_width( int index );
	int ds_array_height( int index );

	/// Meta
	/*uint ds_array_find_first();
	uint ds_array_find_next( uint ds_array );*/
	int ds_array_indices();
	int ds_array_count();
	bool ds_array_set_target( int index );
	int ds_array_get_target();
	bool ds_array_has_target();
	void ds_array_reset_target();
};



inline DSArray * DSArrayManager::get_ptr( int index )
{
	if( index >= 0 && index < ( int ) this->ds_arrays.size() ) {
		return this->ds_arrays[ index ];
	} else {
		return nullptr;
	}
}



inline DSArrayManager::DSArrayManager()
{
	//ds_arrays = new vector<DSArray*>;
}

inline DSArrayManager::~DSArrayManager()
{
	for( auto child : ds_arrays ) {
		delete child;
	}
	ds_arrays.clear();
	//delete ds_arrays;
}

inline int DSArrayManager::ds_array_create_fast( int d_type, int width, int height )
{
	DSArray* ds_array = new DSArray( d_type, width, height );

	int index;
	ds_arrays.push_back( ds_array );
	index = ds_arrays.size() - 1;

	this->array_count++;

	return index;
}

inline int DSArrayManager::ds_array_create( int d_type, int width, int height )
{
	DSArray* ds_array = new DSArray( d_type, width, height );

	int index;
	auto free_it = std::find( ds_arrays.begin(), ds_arrays.end(), nullptr );
	if( free_it != ds_arrays.end() ) {
		index = free_it - ds_arrays.begin();
		ds_arrays[ index ] = ds_array;
	} else {
		ds_arrays.push_back( ds_array );
		index = ds_arrays.size() - 1;
	}

	this->array_count++;

	return index;
}

inline bool DSArrayManager::ds_array_exists( int index )
{
	return ( this->get_ptr( index ) != nullptr );
}

inline bool DSArrayManager::ds_array_destroy( int index )
{
	if( this->ds_array_exists( index ) ) {
		DSArray* ptr = this->get_ptr( index );
		ds_arrays[ index ] = nullptr;
		delete ptr;

		if( this->ds_array_active == ptr ) {
			this->ds_array_active = nullptr;
			this->ds_array_active_index = -1;
		}

		this->array_count--;
		return true;
	}
	return false;
}

inline int DSArrayManager::ds_array_copy( int index )
{
	if( this->ds_array_exists( index ) ) {
		DSArray* ptr = this->get_ptr( index );
		int copy = this->ds_array_create( ptr->type, ptr->width, ptr->height );
		DSArray* copy_ptr = this->get_ptr( copy );
		ptr->write( ( char* ) copy_ptr->data->get_address(), 0 );
		return copy;
	}
	return -1;
}

/// Handling

inline void DSArrayManager::ds_array_set( int index, int x, int y, double val )
{
	this->ds_arrays[ index ]->set( x, y, val );
}

inline double DSArrayManager::ds_array_get( int index, int x, int y )
{
	return this->ds_arrays[ index ]->get( x, y );
}

inline void DSArrayManager::ds_array_set_region( int index, int x1, int y1, int x2, int y2, double val )
{
	this->ds_arrays[ index ]->set_region( x1, y1, x2, y2, val );
}

inline void DSArrayManager::ds_array_clear( int index, double val )
{
	return this->ds_arrays[ index ]->clear( val );
}

inline int DSArrayManager::ds_array_find_index( int index, double val, int offset )
{
	return this->ds_arrays[ index ]->find_index( val, offset );
}

/// Targets

inline void DSArrayManager::ds_array_target_set( int x, int y, double val )
{
	ds_array_active->set( x, y, val );
}

inline double DSArrayManager::ds_array_target_get( int x, int y )
{
	return ds_array_active->get( x, y );
}

inline void DSArrayManager::ds_array_target_set_region( int x1, int y1, int x2, int y2, double val )
{
	this->ds_array_active->set_region( x1, y1, x2, y2, val );
}

inline void DSArrayManager::ds_array_target_clear( double val )
{
	return this->ds_array_active->clear( val );
}

inline int DSArrayManager::ds_array_target_find_index( double val, int offset )
{
	return this->ds_array_active->find_index( val, offset );
}

/// Buffers

inline int DSArrayManager::ds_array_poke( int index, char * address_buffer, size_t offset )
{
	return this->ds_arrays[ index ]->write( address_buffer, offset );
}

inline int DSArrayManager::ds_array_peek( int index, char * address_buffer, size_t offset )
{
	return this->ds_arrays[ index ]->read( address_buffer, offset );
}

inline int DSArrayManager::ds_array_poke_compressed( int index, char * address_buffer, size_t offset )
{
	return this->ds_arrays[ index ]->write_compressed( address_buffer, offset );
}

inline int DSArrayManager::ds_array_peek_compressed( int index, char * address_buffer, size_t offset )
{
	return this->ds_arrays[ index ]->read_compressed( address_buffer, offset );
}

/// Information & Utility

inline int DSArrayManager::ds_array_size( int index )
{
	return this->ds_arrays[ index ]->data->bytesize();
}

inline int DSArrayManager::ds_array_type( int index )
{
	return this->ds_arrays[ index ]->type;
}

inline int DSArrayManager::ds_array_width( int index )
{
	return this->ds_arrays[ index ]->width;
}

inline int DSArrayManager::ds_array_height( int index )
{
	return this->ds_arrays[ index ]->height;
}

/// Meta
inline int DSArrayManager::ds_array_count()
{
	return this->array_count;
}

inline int DSArrayManager::ds_array_indices()
{
	return ds_arrays.size();
}

inline bool DSArrayManager::ds_array_set_target( int index )
{
	DSArray* ptr = this->get_ptr( index );
	if( ptr != nullptr ) {
		ds_array_active_index = index;
		ds_array_active = ptr;
		return true;
	}
	return false;
}

inline int DSArrayManager::ds_array_get_target()
{
	return ds_array_active_index;
}

inline bool DSArrayManager::ds_array_has_target()
{
	return( ds_array_active_index != -1);
}

inline void DSArrayManager::ds_array_reset_target()
{
	ds_array_active = nullptr;
	ds_array_active_index = -1;
}


#endif
