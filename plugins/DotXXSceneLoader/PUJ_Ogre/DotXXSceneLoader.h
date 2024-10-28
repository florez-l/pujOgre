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

    void _parametric2( pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0 );
    void _vtk( pugi::xml_node& XMLNode, Ogre::SceneNode* p = 0 );

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
      const Ogre::Quaternion& defaultValue = Ogre::Quaternion( 0, 0, 0, 1 )
      );
    
    /* TODO
       void writeNode( pugi::xml_node& parentXML, const SceneNode* node );

       void processExternals( pugi::xml_node& XMLNode );
       void processEnvironment( pugi::xml_node& XMLNode );
       void processTerrainGroup( pugi::xml_node& XMLNode );
       void processBlendmaps( pugi::xml_node& XMLNode );
       void processUserData( pugi::xml_node& XMLNode, Ogre::UserObjectBindings& userData );
       void processLight( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent = 0 );
       void processCamera( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent = 0 );

       void processNode( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent = 0 );
       void processLookTarget( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processTrackTarget( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processEntity( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processParticleSystem( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processBillboardSet( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processPlane( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processNodeAnimations( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processNodeAnimation( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
       void processKeyframe( pugi::xml_node& XMLNode, Ogre::NodeAnimationTrack* pTrack );

       void processFog( pugi::xml_node& XMLNode );
       void processSkyBox( pugi::xml_node& XMLNode );
       void processSkyDome( pugi::xml_node& XMLNode );
       void processSkyPlane( pugi::xml_node& XMLNode );

       void processLightRange( pugi::xml_node& XMLNode, Ogre::Light* pLight );
       void processLightAttenuation( pugi::xml_node& XMLNode, Ogre::Light* pLight );
       void processLightSourceSize( pugi::xml_node& XMLNode, Ogre::Light* pLight );
    */
  protected:
    Ogre::SceneManager* m_SceneMgr;
    Ogre::SceneNode*    m_AttachNode;
    Ogre::String        m_sGroupName;
    Ogre::ColourValue   m_BackgroundColour;
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
