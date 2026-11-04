// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Application.h"
#include <filesystem>
#include <Ogre.h>
#include <OgreCameraMan.h>

// -------------------------------------------------------------------------
Application::
Application( int& ac, char** av )
  : TContext( std::filesystem::path( av[ 0 ] ).filename( ).stem( ).string( ) ),
    TListener( ),
    m_ExecutionDir(
      std::filesystem::canonical(
        std::filesystem::path( av[ 0 ] ).parent_path( )
        ).string( )
      ),
    m_SceneFile( av[ 1 ] )
{
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
    std::filesystem::canonical(
      std::filesystem::path( this->m_SceneFile ).parent_path( )
      ).string( );
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
void Application::
_loadExtraPlugins( )
{
  std::string extra_plugins[]
    =
    { "Plugin_pujOgreDotPPSceneCodec.so", "Plugin_pujOgreVTKCodec.so" };
    
  auto* root = this->getRoot( );
  for( int i = 0; i < 2; ++i )
    root->loadPlugin(
      std::filesystem::path( this->m_ExecutionDir )
      /
      extra_plugins[ i ]
      );

  /* TODO
     Ogre::String cfgPath = mFSLayer->getConfigFilePath("plugins.cfg");
     if (!Ogre::FileSystemLayer::fileExists(cfgPath))
     return;

     Ogre::ConfigFile cfg;
     cfg.load(cfgPath);
     for (Ogre::String plugin : cfg.getMultiSetting("ExtraPlugin"))
     {
     Ogre::StringUtil::trim(plugin);
     if (!plugin.empty())
     getRoot()->loadPlugin(plugin);
  */
}

// -------------------------------------------------------------------------
void Application::
_initSceneManager( )
{
  this->TContext::setup( );
  this->addInputListener( this );

  auto* root = this->getRoot( );
  auto mgr = root->createSceneManager( "DefaultSceneManager", "scnMgr" );
  // TODO: mgr->setAmbientLight( Ogre::ColourValue( 0.9, 0.9, 0.9 ) );

  // register our scene with the RTSS
  auto* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr( );
  shadergen->addSceneManager( mgr );
}

// -------------------------------------------------------------------------
void Application::
_configureCamera( )
{
  auto* scnMgr = this->getRoot( )->getSceneManager( "scnMgr" );
  auto* cam = scnMgr->getCameras( ).begin( )->second;
  cam->setAutoAspectRatio( true );

  auto* camman = new OgreBites::CameraMan( cam->getParentSceneNode( ) );
  camman->setStyle( OgreBites::CS_ORBIT );
  this->addInputListener( camman );
  this->getRenderWindow( )->addViewport( cam );
}
// -------------------------------------------------------------------------
void Application::
_loadScene( )
{
  auto* root
    =
    this->getRoot( )->getSceneManager( "scnMgr" )->getRootSceneNode( );
  root->loadChildren( this->m_SceneFile );
}

// eof - Application.cxx
