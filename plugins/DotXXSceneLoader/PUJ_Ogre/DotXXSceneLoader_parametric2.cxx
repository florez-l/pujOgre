// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <PUJ_Ogre/DotXXSceneLoader.h>
#include <PUJ_Ogre/vtk/ParametricFunction.h>
#include <vtkFloatArray.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPointData.h>
#include <vtkNew.h>




#include <vtkCellData.h>




#include <Ogre.h>
#include <pugixml.hpp>

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
_parametric2( pugi::xml_node& XMLNode, Ogre::SceneNode* p )
{
  Ogre::String material = Self::_attrib( XMLNode, "material" );
  Ogre::String urange = Self::_attrib( XMLNode, "urange", "0 0 open" );
  Ogre::String vrange = Self::_attrib( XMLNode, "vrange", "0 0 open" );
  Ogre::String sampling = Self::_attrib( XMLNode, "sampling", "0 0" );
  Ogre::String x = Self::_attrib( XMLNode.child( "x" ), "function", "0" );
  Ogre::String y = Self::_attrib( XMLNode.child( "y" ), "function", "0" );
  Ogre::String z = Self::_attrib( XMLNode.child( "z" ), "function", "0" );

  std::stringstream params;
  params << urange << " " << vrange;

  unsigned int usamples, vsamples;
  std::istringstream( sampling ) >> usamples >> vsamples;

  // Node
  Ogre::SceneNode* n = p;
  if( n == 0 )
    n = this->m_AttachNode->createChildSceneNode( "unnamed_parametric2" );

  // Create model
  vtkNew< PUJ_Ogre::vtk::ParametricFunction > model;
  model->SetParameters( params.str( ).c_str( ) );
  model->SetXExpr( x.c_str( ) );
  model->SetYExpr( y.c_str( ) );
  model->SetZExpr( z.c_str( ) );

  // Sample the model
  vtkNew< vtkParametricFunctionSource > src;
  src->SetParametricFunction( model );
  src->SetUResolution( usamples );
  src->SetVResolution( vsamples );
  src->SetWResolution( 1 );
  src->GenerateNormalsOn( );
  src->GenerateTextureCoordinatesOn( );
  src->Update( );

  // Create manual object
  auto pdata = src->GetOutput( );
  auto points = pdata->GetPoints( );
  auto normals = pdata->GetPointData( )->GetNormals( );
  auto tcoords = pdata->GetPointData( )->GetAbstractArray( "Textures" );
  auto polys = pdata->GetPolys( );

  Ogre::ManualObject* man
    =
    this->m_SceneMgr->createManualObject( n->getName( ) + "_parametric2" );
  man->begin( material, Ogre::RenderOperation::OT_TRIANGLE_LIST );
  man->estimateVertexCount( points->GetNumberOfPoints( ) );
  man->estimateIndexCount( polys->GetNumberOfCells( ) * 3 );

  for( unsigned long long i = 0; i < points->GetNumberOfPoints( ); ++i )
  {
    double* p = points->GetPoint( i );
    double* n = normals->GetTuple( i );
    double* t = tcoords->GetTuple( i );

    man->position( p[ 0 ], p[ 1 ], p[ 2 ] );
    man->normal( n[ 0 ], n[ 1 ], n[ 2 ] );
    man->textureCoord( t[ 0 ], t[ 1 ] );
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
  n->attachObject( man );
}

// eof - $RCSfile$
