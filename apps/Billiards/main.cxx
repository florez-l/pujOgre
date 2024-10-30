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
class PUJ_Ogre_Billiards_App
  : public PUJ_Ogre::ApplicationWithBullet
{
public:
  using Superclass = PUJ_Ogre::ApplicationWithBullet;

public:
  PUJ_Ogre_Billiards_App( const std::string& app_exec );
  virtual ~PUJ_Ogre_Billiards_App( ) override;

  virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
  virtual bool frameStarted( const Ogre::FrameEvent& evt ) override;

protected:
  virtual void _configureCamera( ) override;
  virtual void _loadScene( ) override;

protected:
  bool                  m_Simulating { false };
  OgreBites::CameraMan* m_CamMan;
};

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  PUJ_Ogre_Billiards_App app( argv[ 0 ] );
  app.go( );
  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
PUJ_Ogre_Billiards_App::
PUJ_Ogre_Billiards_App( const std::string& app_exec )
  : Superclass( app_exec, "PUJ_Ogre_Billiards_resources.cfg" )
{
}

// -------------------------------------------------------------------------
PUJ_Ogre_Billiards_App::
~PUJ_Ogre_Billiards_App( )
{
}

// -------------------------------------------------------------------------
bool PUJ_Ogre_Billiards_App::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
    this->getRoot( )->queueEndRendering( );
  if( evt.keysym.sym == 'g' || evt.keysym.sym == 'G' )
    this->m_Simulating = true;
  return( true );
}

// -------------------------------------------------------------------------
bool PUJ_Ogre_Billiards_App::
frameStarted( const Ogre::FrameEvent& evt )
{
  if( this->m_Simulating )
    this->_simulateOneStep( evt.timeSinceLastFrame, 1 );
  return( this->Superclass::frameStarted( evt ) );
}

// -------------------------------------------------------------------------
void PUJ_Ogre_Billiards_App::
_configureCamera( )
{
  this->Superclass::_configureCamera( );

  auto* cam = this->m_SceneMgr->getCameras( ).begin( )->second;
  if( cam != nullptr )
  {
    this->m_CamMan = new OgreBites::CameraMan( cam->getParentSceneNode( ) );
    this->m_CamMan->setStyle( OgreBites::CS_FREELOOK );
    this->addInputListener( this->m_CamMan );
  } // end if
}

// -------------------------------------------------------------------------
void PUJ_Ogre_Billiards_App::
_loadScene( )
{
  this->m_SceneMgr->getRootSceneNode( )->loadChildren( "billiards.scenexx" );
}

// eof - $RCSfile$
