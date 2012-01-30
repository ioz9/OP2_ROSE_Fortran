#pragma once
#ifndef COMPILER_GENERATED_NAMES_H
#define COMPILER_GENERATED_NAMES_H

#include <string>

class SgType;
class SgStatement;
class SgExpression;
class SgScopeStatement;
class SgFunctionCallExp;

namespace ReductionVariableNames
{
  /*
   * ======================================================
   * Variable names used in reductions
   * ======================================================
   */

  std::string const reductionInput = "inputValue";
  std::string const reductionResult = "reductionResult";
  std::string const reductionOperation = "reductionOperation";
  std::string const reductionBytes = "reductionBytes";
  std::string const reductionSharedMemorySize = "reductionSharedMemorySize";

  std::string const maxBlocksPerGrid = "maxBlocksPerGrid";
  
  std::string const
  getReductionArrayHostName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getReductionArrayDeviceName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getReductionCardinalityName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getTemporaryReductionArrayName (unsigned int OP_DAT_ArgumentGroup);
}

namespace LoopVariableNames
{
  /*
   * ======================================================
   * Get a for loop counter variable name with this integer
   * suffix
   * ======================================================
   */

  std::string const
  getIterationCounterVariableName (unsigned int n);

  /*
   * ======================================================
   * Get an upper bound variable name with this integer
   * suffix
   * ======================================================
   */

  std::string const
  getUpperBoundVariableName (unsigned int n);
}

namespace BooleanVariableNames
{
  /*
   * ======================================================
   * Variable name to determine whether this is the first
   * time to execute a kernel
   * ======================================================
   */

  std::string const
  getFirstTimeExecutionVariableName (std::string const & suffix);
}

namespace OP2VariableNames
{
  std::string const localOffset = "localOffset";
  std::string const moduloResult = "moduloResult";
  std::string const nbytes = "nbytes";
  std::string const numberOfActiveThreads = "numberOfActiveThreads";
  std::string const numberOfActiveThreadsCeiling =
      "numberOfActiveThreadsCeiling";
  std::string const opDatCardinalities = "opDatCardinalities";
  std::string const opDatDimensions = "opDatDimensions";
  std::string const partitionSize = "partitionSize";
  std::string const setSize = "setSize";
  std::string const sharedMemoryOffset = "sharedMemoryOffset";
  std::string const sharedMemoryBytes = "sharedMemoryBytes";
  std::string const threadID = "threadID";
  std::string const threadItems = "threadItems";
  std::string const warpSize = "warpSize";
  std::string const warpMemorySize = "warpMemorySize";

  std::string const
  getBlockSizeVariableName (std::string const & suffix);

  std::string const
  getPartitionSizeVariableName (std::string const & suffix);

  /*
   * ======================================================
   * Returns the name of the formal parameter with type
   * character array which models the name of the user
   * subroutine
   * ======================================================
   */
  std::string const
  getUserSubroutineName ();

  /*
   * ======================================================
   * Returns the name of the formal parameter with type
   * OP_SET
   * ======================================================
   */
  std::string const
  getOpSetName ();

  /*
   * ======================================================
   * Returns the name of the variable for the type OP_SET_CORE
   * in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpSetCoreName ();

  /*
   * ======================================================
   * Returns the name of the variable for the type OP_SET_CORE
   * in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpSetCoreName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of an OP_DAT variable in this OP_DAT
   * argument group
   * ======================================================
   */
  std::string const
  getOpDatName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of an OP_DAT variable in this OP_DAT
   * argument group on the host
   * ======================================================
   */
  std::string const
  getOpDatHostName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of a local OP_DAT variable in this
   * OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpDatLocalName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of a global OP_DAT variable in this
   * OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpDatGlobalName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the variable modelling the size of
   * an OP_DAT in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpDatCardinalityName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the OP_DAT device variable
   * in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpDatDeviceName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the OP_DAT core variable
   * in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpDatCoreName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the OP_DAT dimension variable
   * in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpDatDimensionName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the indirection variable in this
   * OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpIndirectionName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the mapping variable in this OP_DAT
   * argument group
   * ======================================================
   */
  std::string const
  getOpMapName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the variable for the type OP_MAP_CORE
   * in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getOpMapCoreName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the access variable in this OP_DAT
   * argument group
   * ======================================================
   */
  std::string const
  getOpAccessName (unsigned int OP_DAT_ArgumentGroup);

  /*
   * ======================================================
   * Returns the name of the C to Fortran variable
   * in this OP_DAT argument group
   * ======================================================
   */
  std::string const
  getCToFortranVariableName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getNumberOfBytesVariableName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getRoundUpVariableName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getIncrementAccessMapName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getIndirectOpDatSizeName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getIndirectOpDatMapName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getIndirectOpDatSharedMemoryName (unsigned int OP_DAT_ArgumentGroup);

  std::string const
  getSharedMemoryDeclarationName (SgType * type, unsigned int size);

  std::string const
  getSharedMemoryDeclarationName (std::string suffix);

  std::string const
  getVolatileSharedMemoryDeclarationName (SgType * type, unsigned int size);

  std::string const
  getSharedMemoryOffsetDeclarationName (SgType * type, unsigned int size);

  std::string const
  getSharedMemoryOffsetDeclarationName (std::string suffix);

  std::string const
  getSharedMemoryPointerDeclarationName (SgType * type, unsigned int size);

  std::string const
  getSharedMemoryPointerDeclarationName (std::string suffix);
}

#endif
