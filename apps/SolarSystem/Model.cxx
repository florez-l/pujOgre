// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Model.h"
#include <cmath>

// -------------------------------------------------------------------------
template< typename T >
bool getUserValue(
  const Ogre::UserObjectBindings& ud, const Ogre::String& key, T& out
  )
{
  const Ogre::Any& a = ud.getUserAny( key );
  if( const T* p = Ogre::any_cast< T >( &a ) )
  {
    out = *p;
    return( true );
  } // end if
  if( const Ogre::String* s = Ogre::any_cast< Ogre::String >( &a ) )
    return( Ogre::StringConverter::parse( *s, out ) );
  return( false );
}

// -------------------------------------------------------------------------
Model::
Model( )
{
}

// -------------------------------------------------------------------------
Model::
~Model( )
{
}

// -------------------------------------------------------------------------
void Model::
build( Ogre::SceneNode* node )
{
  const auto& ud = node->getUserObjectBindings( );

  StrOrbit orbit { node };
  if( getUserValue( ud, "orbitPeriod", orbit.period ) && orbit.period != 0 )
  {
    getUserValue( ud, "orbitRadius", orbit.radius );
    Ogre::Real phaseDeg = 0, inclinationDeg = 0;
    getUserValue( ud, "orbitPhase", phaseDeg );
    getUserValue( ud, "orbitInclination", inclinationDeg );
    orbit.phase = Ogre::Degree( phaseDeg ).valueRadians( );
    orbit.plane
      =
      Ogre::Quaternion( Ogre::Degree( inclinationDeg ), Ogre::Vector3::UNIT_X );
    this->m_Orbits.push_back( orbit );
  } // end if

  StrSpin spin { node };
  if( getUserValue( ud, "spinPeriod", spin.period ) && spin.period != 0 )
  {
    Ogre::Real tiltDeg = 0;
    getUserValue( ud, "axialTilt", tiltDeg );
    spin.tilt
      =
      Ogre::Quaternion( Ogre::Degree( tiltDeg ), Ogre::Vector3::UNIT_Z );
    this->m_Spins.push_back( spin );
  } // end if

  for( Ogre::Node* child: node->getChildren( ) )
    this->build( static_cast< Ogre::SceneNode* >( child ) );
}

// -------------------------------------------------------------------------
void Model::
update( double t )
{
  for( const StrOrbit& o: this->m_Orbits )
  {
    Ogre::Radian a(
      o.phase
      +
      Ogre::Real( Ogre::Math::TWO_PI * std::fmod( t / o.period, 1.0 ) )
      );
    Ogre::Vector3 p(
      o.radius * Ogre::Math::Cos( a ), 0, -o.radius * Ogre::Math::Sin( a )
      );
    o.node->setPosition( o.plane * p );
  } // end for
  for( const StrSpin& s: this->m_Spins )
  {
    Ogre::Radian a(
      Ogre::Real( Ogre::Math::TWO_PI * std::fmod( t / s.period, 1.0 ) )
      );
    s.node->setOrientation(
      s.tilt * Ogre::Quaternion( a, Ogre::Vector3::UNIT_Y )
      );
  } // end for
}

// eof - Model.cxx
