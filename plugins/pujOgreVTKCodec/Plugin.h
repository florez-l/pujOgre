// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__VTKCodec__Plugin__h__
#define __pujOgre__VTKCodec__Plugin__h__

#include <plugins/pujOgreVTKCodec/Export.h>
#include <vector>
#include <OgrePlugin.h>

/**
 */
class Plugin_pujOgreVTKCodec_EXPORT pujOgreVTKPlugin
  : public Ogre::Plugin
{
public:
  virtual const Ogre::String& getName( ) const override;
  virtual void install( ) override;
  virtual void initialise( ) override;
  virtual void shutdown  ( ) override;
  virtual void uninstall( ) override;

private:
  std::vector< Ogre::Codec* > m_Codecs;
};

#endif // __pujOgre__VTKCodec__Plugin__h__

// eof - Plugin.h
