// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <PUJ_Ogre/vtk/ParametricFunction.h>

#include <sstream>

#include <PUJ_Ogre/third_party/exprtk.h>
#include <PUJ_Ogre/third_party/ev3/expression.h>
#include <PUJ_Ogre/third_party/ev3/parser.h>

#include <vtkObjectFactory.h>

namespace PUJ_Ogre
{
  namespace vtk
  {
    /**
     */
    class ParametricFunction_Expression
    {
    public:
      ParametricFunction_Expression( const std::string& e, bool d = true );
      virtual ~ParametricFunction_Expression( );

      double eval( const double& u, const double& v ) const;
      const std::string& expression( ) const;
      const ParametricFunction_Expression* du( ) const;
      const ParametricFunction_Expression* dv( ) const;

    protected:
      std::string Expression;

      mutable double U;
      mutable double V;
      exprtk::symbol_table< double > Table;
      exprtk::expression< double >   Expr;
      exprtk::parser< double >       Parser;

      ParametricFunction_Expression* Du { nullptr };
      ParametricFunction_Expression* Dv { nullptr };
    };

    vtkStandardNewMacro( ParametricFunction );
  } // end namespace
} // end namespace


// -------------------------------------------------------------------------
void PUJ_Ogre::vtk::ParametricFunction::
PrintSelf( std::ostream& os, vtkIndent indent )
{
  this->Superclass::PrintSelf(os, indent);
  os
    << indent << "X : \"" << this->XExpr->expression( ) << "\"" << std::endl
    << indent << "Y : \"" << this->YExpr->expression( ) << "\"" << std::endl
    << indent << "Y : \"" << this->ZExpr->expression( ) << "\"" << std::endl;
}

// -------------------------------------------------------------------------
int PUJ_Ogre::vtk::ParametricFunction::
GetDimension( )
{
  return( 2 );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::vtk::ParametricFunction::
Evaluate( double uvw[ 3 ], double Pt[ 3 ], double Duvw[ 9 ] )
{
  Pt[ 0 ] = this->XExpr->eval( uvw[ 0 ], uvw[ 1 ] );
  Pt[ 1 ] = this->YExpr->eval( uvw[ 0 ], uvw[ 1 ] );
  Pt[ 2 ] = this->ZExpr->eval( uvw[ 0 ], uvw[ 1 ] );

  Duvw[ 0 ] = this->XExpr->du( )->eval( uvw[ 0 ], uvw[ 1 ] );
  Duvw[ 1 ] = this->YExpr->du( )->eval( uvw[ 0 ], uvw[ 1 ] );
  Duvw[ 2 ] = this->ZExpr->du( )->eval( uvw[ 0 ], uvw[ 1 ] );
  Duvw[ 3 ] = this->XExpr->dv( )->eval( uvw[ 0 ], uvw[ 1 ] );
  Duvw[ 4 ] = this->YExpr->dv( )->eval( uvw[ 0 ], uvw[ 1 ] );
  Duvw[ 5 ] = this->ZExpr->dv( )->eval( uvw[ 0 ], uvw[ 1 ] );
  Duvw[ 6 ] = Duvw[ 7 ] = Duvw[ 8 ] = 0;
}

// -------------------------------------------------------------------------
double PUJ_Ogre::vtk::ParametricFunction::
EvaluateScalar( double uvw[ 3 ], double Pt[ 3 ], double Duvw[ 9 ] )
{
  return( 0 );
}

// -------------------------------------------------------------------------
const std::string& PUJ_Ogre::vtk::ParametricFunction::
GetXExpr( ) const
{
  static const std::string& e = "";
  if( this->XExpr != nullptr )
    return( this->XExpr->expression( ) );
  else
    return( e );
}

// -------------------------------------------------------------------------
const std::string& PUJ_Ogre::vtk::ParametricFunction::
GetYExpr( ) const
{
  static const std::string& e = "";
  if( this->YExpr != nullptr )
    return( this->YExpr->expression( ) );
  else
    return( e );
}

// -------------------------------------------------------------------------
const std::string& PUJ_Ogre::vtk::ParametricFunction::
GetZExpr( ) const
{
  static const std::string& e = "";
  if( this->ZExpr != nullptr )
    return( this->ZExpr->expression( ) );
  else
    return( e );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::vtk::ParametricFunction::
SetParameters( const std::string& p )
{
  std::string minU, maxU, minV, maxV, openU, openV;
  std::istringstream( p )
    >> minU >> maxU >> openU
    >> minV >> maxV >> openV;

  ParametricFunction_Expression minUE( minU.c_str( ), false );
  ParametricFunction_Expression maxUE( maxU.c_str( ), false );
  ParametricFunction_Expression minVE( minV.c_str( ), false );
  ParametricFunction_Expression maxVE( maxV.c_str( ), false );

  this->MinimumU = minUE.eval( 0, 0 );
  this->MaximumU = maxUE.eval( 0, 0 );
  this->MinimumV = minVE.eval( 0, 0 );
  this->MaximumV = maxVE.eval( 0, 0 );
  this->MinimumW = 0;
  this->MaximumW = 0;
  this->JoinU = ( ( openU == "open" )? 0: 1 );
  this->JoinV = ( ( openV == "open" )? 0: 1 );
  this->JoinW = 0;
  this->TwistU = 0;
  this->TwistV = 0;
  this->TwistW = 0;
}

// -------------------------------------------------------------------------
void PUJ_Ogre::vtk::ParametricFunction::
SetXExpr( const std::string& e )
{
  if( this->XExpr != nullptr )
    delete this->XExpr;
  this->XExpr = new ParametricFunction_Expression( e );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::vtk::ParametricFunction::
SetYExpr( const std::string& e )
{
  if( this->YExpr != nullptr )
    delete this->YExpr;
  this->YExpr = new ParametricFunction_Expression( e );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::vtk::ParametricFunction::
SetZExpr( const std::string& e )
{
  if( this->ZExpr != nullptr )
    delete this->ZExpr;
  this->ZExpr = new ParametricFunction_Expression( e );
}

// -------------------------------------------------------------------------
PUJ_Ogre::vtk::ParametricFunction::
ParametricFunction( )
  : vtkParametricFunction( )
{
  this->SetParameters( "0 0 closed 0 0 closed" );
  this->SetXExpr( "0" );
  this->SetYExpr( "0" );
  this->SetZExpr( "0" );
  this->DerivativesAvailable = 1;
}

// -------------------------------------------------------------------------
PUJ_Ogre::vtk::ParametricFunction::
~ParametricFunction( )
{
  if( this->XExpr != nullptr ) delete this->XExpr;
  if( this->YExpr != nullptr ) delete this->YExpr;
  if( this->ZExpr != nullptr ) delete this->ZExpr;
}

// -------------------------------------------------------------------------
PUJ_Ogre::vtk::ParametricFunction_Expression::
ParametricFunction_Expression( const std::string& e, bool d )
  : Expression( e )
{
  this->Table.add_variable( "u", this->U );
  this->Table.add_variable( "v", this->V );
  this->Table.add_constants( );
  this->Expr.register_symbol_table( this->Table );
  this->Parser.compile( this->Expression, this->Expr );

  if( d )
  {
    Ev3::ExpressionParser ev3_p;
    ev3_p.SetVariableID( "u", 0 );
    ev3_p.SetVariableID( "v", 1 );

    int err = 0;
    Ev3::Expression ev3_e = ev3_p.Parse( this->Expression.c_str( ), err );
    if( err == 0 )
    {
      this->Du
        =
        new ParametricFunction_Expression
        ( Ev3::Diff( ev3_e, 0 )->ToString( ), false );
      this->Dv
        =
        new ParametricFunction_Expression
        ( Ev3::Diff( ev3_e, 1 )->ToString( ), false );
    } // end if
  } // end if
}

// -------------------------------------------------------------------------
PUJ_Ogre::vtk::ParametricFunction_Expression::
~ParametricFunction_Expression( )
{
  if( this->Du != nullptr ) delete this->Du;
  if( this->Dv != nullptr ) delete this->Dv;
}

// -------------------------------------------------------------------------
double PUJ_Ogre::vtk::ParametricFunction_Expression::
eval( const double& u, const double& v ) const
{
  this->U = u;
  this->V = v;
  return( this->Expr.value( ) );
}

// -------------------------------------------------------------------------
const std::string& PUJ_Ogre::vtk::ParametricFunction_Expression::
expression( ) const
{
  return( this->Expression );
}

// -------------------------------------------------------------------------
const PUJ_Ogre::vtk::ParametricFunction_Expression*
PUJ_Ogre::vtk::ParametricFunction_Expression::
du( ) const
{
  return( this->Du );
}

// -------------------------------------------------------------------------
const PUJ_Ogre::vtk::ParametricFunction_Expression*
PUJ_Ogre::vtk::ParametricFunction_Expression::
dv( ) const
{
  return( this->Dv );
}

// eof - $RCSfile$
