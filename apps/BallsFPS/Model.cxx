// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Model.h"

// -------------------------------------------------------------------------
Model::
Model( )
  : Superclass( )
{
}

// -------------------------------------------------------------------------
Model::
~Model( )
{
}

// -------------------------------------------------------------------------
void Model::
build( Ogre::SceneNode* root )
{
  this->Superclass::build( root );

  // Bullet speed
  const Ogre::UserObjectBindings& ud = root->getUserObjectBindings( );
  const Ogre::Any& sd = ud.getUserAny( "bullet_speed" );
  const Ogre::Any& md = ud.getUserAny( "motion_threshold" );
  const Ogre::Any& rd = ud.getUserAny( "swept_sphere_radius" );
  if( sd.has_value( ) )
    this->m_BulletSpeed = Ogre::any_cast< float >( sd );
  if( md.has_value( ) )
    this->m_MotionThreshold = Ogre::any_cast< float >( md );
  if( rd.has_value( ) )
    this->m_SweptSphereRadius = Ogre::any_cast< float >( rd );
  
  this->m_Simulating = true;
}

// -------------------------------------------------------------------------
void Model::
spawnBullet(
  Ogre::Entity* e,
  const Ogre::UserObjectBindings& u,
  const Ogre::Quaternion& q
  )
{
  Ogre::Vector3 d = q * Ogre::Vector3::NEGATIVE_UNIT_Z;
  btRigidBody* b = this->_configure( e, u );

  b->setLinearVelocity( Ogre::Bullet::convert( d * this->m_BulletSpeed ) );

  // Continuous collision detection: fast balls can't skip through anything
  // in one step
  const btCollisionShape* s = b->getCollisionShape( );
  float r = dynamic_cast< const btSphereShape* >( s )->getRadius( );
  b->setCcdMotionThreshold( r * this->m_MotionThreshold );
  b->setCcdSweptSphereRadius( r * this->m_SweptSphereRadius );
}

// eof - Model.cxx
