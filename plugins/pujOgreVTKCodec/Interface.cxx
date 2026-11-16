// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Plugin.h"

#include <OgreRoot.h>

namespace
{
  pujOgreVTKPlugin* gPlugin = nullptr;
} // end namespace

// -------------------------------------------------------------------------
extern "C" void Plugin_pujOgreVTKCodec_EXPORT dllStartPlugin( )
{
  gPlugin = new pujOgreVTKPlugin( );
  Ogre::Root::getSingleton( ).installPlugin( gPlugin );
}

// -------------------------------------------------------------------------
extern "C" void Plugin_pujOgreVTKCodec_EXPORT dllStopPlugin( )
{
  Ogre::Root::getSingleton( ).uninstallPlugin( gPlugin );
  delete gPlugin;
  gPlugin = nullptr;
}

// eof - Interface.cxx
