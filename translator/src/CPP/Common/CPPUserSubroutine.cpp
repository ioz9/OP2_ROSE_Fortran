#include "CPPUserSubroutine.h"
#include "CPPParallelLoop.h"
#include "CPPProgramDeclarationsAndDefinitions.h"
#include "RoseStatementsAndExpressionsBuilder.h"
#include "RoseHelper.h"

void
CPPUserSubroutine::createStatements ()
{
  using namespace SageInterface;
  using std::vector;

  Debug::getInstance ()->debugMessage ("Outputting and modifying statements",
      Debug::FUNCTION_LEVEL, __FILE__, __LINE__);

  vector <SgStatement *> originalStatements =
      originalSubroutine->get_definition ()->get_body ()->get_statements ();

  for (vector <SgStatement *>::iterator it = originalStatements.begin (); it
      != originalStatements.end (); ++it)
  {
    appendStatement (*it, subroutineScope);
  }
}

void
CPPUserSubroutine::createLocalVariableDeclarations ()
{
}

void
CPPUserSubroutine::createFormalParameterDeclarations ()
{
  using std::string;
  using std::vector;

  Debug::getInstance ()->debugMessage ("Outputting formal parameters",
      Debug::FUNCTION_LEVEL, __FILE__, __LINE__);

  SgFunctionParameterList * originalParameters =
      originalSubroutine->get_parameterList ();

  for (SgInitializedNamePtrList::iterator paramIt =
      originalParameters->get_args ().begin (); paramIt
      != originalParameters->get_args ().end (); ++paramIt)
  {
    string const variableName = (*paramIt)->get_name ().getString ();

    SgType * type = (*paramIt)->get_type ();

    SgVariableDeclaration
        * variableDeclaration =
            RoseStatementsAndExpressionsBuilder::appendVariableDeclarationAsFormalParameter (
                variableName, type, subroutineScope, formalParameters);
  }
}

CPPUserSubroutine::CPPUserSubroutine (SgScopeStatement * moduleScope,
    CPPParallelLoop * parallelLoop,
    CPPProgramDeclarationsAndDefinitions * declarations) :
      UserSubroutine <SgFunctionDeclaration,
          CPPProgramDeclarationsAndDefinitions> (parallelLoop, declarations)
{
  using namespace SageBuilder;
  using namespace SageInterface;

  subroutineHeaderStatement = buildDefiningFunctionDeclaration (
      this->subroutineName.c_str (), buildVoidType (), formalParameters,
      moduleScope);

  appendStatement (subroutineHeaderStatement, moduleScope);

  subroutineScope = subroutineHeaderStatement->get_definition ()->get_body ();

  originalSubroutine = declarations->getSubroutine (
      parallelLoop->getUserSubroutineName ());

  createFormalParameterDeclarations ();

  createLocalVariableDeclarations ();

  createStatements ();

  RoseHelper::forceOutputOfCodeToFile (subroutineHeaderStatement);
}
