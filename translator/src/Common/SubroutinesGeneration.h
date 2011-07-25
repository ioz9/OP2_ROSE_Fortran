#ifndef SUBROUTINES_GENERATION_H
#define SUBROUTINES_GENERATION_H

#include <rose.h>
#include <Debug.h>

template <typename TDeclarations, typename TParallelLoop,
    typename THostSubroutine>
  class SubroutinesGeneration: public AstSimpleProcessing
  {
    protected:

      /*
       * ======================================================
       * The generated output files for each OP_PAR_LOOP
       * ======================================================
       */
      std::vector <SgSourceFile *> generatedFiles;

      /*
       * ======================================================
       * Used to retrieve base types and dimensions of OP_DAT
       * variables (and all other OP2 declarations, if needed)
       * and subroutines defined in the source files to be
       * processed
       * ======================================================
       */
      TDeclarations * declarations;

      /*
       * ======================================================
       * A mapping from a user subroutine name to our internal
       * representation of an OP_PAR_LOOP
       * ======================================================
       */
      std::map <std::string, TParallelLoop *> parallelLoops;

      /*
       * ======================================================
       * A mapping from a user subroutine name to its
       * generated host subroutine
       * ======================================================
       */
      std::map <std::string, THostSubroutine *> hostSubroutines;

      /*
       * ======================================================
       * The suffix of the file in which the new
       * subroutines are output
       * ======================================================
       */
      std::string fileSuffix;

      /*
       * ======================================================
       * The scope of the module into which the generated code
       * is output
       * ======================================================
       */
      SgScopeStatement * moduleScope;

    protected:

      /*
       * ======================================================
       * Creates the subroutines
       * ======================================================
       */
      virtual THostSubroutine *
      createSubroutines (TParallelLoop * parallelLoop,
          std::string const & userSubroutineName) = 0;

      /*
       * ======================================================
       * Patches the calls to OP_PAR_LOOPs with call to the
       * new host subroutines
       * ======================================================
       */
      virtual void
      patchCallsToParallelLoops () = 0;

      /*
       * ======================================================
       * Creates the Fortran/C/C++ source file to be unparsed
       * that contains the generated subroutines and declarations
       * ======================================================
       */
      virtual SgSourceFile &
      createSourceFile () = 0;

      /*
       * ======================================================
       * Generates output files for each OP_PAR_LOOP
       * ======================================================
       */
      void
      unparse ()
      {
        using std::vector;

        Debug::getInstance ()->debugMessage ("Generating new files",
            Debug::VERBOSE_LEVEL, __FILE__, __LINE__);

        for (vector <SgSourceFile *>::const_iterator it =
            generatedFiles.begin (); it != generatedFiles.end (); ++it)
        {
          Debug::getInstance ()->debugMessage ("Unparsing to '"
              + (*it)->getFileName () + "'", Debug::FUNCTION_LEVEL, __FILE__,
              __LINE__);

          /*
           * ======================================================
           * Unparse the created files after checking consistency
           * of ASTs
           * ======================================================
           */
          SgProject * project = (*it)->get_project ();

          AstTests::runAllTests (project);

          project->unparse ();
        }
      }

      SubroutinesGeneration (TDeclarations * declarations,
          std::string const & fileSuffix) :
        declarations (declarations), fileSuffix (fileSuffix)
      {
      }
  };

#endif
