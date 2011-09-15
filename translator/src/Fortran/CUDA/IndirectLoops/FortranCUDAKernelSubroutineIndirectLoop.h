/*
 * Written by Adam Betts and Carlo Bertolli
 *
 * This class models the kernel subroutine for an indirect loop.
 * Its declarations and statements do the following:
 *
 */

#ifndef FORTRAN_CUDA_KERNEL_SUBROUTINE_INDIRECT_LOOP_H
#define FORTRAN_CUDA_KERNEL_SUBROUTINE_INDIRECT_LOOP_H

#include <FortranCUDAKernelSubroutine.h>
#include <FortranCUDADataSizesDeclarationIndirectLoop.h>

class FortranCUDAKernelSubroutineIndirectLoop: public FortranCUDAKernelSubroutine
{
  private:

    std::map <std::string, unsigned int> positionOfNbytes;

  private:

    virtual SgStatement *
    createUserSubroutineCallStatement ();

    virtual void
    createExecutionLoopStatements ();

    void
    createInnerExecutionLoopStatements (SgScopeStatement * scope);

    void
    createIncrementAdjustmentStatements (SgScopeStatement * scope);

    void
    createInitialiseLocalOpDatStatements (SgScopeStatement * scope);

    void
    createPointeredIncrementsOrWritesStatements ();

    void
    createAutoSharedWhileLoopStatements ();

    void
    createInitialiseLocalVariablesStatements ();

    void
    createThreadZeroStatements ();

    void
    createPlanFormalParameterDeclarations ();

    virtual void
    createOpDatFormalParameterDeclarations ();

    virtual void
    createStatements ();

    virtual void
    createLocalVariableDeclarations ();

    virtual void
    createFormalParameterDeclarations ();

  public:

    FortranCUDAKernelSubroutineIndirectLoop (
        std::string const & subroutineName,
        std::string const & userSubroutineName,
        FortranParallelLoop * parallelLoop, SgScopeStatement * moduleScope,
        FortranReductionSubroutines * reductionSubroutines,
        FortranCUDADataSizesDeclarationIndirectLoop * dataSizesDeclaration,
        FortranOpDatDimensionsDeclaration * opDatDimensionsDeclaration,
        FortranCUDAModuleDeclarations * moduleDeclarations);
};

#endif
