// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __SimpleParametricExpressionViewer__Application__h__
#define __SimpleParametricExpressionViewer__Application__h__

#include <string>
#include <OgreApplicationContext.h>

/**
 */
class Application
  : public OgreBites::ApplicationContext,
    public OgreBites::InputListener
{
public:
  using TContext  = OgreBites::ApplicationContext;
  using TListener = OgreBites::InputListener;

public:
  Application( int& ac, char** av );
  virtual ~Application( ) override;

  // Configuration
  virtual void loadResources( ) override;
  virtual void locateResources( ) override;
  virtual void setup( ) override;

  // Main method
  void go( );

protected:
  void _loadExtraPlugins( );
  void _initSceneManager( );
  void _configureCamera( );
  void _loadScene( );

protected:
  std::string m_ExecutionDir { "" };
  std::string m_SceneFile    { "" };
};

#endif // __SimpleParametricExpressionViewer__Application__h__

// eof - Application.h
