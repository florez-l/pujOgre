// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__Plugin__DotPPSceneLoader__h__
#define __pujOgre__Plugin__DotPPSceneLoader__h__

#include <plugins/DotPPScene/Export.h>

#include <OgreColourValue.h>
#include <OgreQuaternion.h>
#include <OgreResourceGroupManager.h>
#include <OgreString.h>
#include <OgrePlugin.h>
#include <OgreCodec.h>
#include <OgreMesh.h>
#include <OgreNode.h>
#include <OgreUserObjectBindings.h>

#include <map>
#include <memory>

// Forward declarations
namespace pugi
{
  class xml_document;
  class xml_node;
} // end namespace
namespace Ogre
{
  class SceneManager;
  class SceneNode;
} // end namespace

namespace Ogre
{
  /**
   */
  class Plugin_pujOgreDotPPSceneCodec_EXPORT DotPPSceneLoader
  {
  public:
    DotPPSceneLoader( );
    virtual ~DotPPSceneLoader( );

    void load(
      Ogre::DataStreamPtr& stream,
      const Ogre::String& groupName,
      Ogre::SceneNode* rootNode
      );

    void exportScene(
      Ogre::SceneNode* rootNode, const Ogre::String& outFileName
      );

    const Ogre::ColourValue& getBackgroundColour( );

  protected:
    void writeNode( pugi::xml_node& parentXML, const Ogre::SceneNode* node );
    void processScene( pugi::xml_node& XMLRoot );

    void processNodes( pugi::xml_node& XMLNode );
    void processExternals( pugi::xml_node& XMLNode );
    void processEnvironment( pugi::xml_node& XMLNode );
    void processTerrainGroup( pugi::xml_node& XMLNode );
    void processBlendmaps( pugi::xml_node& XMLNode );
    void processUserData(
      pugi::xml_node& XMLNode,
      Ogre::UserObjectBindings& userData
      );
    void processLight( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent = 0 );
    void processCamera( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent = 0 );

    void processNode( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent = 0 );
    void processLookTarget( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
    void processTrackTarget(
      pugi::xml_node& XMLNode, Ogre::SceneNode* pParent
      );

    void processEntity( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
    void processParticleSystem(
      pugi::xml_node& XMLNode, Ogre::SceneNode* pParent
      );
    void processBillboardSet(
      pugi::xml_node& XMLNode, Ogre::SceneNode* pParent
      );
    void processPlane( pugi::xml_node& XMLNode, Ogre::SceneNode* pParent );
    void processParametricSurface(
      pugi::xml_node& XMLNode, Ogre::SceneNode* pParent
      );
    void processParametricSurfaceDefinitions( pugi::xml_node& XMLNode );
    Ogre::Node::TransformSpace parseTransformSpace(
      const pugi::xml_node& XMLNode
      );
    Ogre::MeshPtr createParametricMesh(
      pugi::xml_node& XMLNode, const Ogre::String& meshName
      );
    void registerParametricDefinition(
      const Ogre::String& name, const pugi::xml_node& XMLNode
      );
    void storeParametricUserData(
      Ogre::MeshPtr& mesh, pugi::xml_node& XMLNode
      );
    void processNodeAnimations(
      pugi::xml_node& XMLNode, Ogre::SceneNode* pParent
      );
    void processNodeAnimation(
      pugi::xml_node& XMLNode, Ogre::SceneNode* pParent
      );
    void processKeyframe(
      pugi::xml_node& XMLNode, Ogre::NodeAnimationTrack* pTrack
      );

    void processFog( pugi::xml_node& XMLNode );
    void processSkyBox( pugi::xml_node& XMLNode );
    void processSkyDome( pugi::xml_node& XMLNode );
    void processSkyPlane( pugi::xml_node& XMLNode );

    void processLightRange( pugi::xml_node& XMLNode, Ogre::Light* pLight );
    void processLightAttenuation(
      pugi::xml_node& XMLNode, Ogre::Light* pLight
      );
    void processLightSourceSize(
      pugi::xml_node& XMLNode, Ogre::Light* pLight
      );

  protected:
    Ogre::SceneManager* m_SceneMgr;
    Ogre::SceneNode* m_AttachNode;
    Ogre::String m_sGroupName;
    Ogre::ColourValue m_BackgroundColour;
    Ogre::String m_ItemPrefix;
    std::map< Ogre::String, std::shared_ptr< pugi::xml_document > >
    m_ParametricDefs;
  };

  /**
   */
  class Plugin_pujOgreDotPPSceneCodec_EXPORT DotPPScenePlugin
    : public Ogre::Plugin
  {
    const Ogre::String& getName( ) const override;

    virtual void install( ) override;
    virtual void initialise( ) override;
    virtual void uninstall( ) override;
    virtual void shutdown( ) override;

  private:
    Codec* m_Codec;
  };

  /**
   * User data of the <parametricSurface> definition that built the mesh
   * `meshName` (e.g. "Torus.parametric"), or nullptr if no such mesh exists.
   * It's the mesh's own user data: the same as mesh->getUserObjectBindings( ).
   */
  Plugin_pujOgreDotPPSceneCodec_EXPORT
  const Ogre::UserObjectBindings* getParametricSurfaceUserData(
    const Ogre::String& meshName
    );
} // namespace Ogre

#endif // __pujOgre__Plugin__DotPPSceneLoader__h__

// eof - DotPPSceneLoader.h
