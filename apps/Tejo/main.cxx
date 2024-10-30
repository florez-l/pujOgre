// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <utility>

#include <Ogre.h>
#include <OgreBullet.h>
#include <OgreCameraMan.h>
#include <PUJ_Ogre/ApplicationWithBullet.h>

/**
 */
class PUJ_Ogre_Tejo_App
  : public PUJ_Ogre::ApplicationWithBullet
{
public:
  using Superclass = PUJ_Ogre::ApplicationWithBullet;

public:
  PUJ_Ogre_Tejo_App( const std::string& app_exec );
  virtual ~PUJ_Ogre_Tejo_App( ) override;

  virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
  virtual bool frameStarted( const Ogre::FrameEvent& evt ) override;

protected:
  virtual void _configureCamera( ) override;
  virtual void _loadScene( ) override;

protected:
  bool                  m_Simulating { false };
  OgreBites::CameraMan* m_CamMan;
  Ogre::SceneNode*      m_Cancha;
  std::pair< Ogre::SceneNode*, btRigidBody* > m_Tejo;
};

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  PUJ_Ogre_Tejo_App app( argv[ 0 ] );
  app.go( );
  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
PUJ_Ogre_Tejo_App::
PUJ_Ogre_Tejo_App( const std::string& app_exec )
  : Superclass( app_exec, "PUJ_Ogre_Tejo_resources.cfg" )
{
}

// -------------------------------------------------------------------------
PUJ_Ogre_Tejo_App::
~PUJ_Ogre_Tejo_App( )
{
}

// -------------------------------------------------------------------------
bool PUJ_Ogre_Tejo_App::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
    this->getRoot( )->queueEndRendering( );
  if( evt.keysym.sym == 'g' || evt.keysym.sym == 'G' )
    this->m_Simulating = true;
  return( true );
}

// -------------------------------------------------------------------------
bool PUJ_Ogre_Tejo_App::
frameStarted( const Ogre::FrameEvent& evt )
{
  if( !( this->m_Simulating ) )
  {
    Ogre::SceneNode* cam = this->m_CamMan->getCamera( );
    Ogre::Vector3 dir = cam->getLocalAxes( ).GetColumn( 2 );
    Ogre::Vector3 pos = cam->getPosition( ) - ( 2 * dir );
    this->m_Tejo.first->setPosition( pos );

    auto tr = this->m_Tejo.second->getWorldTransform( );
    tr.setOrigin( btVector3( pos[ 0 ], pos[ 1 ], pos[ 2 ] ) );
    this->m_Tejo.second->setWorldTransform( tr );
    this->m_Tejo.second->setLinearVelocity(
      btVector3( dir[ 0 ], dir[ 1 ], dir[ 2 ] ) * ( -15 )
      );
  }
  else
    this->_simulateOneStep( evt.timeSinceLastFrame, 10 );
  return( this->Superclass::frameStarted( evt ) );
}

// -------------------------------------------------------------------------
void PUJ_Ogre_Tejo_App::
_configureCamera( )
{
  this->Superclass::_configureCamera( );

  auto* cam = this->m_SceneMgr->getCameras( ).begin( )->second;
  if( cam != nullptr )
  {
    this->m_CamMan = new OgreBites::CameraMan( cam->getParentSceneNode( ) );
    this->m_CamMan->setTopSpeed( 1.2 );
    this->m_CamMan->setFixedYaw( true );
    this->m_CamMan->setStyle( OgreBites::CS_FREELOOK );
    this->addInputListener( this->m_CamMan );
  } // end if
}

// -------------------------------------------------------------------------
void PUJ_Ogre_Tejo_App::
_loadScene( )
{
  auto root = this->m_SceneMgr->getRootSceneNode( );
  root->loadChildren( "tejo.scenexx" );

  this->m_Cancha = this->m_SceneMgr->getSceneNode( "cancha" );

  auto tejo_node = this->m_SceneMgr->getSceneNode( "tejo" );
  auto tejo_ent
    =
    dynamic_cast< Ogre::Entity* >( tejo_node->getAttachedObject( 0 ) );
  auto tejo_bind
    =
    tejo_ent->getUserObjectBindings( ).getUserAny( "btRigidBody" );
  btRigidBody* tejo_body = nullptr;
  if( tejo_bind.has_value( ) )
    tejo_body = Ogre::any_cast< btRigidBody* >( tejo_bind );
  this->m_Tejo = std::make_pair( tejo_node, tejo_body );
}

// eof - $RCSfile$
