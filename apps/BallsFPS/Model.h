// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __BallsFPS__Model__h__
#define __BallsFPS__Model__h__

#include <pujOgre/BulletModel.h>

/**
 */
class Model
  : public pujOgre::BulletModel
{
public:
  using Self       = Model;
  using Superclass = pujOgre::BulletModel;

public:
  Model( );
  virtual ~Model( );

  virtual void build( Ogre::SceneNode* root ) override;

  void spawnBullet(
    Ogre::Entity* e,
    const Ogre::UserObjectBindings& u,
    const Ogre::Quaternion& q
    );

protected:
  float m_BulletSpeed       { 20 };
  float m_MotionThreshold   { 0.5 };
  float m_SweptSphereRadius { 0.9 };

};

#endif // __BallsFPS__Model__h__

// eof - Model.h
