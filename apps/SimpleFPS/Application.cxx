// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Application.h"

#include <filesystem>
#include <queue>
#include <sstream>

#include <Ogre.h>

// -------------------------------------------------------------------------
namespace fs = std::filesystem;

// -------------------------------------------------------------------------
Application::
Application( int& ac, char** av )
  : TContext( fs::path( av[ 0 ] ).filename( ).stem( ).string( ) ),
    TListener( )
{
  this->m_ExecutionDir
    = fs::canonical( fs::path( av[ 0 ] ).parent_path( ) ).string( );
}

// -------------------------------------------------------------------------
Application::
~Application( )
{
}

// -------------------------------------------------------------------------
void Application::
loadResources( )
{
  this->enableShaderCache( );
  auto rgm = Ogre::ResourceGroupManager::getSingletonPtr( );
  try
  {
    rgm->initialiseAllResourceGroups( );
    rgm->loadResourceGroup( Ogre::RGN_DEFAULT );
  }
  catch( ... )
  {
    // Do nothing
  }
}

// -------------------------------------------------------------------------
void Application::
locateResources( )
{
  // Required by Ogre itself
  const Ogre::String ogreMedia = this->getDefaultMediaDir( );
  auto rgm = Ogre::ResourceGroupManager::getSingletonPtr( );
  rgm->addResourceLocation(
    ogreMedia + "/Main", "FileSystem", Ogre::RGN_INTERNAL
    );
  rgm->addResourceLocation(
    ogreMedia + "/RTShaderLib", "FileSystem", Ogre::RGN_INTERNAL
    );

  std::string media_dir
    = std::filesystem::path( this->m_ExecutionDir ) / "SimpleFPS_resources";
  rgm->addResourceLocation( media_dir, "FileSystem", Ogre::RGN_DEFAULT );
}

// -------------------------------------------------------------------------
void Application::
setup( )
{
  this->_loadExtraPlugins( );
  this->_initSceneManager( );
  this->_loadScene( );
  this->_configureCamera( );
}

// -------------------------------------------------------------------------
void Application::
go( )
{
  this->initApp( );
  this->getRoot( )->startRendering( );
  this->closeApp( );
}

// -------------------------------------------------------------------------
bool Application::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
  {
    this->getRoot( )->queueEndRendering( );
    return( true );
  }
  else
    return( false );
}

// -------------------------------------------------------------------------
bool Application::
mousePressed( const OgreBites::MouseButtonEvent& evt )
{
  if( evt.button == OgreBites::BUTTON_LEFT )
  {
    Ogre::Vector3 pos
      =
      this->m_CameraMan->getBodyNode( )->_getDerivedPosition( );
    Ogre::Vector3 dir
      =
      this->m_CameraMan->getBodyNode( )->_getDerivedOrientation( )
      *
      Ogre::Vector3::NEGATIVE_UNIT_Z;

    // TODO: shoot!
    
    return( true );
  }
  else
    return( false );
}

// -------------------------------------------------------------------------
void Application::
frameRendered( const Ogre::FrameEvent& evt )
{
  this->m_Model.update( evt.timeSinceLastFrame );
}

// -------------------------------------------------------------------------
void Application::
_loadExtraPlugins( )
{
  auto* root = this->getRoot( );
  std::istringstream in( pujOgre_PLUGINS );
  std::string plugin;
  while( std::getline( in, plugin, '|' ) )
  {
    std::string plg_name
      = std::filesystem::path( this->m_ExecutionDir ) / plugin;
    plg_name += pujOgre_SHARED_EXT;
    root->loadPlugin( plg_name );
  } // end while
}

// -------------------------------------------------------------------------
void Application::
_initSceneManager( )
{
  this->TContext::setup( );
  this->addInputListener( this );

  auto* root = this->getRoot( );
  auto mgr = root->createSceneManager( "DefaultSceneManager", "scnMgr" );
  mgr->setAmbientLight( Ogre::ColourValue( 0.9, 0.9, 0.9 ) );

  // register our scene with the RTSS
  auto* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr( );
  shadergen->addSceneManager( mgr );
}

// -------------------------------------------------------------------------
void Application::
_configureCamera( )
{
  auto* scnMgr = this->getRoot( )->getSceneManager( "scnMgr" );
  Ogre::Camera* cam = scnMgr->getCameras( ).begin( )->second;
  cam->setAutoAspectRatio( true );

  Ogre::SceneNode* floor = scnMgr->getSceneNode( "Ground", false );
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
  root->loadChildren( "fps.scenepp" );
  this->m_Model.build( root );
}

// -------------------------------------------------------------------------
void Application::
_bounds( Ogre::SceneNode* node, Ogre::AxisAlignedBox* box )
{
  for( Ogre::MovableObject* obj: node->getAttachedObjects( ) )
  {
    const Ogre::String& type = obj->getMovableType( );
    if( obj->isVisible( ) && type != "Camera" && type != "Light" )
      box->merge( obj->getWorldBoundingBox( true ) );
  } // end for

  for( Ogre::Node* child: node->getChildren( ) )
    Self::_bounds( static_cast< Ogre::SceneNode* >( child ), box );
}

// eof - Application.cxx
