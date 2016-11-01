#ifndef DOUBLEPROBING_H
#define DOUBLEPROBING_H


#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include<iostream>
using namespace std;

int nextPrime( int n );

template <typename HashedObj>
class DoubleHashTable
{
  public:
    explicit DoubleHashTable( int size = 101 ) : array( nextPrime( size ) )
      { makeEmpty( ); }

    bool contains( const HashedObj & x )
    {
        return isActive( findPos( x ) );
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }

    bool insert( const HashedObj & x )
    {
            // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;

            // Rehash; see Section 5.5
        if( ++currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    bool insert( HashedObj && x )
    {
            // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        array[ currentPos ] = std::move( x );
        array[ currentPos ].info = ACTIVE;

            // Rehash; see Section 5.5
        if( ++currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }

    void print_properties(){
    int tablesize = array.size();
    double loadfactor = (double)currentSize/(double)tablesize;
    double avgcollision = (double)collision/(double)currentSize;
    cout<< "the table is T size:"<<tablesize<<endl;
    cout<< "the table has N elements:"<<currentSize<<endl;
    cout<< "the load factor is:" <<loadfactor<<endl;
    cout<< "the number of collisions, C, that occurred is:" <<collision<<endl;
    cout<< "the average number of collisions is:" <<avgcollision<<endl;
    }

    int getcounter(){return counter;}

    enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY )
          : element{ e }, info{ i } { }

        HashEntry( HashedObj && e, EntryType i = EMPTY )
          : element{ std::move( e ) }, info{ i } { }
    };

    vector<HashEntry> array;
    int currentSize;  // number of elements
    int collision = 0;   //number of collisions
    int counter =0;

    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x )
    {
        counter =0;
        int currentPos = myhash( x );

        while( array[ currentPos ].info != EMPTY &&
               array[ currentPos ].element != x )
        {
            currentPos += myhasht( x );
            counter++;
            if( currentPos >= array.size( ) )
                {currentPos -= array.size( );}
        }
        collision += counter;
        return currentPos;
    }

    void rehash( )
    {
        vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

            // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }

    //hash function
    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }
    //second hash function
    size_t myhasht( const HashedObj & x ) const
    {
        int R = 47;
        return R - myhash( x )%R;
    }
};

#endif // DOUBLEHASH_H
