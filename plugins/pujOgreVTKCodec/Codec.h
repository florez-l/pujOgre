// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__VTKCodec__Codec__h__
#define __pujOgre__VTKCodec__Codec__h__

#include <plugins/pujOgreVTKCodec/Export.h>
#include <OgreCodec.h>

/**
 */
class Plugin_pujOgreVTKCodec_EXPORT pujOgreVTKCodec
  : public Ogre::Codec
{
public:
  explicit pujOgreVTKCodec( const Ogre::String& extension );
  virtual ~pujOgreVTKCodec( ) override = default;

  virtual Ogre::String getType( ) const override;

  virtual Ogre::String magicNumberToFileExt(
    const char*, std::size_t
    ) const override;

  virtual void decode(
    const Ogre::DataStreamPtr& input, const Ogre::Any& output
    ) const override;

private:
  Ogre::String m_Type;
};

#endif // __pujOgre__VTKCodec__Codec__h__

// eof - Codec.h
