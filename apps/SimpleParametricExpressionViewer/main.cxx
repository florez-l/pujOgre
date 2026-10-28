// Loads media/torus.scene (a red torus defined by its parametric expression)
// and shows it with a fly camera: WASD / arrows to move, mouse to look,
// Shift to go faster, Esc to quit.

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreCameraMan.h>
#include <OgreCodec.h>
#include <OgreConfigFile.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>

#include <memory>

namespace
{
const char* const SCENE_FILE = "torus.scenepp";   // use "torus.scenepp" if you renamed the codec

class TorusApp : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    TorusApp() : OgreBites::ApplicationContext("Parametric Torus") {}

    // Add our media folder to the default group, next to Ogre's own resources.
    void locateResources() override
    {
        OgreBites::ApplicationContext::locateResources();
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            MEDIA_DIR, "FileSystem", Ogre::RGN_DEFAULT);
    }

    // Ogre itself only loads the Plugin= entries of plugins.cfg, all from PluginFolder.
    // Plugins elsewhere are listed as ExtraPlugin=/full/path and loaded here.
    void loadExtraPlugins()
    {
        Ogre::String cfgPath = mFSLayer->getConfigFilePath("plugins.cfg");
        if (!Ogre::FileSystemLayer::fileExists(cfgPath))
            return;

        Ogre::ConfigFile cfg;
        cfg.load(cfgPath);
        for (Ogre::String plugin : cfg.getMultiSetting("ExtraPlugin"))
        {
            Ogre::StringUtil::trim(plugin);
            if (!plugin.empty())
                getRoot()->loadPlugin(plugin);
        }
    }

    void setup() override
    {
        loadExtraPlugins();
        OgreBites::ApplicationContext::setup();

        Ogre::Root* root = getRoot();
        Ogre::LogManager::getSingleton().logMessage("TorusApp: using plugins.cfg at " +
                                                    mFSLayer->getConfigFilePath("plugins.cfg"));

        // Normally listed in plugins.cfg; load it here only if it isn't.
        if (!Ogre::Codec::isCodecRegistered("scene"))
            root->loadPlugin("Plugin_DotScene");

        Ogre::SceneManager* scnMgr = root->createSceneManager();
        Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(scnMgr);
        scnMgr->setAmbientLight(Ogre::ColourValue(0.15f, 0.15f, 0.15f));

        // Camera
        Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
        camNode->setPosition(0, 0, 8);
        camNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);

        Ogre::Camera* cam = scnMgr->createCamera("MainCamera");
        cam->setNearClipDistance(0.05f);
        cam->setAutoAspectRatio(true);
        camNode->attachObject(cam);

        Ogre::Viewport* vp = getRenderWindow()->addViewport(cam);
        vp->setBackgroundColour(Ogre::ColourValue(0.1f, 0.1f, 0.12f));

        // Load the scene: torus + light
        scnMgr->getRootSceneNode()->loadChildren(SCENE_FILE);

        // Fly camera
        mCameraMan = std::make_unique<OgreBites::CameraMan>(camNode);
        mCameraMan->setStyle(OgreBites::CS_FREELOOK);
        mCameraMan->setTopSpeed(5);
        addInputListener(this);
        addInputListener(mCameraMan.get());
        setWindowGrab(true);   // keep the mouse inside the window for mouse-look
    }

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override
    {
        if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
        {
            getRoot()->queueEndRendering();
            return true;
        }
        return false;   // let the camera handle everything else
    }

private:
    std::unique_ptr<OgreBites::CameraMan> mCameraMan;
};
} // namespace

int main()
{
    try
    {
        TorusApp app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        Ogre::LogManager* log = Ogre::LogManager::getSingletonPtr();
        if (log)
            log->logError(e.what());
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }
    return 0;
}
