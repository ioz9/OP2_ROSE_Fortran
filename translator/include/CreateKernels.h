/*
 * Written by Adam Betts and Carlo Bertolli
 *
 * The parallel loop class used to recognise and manipulate 'op_par_loop' calls
 * in Fortran code.
 *
 * This class extends AstSimpleProcessing since that class provides
 * utilities to traverse the Abstract Syntax Tree (AST).
 */

// TODO: restructure in such a way that we have a single OpParLoop object per each op_par_loop call 
// found in the code (or an additional data structure per each call)

#ifndef CREATEKERNELS_H
#define CREATEKERNELS_H

#include <rose.h>

#include "OP2CommonDefinitions.h"
#include "OpDeclaredVariables.h"

using namespace std;

class CreateKernels: public AstSimpleProcessing
{
  private:
    /*
     * ====================================================================================================
     * Private data
     * ====================================================================================================
     */

    /*
     * The project which the source-to-source translator operates upon
     */
    SgProject * project;

    /*
     * The previously filled OpDeclaredVariables object, used to retrieve op_dat types
     * (and all other declarations, if needed)
     */
    OpDeclaredVariables * opDeclaredVariables;

    /*
     * The name of the kernel function
     */
    string kernelName;

    /*
     * The kernel output files
     */
    vector <SgSourceFile *> kernelOutputFiles;

    /*
     * Each 'op_par_loop' in Fortran takes 2+4n arguments.
     * We need the number n to declare a certain number of variables
     * in the host function
     */
    int numberOfArgumentGroups;

    /*
     * References to all subroutines found during the visit of the input files
     */
    vector <SgProcedureHeaderStatement *> inputSubroutines;

    /*
     * set of declarations of local variables in the host routine (CRet, data0Size, ..., dataN-1Size)
     */
    vector <SgVariableDeclaration *> declaredHostRoutineLocals;

    /*
     * set of declarations of variables used in the host routine to transform a C pointer into a Fortran pointer
     * (argument0, ..., argumentN-1, c2fPtr0, ..., c2fPtrN-1)
     */
    vector <SgVariableDeclaration *> declaredC2FortranVariables;

    /*
     * set of declarations of CUDA configuration variables
     * (bsize, gsize, etc.. )
     */
    vector <SgVariableDeclaration *> declaredCUDAConfigurationParameters;

    /*
     * set of declarations of op_dat formal parameters
     * (arg0, ..., argN-1)
     */
    vector <SgVariableDeclaration *> opDatFormalArgs;

    /*
     * data types of the op_par_loop actual arguments ordered w.r.t. the input ordering
     */
    vector <SgType *> actualArgumentsTypes;

    /*
     * dimension values passed to op_decl_dat calls ordered w.r.t. the input ordering (the same of actualArgumentTypes)
     */
    vector <int> actualArgumentsDimValues;

    /*
     * set of declarations of indirection index formal parameters
     * (idx0, ..., idxN-1)
     */
    vector <SgVariableDeclaration *> indirectionFormalArgs;

    /*
     * set of declarations of op_map formal parameters
     * (map0, ..., mapN-1)
     */
    vector <SgVariableDeclaration *> mapFormalArgs;

    /*
     * set of declarations of access formal parameters
     * (access0, ..., accessN-1)
     */
    vector <SgVariableDeclaration *> accessFormalArgs;

    /*
     * declaration of formal argument corresponding to op_set iteration variable
     */
    SgVariableDeclaration * iterationSetFormalArg;

    /*
     * ====================================================================================================
     * Private functions
     * ====================================================================================================
     */

    bool
    checkAlreadyDeclared (SgVariableDeclaration * possiblyAvoidCopy,
        SgFunctionParameterList * newParList);

    void
    createAndAppendUserKernelFormalParameters (
        SgFunctionParameterList * newParList,
        SgFunctionParameterList * originalParams,
        SgScopeStatement * subroutineScope);

    /*
     * This function is called before the creation of the CUDA module to retrieve the op_par_loop arguments data types
     * It fills up the actualArgumentsTypes vector by comparing op_par_loop arguments with opDeclaredVariables variables
     */
    void
    retrieveArgumentsTypes (SgExpressionPtrList& args);

    SgModuleStatement *
    buildClassDeclarationAndDefinition (string name, SgScopeStatement* scope);

    SgContainsStatement *
    buildContainsStatement (Sg_File_Info * fileInfo, SgScopeStatement * scope);

    void
    addImplicitStatement (SgSourceFile& sourceFile,
        SgDeclarationStatement* definingDeclaration, SgScopeStatement* scope);

    void
    generateKernelSubroutine (SgSourceFile &sourceFile, string subroutineName,
        SgExpressionPtrList& args);

    void
    declareCUDAConfigurationParameters (SgScopeStatement * scope);

    void
    declareC2FortranVariables (SgScopeStatement * scope);

    void
    createHostDeviceLocals (SgScopeStatement* scope, SgExpressionPtrList& args);

    void
    createMainRoutineStatements (SgScopeStatement * scope,
        SgExpressionPtrList& args);

    /*
     * Creates an integer declaration, representing the indirection of the data
     * set to be iterated, and attaches it to the formal parameter list
     */
    void
    createIndirectionDeclaration (SgFunctionParameterList* parameters,
        SgScopeStatement* scope);

    /*
     * Creates an OP_ACCESS declaration and attaches it to the
     * formal parameter list
     */
    void
    createOpAccessDeclaration (SgFunctionParameterList* parameters,
        SgScopeStatement* scope);

    /*
     * Creates an OP_DAT declaration and attaches it to the
     * formal parameter list
     */
    void
    createOpDatDeclaration (SgFunctionParameterList* parameters,
        SgScopeStatement* scope, SgType* opDatType);

    /*
     * Creates an OP_Map declaration and attaches it to the
     * formal parameter list
     */
    void
    createOpMapDeclaration (SgFunctionParameterList* parameters,
        SgScopeStatement* scope, SgType* opMapType);

    /*
     * Creates an OP_SET declaration and attaches it to the
     * formal parameter list
     */
    void
    createOpSetDeclaration (SgFunctionParameterList* parameters,
        SgScopeStatement* scope, SgType* opSetType);

    /*
     * Creates a char array declaration, which is the kernel name,
     * and attaches it to the formal parameter list
     */
    void
    createSubroutineName (SgFunctionParameterList* parameters,
        SgScopeStatement* scope);

    /*
     * Creates a host subroutine for the kernel
     */
    SgProcedureHeaderStatement *
    createHostSubroutine (string kernelName, SgExpressionPtrList& args,
        SgSourceFile& sourceFile, SgScopeStatement * scope);

    /*
     * Copies the user kernel function definition, makes some modifications and append it to the scope
     */
    SgProcedureHeaderStatement *
    generateCUDAUserKernel (SgScopeStatement * scope);

    /*
     * The following two functions are used to generate the main CUDA kernel routine: not implemented for now
     */
    void
    lookupArgumentsTypes (vector <SgType *> * argTypes,
        SgExpressionPtrList& args, int loopDatArgNumber,
        SgVariableDeclaration * setSizeFormalPar);

    /*
     * Builds the parameter of the user kernel subroutine called by the main kernel subroutine
     */
    SgExprListExp *
    buildUserKernelParams (SgFunctionParameterList * mainKernelParameters,
        SgVarRefExp * iterSetVarRef, SgScopeStatement * subroutineScope);

    void
    buildMainKernelRoutine (SgScopeStatement * scope,
        SgExpressionPtrList& args, SgName deviceKernelName,
        Sg_File_Info & fileInfo);

    /*
     * Creates the Fortran module including all subroutines implementing an OP_PAR_LOOP
     */
    SgScopeStatement *
    createDirectLoopCUDAModule (SgSourceFile& sourceFile);

    /*
     * Transforms a op_par_loop_* in call to proper stub function, like op_par_loop_save_soln
     */
    void
    fixParLoops (SgFunctionCallExp * functionCallExp, string kernelName,
        SgProcedureHeaderStatement * hostSubroutine, SgScopeStatement * scope,
        string createdCUDAModuleName);
    /*
     * Creates the source file to be unparsed
     */
    SgSourceFile*
    createSourceFile (string kernelName);

  public:
    /*
     * ====================================================================================================
     * Public functions
     * ====================================================================================================
     */

    /*
     * Constructor requires the source files contained in the
     * project to detect 'op_par_loops'
     */
    CreateKernels (SgProject * project,
        OpDeclaredVariables * opDeclaredVariables)
    {
      this->project = project;
      this->opDeclaredVariables = opDeclaredVariables;
    }

    /*
     * Visit each vertex in the AST
     */
    virtual void
    visit (SgNode *n);

    /*
     * Generate output files for each kernel
     */
    void
    unparse ();
};

class deepVisitAndSetGeneration: public AstSimpleProcessing
{
  public:

    deepVisitAndSetGeneration ()
    {
    }

    /*
     * Special visit: for each node, sets its OutputInCodeGeneration flag to enable unparsing
     */
    virtual void
    visit (SgNode *n);

};

#endif
