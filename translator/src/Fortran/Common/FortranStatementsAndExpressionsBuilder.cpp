#include <FortranStatementsAndExpressionsBuilder.h>
#include <FortranTypesBuilder.h>
#include <CommonNamespaces.h>
#include <RoseHelper.h>
#include <Debug.h>

/*
 * ======================================================
 * Private functions
 * ======================================================
 */

void
FortranStatementsAndExpressionsBuilder::setFortranAttributes (
    SgVariableDeclaration * variableDeclaration, int remainingArguments,
    va_list attributeArguments)
{
  /*
   * ======================================================
   * The variable declaration is always undefined
   * ======================================================
   */
  variableDeclaration->get_declarationModifier ().get_accessModifier ().setUndefined ();

  for (int i = 0; i < remainingArguments; ++i)
  {
    int attribute = va_arg(attributeArguments, int);

    switch (attribute)
    {
      case ALLOCATABLE:
      {
        variableDeclaration->get_declarationModifier ().get_typeModifier ().setAllocatable ();
        break;
      }

      case CONSTANT:
      {
        variableDeclaration->get_declarationModifier ().get_storageModifier ().setCudaConstant ();
        break;
      }

      case DEVICE:
      {
        variableDeclaration->get_declarationModifier ().get_storageModifier ().setCudaDevice ();
        break;
      }

      case SHARED:
      {
        variableDeclaration->get_declarationModifier ().get_storageModifier ().setCudaShared ();
        break;
      }

      case VALUE:
      {
        variableDeclaration->get_declarationModifier ().get_typeModifier ().setValue ();
        break;
      }

      case INTENT_IN:
      {
        variableDeclaration->get_declarationModifier ().get_typeModifier ().setIntent_in ();
        break;
      }

      case INTENT_OUT:
      {
        variableDeclaration->get_declarationModifier ().get_typeModifier ().setIntent_out ();
        break;
      }

      case INTENT_INOUT:
      {
        variableDeclaration->get_declarationModifier ().get_typeModifier ().setIntent_inout ();
        break;
      }

      default:
      {
        Debug::getInstance ()->errorMessage ("Unhandled Fortran attribute",
            __FILE__, __LINE__);
      }
    }
  }
}

/*
 * ======================================================
 * Public functions
 * ======================================================
 */

SgFortranDo *
FortranStatementsAndExpressionsBuilder::buildFortranDoStatement (
    SgExpression * initialization, SgExpression * bound,
    SgExpression * increment, SgBasicBlock * loopBody)
{
  using SageInterface::setOneSourcePositionForTransformation;

  SgFortranDo * fortranDoStatement = new SgFortranDo (initialization, bound,
      increment, loopBody);
  fortranDoStatement->setCaseInsensitive (true);
  fortranDoStatement->set_has_end_statement (true);

  setOneSourcePositionForTransformation (fortranDoStatement);

  initialization->set_parent (fortranDoStatement);
  bound->set_parent (fortranDoStatement);
  increment->set_parent (fortranDoStatement);
  loopBody->set_parent (fortranDoStatement);

  return fortranDoStatement;
}

SgExpression *
FortranStatementsAndExpressionsBuilder::buildShapeExpression (
    SgVariableDeclaration * variableDeclaration, SgScopeStatement * scope)
{
  using SageBuilder::buildOpaqueVarRefExp;

  SgExpression * shapeExpression = buildOpaqueVarRefExp ("(/"
      + RoseHelper::getFirstVariableName (variableDeclaration) + "/)", scope);

  return shapeExpression;
}

SgVariableDeclaration *
FortranStatementsAndExpressionsBuilder::appendVariableDeclaration (
    std::string const & variableName, SgType * type, SgScopeStatement * scope,
    int remainingArguments, ...)
{
  using SageBuilder::buildVariableDeclaration;
  using SageInterface::appendStatement;

  SgVariableDeclaration * variableDeclaration = buildVariableDeclaration (
      variableName, type, NULL, scope);

  appendStatement (variableDeclaration, scope);

  va_list fortranAttributes;

  va_start (fortranAttributes, remainingArguments);

  setFortranAttributes (variableDeclaration, remainingArguments,
      fortranAttributes);

  va_end (fortranAttributes);

  return variableDeclaration;
}

SgVariableDeclaration *
FortranStatementsAndExpressionsBuilder::appendVariableDeclarationAsFormalParameter (
    std::string const & variableName, SgType * type, SgScopeStatement * scope,
    SgFunctionParameterList * formalParameters, int remainingArguments, ...)
{
  using SageBuilder::buildVariableDeclaration;
  using SageInterface::appendStatement;

  SgVariableDeclaration * variableDeclaration = buildVariableDeclaration (
      variableName, type, NULL, scope);

  formalParameters->append_arg (
      *(variableDeclaration->get_variables ().begin ()));

  appendStatement (variableDeclaration, scope);

  va_list fortranAttributes;

  va_start (fortranAttributes, remainingArguments);

  setFortranAttributes (variableDeclaration, remainingArguments,
      fortranAttributes);

  va_end (fortranAttributes);

  return variableDeclaration;
}

void
FortranStatementsAndExpressionsBuilder::appendAllocateStatement (
    SgExprListExp * allocateParameters, SgScopeStatement * scope)
{
  using SageInterface::appendStatement;

  SgAllocateStatement * allocateStatement = new SgAllocateStatement (
      RoseHelper::getFileInfo ());
  allocateStatement->set_endOfConstruct (RoseHelper::getFileInfo ());
  allocateStatement->set_expr_list (allocateParameters);

  appendStatement (allocateStatement, scope);
}

void
FortranStatementsAndExpressionsBuilder::appendDeallocateStatement (
    SgExprListExp * deallocateParameters, SgScopeStatement * scope)
{
  using SageInterface::appendStatement;

  SgDeallocateStatement * deallocateStatement = new SgDeallocateStatement (
      RoseHelper::getFileInfo ());
  deallocateStatement->set_endOfConstruct (RoseHelper::getFileInfo ());
  deallocateStatement->set_expr_list (deallocateParameters);

  appendStatement (deallocateStatement, scope);
}

SgExpression *
FortranStatementsAndExpressionsBuilder::getFortranKindOfOpDat (
    SgType * OpDatBaseType)
{
  using SageBuilder::buildIntVal;

  SgArrayType * isArrayType = isSgArrayType (OpDatBaseType);

  ROSE_ASSERT (isArrayType != NULL);

  SgType * arrayBaseType = isArrayType->get_base_type ();

  SgExpression * sizeOfOpDatKind = arrayBaseType->get_type_kind ();

  /*
   * ======================================================
   * If a Fortran kind has not been specified, then we
   * have to assume standard ones: integer(4) and real(4)
   * ======================================================
   */
  if (sizeOfOpDatKind == NULL)
  {
    sizeOfOpDatKind = buildIntVal (4);
  }

  return sizeOfOpDatKind;
}

SgImplicitStatement *
FortranStatementsAndExpressionsBuilder::buildImplicitNoneStatement ()
{
  SgImplicitStatement * implicitStatement = new SgImplicitStatement (
      RoseHelper::getFileInfo (), true);

  implicitStatement->set_endOfConstruct (RoseHelper::getFileInfo ());
  implicitStatement->set_definingDeclaration (implicitStatement);

  return implicitStatement;
}
