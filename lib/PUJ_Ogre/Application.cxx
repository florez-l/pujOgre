// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <PUJ_Ogre/Application.h>
#include <PUJ_Ogre/Config.h>
#include <filesystem>
#include <Ogre.h>
#include <OgreOverlaySystem.h>

// -------------------------------------------------------------------------
PUJ_Ogre::Application::
Application( const std::string& app_exec, const std::string& resources )
  : TContext( app_exec ),
    TListener( )
{
  this->m_Path =
    std::filesystem::canonical( std::filesystem::path( app_exec ) )
    .parent_path( ).string( );
  this->m_Resources = resources;
}

// -------------------------------------------------------------------------
PUJ_Ogre::Application::
~Application( )
{
}

// -------------------------------------------------------------------------
void PUJ_Ogre::Application::
locateResources( )
{
}

// -------------------------------------------------------------------------
void PUJ_Ogre::Application::
loadResources( )
{
  auto res_mgr = Ogre::ResourceGroupManager::getSingletonPtr( );

  this->enableShaderCache( );
  Ogre::ConfigFile cf = Ogre::ConfigFile( );
  cf.loadDirect( this->m_Resources );

  auto settings = cf.getSettingsBySection( );
  for( auto s = settings.begin( ); s != settings.end( ); ++s )
    for( auto f = s->second.begin( ); f != s->second.end( ); ++f )
      res_mgr->addResourceLocation( f->second, f->first, s->first );
  try
  {
    res_mgr->initialiseAllResourceGroups( );
    res_mgr->loadResourceGroup( "General" );
  }
  catch( ... )
  {
    // Do nothing
  }
}

// -------------------------------------------------------------------------
void PUJ_Ogre::Application::
setup( )
{
  this->_initSceneManager( );
  this->_loadScene( );
  this->_configureCamera( );
  this->setWindowGrab( true );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::Application::
createRoot( )
{
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  this->mRoot = OGRE_NEW Ogre::Root( "" );
#else
  auto path = std::filesystem::path( this->m_Path );
  this->mRoot =
    OGRE_NEW Ogre::Root(
      ( path / "plugins.cfg" ).string( ),
      ( path / "ogre.cfg" ).string( ),
      ( path / "ogre.log" ).string( )
      );
#endif
#ifdef OGRE_BITES_STATIC_PLUGINS
  this->mStaticPluginLoader.load( );
#endif
  this->mOverlaySystem = OGRE_NEW Ogre::OverlaySystem( );

  // Load plugins
  std::set< std::string > plugins;
  std::string plgs( PUJ_Ogre_Plugins );
  std::string sfx( PUJ_Ogre_Plugin_suffix );
  std::string pfx( PUJ_Ogre_Plugin_prefix );
  std::size_t j = plgs.find( "|", 0 );
  std::size_t i = 0;
  while( j != std::string::npos )
  {
    // Get filename template
    std::string plg = plgs.substr( i, j - i );
    plg = plg.substr( plg.find_first_not_of( "|" ) );

    // Check possibilities
    auto path_plg = std::filesystem::path( plg );

    std::set< std::string > options;
    options.insert( plg );
    options.insert( plg + sfx );
    options.insert(
      path_plg.parent_path( ) / ( pfx + path_plg.stem( ).string( ) )
      );
    options.insert(
      ( path_plg.parent_path( ) / ( pfx + path_plg.stem( ).string( ) ) )
      .string( ) + sfx
      );
    options.insert( path / ( pfx + path_plg.stem( ).string( ) ) );
    options.insert(
      ( path / ( pfx + path_plg.stem( ).string( ) ) ).string( ) + sfx
      );

    bool ok = false;
    for( const std::string& fname: options )
    {
      if( std::filesystem::exists( fname ) && !ok )
      {
        plugins.insert( fname );
        ok = true;
      } // end if
    } // end for

    i = j;
    j = plgs.find( "|", j + 1 );
  } // end while

  for( const std::string& plg: plugins )
  {
    try
    {
      this->mRoot->loadPlugin( plg );
    }
    catch( const std::exception& exc )
    {
      // TODO: Ignore?
    } // end try
  } // end for
}

// -------------------------------------------------------------------------
bool PUJ_Ogre::Application::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
    this->getRoot( )->queueEndRendering( );
  return( true );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::Application::
go( )
{
  this->initApp( );
  this->getRoot( )->startRendering( );
  this->closeApp( );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::Application::
_initSceneManager( )
{
  this->TContext::setup( );
  this->addInputListener( this );

  auto* root = this->getRoot( );
  this->m_SceneMgr = root->createSceneManager( "DefaultSceneManager" );

  // register our scene with the RTSS
  auto* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr( );
  shadergen->addSceneManager( this->m_SceneMgr );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::Application::
_configureCamera( )
{
}

// eof - $RCSfile$
