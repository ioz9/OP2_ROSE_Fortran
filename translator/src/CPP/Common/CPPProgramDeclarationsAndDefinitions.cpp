#include <CPPProgramDeclarationsAndDefinitions.h>
#include <CPPOP2Definitions.h>
#include <CommonNamespaces.h>

/*
 * ======================================================
 * Private functions
 * ======================================================
 */

void
CPPProgramDeclarationsAndDefinitions::detectAndHandleOP2Definition (
    SgVariableDeclaration * variableDeclaration,
    std::string const variableName, SgTypedefType * typeDefinition)
{
  using boost::iequals;
  using std::string;

  string const typeName = typeDefinition->get_name ().getString ();

  if (iequals (typeName, OP2::OP_SET))
  {
    SgAssignInitializer * assignmentInitializer = isSgAssignInitializer (
        variableDeclaration->get_decl_item (variableName)->get_initializer ());

    ROSE_ASSERT (assignmentInitializer != NULL);

    SgFunctionCallExp * functionCallExpression = isSgFunctionCallExp (
        assignmentInitializer->get_operand ());

    ROSE_ASSERT (functionCallExpression != NULL);

    SgExpressionPtrList & actualArguments =
        functionCallExpression->get_args ()->get_expressions ();

    CPPImperialOpSetDefinition * opSetDeclaration =
        new CPPImperialOpSetDefinition (actualArguments, variableName);

    OpSetDefinitions[opSetDeclaration->getVariableName ()] = opSetDeclaration;
  }
  else if (iequals (typeName, OP2::OP_MAP))
  {
    SgAssignInitializer * assignmentInitializer = isSgAssignInitializer (
        variableDeclaration->get_decl_item (variableName)->get_initializer ());

    ROSE_ASSERT (assignmentInitializer != NULL);

    SgFunctionCallExp * functionCallExpression = isSgFunctionCallExp (
        assignmentInitializer->get_operand ());

    ROSE_ASSERT (functionCallExpression != NULL);

    SgExpressionPtrList & actualArguments =
        functionCallExpression->get_args ()->get_expressions ();

    CPPImperialOpMapDefinition * opMapDeclaration =
        new CPPImperialOpMapDefinition (actualArguments, variableName);

    OpMapDefinitions[opMapDeclaration->getVariableName ()] = opMapDeclaration;
  }
  else if (iequals (typeName, OP2::OP_DAT))
  {
    SgAssignInitializer * assignmentInitializer = isSgAssignInitializer (
        variableDeclaration->get_decl_item (variableName)->get_initializer ());

    ROSE_ASSERT (assignmentInitializer != NULL);

    SgFunctionCallExp * functionCallExpression = isSgFunctionCallExp (
        assignmentInitializer->get_operand ());

    ROSE_ASSERT (functionCallExpression != NULL);

    SgExpressionPtrList & actualArguments =
        functionCallExpression->get_args ()->get_expressions ();

    CPPImperialOpDatDefinition * opDatDeclaration =
        new CPPImperialOpDatDefinition (actualArguments, variableName);

    OpDatDefinitions[opDatDeclaration->getVariableName ()] = opDatDeclaration;
  }
}

void
CPPProgramDeclarationsAndDefinitions::visit (SgNode * node)
{
  using boost::iequals;
  using std::string;

  switch (node->variantT ())
  {
    case V_SgVariableDeclaration:
    {
      SgVariableDeclaration * variableDeclaration = isSgVariableDeclaration (
          node);

      string const
          variableName =
              variableDeclaration->get_variables ().front ()->get_name ().getString ();

      SgType * type =
          variableDeclaration->get_decl_item (variableName)->get_type ();

      if (isSgTypedefType (type) != NULL)
      {
        detectAndHandleOP2Definition (variableDeclaration, variableName,
            isSgTypedefType (type));
      }
      else if (isSgPointerType (type) != NULL)
      {
        Debug::getInstance ()->debugMessage ("'" + variableName
            + "' is a pointer", Debug::FUNCTION_LEVEL, __FILE__, __LINE__);

        handleBaseTypeDeclaration (isSgPointerType (type)->get_base_type (),
            variableName);
      }
      else if (isSgArrayType (type) != NULL)
      {
        Debug::getInstance ()->debugMessage ("'" + variableName
            + "' is a array", Debug::FUNCTION_LEVEL, __FILE__, __LINE__);

        handleBaseTypeDeclaration (isSgArrayType (type)->get_base_type (),
            variableName);
      }
      else
      {
        handleBaseTypeDeclaration (type, variableName);
      }

      break;
    }
      /*
       case V_SgInitializedName:
       {
       SgInitializedName *initializedName = isSgInitializedName(node);

       if (initializedName)
       {
       string const varName = initializedName->get_name().getString();

       SgAssignInitializer *assignInitializer = isSgAssignInitializer(initializedName->get_initializer());

       if (assignInitializer)
       {
       SgFunctionCallExp *functionCallExp = isSgFunctionCallExp (assignInitializer->get_operand());
       if (functionCallExp)
       {
       Debug::getInstance ()->debugMessage ("Found initializer for '"
       + varName + "'", Debug::FUNCTION_LEVEL, __FILE__, __LINE__);
       }
       }
       }
       break;
       }*/

    case V_SgFunctionDefinition:
    {
      SgFunctionDefinition * functionDeclaration =
          isSgFunctionDefinition (node);

      Debug::getInstance ()->debugMessage ("Found definition: "
          + functionDeclaration->get_declaration ()->get_name ().getString (),
          Debug::INNER_LOOP_LEVEL, __FILE__, __LINE__);

      break;
    }

    case V_SgFunctionCallExp:
    {
      /*
       * ======================================================
       * Function call found in the AST. Get its actual arguments
       * and the callee name
       * ======================================================
       */
      SgFunctionCallExp * functionCallExp = isSgFunctionCallExp (node);

      string const
          calleeName =
              functionCallExp->getAssociatedFunctionSymbol ()->get_name ().getString ();

      Debug::getInstance ()->debugMessage ("Found function call to '"
          + calleeName + "'", Debug::FUNCTION_LEVEL, __FILE__, __LINE__);

      if (iequals (calleeName, OP2::OP_DECL_CONST))
      {
        /*
         * ======================================================
         * An OP_CONST variable defined through an OP_DECL_CONST call
         * ======================================================
         */

        CPPImperialOpConstDefinition * opConstDeclaration =
            new CPPImperialOpConstDefinition (
                functionCallExp->get_args ()->get_expressions ());

        OpConstDefinitions[opConstDeclaration->getVariableName ()]
            = opConstDeclaration;
      }
      if (iequals (calleeName, OP2::OP_PAR_LOOP))
      {
        SgExpressionPtrList & actualArguments =
            functionCallExp->get_args ()->get_expressions ();

        SgFunctionRefExp * functionRefExpression = isSgFunctionRefExp (
            actualArguments.front ());

        ROSE_ASSERT (functionRefExpression != NULL);

        string const
            userSubroutineName =
                functionRefExpression->getAssociatedFunctionDeclaration ()->get_name ().getString ();

        Debug::getInstance ()->debugMessage ("Found '" + calleeName
            + "' with (host) user subroutine '" + userSubroutineName + "'",
            Debug::OUTER_LOOP_LEVEL, __FILE__, __LINE__);

        if (parallelLoops.find (userSubroutineName) == parallelLoops.end ())
        {
          /*
           * ======================================================
           * If this kernel has not been previously encountered then
           * build a new parallel loop representation
           * ======================================================
           */

          CPPParallelLoop * parallelLoop =
              new CPPParallelLoop (functionCallExp);

          parallelLoops[userSubroutineName] = parallelLoop;
        }
        else
        {
          Debug::getInstance ()->debugMessage ("Parallel loop for '"
              + userSubroutineName + "' already created",
              Debug::OUTER_LOOP_LEVEL, __FILE__, __LINE__);
        }

      }

      break;
    }

    default:
    {
      break;
    }
  }
}

/*
 * ======================================================
 * Public functions
 * ======================================================
 */

CPPProgramDeclarationsAndDefinitions::CPPProgramDeclarationsAndDefinitions (
    SgProject * project)
{
  traverseInputFiles(project, preorder);
}
