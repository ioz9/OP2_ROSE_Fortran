/*
 * Written by Adam Betts and Carlo Bertolli
 *
 * This class models an OP_PAR_LOOP call discovered in the
 * user-supplied code
 */

#ifndef PARALLEL_LOOP_H
#define PARALLEL_LOOP_H

#include <CommonNamespaces.h>
#include <ProgramDeclarationsAndDefinitions.h>
#include <OP2Definitions.h>
#include <Reduction.h>
#include <boost/lexical_cast.hpp>

enum MAPPING_VALUE
{
  DIRECT, INDIRECT, GLOBAL
};

enum ACCESS_CODE_VALUE
{
  READ_ACCESS, WRITE_ACCESS, RW_ACCESS, INC_ACCESS, MAX_ACCESS, MIN_ACCESS
};

class ParallelLoop
{
  protected:

    /*
     * ======================================================
     * The function call expression where the call to the
     * OP_PAR_LOOP occurs
     * ======================================================
     */
    SgFunctionCallExp * functionCallExpression;

    /*
     * ======================================================
     * The same OP_DAT variable can be passed as an argument
     * to OP_PAR_LOOP. This vector records the names of the
     * unique OP_DATs
     * ======================================================
     */
    std::vector <std::string> uniqueOpDats;

    /*
     * ======================================================
     * The base type of each OP_DAT. The map is indexed by
     * the argument group number
     * ======================================================
     */
    std::map <unsigned int, SgType *> OpDatTypes;

    /*
     * ======================================================
     * The dimension of each OP_DAT. The map is indexed by
     * the argument group number
     * ======================================================
     */
    std::map <unsigned int, unsigned int> OpDatDimensions;

    /*
     * ======================================================
     * The same OP_DAT variable can be passed as an argument
     * to OP_PAR_LOOP. This map records whether the OP_DAT argument
     * at this position (in the actual arguments) is a duplicate
     * of a previous OP_DAT argument
     * ======================================================
     */
    std::map <unsigned int, bool> OpDatDuplicates;

    /*
     * ======================================================
     * How is the data for the OP_DAT variables in this position
     * (in the actual arguments) accessed: through
     * a mapping or directly or as a global variable?
     * ======================================================
     */
    std::map <unsigned int, MAPPING_VALUE> OpDatMappingDescriptors;

    /*
     * ======================================================
     * How is the data for the OP_DAT variables in this position
     * (in the actual arguments) accessed: read, written, etc.
     * ======================================================
     */
    std::map <unsigned int, ACCESS_CODE_VALUE> OpDatAccessDescriptors;

    /*
     * ======================================================
     * What is OP_DAT variable name in this position
     * (in the actual arguments)?
     * ======================================================
     */
    std::map <unsigned int, std::string> OpDatVariableNames;

    /*
     * ======================================================
     * The reductions needed in this Parallel loop
     * ======================================================
     */
    std::vector <Reduction *> reductionsNeeded;

  protected:

    ParallelLoop (SgFunctionCallExp * functionCallExpression);

  public:

    /*
     * ======================================================
     * How many OP_DAT argument groups are there?
     * ======================================================
     */
    virtual unsigned int
    getNumberOfOpDatArgumentGroups () = 0;

    /*
     * ======================================================
     * Is this parallel loop direct or indirect?
     * ======================================================
     */
    bool
    isDirectLoop ();

    /*
     * ======================================================
     * How many OP_DAT arguments have indirect access
     * descriptors?
     * ======================================================
     */
    unsigned int
    getNumberOfDistinctIndirectOpDatArguments ();

    void
    setOpDatType (unsigned int OP_DAT_ArgumentGroup, SgType * type);

    /*
     * ======================================================
     * What is the type which is wrapped by the OP_DAT declaration
     * in the particular OP_DAT argument group?
     * ======================================================
     */
    SgType *
    getOpDatType (unsigned int OP_DAT_ArgumentGroup);

    /*
     * ======================================================
     * What is the base type of the (assumed) array type wrapped
     * by the OP_DAT declaration, in the particular OP_DAT
     * argument group?
     * ======================================================
     */
    SgType *
    getOpDatBaseType (unsigned int OP_DAT_ArgumentGroup);

    void
    setOpDatDimension (unsigned int OP_DAT_ArgumentGroup,
        unsigned int dimension);

    /*
     * ======================================================
     * What is the dimension of the OP_DAT variable in
     * this OP_DAT argument group?
     * ======================================================
     */
    unsigned int
    getOpDatDimension (unsigned int OP_DAT_ArgumentGroup);

    void
    setDuplicateOpDat (unsigned int OP_DAT_ArgumentGroup, bool value);

    /*
     * ======================================================
     * Is the OP_DAT variable in this OP_DAT argument group
     * the same as another OP_DAT in the actual arguments
     * passed to the OP_PAR_LOOP?
     * ======================================================
     */
    bool
    isDuplicateOpDat (unsigned int OP_DAT_ArgumentGroup);

    void
    setOpMapValue (unsigned int OP_DAT_ArgumentGroup, MAPPING_VALUE value);

    bool
    isIndirect (unsigned int OP_DAT_ArgumentGroup);

    bool
    isDirect (unsigned int OP_DAT_ArgumentGroup);

    bool
    isGlobal (unsigned int OP_DAT_ArgumentGroup);

    bool
    isGlobalScalar (unsigned int OP_DAT_ArgumentGroup);

    bool
    isGlobalNonScalar (unsigned int OP_DAT_ArgumentGroup);

    void
    setOpAccessValue (unsigned int OP_DAT_ArgumentGroup,
        ACCESS_CODE_VALUE value);

    bool
    isRead (unsigned int OP_DAT_ArgumentGroup);

    bool
    isWritten (unsigned int OP_DAT_ArgumentGroup);

    bool
    isReadAndWritten (unsigned int OP_DAT_ArgumentGroup);

    bool
    isIncremented (unsigned int OP_DAT_ArgumentGroup);

    bool
    isMaximised (unsigned int OP_DAT_ArgumentGroup);

    bool
    isMinimised (unsigned int OP_DAT_ArgumentGroup);

    /*
     * ======================================================
     * How many OP_DATs are indirectly accessed?
     * ======================================================
     */
    unsigned int
    getNumberOfIndirectOpDats ();

    /*
     * ======================================================
     * How many OP_DATs are indirectly accessed and are
     * unique in the actual arguments passed to the OP_PAR_LOOP?
     * ======================================================
     */
    unsigned int
    getNumberOfDifferentIndirectOpDats ();

    /*
     * ======================================================
     * Does the OP_DAT in this argument group require a reduction?
     * ======================================================
     */
    bool
    isReductionRequired (int OP_DAT_ArgumentGroup);

    /*
     * ======================================================
     * Does this parallel loop require a reduction?
     * ======================================================
     */
    bool
    isReductionRequired ();

    void
    setOpDatVariableName (unsigned int OP_DAT_ArgumentGroup,
        std::string const variableName);

    /*
     * ======================================================
     * What is the name of the OP_DAT variable in this OP_DAT
     * argument group
     * ======================================================
     */
    std::string
    getOpDatVariableName (unsigned int OP_DAT_ArgumentGroup);

    unsigned int
    getSizeOfOpDat (unsigned int OP_DAT_ArgumentGroup);

    /*
     * ======================================================
     * What is the maximum size of an individual element of an
     * OP_DAT in terms of bytes?
     * ======================================================
     */
    unsigned int
    getMaximumSizeOfOpDat ();

    bool
    isUniqueOpDat (std::string const & variableName);

    void
    setUniqueOpDat (std::string const & variableName);

    /*
     * ======================================================
     * Returns the actual arguments passed to the OP_PAR_LOOP
     * ======================================================
     */
    SgExpressionPtrList &
    getActualArguments ();

    /*
     * ======================================================
     * Returns the node in the AST modelling the call to
     * to this OP_PAR_LOOP
     * ======================================================
     */
    SgFunctionCallExp *
    getFunctionCall ();

    /*
     * ======================================================
     * Returns the name of the file in which the OP_PAR_LOOP
     * call is contained
     * ======================================================
     */
    std::string const &
    getFileName () const;

    Reduction *
    getReductionTuple (unsigned int OP_DAT_ArgumentGroup);

    void
    getReductionsNeeded (std::vector <Reduction *> & reductions);
};

#endif
