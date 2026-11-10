// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __SolarSystem__Model__h__
#define __SolarSystem__Model__h__

#include <Ogre.h>
#include <vector>

/**
 */
class Model
{
public:
  using Self = Model;

public:
  Model( );
  virtual ~Model( );

  void build( Ogre::SceneNode* node );
  void update( double t );

private:
  struct StrOrbit
  {
    Ogre::SceneNode* node;
    Ogre::Real radius = 0, period = 0, phase = 0;
    Ogre::Quaternion plane = Ogre::Quaternion::IDENTITY;
  };
  std::vector< StrOrbit > m_Orbits;

  struct StrSpin
  {
    Ogre::SceneNode* node;
    Ogre::Real period = 0;
    Ogre::Quaternion tilt = Ogre::Quaternion::IDENTITY;
  };
  std::vector< StrSpin > m_Spins;
};

#endif // __SolarSystem__Model__h__

// eof - Model.h
