#include "FortranOpenMPSubroutinesGeneration.h"
#include "FortranParallelLoop.h"
#include "FortranUserSubroutine.h"
#include "FortranOpenMPModuleDeclarationsIndirectLoop.h"
#include "FortranOpenMPKernelSubroutineDirectLoop.h"
#include "FortranOpenMPKernelSubroutineIndirectLoop.h"
#include "FortranOpenMPHostSubroutineDirectLoop.h"
#include "FortranOpenMPHostSubroutineIndirectLoop.h"
#include "FortranProgramDeclarationsAndDefinitions.h"
#include "FortranOpDatDimensionsDeclaration.h"
#include "RoseHelper.h"
#include <boost/algorithm/string.hpp>

void
FortranOpenMPSubroutinesGeneration::createReductionSubroutines ()
{
}

void
FortranOpenMPSubroutinesGeneration::createSubroutines ()
{
  using std::string;
  using std::map;

  for (map <string, ParallelLoop *>::const_iterator it =
      declarations->firstParallelLoop (); it
      != declarations->lastParallelLoop (); ++it)
  {
    string const userSubroutineName = it->first;

    FortranParallelLoop * parallelLoop =
        static_cast <FortranParallelLoop *> (it->second);

    FortranUserSubroutine * userSubroutine = new FortranUserSubroutine (
        moduleScope, parallelLoop, declarations);

    FortranOpenMPKernelSubroutine * kernelSubroutine;

    if (parallelLoop->isDirectLoop ())
    {
      kernelSubroutine = new FortranOpenMPKernelSubroutineDirectLoop (
          moduleScope, userSubroutine, parallelLoop);

      hostSubroutines[userSubroutineName]
          = new FortranOpenMPHostSubroutineDirectLoop (moduleScope,
              kernelSubroutine, parallelLoop);
    }
    else
    {
      kernelSubroutine = new FortranOpenMPKernelSubroutineIndirectLoop (
          moduleScope, userSubroutine, parallelLoop);

      hostSubroutines[userSubroutineName]
          = new FortranOpenMPHostSubroutineIndirectLoop (
              moduleScope,
              kernelSubroutine,
              parallelLoop,
              static_cast <FortranOpenMPModuleDeclarationsIndirectLoop *> (moduleDeclarations[userSubroutineName]));
    }
  }
}

void
FortranOpenMPSubroutinesGeneration::createModuleDeclarations ()
{
  using std::map;
  using std::string;

  /*
   * ======================================================
   * First declare the type declarations
   * ======================================================
   */

  for (map <string, ParallelLoop *>::const_iterator it =
      declarations->firstParallelLoop (); it
      != declarations->lastParallelLoop (); ++it)
  {
    string const userSubroutineName = it->first;

    FortranParallelLoop * parallelLoop =
        static_cast <FortranParallelLoop *> (it->second);

    dimensionsDeclarations[userSubroutineName]
        = new FortranOpDatDimensionsDeclaration (parallelLoop, moduleScope);
  }

  /*
   * ======================================================
   * Now declare the variables
   * ======================================================
   */

  for (map <string, ParallelLoop *>::const_iterator it =
      declarations->firstParallelLoop (); it
      != declarations->lastParallelLoop (); ++it)
  {
    string const userSubroutineName = it->first;

    FortranParallelLoop * parallelLoop =
        static_cast <FortranParallelLoop *> (it->second);

    if (parallelLoop->isDirectLoop () == false)
    {
      moduleDeclarations[userSubroutineName]
          = new FortranOpenMPModuleDeclarationsIndirectLoop (parallelLoop,
              moduleScope);
    }
  }
}

void
FortranOpenMPSubroutinesGeneration::addLibraries ()
{
  using boost::iequals;
  using SageInterface::appendStatement;
  using SageInterface::addTextForUnparser;
  using std::string;
  using std::vector;

  Debug::getInstance ()->debugMessage (
      "Adding 'use' statements to OpenMP module", Debug::FUNCTION_LEVEL,
      __FILE__, __LINE__);

  vector <string> libs;
  libs.push_back ("OP2_C");
  libs.push_back ("OMP_LIB");

  for (vector <string>::const_iterator it = libs.begin (); it != libs.end (); ++it)
  {
    SgUseStatement* useStatement = new SgUseStatement (
        RoseHelper::getFileInfo (), *it, false);

    useStatement->set_definingDeclaration (useStatement);

    appendStatement (useStatement, moduleScope);

    if (iequals (*it, "OMP_LIB"))
    {
      addTextForUnparser (useStatement, "#ifdef _OPENMP\n",
          AstUnparseAttribute::e_before);

      addTextForUnparser (useStatement, "#endif\n",
          AstUnparseAttribute::e_after);
    }
  }
}
