#ifndef POINT3_H
#define POINT3_H

#include <cassert>
#include <iostream>

/***************************************************************************
Point.h
Comment:  This file contains all point definitions.
***************************************************************************/

//-----------------------------------------------------------------------------
/// Class for vectors or points with 3 double coordinates.
/**
* This is the main class for all 3d points.
* The type of the three coordinates is variable.
*/
class Point3d {
  
//-----------------------------------------------------------------------------
public:

  /**
  * Standard constructor. Point will be set to 0.0.
  */
  Point3d() 
    : _x( 0.0 ), _y( 0.0 ), _z( 0.0 ) {}

  /**
  * Constructor with given value that will be set to all coordinates.
  * @param v - the value
  */
  Point3d( double v )
    : _x( v ), _y( v ), _z( v ) {}

  /**
  * Constructor with given values for all 3 coordinates.
  * @param x - first coordinate of this point
  * @param y - second coordinate of this point
  * @param z - third coordinate of this point
  */
  Point3d( double x, double y, double z ) 
    : _x( x ), _y( y ), _z( z ) {}

  /**
  * Returns the first coordinate of this point.
  * @return the \b first coordinate
  */
  double& x() { return _x; }

  /**
  * Returns the first coordinate of this point (constant).
  * @return the \b first coordinate
  */
  double x() const { return _x; }

  /**
  * Returns the second coordinate of this point.
  * @return the \b second coordinate
  */
  double& y() { return _y; }

  /**
  * Returns the second coordinate of this point (constant).
  * @return the \b second coordinate
  */
  double y() const { return _y; }

  /**
  * Returns the third coordinate of this point.
  * @return the \b third coordinate
  */
  double& z() { return _z; }

  /**
  * Returns the third coordinate of this point (constant).
  * @return the \b third coordinate
  */
  double z() const { return _z; }

  /**
  * Set the coords of this point.
  * @param x - first coordinate of this point
  * @param y - second coordinate of this point
  * @param z - third coordinate of this point
  */
  void setCoords( const double x, const double y, const double z ) {
    _x = x; _y = y; _z = z;
  }

  /**
  * Operator that returns the coordinate at the given index.
  * @param i - index of the coordinate
  * @return the \b coordinate at index \em i
  */
  double& operator [] ( const int i ) {
    assert( i < 3 );
    if ( i == 0 )
      return _x;
    if ( i == 1 )
      return _y;

    return _z;
  }

  /**
  * Operator that returns the coordinate at the given index (constant).
  * @param i - index of the coordinate
  * @return the \b coordinate at index \em i
  */
  double operator [] ( const int i ) const {
    assert( i < 3 );
    if ( i == 0 )
      return _x;
    if ( i == 1 )
      return _y;
    
    return _z;
  }

  /**
  * Equality operator based on the coordinates of the points.
  * @param p - point to compare with
  * @return \b true if this point is equal to p, else \b false
  */
  bool operator == ( const Point3d& p ) const {
    if ( _x == p.x() && _y == p.y() && _z == p.z() )
      return true;
    return false;
  }

  /** 
  * Operator that returns the inverted point.
  * @return the <b> inverted point </b>
  */
  const Point3d operator - () const {
    return Point3d( -_x, -_y, -_z );
  }

  /**
  * Adding Operator.
  * @param p - the addend
  * @return the \b sum of the points
  */
  const Point3d operator + ( const Point3d& p ) const {
    return Point3d( _x + p.x(), _y + p.y(), _z + p.z() );
  }

  /**
  * Add a point to this point.
  * @param p - the addend
  */
  void operator += ( const Point3d& p ) {
    _x += p.x(); _y += p.y(); _z += p.z();
  }

  /**
  * Minus operator.
  * @param p - the subtrahend
  * @return the \b difference point
  */
  const Point3d operator - ( const Point3d& p ) const {
    return Point3d( _x - p.x(), _y - p.y(), _z - p.z() );
  }

  /**
  * Substract a point from this point.
  * @param p - the subtrahend
  */
  void operator -= ( const Point3d& p ) {
    _x -= p.x(); _y -= p.y(); _z -= p.z();
  }

  /**
  * Division operator for a single value. 
  * All coordinates will be divided by the given value.
  * @param w - the divisor
  * @return the <b> new point </b>
  */
  const Point3d operator / ( const double w ) const {
    return Point3d( _x / w, _y / w, _z / w );
  }

  /**
  * Division operator for a single value. 
  * All coordinates will be divided by the given value.
  * @param w - the divisor
  * @return the <b> new point </b>
  */
  friend const Point3d operator / ( const double w, const Point3d& p ) {
    return p / w;
  }

  /**
  * Divide all coordinates of this point by the given value.
  * @param w - the divisor
  */
  void operator /= ( const double w ) {
    _x /= w; _y /= w; _z /= w;
  }

  /**
  * Multiply operator for a single value. 
  * All coordinates will be multiplied with the given value.
  * @param w - the multiplier
  * @return the <b> new point </b>
  */
  const Point3d operator * ( const double w ) const {
    return Point3d( _x * w, _y * w, _z * w );
  }

  /**
  * Multiply operator for a single value. 
  * All coordinates will be multiplied with the given value.
  * @param w - the multiplier
  * @return the <b> new point </b>
  */
  friend const Point3d operator * ( const double w, const Point3d& p ) {
    return p * w;
  }

  /**
  * Multiply all coordinates of this point with the given value.
  * @param w - the multiplier
  */
  void operator *= ( const double w ) {
    _x *= w; _y *= w; _z *= w;
  }

  /**
  * Cross product operator.
  * @param p - another point
  * @return the <b> cross product </b> of the two points
  */
  const Point3d operator ^ ( const Point3d& p ) const {
    return Point3d( ( _y * p.z() ) - ( p.y() * _z ),
      ( _z * p.x() ) - ( p.z() * _x ), ( _x * p.y() ) - ( p.x() * _y ) );
  }

  /**
  * Dot product operator.
  * @param p - another point
  * @return the <b> dot product </b> of the two points
  */
  double operator * ( const Point3d& p ) const {
    return ( _x * p.x() + _y * p.y() + _z * p.z() );
  }

  /**
  * Returns the norm of this vector.
  * @return the \b norm
  */
  double norm() const {
    return sqrt( _x * _x + _y * _y + _z * _z );
  }

  /**
  * returns the squared norm of this vector
  * @return the <b> squared norm </b>
  */
  double squaredNorm() const {
    return _x * _x + _y * _y + _z * _z;
  }

  /**
  * Normalize this point and return a new point with the calculated coordinates.
  * @return the <b> normalized point </b>
  */
  const Point3d normalized() const {
    return ( *this / norm() );
  }

  /** Normalize this point. */
  void normalize() {
    const double n = norm();
    _x /= n; _y /= n; _z /= n;
  }

  /**
  * Get the angle between this vector and another vector.
  * @param p - another vector
  * @return the \b angle between the vectors
  */
  double getAngle( const Point3d& p ) const {
    return ( atan2( ( *this ^ p ).norm(), ( *this * p ) ) );
  }

  /** 
  * Write a point to the given stream.
  * @param s - the stream
  * @param p - the point
  */
  friend std::ostream& operator << ( std::ostream& s, const Point3d& p )  {    
    s  << p.x() << "," << p.y()<< "," <<p.z() << std::endl;
    return s;
  }

//-----------------------------------------------------------------------------
private:
  /** The first coordinate. */
  double _x;
  /** The second coordinate. */
  double _y;
  /** The third coordinate. */
  double _z;

};

#endif
