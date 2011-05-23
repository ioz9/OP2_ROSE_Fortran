/*
 * Written by Adam Betts and Carlo Bertolli
 *
 * This class models a variable declaration: it includes
 * fields for the sizes of data passed to the device.
 * These sizes are needed in Fortran to prevent segmentation
 * faults.
 *
 * These sizes must be passed in a struct-like data
 * structure as the number of kernel parameters can be become
 * unwieldly. In these cases, the code might not compile
 * with current-generation CUDA compilers due to the number of
 * parameters, so the solution is to instead pack them into
 * a struct
 */

#ifndef DATA_SIZES_DECLARATION_H
#define DATA_SIZES_DECLARATION_H

#include<ParallelLoop.h>

class DataSizesDeclaration
{
  protected:

    /*
     * ======================================================
     * The scope of the declaration
     * ======================================================
     */
    SgScopeStatement * moduleScope;

    /*
     * ======================================================
     * The type definition declaration statement
     * ======================================================
     */
    SgDerivedTypeStatement * deviceDatatypeStatement;

    /*
     * ======================================================
     * A mapping to the other fields inside the above type
     * definition declaration statement
     * ======================================================
     */
    std::map <std::string, SgVariableDeclaration *> fieldDeclarations;

  protected:

    virtual void
    addFields (ParallelLoop & parallelLoop) = 0;

    DataSizesDeclaration (std::string const & subroutineName,
        SgScopeStatement * moduleScope);

  public:

    /*
     * ======================================================
     * Name of the field which represents the size of an
     * OP_DAT argument
     * ======================================================
     */
    std::string
    get_OP_DAT_SizeFieldName (unsigned int OP_DAT_ArgumentGroup);

    /*
     * ======================================================
     * What is the type of the data sizes declaration?
     * ======================================================
     */
    SgClassType *
    getType ();

    /*
     * ======================================================
     * Get the declaration corresponding to this field name
     * ======================================================
     */
    SgVariableDeclaration *
    getFieldDeclaration (std::string fieldName);
};

#endif