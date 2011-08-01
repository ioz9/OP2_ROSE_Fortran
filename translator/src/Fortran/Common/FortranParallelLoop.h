/*
 * Written by Adam Betts and Carlo Bertolli
 */

#ifndef FORTRAN_PARALLEL_LOOP_H
#define FORTRAN_PARALLEL_LOOP_H

#include <ParallelLoop.h>

class FortranParallelLoop: public ParallelLoop
{
  private:

    /*
     * ======================================================
     * The following constants assume that each call to an
     * OP_PAR_LOOP in Fortran is constructed as follows:
     *
     * OP_PAR_LOOP_<N>
     *   (nameOfUserSubroutine,
     *    OP_SET,
     *    OP_DAT,   int,   OP_MAP,   OP_ACCESS,
     *    ...
     *    OP_DAT_N, int_N, OP_MAP_N, OP_ACCESS_N)
     * ======================================================
     */

    /*
     * ======================================================
     * Currently, for every OP_DAT, there is an int, an OP_MAP,
     * and an OP_ACCESS
     * ======================================================
     */
    static unsigned int const NUMBER_OF_ARGUMENTS_PER_OP_DAT = 4;

    /*
     * ======================================================
     * There are 2 non-OP_DAT arguments, namely
     * 'nameOfUserSubroutine' and 'OP_SET'
     * ======================================================
     */
    static unsigned int const NUMBER_OF_NON_OP_DAT_ARGUMENTS = 2;

  public:

    virtual unsigned int
    getNumberOfOpDatArgumentGroups ()
    {
      return (getActualArguments ().size () - NUMBER_OF_NON_OP_DAT_ARGUMENTS)
          / NUMBER_OF_ARGUMENTS_PER_OP_DAT;
    }

    FortranParallelLoop (SgFunctionCallExp * functionCallExpression) :
      ParallelLoop (functionCallExpression)
    {
    }
};

#endif
