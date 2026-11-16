// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <filesystem>
#include <iostream>
#include <Ogre.h>
#include <OgreCameraMan.h>
#include <pujOgre/BaseApplication.h>

/**
 */
class Application
  : public pujOgre::BaseApplication
{
public:
  using Self = Application;
  using Superclass = pujOgre::BaseApplication;

public:
  // -----------------------------------------------------------------------
  Application( int& ac, char** av )
    : Superclass( ac, av )
    {
      namespace fs = std::filesystem;

      this->m_MediaDir
        =
        fs::canonical( fs::path( av[ 1 ] ).parent_path( ) ).string( );
      this->m_SceneFile = fs::path( av[ 1 ] ).filename( ).string( );
    }

  // -----------------------------------------------------------------------
  virtual ~Application( ) override = default;

protected:

  // -----------------------------------------------------------------------
  virtual void _configureCamera( ) override
    {
      this->Superclass::_configureCamera( );

      auto* scnMgr = this->getRoot( )->getSceneManager( "scnMgr" );
      Ogre::Camera* cam = scnMgr->getCameras( ).begin( )->second;

      auto* camman = new OgreBites::CameraMan( cam->getParentSceneNode( ) );
      camman->setStyle( OgreBites::CS_FREELOOK );

      this->addInputListener( camman );
      this->setWindowGrab( true );

      Ogre::Viewport* vp = this->getRenderWindow( )->addViewport( cam );
      vp->setBackgroundColour( Ogre::ColourValue( 0.5f, 0.5f, 0.52f ) );
    }

  // -----------------------------------------------------------------------
  virtual void _loadScene( ) override
    {
      this->getRoot( )->getSceneManager( "scnMgr" )->getRootSceneNode( )
        ->loadChildren( this->m_SceneFile );
    }

protected:
  std::string m_SceneFile { "" };
};

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " scene_file" << std::endl;
    return( EXIT_FAILURE );
  } // end if
  
  try
  {
    Application app( argc, argv );
    app.go( );
    return( EXIT_SUCCESS );
  }
  catch( const std::exception& e )
  {
    Ogre::LogManager* log = Ogre::LogManager::getSingletonPtr( );
    if( log != nullptr )
      log->logError( e.what( ) );
    std::cerr << "Error: " << e.what( ) << std::endl;
    return( EXIT_FAILURE );
  } // end try
}

// eof - main.cxx
