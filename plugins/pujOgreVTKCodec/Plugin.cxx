// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Plugin.h"
#include "Codec.h"

// -------------------------------------------------------------------------
const Ogre::String& pujOgreVTKPlugin::
getName( ) const
{
  static const Ogre::String name = "pujOgre VTK PolyData Codec";
  return( name );
}

// -------------------------------------------------------------------------
void pujOgreVTKPlugin::
install( )
{
  for( const char* ext: { "obj", "vtp", "vtk" } )
  {
    auto* codec = new pujOgreVTKCodec( ext );
    Ogre::Codec::registerCodec( codec );
    this->m_Codecs.push_back( codec );
  } // end for
}

// -------------------------------------------------------------------------
void pujOgreVTKPlugin::
initialise( )
{
}

// -------------------------------------------------------------------------
void pujOgreVTKPlugin::
shutdown( )
{
}

// -------------------------------------------------------------------------
void pujOgreVTKPlugin::
uninstall( )
{
  for( auto* codec: this->m_Codecs )
  {
    Ogre::Codec::unregisterCodec( codec );
    delete codec;
  } // end for
  this->m_Codecs.clear( );
}

// eof - Plugin.cxx
