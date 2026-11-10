// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/vtkParametricExpression.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include <vtkFunctionParser.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>

namespace pujOgre { vtkStandardNewMacro( vtkParametricExpression ); }

// -------------------------------------------------------------------------
int pujOgre::vtkParametricExpression::
GetDimension( )
{
  return( 2 );
}

// -------------------------------------------------------------------------
void pujOgre::vtkParametricExpression::
SetPointExpressions(
  const std::string& x, const std::string& y, const std::string& z
  )
{
  this->m_PointExpr[ 0 ] = x;
  this->m_PointExpr[ 1 ] = y;
  this->m_PointExpr[ 2 ] = z;
  this->m_Compiled = false;
  this->Modified( );
}

// -------------------------------------------------------------------------
void pujOgre::vtkParametricExpression::
SetNormalExpressions(
  const std::string& nx, const std::string& ny, const std::string& nz
  )
{
  this->m_NormalExpr[ 0 ] = nx;
  this->m_NormalExpr[ 1 ] = ny;
  this->m_NormalExpr[ 2 ] = nz;
  this->m_HasNormals = !( nx.empty( ) && ny.empty( ) && nz.empty( ) );
  this->m_Compiled = false;
  this->Modified( );
}

// -------------------------------------------------------------------------
void pujOgre::vtkParametricExpression::
SetConstant( const std::string& name, double value )
{
  if( name == "u" || name == "v" )
    throw std::runtime_error(
      "'u' and 'v' are reserved and cannot be used as constant names"
      );
  this->m_Constants[ name ] = value;
  this->m_Compiled = false;
  this->Modified( );
}

// -------------------------------------------------------------------------
void pujOgre::vtkParametricExpression::
Compile( )
{
  static const char* labels[] = {"x", "y", "z", "nx", "ny", "nz"};

  auto make = [this](const std::string& expr, const char* label) {
    if( expr.empty( ) )
      throw std::runtime_error(
        std::string("missing expression for '") + label + "'"
        );

    auto parser = vtkSmartPointer< vtkFunctionParser >::New( );
    parser->SetScalarVariableValue( "u", 0.0 );
    parser->SetScalarVariableValue( "v", 0.0 );
    for( const auto& c : this->m_Constants )
      parser->SetScalarVariableValue( c.first, c.second );
    parser->ReplaceInvalidValuesOn( );
    parser->SetReplacementValue( 0.0 );
    parser->SetFunction( expr.c_str( ) );
    if( !parser->IsScalarResult( ) )
      throw std::runtime_error(
        std::string("invalid expression for '") + label + "': " + expr
        );
    return( parser );
  };

  for( int i = 0; i < 3; ++i )
    this->m_Point[ i ] = make( this->m_PointExpr[ i ], labels[ i ] );

  for( int i = 0; i < 3; ++i )
    this->m_Normal[ i ]
      =
      ( this->m_HasNormals )
      ? make( this->m_NormalExpr[ i ], labels[ i + 3 ] )
      : nullptr;
  this->m_Compiled = true;
}

// -------------------------------------------------------------------------
void pujOgre::vtkParametricExpression::
Evaluate( double uvw[ 3 ], double Pt[ 3 ], double Duvw[ 9 ] )
{
  if( !this->m_Compiled )
    this->Compile( );

  const double u = uvw[ 0 ];
  const double v = uvw[ 1 ];
  double* Du = Duvw;
  double* Dv = Duvw + 3;
  Duvw[ 6 ] = Duvw[ 7 ] = Duvw[ 8 ] = 0.0;
  this->_Point( u, v, Pt );
  if( this->m_HasNormals )
  {
    // vtkParametricFunctionSource computes the normal as Du x Dv
    // (clockwise ordering) or Dv x Du (anticlockwise). Choose a tangent
    // pair whose cross product is exactly the user's normal, so the
    // output normals are the expression's normals.
    double n[ 3 ];
    for( int i = 0; i < 3; ++i )
      n[ i ] = this->_Scalar( this->m_Normal[ i ], u, v );

    if( vtkMath::Normalize( n ) == 0.0 )
    {
      std::fill( Duvw, Duvw + 6, 0.0 );
      return;
    } // end if

    double t[ 3 ], unused[ 3 ];
    vtkMath::Perpendiculars( n, t, unused, 0.0 );
    std::copy( t, t + 3, Du );

    if( this->ClockwiseOrdering )
      vtkMath::Cross( n, t, Dv ); // Du x Dv = t x (n x t) = n
    else
      vtkMath::Cross( t, n, Dv ); // Dv x Du = (t x n) x t = n
    return;
  } // end if

  // Numerical partial derivatives (central differences).
  const double hu
    = 1e-5 * std::max( 1e-12, std::abs(this->MaximumU - this->MinimumU ) );
  const double hv
    = 1e-5 * std::max( 1e-12, std::abs( this->MaximumV - this->MinimumV ) );
  double p0[ 3 ], p1[ 3 ];

  this->_Point( u - hu, v, p0 );
  this->_Point( u + hu, v, p1 );
  for( int i = 0; i < 3; ++i )
    Du[ i ] = ( p1[ i ] - p0[ i ] ) / ( 2.0 * hu );

  this->_Point( u, v - hv, p0 );
  this->_Point( u, v + hv, p1 );
  for( int i = 0; i < 3; ++i )
    Dv[ i ] = ( p1[ i ] - p0[ i ] ) / ( 2.0 * hv );
}

// -------------------------------------------------------------------------
double pujOgre::vtkParametricExpression::
EvaluateScalar( double*, double*, double* )
{
  return( double( 0 ) );
}

// -------------------------------------------------------------------------
pujOgre::vtkParametricExpression::
vtkParametricExpression( )
{
  this->MinimumU = 0.0;
  this->MaximumU = 1.0;
  this->MinimumV = 0.0;
  this->MaximumV = 1.0;
  this->JoinU = 0;
  this->JoinV = 0;
  this->DerivativesAvailable = 1;
  this->m_Constants[ "pi" ] = vtkMath::Pi( );
}

// -------------------------------------------------------------------------
pujOgre::vtkParametricExpression::
~vtkParametricExpression( )
{
}

// -------------------------------------------------------------------------
double pujOgre::vtkParametricExpression::
_Scalar( vtkFunctionParser* parser, double u, double v )
{
  parser->SetScalarVariableValue( "u", u );
  parser->SetScalarVariableValue( "v", v );
  return( parser->GetScalarResult( ) );
}

// -------------------------------------------------------------------------
void pujOgre::vtkParametricExpression::
_Point( double u, double v, double P[ 3 ] )
{
  for( int i = 0; i < 3; ++i )
    P[ i ] = this->_Scalar( this->m_Point[ i ], u, v );
}

// eof - vtkParametricExpression.cxx
