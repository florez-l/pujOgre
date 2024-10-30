// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <PUJ_Ogre/DotXXSceneLoader.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>

#include <vtkFloatArray.h>
#include <vtkOBJReader.h>
#include <vtkPointData.h>
#include <vtkPLYReader.h>
#include <vtkSTLReader.h>
#include <vtkNew.h>

#include <Ogre.h>

#include <pugixml.hpp>

// -------------------------------------------------------------------------
Ogre::Entity* PUJ_Ogre::DotXXSceneLoader::
_vtk( pugi::xml_node& XMLNode, Ogre::SceneNode* p )
{
  Ogre::String XMLfilename = Self::_attrib( XMLNode, "filename" );
  Ogre::String XMLmaterial = Self::_attrib( XMLNode, "material" );

  // Node
  Ogre::SceneNode* n = p;
  if( n == 0 )
    n = this->m_AttachNode->createChildSceneNode( "unnamed_vtk" );

  // Create temporary resources file
  std::stringstream str;
  str << "_vtk_resources_XXXXXX";
  std::string fname =
    ( std::filesystem::temp_directory_path( ) / str.str( ) ).string( );
  char* filename = new char[ fname.size( ) + 1 ];
  std::strcpy( filename, fname.c_str( ) );
  int fd = mkstemp( filename );
  if( fd == -1 )
  {
    /* TODO : SHOW ERROR
       std::cerr
       << "Error: could not create temporary resources file."
       << std::endl;
       return( EXIT_FAILURE );
    */
  } // end if

  // Save resources
  auto c
    =
    Ogre::ResourceGroupManager::getSingleton( ).openResource( XMLfilename );
  unsigned long long csize = c->size( );
  void* buf = std::calloc( csize, sizeof( char ) );
  c->read( buf, csize );
  write( fd, buf, csize );
  delete reinterpret_cast< char* >( buf );

  // Use VTK to read data
  std::string ext = XMLfilename.substr( XMLfilename.find_last_of( "." ) );
  std::transform(
    ext.begin( ), ext.end( ), ext.begin( ),
    []( const unsigned char& c )
    {
      return( std::tolower( c ) );
    }
    );

  vtkNew< vtkPolyData > pdata;
  if( ext == ".obj" )
  {
    vtkNew< vtkOBJReader > reader;
    reader->SetFileName( filename );
    reader->Update( );
    pdata->DeepCopy( reader->GetOutput( ) );
  }
  else if( ext == ".ply" )
  {
    vtkNew< vtkPLYReader > reader;
    reader->SetFileName( filename );
    reader->Update( );
    pdata->DeepCopy( reader->GetOutput( ) );
  }
  else if( ext == ".stl" )
  {
    vtkNew< vtkSTLReader > reader;
    reader->SetFileName( filename );
    reader->Update( );
    pdata->DeepCopy( reader->GetOutput( ) );
  } // end if

  // Finish loading
  close( fd );
  unlink( filename );
  delete filename;

  // Create manual object
  auto points = pdata->GetPoints( );
  auto normals = pdata->GetPointData( )->GetNormals( );
  auto tcoords
    =
    vtkFloatArray::SafeDownCast(
      pdata->GetPointData( )->GetAbstractArray( "Textures" )
      );
  auto polys = pdata->GetPolys( );
  Ogre::ManualObject* man
    =
    this->m_SceneMgr->createManualObject( n->getName( ) + "_vtk_" + ext );
  man->begin( XMLmaterial, Ogre::RenderOperation::OT_TRIANGLE_LIST );
  man->estimateVertexCount( points->GetNumberOfPoints( ) );
  man->estimateIndexCount( polys->GetNumberOfCells( ) * 3 );

  for( unsigned long long i = 0; i < points->GetNumberOfPoints( ); ++i )
  {
    double* p = points->GetPoint( i );
    double* n = normals->GetTuple( i );
    man->position( p[ 0 ], p[ 1 ], p[ 2 ] );
    man->normal( n[ 0 ], n[ 1 ], n[ 2 ] );

    if( tcoords != nullptr )
    {
      double* t = tcoords->GetTuple( i );
      man->textureCoord( t[ 0 ], t[ 1 ] );
    } // end if
  } // end for

  vtkNew< vtkIdList > ids;
  polys->InitTraversal( );
  for( unsigned long long i = 0; i < polys->GetNumberOfCells( ); ++i )
  {
    polys->GetNextCell( ids );
    if( ids->GetNumberOfIds( ) == 3 )
      man->triangle( ids->GetId( 0 ), ids->GetId( 1 ), ids->GetId( 2 ) );
    else if( ids->GetNumberOfIds( ) == 4 )
      man->quad(
        ids->GetId( 0 ), ids->GetId( 1 ), ids->GetId( 2 ), ids->GetId( 3 )
        );
  } // end for

  man->end( );
  auto mesh = man->convertToMesh( n->getName( ) + "_mesh" );
  auto ent = this->m_SceneMgr->createEntity( mesh );
  n->attachObject( ent );
  return( ent );
}

// eof - $RCSfile$
