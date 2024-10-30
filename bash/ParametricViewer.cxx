// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>



#include <OgreApplicationContext.h>


#include <Ogre.h>
#include <OgreBullet.h>
#include <OgreCameraMan.h>
#include <OgreOverlaySystem.h>
#include <PUJ_Ogre/Config.h>

namespace PUJ_Ogre
{
  /**
   */
  class BaseApplication
    : public OgreBites::ApplicationContext,
      public OgreBites::InputListener
  {
  public:
    using TContext  = OgreBites::ApplicationContext;
    using TListener = OgreBites::InputListener;

  public:
    BaseApplication(
      const std::string& app_exec,
      const std::string& resources
      )
      : TContext( app_exec ),
        TListener( )
      {
        this->m_Path =
          std::filesystem::canonical( std::filesystem::path( app_exec ) )
          .parent_path( ).string( );
        this->m_Resources = resources;
      }
    virtual ~BaseApplication( ) override
      {
      }

    // Configuration
    virtual void locateResources( ) override
      {
      }
    virtual void loadResources( ) override
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
    virtual void setup( ) override
      {
        this->_initSceneManager( );
        this->_loadScene( );
        this->setWindowGrab( true );
      }

    virtual void createRoot( ) override
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
          options.insert( path_plg.parent_path( ) / ( pfx + path_plg.stem( ).string( ) ) );
          options.insert( ( path_plg.parent_path( ) / ( pfx + path_plg.stem( ).string( ) ) ).string( ) + sfx );
          options.insert( path / ( pfx + path_plg.stem( ).string( ) ) );
          options.insert( ( path / ( pfx + path_plg.stem( ).string( ) ) ).string( ) + sfx );

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

    // Iterative methods
    virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override
      {
        if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
          this->getRoot( )->queueEndRendering( );
        if( evt.keysym.sym == 'g' || evt.keysym.sym == 'G' )
          this->m_Simulating = true;
        return( true );
      }

    virtual bool frameStarted( const Ogre::FrameEvent& evt ) override
      {
        if( this->m_Simulating )
          this->m_DynamicsWorld->getBtWorld( )->
            stepSimulation( evt.timeSinceLastFrame, 10 );
        return( this->TContext::frameStarted( evt ) );
      }


    // Main method
    virtual void go( )
      {
        this->initApp( );
        this->getRoot( )->startRendering( );
        this->closeApp( );
      }

  protected:
    virtual void _initSceneManager( )
      {
        this->TContext::setup( );
        this->addInputListener( this );

        this->m_DynamicsWorld
          =
          new Ogre::Bullet::DynamicsWorld( Ogre::Vector3( 0, -9.8, 0 ) );

        auto* root = this->getRoot( );
        this->m_SceneMgr = root->createSceneManager( "DefaultSceneManager" );
        auto* rootNode = this->m_SceneMgr->getRootSceneNode( );
        rootNode->getUserObjectBindings( )
          .setUserAny(
            "DynamicsWorld", Ogre::Any( this->m_DynamicsWorld )
            );

        // register our scene with the RTSS
        auto* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr( );
        shadergen->addSceneManager( this->m_SceneMgr );
      }
    virtual void _configureCamera( const Ogre::AxisAlignedBox& bbox )
      {
      }
    virtual void _loadScene( )
      {
        auto* root = this->m_SceneMgr->getRootSceneNode( );
        root->loadChildren( "sphere.scenexx" );

        auto* cam = this->m_SceneMgr->getCameras( ).begin( )->second;
        if( cam != nullptr )
        {
          this->getRenderWindow( )->addViewport( cam );

          auto* camman = new OgreBites::CameraMan( cam->getParentSceneNode( ) );
          camman->setStyle( OgreBites::CS_ORBIT );
          this->addInputListener( camman );
        } // end if
      }

  protected:
    std::string         m_Resources;
    std::string         m_Path;
    Ogre::SceneManager* m_SceneMgr { nullptr };

    bool                         m_Simulating    { false };
    Ogre::Bullet::DynamicsWorld* m_DynamicsWorld { nullptr };

    /* TODO
       bool                  m_RealResourcesFile;
       OgreBites::CameraMan* m_CamMan;
    */
  };
} // end namespace

int main( int argc, char** argv )
{
  // Create temporary resources file
  std::stringstream str;
  str
    << std::filesystem::path( argv[ 0 ] ).filename( ).stem( ).string( )
    << "_resources_XXXXXX";
  std::string fname =
    ( std::filesystem::temp_directory_path( ) / str.str( ) ).string( );
  char* filename = new char[ fname.size( ) + 1 ];
  std::strcpy( filename, fname.c_str( ) );
  int fd = mkstemp( filename );
  if( fd == -1 )
  {
    std::cerr
      << "Error: could not create temporary resources file."
      << std::endl;
    return( EXIT_FAILURE );
  } // end if

  // Save resources
  std::stringstream data;
  data
    << "[General]" << std::endl
    << "FileSystem="
    << std::filesystem::canonical( std::filesystem::path( argv[ 1 ] ) ).parent_path( ).string( );
  write( fd, data.str( ).c_str( ), data.str( ).size( ) );

  PUJ_Ogre::BaseApplication app( argv[ 0 ], filename );
  app.go( );

  // Close application
  close( fd );
  unlink( filename );
  delete filename;
  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
