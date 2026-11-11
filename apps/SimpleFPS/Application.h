// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __SolarSystem__Application__h__
#define __SolarSystem__Application__h__

#include <string>
#include <OgreApplicationContext.h>
#include <pujOgre/FPSCamera.h>
#include "Model.h"

/**
 */
class Application
  : public OgreBites::ApplicationContext,
    public OgreBites::InputListener
{
public:
  using Self      = Application;
  using TContext  = OgreBites::ApplicationContext;
  using TListener = OgreBites::InputListener;

public:
  Application( int& ac, char** av );
  virtual ~Application( ) override;

  // Configuration
  virtual void loadResources( ) override;
  virtual void locateResources( ) override;
  virtual void setup( ) override;

  // Interactions
  virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
  virtual bool mousePressed( const OgreBites::MouseButtonEvent& evt ) override;

  // Animation
  virtual void frameRendered( const Ogre::FrameEvent& evt ) override;

  // Main method
  void go( );

protected:
  void _loadExtraPlugins( );
  void _initSceneManager( );
  void _configureCamera( );
  void _loadScene( );
  static void _bounds( Ogre::SceneNode* node, Ogre::AxisAlignedBox* box );

protected:
  std::string m_ExecutionDir { "" };
  Model m_Model;
  pujOgre::FPSCamera* m_CameraMan { nullptr };
};

#endif // __SolarSystem__Application__h__

// eof - Application.h
