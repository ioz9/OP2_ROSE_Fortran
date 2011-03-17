/*
 * Written by Adam Betts and Carlo Bertolli
 *  
 * A namespace for the various names that OP2 employs
 */

#ifndef OP2_COMMON_DEFINITIONS_H
#define OP2_COMMON_DEFINITIONS_H

namespace OP2
{
  /*
   * Declaration of names specific to OP2
   */
  std::string const OP_SET_NAME = "op_set";
  std::string const OP_DAT_NAME = "op_dat";
  std::string const OP_MAP_NAME = "op_map";
  std::string const OP_ID_NAME = "op_id";
  std::string const OP_GBL_NAME = "op_gbl";
  std::string const OP_READ_NAME = "op_read";
  std::string const OP_WRITE_NAME = "op_write";
  std::string const OP_RW_NAME = "op_rw";
  std::string const OP_INC_NAME = "op_inc";
  std::string const OP_MAX_NAME = "op_max";
  std::string const OP_MIN_NAME = "op_min";
  std::string const OP_PAR_LOOP_PREFIX = "op_par_loop";
  std::string const OP_DECL_SET_PREFIX = "op_decl_set";
  std::string const OP_DECL_MAP_PREFIX = "op_decl_map";
  std::string const OP_DECL_DAT_PREFIX = "op_decl_dat";
  std::string const OP_DECL_GBL_PREFIX = "op_decl_gbl";

  std::string const OP_INDIRECTION_VariableNamePrefix = "opIndirection";
  std::string const OP_ACCESS_VariableNamePrefix = "opAccess";
  std::string const OP_DAT_VariableNamePrefix = "opDat";
  std::string const OP_MAP_VariableNamePrefix = "opMap";

  /*
   * Positions of arguments inside OP_PAR_LOOP
   * The following style is assumed:
   * OP_PAR_LOOP_<N> (kernelName, OP_SET,
   *                  OP_DAT,     int,     OP_MAP,     OP_ACCESS,
   *                  ...
   *                  OP_DAT_N-1, int_N-1, OP_MAP_N-1, OP_ACCESS_N-1)
   */
  unsigned int const NUMBER_OF_ARGUMENTS_PER_OP_DAT = 4; // For every OP_DAT, there is an int, OP_MAP, and an OP_ACCESS
  unsigned int const NUMBER_OF_NON_OP_DAT_ARGUMENTS = 2; // 2 because of 'kernelName' and 'OP_SET' arguments
}

#endif