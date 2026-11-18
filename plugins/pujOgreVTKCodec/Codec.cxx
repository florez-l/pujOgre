// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Codec.h"

#include <OgreDataStream.h>

#include <vtkCenterOfMass.h>
#include <vtkMemoryResourceStream.h>
#include <vtkOBJReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>

#include <pujOgre/vtkPolyData2OgreMesh.h>

// -------------------------------------------------------------------------
pujOgreVTKCodec::
pujOgreVTKCodec( const Ogre::String& extension )
  : m_Type( extension )
{
}

// -------------------------------------------------------------------------
Ogre::String pujOgreVTKCodec::
getType( ) const
{
  return( this->m_Type );
}

// -------------------------------------------------------------------------
Ogre::String pujOgreVTKCodec::
magicNumberToFileExt( const char*, std::size_t ) const
{
  return( "" );
}

// -------------------------------------------------------------------------
void pujOgreVTKCodec::
decode( const Ogre::DataStreamPtr& input, const Ogre::Any& output ) const
{
  Ogre::Mesh* mesh = Ogre::any_cast< Ogre::Mesh* >( output );
  const std::string data = input->getAsString( );

  vtkSmartPointer< vtkPolyData > pd;
  if( this->m_Type == "obj" )
  {
    auto stream = vtkSmartPointer< vtkMemoryResourceStream >::New( );
    stream->SetBuffer( data.data( ), data.size( ), false );

    auto reader = vtkSmartPointer< vtkOBJReader >::New( );
    reader->SetStream( stream );
    reader->Update( );
    pd = reader->GetOutput( );
  }
  else if( this->m_Type == "vtp" )
  {
    auto reader = vtkSmartPointer< vtkXMLPolyDataReader >::New( );
    reader->ReadFromInputStringOn( );
    reader->SetInputString( data );
    reader->Update( );
    pd = reader->GetOutput( );
  }
  else
  {
    auto reader = vtkSmartPointer< vtkPolyDataReader >::New( );
    reader->ReadFromInputStringOn( );
    reader->SetBinaryInputString( data.data( ), int( data.size( ) ) );
    reader->Update( );
    pd = reader->GetOutput( );
  } // end if

  if( pd.GetPointer( ) == nullptr || pd->GetNumberOfPoints( ) == 0 )
    OGRE_EXCEPT(
      Ogre::Exception::ERR_INVALIDPARAMS,
      "[pujOgreVTKCodec::decode]: No polydata could be read from '"
      + input->getName( ) + "'"
      );

  double center[ 3 ];
  auto cog = vtkSmartPointer< vtkCenterOfMass >::New( );
  cog->SetInputData( pd );
  cog->SetUseScalarsAsWeights( false );
  cog->Update( );
  cog->GetCenter( center );

  // Material can be changed later with entity->setMaterialName(...)
  pujOgre::FillOgreMeshFromvtkPolyData(
    pd, mesh, center, "BaseWhite", true, true
    );
}

// eof - Codec.cxx
