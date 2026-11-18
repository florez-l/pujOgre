// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Application.h"

#include <filesystem>

// -------------------------------------------------------------------------
namespace fs = std::filesystem;

// -------------------------------------------------------------------------
Application::
Application( int& ac, char** av )
  : Superclass( ac, av )
{
  this->m_ExecutionDir
    = fs::canonical( fs::path( av[ 0 ] ).parent_path( ) ).string( );
  this->m_MediaDir = fs::path( this->m_ExecutionDir ) / "BallsFPS_resources";
}

// -------------------------------------------------------------------------
Application::
~Application( )
{
}

// -------------------------------------------------------------------------
bool Application::
mousePressed( const OgreBites::MouseButtonEvent& evt )
{
  if( evt.button == OgreBites::BUTTON_LEFT )
  {
    auto* scnMgr = this->getRoot( )->getSceneManager( "scnMgr" );
    auto* root = scnMgr->getRootSceneNode( );
    auto* meshMgr = Ogre::MeshManager::getSingletonPtr( );
    auto* head = this->m_CameraMan->getHeadNode( );

    Ogre::MeshPtr mesh
      = meshMgr->getByName( "Bullet.parametric", Ogre::RGN_DEFAULT );
    Ogre::SceneNode* node
      = root->createChildSceneNode( head->_getDerivedPosition( ) );
    Ogre::Entity* ent = scnMgr->createEntity( mesh );
    node->attachObject( ent );
    this->m_BallsFPSModel.spawnBullet(
      ent, mesh->getUserObjectBindings( ), head->_getDerivedOrientation( )
      );

    return( true );
  }
  else
    return( false );
}

// -------------------------------------------------------------------------
void Application::
_configureCamera( )
{
  this->Superclass::_configureCamera( );

  auto* scnMgr = this->getRoot( )->getSceneManager( "scnMgr" );
  Ogre::Camera* cam = scnMgr->getCameras( ).begin( )->second;

  Ogre::SceneNode* floor = scnMgr->getSceneNode( "BottomCollider", false );
  this->m_CameraMan = new pujOgre::FPSCamera( cam, floor );
  this->m_CameraMan->setWalkSpeed( 3 );
  this->addInputListener( this->m_CameraMan );

  this->setWindowGrab( true );
  Ogre::Viewport* vp = this->getRenderWindow( )->addViewport( cam );
  vp->setBackgroundColour( Ogre::ColourValue( 0.5f, 0.5f, 0.52f ) );
}

// -------------------------------------------------------------------------
void Application::
_loadScene( )
{
  auto* root
    =
    this->getRoot( )->getSceneManager( "scnMgr" )->getRootSceneNode( );
  root->loadChildren( "scene.scenepp" );
  this->setModel( &( this->m_BallsFPSModel ), false );
  this->Superclass::_loadScene( );
}

// eof - Application.cxx
