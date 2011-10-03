# 1 "/home/abetts/SILOET/OP2_ROSE_Fortran/examples/Fortran/tests/airfoilHeteroDatsToDirectLoop/OP2-RRJ9A6.F90"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/abetts/SILOET/OP2_ROSE_Fortran/examples/Fortran/tests/airfoilHeteroDatsToDirectLoop/OP2-RRJ9A6.F90"
module OP2_C

use, intrinsic :: ISO_C_BINDING

  integer, parameter :: MAX_NAME_LEN = 100

  integer(4) :: OP_READ  = 1
  integer(4) :: OP_WRITE = 2
  integer(4) :: OP_INC   = 3
  integer(4) :: OP_RW    = 4
  integer(4) :: OP_MAX   = 5
  integer(4) :: OP_MIN   = 6

  type op_set
    integer:: id
  end type op_set

  type op_map
    integer:: id
  end type op_map

  type op_dat
    integer:: id
  end type op_dat

  type(op_map) :: OP_ID, OP_GBL

  interface op_decl_dat
    module procedure op_decl_dat_real_8, op_decl_dat_integer_4
  end interface op_decl_dat
 
  interface op_decl_gbl
     module procedure op_decl_gbl_real_8, &
     & op_decl_gbl_integer_4, &
     & op_decl_gbl_scalar_integer_4, &
     & op_decl_gbl_scalar_real_8
  end interface op_decl_gbl     

 interface op_decl_const
     module procedure op_decl_const_scalar_logical, &
                    & op_decl_const_scalar_real_4, &
                    & op_decl_const_scalar_real_8, &
                    op_decl_const_scalar_integer_4, &
                    op_decl_const_logical, &
                    & op_decl_const_real_4, &
                    & op_decl_const_real_8, &
                    op_decl_const_integer_4
  end interface op_decl_const

contains

  subroutine op_init ()
  end subroutine op_init

  ! OP_SET declaration

  subroutine op_decl_set ( size, set, setname )
    integer :: size
    type(op_set) :: set
    character(kind=c_char,len=*) :: setname
  end subroutine op_decl_set

  ! OP_MAP declaration

  subroutine op_decl_map ( from , to, dim, dat, outmap, mapname )
    type(op_set) :: from, to
    integer :: dim
    integer(4), dimension(:) :: dat
    type(op_map) :: outmap
    character(kind=c_char,len=*) :: mapname
  end subroutine op_decl_map

  ! OP_DAT declarations

  subroutine op_decl_dat_real_8(set, dim, dat, data, datname)
    type(op_set) :: set
    integer :: dim
    real(8), dimension(:) :: dat
    type(op_dat) :: data
    character(kind=c_char,len=*) :: datname
  end subroutine op_decl_dat_real_8
  
  subroutine op_decl_dat_integer_4(set, dim, dat, data, datname)
    type(op_set) :: set
    integer :: dim
    integer(4), dimension(:) :: dat
    type(op_dat) :: data  
    character(kind=c_char,len=*) :: datname
  end subroutine op_decl_dat_integer_4

  ! OP_GBL declarations
 
  subroutine op_decl_gbl_real_8 ( dat, dim, data)
    real(8), dimension(:) :: dat
    integer :: dim
    type(op_dat) :: data    
  end subroutine op_decl_gbl_real_8

  subroutine op_decl_gbl_integer_4 ( dat, dim, data)
    integer(4), dimension(:) :: dat
    integer :: dim
    type(op_dat) :: data    
  end subroutine op_decl_gbl_integer_4

  subroutine op_decl_gbl_scalar_integer_4 ( dat, data)
    integer(4) :: dat
    type(op_dat) :: data    
  end subroutine op_decl_gbl_scalar_integer_4
 
  subroutine op_decl_gbl_scalar_real_8 ( dat, data)
    real(8) :: dat
    type(op_dat) :: data    
  end subroutine op_decl_gbl_scalar_real_8

  ! OP_CONST declarations

  subroutine op_decl_const_scalar_logical ( dim, data )
    integer(4) :: dim
    logical :: data
  end subroutine op_decl_const_scalar_logical

  subroutine op_decl_const_scalar_real_4 ( dim, data )
    integer(4) :: dim
    real(4) :: data
  end subroutine op_decl_const_scalar_real_4

  subroutine op_decl_const_scalar_real_8 ( dim, data )
    integer(4) :: dim
    real(8) :: data
  end subroutine op_decl_const_scalar_real_8

  subroutine op_decl_const_scalar_integer_4 ( dim, data )
    integer(4) :: dim
    integer(4) :: data
  end subroutine op_decl_const_scalar_integer_4

  subroutine op_decl_const_logical ( dim, data )
    integer(4) :: dim
    logical, dimension(:) :: data
  end subroutine op_decl_const_logical

  subroutine op_decl_const_real_4 ( dim, data )
    integer(4) :: dim
    real(4), dimension(:) :: data
  end subroutine op_decl_const_real_4

  subroutine op_decl_const_real_8 ( dim, data )
    integer(4) :: dim
    real(8), dimension(:) :: data
  end subroutine op_decl_const_real_8

  subroutine op_decl_const_integer_4 ( dim, data )
    integer(4) :: dim
    integer(4), dimension(:) :: data  
  end subroutine op_decl_const_integer_4

  ! OP_PAR_LOOPs

  subroutine op_par_loop_1 ( subroutineName, set, &
                           & data0, itemSel0, map0, access0 &
                         & )
    external subroutineName
    type(op_set) :: set
    type(op_dat) :: data0
    integer:: itemSel0, access0
    type(op_map) :: map0
  end subroutine op_par_loop_1 
  
  subroutine op_par_loop_2 ( subroutineName, set, &
                           & data0, itemSel0, map0, access0, &
                           & data1, itemSel1, map1, access1 &
                         & )
    external subroutineName
    type(op_set) :: set
    type(op_dat) :: data0, data1
    integer:: itemSel0, itemSel1, access0, access1
    type(op_map) :: map0, map1
  end subroutine op_par_loop_2

  subroutine op_par_loop_3 ( subroutineName, set, &
                           & data0, itemSel0, map0, access0, &
                           & data1, itemSel1, map1, access1, &
                           & data2, itemSel2, map2, access2 &
                         & )
    external subroutineName
    type(op_set) :: set
    type(op_dat) :: data0, data1, data2
    integer:: itemSel0, itemSel1, itemSel2, access0, access1, access2
    type(op_map) :: map0, map1, map2
  end subroutine op_par_loop_3

  subroutine op_par_loop_4 ( subroutineName, set, &
                           & data0, itemSel0, map0, access0, &
                           & data1, itemSel1, map1, access1, &
                           & data2, itemSel2, map2, access2, &
                           & data3, itemSel3, map3, access3 &
                         & )
    external subroutineName
    type(op_set) :: set
    type(op_dat) :: data0, data1, data2, data3
    integer:: itemSel0, itemSel1, itemSel2, itemSel3, access0, access1, access2, access3
    type(op_map) :: map0, map1, map2, map3
  end subroutine op_par_loop_4  

  subroutine op_par_loop_5 ( subroutineName, set, &
                             & data0, itemSel0, map0, access0, &
                             & data1, itemSel1, map1, access1, &
                             & data2, itemSel2, map2, access2, &
                             & data3, itemSel3, map3, access3, &
                             & data4, itemSel4, map4, access4  &
                           & )
    external subroutineName
    type(op_set) :: set
    type(op_dat) :: data0, data1, data2, data3, data4
    integer :: itemSel0, itemSel1, itemSel2, itemSel3, itemSel4
    integer :: access0, access1, access2, access3, access4
    type(op_map) :: map0, map1, map2, map3, map4
  end subroutine op_par_loop_5

  subroutine op_par_loop_6 ( subroutineName, set, &
                             & data0, itemSel0, map0, access0, &
                             & data1, itemSel1, map1, access1, &
                             & data2, itemSel2, map2, access2, &
                             & data3, itemSel3, map3, access3, &
                             & data4, itemSel4, map4, access4, &
                             & data5, itemSel5, map5, access5  &
                           & )
    external subroutineName
    type(op_set) :: set
    type(op_dat) :: data0, data1, data2, data3, data4, data5
    integer :: itemSel0, itemSel1, itemSel2, itemSel3, itemSel4, itemSel5 
    integer :: access0, access1, access2, access3, access4, access5
    type(op_map) :: map0, map1, map2, map3, map4, map5
  end subroutine op_par_loop_6
  
  subroutine op_par_loop_8 ( subroutineName, set, &
                             & data0, itemSel0, map0, access0, &
                             & data1, itemSel1, map1, access1, &
                             & data2, itemSel2, map2, access2, &
                             & data3, itemSel3, map3, access3, &
                             & data4, itemSel4, map4, access4, &
                             & data5, itemSel5, map5, access5, &
                             & data6, itemSel6, map6, access6, &
                             & data7, itemSel7, map7, access7  &
                           & )
    external subroutineName
    type(op_set) :: set
    type(op_dat) :: data0, data1, data2, data3, data4, data5, data6, data7
    integer :: itemSel0, itemSel1, itemSel2, itemSel3, itemSel4, itemSel5, itemSel6, itemSel7
    integer :: access0, access1, access2, access3, access4, access5, access6, access7
    type(op_map) :: map0, map1, map2, map3, map4, map5, map6, map7            
  end subroutine op_par_loop_8

end module OP2_C
