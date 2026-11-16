// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/BaseApplication.h>

#include <filesystem>
#include <sstream>

#include <Ogre.h>

// -------------------------------------------------------------------------
namespace fs = std::filesystem;

// -------------------------------------------------------------------------
pujOgre::BaseApplication::
BaseApplication( int& ac, char** av )
  : TContext( fs::path( av[ 0 ] ).filename( ).stem( ).string( ) ),
    TListener( )
{
  this->m_ExecutionDir
    = fs::canonical( fs::path( av[ 0 ] ).parent_path( ) ).string( );
  this->m_PluginsDir = this->m_ExecutionDir;
  this->m_MediaDir = "";
  this->m_ResourcesFile = "";
}

// -------------------------------------------------------------------------
pujOgre::BaseApplication::
~BaseApplication( )
{
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
loadResources( )
{
  // TODO: Load from m_ResourcesFile
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
void pujOgre::BaseApplication::
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

  if( this->m_MediaDir != "" )
    rgm->addResourceLocation(
      this->m_MediaDir, "FileSystem", Ogre::RGN_DEFAULT
      );
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
setup( )
{
  this->_loadExtraPlugins( );
  this->_initSceneManager( );
  this->_loadScene( );
  this->_configureCamera( );
}

// -------------------------------------------------------------------------
bool pujOgre::BaseApplication::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
  {
    this->getRoot( )->queueEndRendering( );
    return( true );
  }
  else
    return( false ); // let the camera handle everything else
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
go( )
{
  this->initApp( );
  this->getRoot( )->startRendering( );
  this->closeApp( );
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
_loadExtraPlugins( )
{
  auto* root = this->getRoot( );
  std::istringstream in( pujOgre_PLUGINS );
  std::string plugin;
  while( std::getline( in, plugin, '|' ) )
  {
    std::string plg_name
      = std::filesystem::path( this->m_PluginsDir ) / plugin;
    plg_name += pujOgre_SHARED_EXT;
    root->loadPlugin( plg_name );
  } // end while
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
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
void pujOgre::BaseApplication::
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
    cam->setFarClipDistance( ( eye - target ).length( ) + ( size * 2 ) );
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
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
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

// eof - BaseApplication.cxx
