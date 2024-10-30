// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <cstdio>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>
#include <unistd.h>
#include <Ogre.h>
#include <OgreCameraMan.h>
#include <PUJ_Ogre/ApplicationWithBullet.h>

/**
 */
class MyApp
  : public PUJ_Ogre::ApplicationWithBullet
{
public:
  using Superclass = PUJ_Ogre::ApplicationWithBullet;

public:
  MyApp(
    const std::string& app_exec, const std::string& resources,
    const std::string& description_file
    )
    : Superclass( app_exec, resources ),
      m_FileName( description_file )
    {
    }
  virtual ~MyApp( ) override
    {
    }

  // Interactive methods
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
        this->_simulateOneStep( evt.timeSinceLastFrame, 10 );
      return( this->Superclass::frameStarted( evt ) );
    }

protected:
  virtual void _configureCamera( ) override
    {
      this->Superclass::_configureCamera( );

      auto* cam = this->m_SceneMgr->getCameras( ).begin( )->second;
      if( cam != nullptr )
      {
        auto* camman = new OgreBites::CameraMan( cam->getParentSceneNode( ) );
        camman->setStyle( OgreBites::CS_ORBIT );
        this->addInputListener( camman );
      } // end if
    }
  virtual void _loadScene( )
    {
      this->m_SceneMgr->getRootSceneNode( )->loadChildren( this->m_FileName );
    }

protected:
  std::string m_FileName;
  bool        m_Simulating { false };
};

int main( int argc, char** argv )
{
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " input_scene" << std::endl;
    return( EXIT_FAILURE );
  } // end if

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

  // Launch application
  MyApp app(
    argv[ 0 ], filename,
    std::filesystem::path( argv[ 1 ] ).filename( ).string( )
    );
  app.go( );

  // Close application
  close( fd );
  unlink( filename );
  delete filename;
  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
