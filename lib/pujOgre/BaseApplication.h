// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__BaseApplication__h__
#define __pujOgre__BaseApplication__h__

#include <pujOgre/Export.h>

#include <string>
#include <OgreApplicationContext.h>

namespace pujOgre
{
  /**
   */
  class pujOgre_EXPORT BaseApplication
  : public OgreBites::ApplicationContext,
    public OgreBites::InputListener
  {
  public:
    using Self      = BaseApplication;
    using TContext  = OgreBites::ApplicationContext;
    using TListener = OgreBites::InputListener;

  public:
    BaseApplication( int& ac, char** av );
    virtual ~BaseApplication( ) override;

    // Configuration
    virtual void loadResources( ) override;
    virtual void locateResources( ) override;
    virtual void setup( ) override;

    // Interactions
    virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;

    // Main method
    virtual void go( );

  protected:
    virtual void _loadExtraPlugins( );
    virtual void _initSceneManager( );
    virtual void _configureCamera( );
    virtual void _loadScene( ) = 0;
    static void _bounds( Ogre::SceneNode* node, Ogre::AxisAlignedBox* box );

  protected:
    std::string m_ExecutionDir  { "" };
    std::string m_PluginsDir    { "" };
    std::string m_MediaDir      { "" };
    std::string m_ResourcesFile { "" };
  };
} // end namespace

#endif // __pujOgre__BaseApplication__h__

// eof - BaseApplication.h
