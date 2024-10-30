// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <PUJ_Ogre/ApplicationWithBullet.h>

#include <OgreBullet.h>

// -------------------------------------------------------------------------
PUJ_Ogre::ApplicationWithBullet::
ApplicationWithBullet(
  const std::string& app_exec, const std::string& resources
  )
  : Superclass( app_exec, resources )
{
}

// -------------------------------------------------------------------------
PUJ_Ogre::ApplicationWithBullet::
~ApplicationWithBullet( )
{
}

// -------------------------------------------------------------------------
void PUJ_Ogre::ApplicationWithBullet::
_initSceneManager( )
{
  this->Superclass::_initSceneManager( );

  this->m_DynamicsWorld
    =
    new Ogre::Bullet::DynamicsWorld( Ogre::Vector3( 0, -9.8, 0 ) );
  auto* root = this->m_SceneMgr->getRootSceneNode( );
  root->getUserObjectBindings( )
    .setUserAny(
      "DynamicsWorld", Ogre::Any( this->m_DynamicsWorld )
      );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::ApplicationWithBullet::
_simulateOneStep(
  const double& time,
  const int& maxSubSteps,
  const double& fixedTimeStep
  )
{
  if( this->m_DynamicsWorld != nullptr )
    this->m_DynamicsWorld->getBtWorld( )->
      stepSimulation( time, maxSubSteps, fixedTimeStep );
}

// eof - $RCSfile$
