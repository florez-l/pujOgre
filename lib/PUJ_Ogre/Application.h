// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __PUJ_Ogre__Application__h__
#define __PUJ_Ogre__Application__h__

#include <PUJ_Ogre/Export.h>
#include <string>
#include <OgreApplicationContext.h>

namespace PUJ_Ogre
{
  /**
   */
  class PUJ_Ogre_EXPORT Application
    : public OgreBites::ApplicationContext,
      public OgreBites::InputListener
  {
  public:
    using TContext  = OgreBites::ApplicationContext;
    using TListener = OgreBites::InputListener;

  public:
    Application( const std::string& app_exec, const std::string& resources );
    virtual ~Application( ) override;

    // Configuration
    virtual void locateResources( ) override;
    virtual void loadResources( ) override;
    virtual void setup( ) override;
    virtual void createRoot( ) override;

    // Interaction and rendering
    virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;

    // Main method
    virtual void go( );

  protected:
    virtual void _initSceneManager( );
    virtual void _configureCamera( );
    virtual void _loadScene( ) = 0;

  protected:
    std::string         m_Resources;
    std::string         m_Path;
    Ogre::SceneManager* m_SceneMgr { nullptr };
  };
} // end namespace

#endif // __PUJ_Ogre__Application__h__

// eof - $RCSfile$
