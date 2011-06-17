#ifndef FORTRAN_SUBROUTINE_H
#define FORTRAN_SUBROUTINE_H

#include <rose.h>

class FortranSubroutine
{
  protected:

    /*
     * ======================================================
     * Access to this statement allows ROSE to build function
     * call expressions to the generated subroutine. This
     * is needed, for example, when patching the user-supplied
     * code
     * ======================================================
     */
    SgProcedureHeaderStatement * subroutineHeaderStatement;

  protected:

    /*
     * ======================================================
     * Fortran subroutine constructor
     * ======================================================
     */
    FortranSubroutine ();

  public:

    /*
     * ======================================================
     * Returns the procedure header statement used internally
     * by ROSE in its abstract syntax tree
     * ======================================================
     */
    SgProcedureHeaderStatement *
    getProcedureHeaderStatement ();
};

#endif