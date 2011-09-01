/*
 * Written by Adam Betts and Carlo Bertolli
 */

#ifndef CPP_OpenCL_MODULE_DECLARATIONS_H
#define CPP_OpenCL_MODULE_DECLARATIONS_H

#include <CPPModuleDeclarations.h>
#include <CPPOpenCLDataSizesDeclaration.h>
#include <CPPOpDatDimensionsDeclaration.h>

class CPPOpenCLModuleDeclarations: public CPPModuleDeclarations
{
  protected:

    CPPOpenCLDataSizesDeclaration * dataSizesDeclaration;

    CPPOpDatDimensionsDeclaration * dimensionsDeclaration;

  protected:

    void
    createReductionDeclarations ();

    void
    createDataSizesDeclaration ();

    void
    createDimensionsDeclaration ();

  public:

    SgVariableDeclaration *
    getReductionArrayHostVariableDeclaration ();

    SgVariableDeclaration *
    getReductionArrayDeviceVariableDeclaration ();

    SgVariableDeclaration *
    getDataSizesVariableDeclaration ();

    SgVariableDeclaration *
    getDimensionsVariableDeclaration ();

    CPPOpenCLModuleDeclarations (std::string const & userSubroutineName,
        CPPParallelLoop * parallelLoop, SgScopeStatement * moduleScope,
        CPPOpenCLDataSizesDeclaration * dataSizesDeclaration,
        CPPOpDatDimensionsDeclaration * dimensionsDeclaration);
};

#endif
