// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Application.h"

#include <filesystem>
#include <queue>
#include <sstream>

#include <Ogre.h>
#include <OgreCameraMan.h>

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
  this->m_SceneFile = av[ 1 ];
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
    =
    fs::canonical( fs::path( this->m_SceneFile ).parent_path( ) ).string( );
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
void Application::
frameRendered( const Ogre::FrameEvent& evt )
{
  this->m_SimTime
    += std::min( evt.timeSinceLastFrame, 0.1f ) * this->m_DaysPerSecond;
  this->m_Model.update( this->m_SimTime );
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

  // If no camera, create one
  if( cam == nullptr )
  {
    Ogre::SceneNode* root = scnMgr->getRootSceneNode( );
    root->_update( true, false );
    Ogre::AxisAlignedBox box;
    Self::_bounds( root, &box );

    Ogre::Vector3 corner = box.getMaximum( );
    Ogre::Vector3 target = box.getCenter( );
    Ogre::Real size = std::max< Ogre::Real >( box.getSize( ).length( ), 1e-3f );
    Ogre::Vector3 dir = corner - target;
    if( dir.squaredLength( ) < 1e-12f )
      dir = Ogre::Vector3( 1, 1, 1 );
    Ogre::Vector3 eye = target + ( dir * 2.f );

    cam = scnMgr->createCamera( "MainCamera" );
    cam->setNearClipDistance( size * 0.001f );
    cam->setFarClipDistance( ( eye - target ).length( ) + ( size * 200 ) );
    cam->setFOVy( Ogre::Degree( 45 ) );

    Ogre::SceneNode* camNode
      =
      scnMgr->getRootSceneNode( )->createChildSceneNode( "MainCameraNode" );
    camNode->setFixedYawAxis( true );
    camNode->setPosition( eye );
    camNode->lookAt( target, Ogre::Node::TS_WORLD );
    camNode->attachObject( cam );
  } // end if

  cam->setAutoAspectRatio( true );

  auto* camman = new OgreBites::CameraMan( cam->getParentSceneNode( ) );
  camman->setStyle( OgreBites::CS_ORBIT );
  this->addInputListener( camman );
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
  root->loadChildren( this->m_SceneFile );
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
