// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef PUJ_Ogre_plugins_DotXXSceneLoader__h__
#define PUJ_Ogre_plugins_DotXXSceneLoader__h__

#include <PUJ_Ogre/Plugin_PUJ_Ogre_DotXXSceneLoader_Export.h>

#include <OgreColourValue.h>
#include <OgreQuaternion.h>
#include <OgreResourceGroupManager.h>
#include <OgreString.h>
#include <OgrePlugin.h>
#include <OgreCodec.h>

// Forward declarations
namespace pugi { class xml_node; }
namespace Ogre
{
  class SceneManager;
  class SceneNode;
  namespace Bullet { class DynamicsWorld; }
} // end namespace

namespace PUJ_Ogre
{
  /**
   */
  class Plugin_PUJ_Ogre_DotXXSceneLoader_EXPORT DotXXSceneLoader
  {
  public:
    using Self = DotXXSceneLoader;

  public:
    DotXXSceneLoader( );
    virtual ~DotXXSceneLoader( );

    void load( 
      Ogre::DataStreamPtr& stream,
      const Ogre::String& groupName,
      Ogre::SceneNode* rootNode
      );

    void exportScene( 
      Ogre::SceneNode* rootNode,
      const Ogre::String& outFileName
      );
    const Ogre::ColourValue& getBackgroundColour( ) const;

  protected:
    void _process( pugi::xml_node& XMLRoot );
    void _nodes( pugi::xml_node& XMLNode );
    void _node( pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0 );
    void _entity( pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0 );
    void _camera( pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0 );
    void _light( pugi::xml_node& XMLNode,  Ogre::SceneNode* p = 0 );

    Ogre::Entity* _mesh( pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0 );
    Ogre::Entity* _parametric2(
      pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0
      );
    Ogre::Entity* _vtk( pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0 );

    // XML-related helpers
    static Ogre::String _attrib(
      const pugi::xml_node& XMLNode, const Ogre::String& attrib,
      const Ogre::String& defaultValue = ""
      );
    static Ogre::Real _real(
      const pugi::xml_node& XMLNode, const Ogre::String& attrib,
      Ogre::Real defaultValue = 0
      );
    static bool _bool(
      const pugi::xml_node& XMLNode, const Ogre::String& attrib,
      bool defaultValue = false
      );
    static Ogre::ColourValue _colour(
      const pugi::xml_node& XMLNode, const Ogre::String& attrib,
      const Ogre::ColourValue& defaultValue = Ogre::ColourValue( )
      );
    static Ogre::Vector3 _vector(
      const pugi::xml_node& XMLNode, const Ogre::String& attrib,
      const Ogre::Vector3& defaultValue = Ogre::Vector3( 0, 0, 0 )
      );
    static Ogre::Quaternion _quaternion(
      const pugi::xml_node& XMLNode, const Ogre::String& attrib,
      const Ogre::Quaternion& defaultValue = Ogre::Quaternion( 1, 0, 0, 0 )
      );
    
  protected:
    Ogre::SceneManager* m_SceneMgr;
    Ogre::SceneNode*    m_AttachNode;
    Ogre::String        m_sGroupName;
    Ogre::ColourValue   m_BackgroundColour;

    Ogre::Bullet::DynamicsWorld* m_DynamicsWorld;
  };

  /**
   */
  class Plugin_PUJ_Ogre_DotXXSceneLoader_EXPORT DotXXScenePlugin
    : public Ogre::Plugin
  {
    const Ogre::String& getName( ) const override;

    void install( ) override;
    void initialise( ) override;
    void shutdown( ) override;
    void uninstall( ) override;

  private:
    Ogre::Codec* m_Codec;
  };
} // end namespace

#endif // PUJ_Ogre_plugins_DotXXSceneLoader__h__

// eof - $RCSfile$
