// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __BallFPS__Application__h__
#define __BallFPS__Application__h__

#include <pujOgre/ApplicationWithBullet.h>
#include <pujOgre/FPSCamera.h>

#include "Model.h"

/**
 */
class Application
  : public pujOgre::ApplicationWithBullet
{
public:
  using Self       = Application;
  using Superclass = pujOgre::ApplicationWithBullet;

public:
  Application( int& ac, char** av );
  virtual ~Application( ) override;

  // Interactions
  virtual bool mousePressed( const OgreBites::MouseButtonEvent& evt ) override;

protected:
  void _configureCamera( );
  void _loadScene( );

protected:
  Model m_BallsFPSModel;
  pujOgre::FPSCamera* m_CameraMan { nullptr };
};

#endif // __BallFPS__Application__h__

// eof - Application.h
