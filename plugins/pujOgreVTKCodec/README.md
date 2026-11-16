// Ogre plugin that registers VTK file formats as mesh codecs (Ogre 13+),
// so that sceneMgr->createEntity("model.vtp") just works.
//
// Load with plugins.cfg:   Plugin=Plugin_VtkCodec
// or in code:              root->loadPlugin("Plugin_VtkCodec");

