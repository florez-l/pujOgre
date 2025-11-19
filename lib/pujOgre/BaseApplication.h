// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__BaseApplication__h__
#define __pujOgre__BaseApplication__h__

#include <pujOgre/Config.h>
#include <OgreApplicationContext.h>

namespace pujOgre
{
  /**
   */
  class pujOgre_EXPORT BaseApplication
    : public OgreBites::ApplicationContext
  {
  public:
    using Self = BaseApplication;
    using Superclass = OgreBites::ApplicationContext;

  public:
    BaseApplication( const std::string& title, const std::string& res_file );
    virtual ~BaseApplication( );

    OgreBites::InputListener* getListener( ) const;
    void setListener( OgreBites::InputListener* l );

    virtual void locateResources( ) override;
    virtual void loadResources( ) override;
    virtual void setup( ) override;
    virtual void setup( const std::string& mgr_type );

    virtual void go( );

  protected:
    virtual void _initSceneManager( const std::string& mgr_type );
    virtual void _loadScene( ) = 0;

  protected:
    Ogre::SceneManager*       m_SceneMgr      { nullptr };
    OgreBites::InputListener* m_Listener      { nullptr };
    std::string               m_ResourcesFile;
  };
} // end namespace

#endif // __pujOgre__BaseApplication__h__

// eof - $RCSfile$
