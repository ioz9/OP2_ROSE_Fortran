


program airfoil

	use, intrinsic :: ISO_C_BINDING


	use OP2_C
	use constantVars
	use airfoil_seq

	use save_soln_cuda_module
	use adt_calc_cuda_module
	use res_calc_cuda_module
	use bres_calc_cuda_module
	use update_cuda_module
	use constantVarsCuda

  implicit none

  intrinsic :: sqrt, real

  integer(4) :: iter, k, i

	! debug variables
	integer(c_int) :: retDebug, dataSize
	character(kind=c_char) :: debfilename(20)
	real(c_double) :: datad
	integer(4) :: debugiter

  integer(4), parameter :: maxnode = 9900
  integer(4), parameter :: maxcell = (9702+1)
  integer(4), parameter :: maxedge = 19502

	integer(4), parameter :: iterationNumber = 1000


  integer(4) :: nnode, ncell, nbedge, nedge, niter
  real(8) :: ncellr

	! debug
	integer(4) :: CRet

	! profiling
	integer :: istat
  type (cudaEvent) :: startEvent, stopEvent, iterationStartTime, iterationEndTime
	real(4) :: totalExecutiontime, iterationTime, iterationPartialTime, iterationFullTimes(iterationNumber)
	real(4) :: save_soln_kernel_time, adt_calc_kernel_time, res_calc_kernel_time, bres_calc_kernel_time, update_kernel_time
	real(4) :: save_soln_host_time, adt_calc_host_time, res_calc_host_time, bres_calc_host_time, update_host_time
	integer(4) :: save_soln_count, adt_calc_count, res_calc_count, bres_calc_count, update_count
	type(profInfo) :: save_soln_info, adt_calc_info, res_calc_info, bres_calc_info, update_info


	type(c_funptr) :: save_soln_cptr

  ! integer references (valid inside the OP2 library) for op_set
  type(op_set) :: nodes, edges, bedges, cells

  ! integer references (valid inside the OP2 library) for pointers between data sets
  type(op_map) :: pedge, pecell, pcell, pbedge, pbecell

  ! integer reference (valid inside the OP2 library) for op_data 
  type(op_dat) :: p_bound, p_x, p_q, p_qold, p_adt, p_res, p_rms
  
  ! arrays used in data
  integer(4), dimension(:), allocatable, target :: ecell, bound, edge, bedge, becell, cell
  real(8), dimension(:), allocatable, target :: x, q, qold, adt, res, rms

	! strings with kernel, set, map and dat names, interoperable with C
	character(kind=c_char,len=10) :: savesolnName = C_CHAR_'save_soln'//C_NULL_CHAR
	character(kind=c_char, len=9) :: adtcalcName = C_CHAR_'adt_calc' // C_NULL_CHAR
	character(kind=c_char, len=9) :: rescalcName = C_CHAR_'res_calc' // C_NULL_CHAR
	character(kind=c_char,len=10) :: brescalcName = C_CHAR_'bres_calc' // C_NULL_CHAR
	character(kind=c_char, len=7) :: updateName = C_CHAR_'update' // C_NULL_CHAR

	character(kind=c_char,len=6) :: nodesName =	C_CHAR_'nodes'//C_NULL_CHAR
	character(kind=c_char,len=6) :: edgesName =	C_CHAR_'edges'//C_NULL_CHAR
	character(kind=c_char,len=7) :: bedgesName = C_CHAR_'bedges'//C_NULL_CHAR
	character(kind=c_char,len=6) :: cellsName =	C_CHAR_'cells'//C_NULL_CHAR

	character(kind=c_char,len=6) :: pedgeName =	C_CHAR_'pedge'//C_NULL_CHAR
	character(kind=c_char,len=7) :: pecellName = C_CHAR_'pecell'//C_NULL_CHAR
	character(kind=c_char,len=6) :: pcellName =	C_CHAR_'pcell'//C_NULL_CHAR
	character(kind=c_char,len=7) :: pbedgeName =	C_CHAR_'pbedge'//C_NULL_CHAR
	character(kind=c_char,len=8) :: pbecellName =	C_CHAR_'pbecell'//C_NULL_CHAR

	character(kind=c_char,len=6) :: boundName =	C_CHAR_'bound'//C_NULL_CHAR
	character(kind=c_char,len=2) :: xName =	C_CHAR_'x'//C_NULL_CHAR
	character(kind=c_char,len=2) :: qName =	C_CHAR_'q'//C_NULL_CHAR
	character(kind=c_char,len=5) :: qoldName =	C_CHAR_'qold'//C_NULL_CHAR
	character(kind=c_char,len=4) :: adtName =	C_CHAR_'adt'//C_NULL_CHAR
	character(kind=c_char,len=4) :: resName =	C_CHAR_'res'//C_NULL_CHAR

	! read set sizes from input file (input is subdivided in two routines as we cannot allocate arrays in subroutines in
	! fortran 90)
	call getSetSizes ( nnode, ncell, nedge, nbedge )
	
	! allocate sets (cannot allocate in subroutine in F90)
	allocate ( cell ( 4 * ncell ) )
	allocate ( edge ( 2 * nedge ) )
	allocate ( ecell ( 2 * nedge ) )
	allocate ( bedge ( 2 * nbedge ) )
	allocate ( becell ( nbedge ) )
	allocate ( bound ( nbedge ) )
		
	allocate ( x ( 2 * nnode ) )

	allocate ( q ( 4 * ncell ) )
	allocate ( qold ( 4 * ncell ) )
	allocate ( res ( 4 * ncell ) )
	allocate ( adt ( ncell ) )

	allocate ( rms ( 1 ) )

	! fill up arrays from file
	call getSetInfo ( nnode, ncell, nedge, nbedge, cell, edge, ecell, bedge, becell, bound, x, q, qold, res, adt )

  ! set constants and initialise flow field and residual
	call initialise_flow_field ( ncell, q, res )
	
  do iter = 1, 4*ncell
    res(iter) = 0.0
  end do 

  ! OP initialisation
	call op_init()

  ! declare sets, pointers, datasets and global constants (for now, no new partition info)
  call op_decl_set ( nnode, nodes, nodesName )
  call op_decl_set ( nedge, edges, edgesName )
	call op_decl_set ( nbedge, bedges, bedgesName )
  call op_decl_set ( ncell, cells, cellsName )

  call op_decl_map ( edges, nodes, 2, edge, pedge, pedgeName ) 
  call op_decl_map ( edges, cells, 2, ecell, pecell, pecellName )
	call op_decl_map ( bedges, nodes, 2, bedge, pbedge, pbedgeName )
	call op_decl_map ( bedges, cells, 1, becell, pbecell, pecellName )
  call op_decl_map ( cells, nodes, 4, cell, pcell, pcellName )

  call op_decl_dat ( bedges, 1, bound, p_bound, boundName )	
  call op_decl_dat ( nodes, 2, x, p_x, xName )
  call op_decl_dat ( cells, 4, q, p_q, qName )
	call op_decl_dat ( cells, 4, qold, p_qold, qoldName )
  call op_decl_dat ( cells, 1, adt, p_adt, adtName )
  call op_decl_dat ( cells, 4, res, p_res, resName )

	call op_decl_gbl ( rms, 1, p_rms )
		
	
	! profiling
  istat = cudaEventCreate(startEvent)
  istat = cudaEventCreate(stopEvent)
	
  istat = cudaEventRecord ( startEvent, 0 )
	
	save_soln_kernel_time = 0
	adt_calc_kernel_time = 0
	res_calc_kernel_time = 0
	bres_calc_kernel_time = 0
	update_kernel_time = 0

	save_soln_host_time = 0
	adt_calc_host_time = 0
	res_calc_host_time = 0
	bres_calc_host_time = 0
	update_host_time = 0

	save_soln_count = 0
	adt_calc_count = 0
	res_calc_count = 0
	bres_calc_count = 0
	update_count = 0

	iterationTime = 0
	
	! main time-marching loop
	
  do niter = 1, iterationNumber
	
		istat = cudaEventCreate ( iterationStartTime )
		istat = cudaEventCreate ( iterationEndTime )
		
		istat = cudaEventRecord ( iterationStartTime, 0 )

		! save old flow solution
		save_soln_info = op_par_loop_save_soln ( savesolnName, cells, &
																					 & p_q,    -1, OP_ID, OP_READ, &
																					 & p_qold, -1, OP_ID, OP_WRITE &
																				 & )

		save_soln_kernel_time = save_soln_kernel_time + save_soln_info%kernelTime
		save_soln_host_time = save_soln_host_time + save_soln_info%hostTime
		save_soln_count = save_soln_count + 1
				
		! predictor/corrector update loop
				
    do k = 1, 2

      ! calculate area/timstep

      adt_calc_info = op_par_loop_adt_calc ( adtcalcName, cells, &
																					 & p_x,    1, pcell, OP_READ, &
																					 & p_x,    2, pcell, OP_READ, &
																					 & p_x,    3, pcell, OP_READ, &
																					 & p_x,    4, pcell, OP_READ, &
																					 & p_q,   -1, OP_ID, OP_READ, &
																					 & p_adt, -1, OP_ID, OP_WRITE &
																				 & )

			adt_calc_kernel_time = adt_calc_kernel_time + adt_calc_info%kernelTime
			adt_calc_host_time = adt_calc_host_time + adt_calc_info%hostTime
		
			adt_calc_count = adt_calc_count + 1

!      call op_par_loop_6 ( adt_calc, cells, &
!											   & p_x,   1, pcell, OP_READ, &
!												 & p_x,   2, pcell, OP_READ, &
!												 & p_x,   3, pcell, OP_READ, &
!												 & p_x,   4, pcell, OP_READ, &
!												 & p_q,   -1, OP_ID, OP_READ, &
!												 & p_adt, -1, OP_ID, OP_WRITE &
!											 & )

      ! calculate flux residual

      res_calc_info = op_par_loop_res_calc ( rescalcName, edges, &
																					 & p_x,    1, pedge,  OP_READ, &
																					 & p_x,    2, pedge,  OP_READ, &
																					 & p_q,    1, pecell, OP_READ, &
																					 & p_q,    2, pecell, OP_READ, &
																					 & p_adt,  1, pecell, OP_READ, &
																					 & p_adt,  2, pecell, OP_READ, &
																					 & p_res,  1, pecell, OP_INC,  &
																					 & p_res,  2, pecell, OP_INC   &
																				 & )

			res_calc_kernel_time = res_calc_kernel_time + res_calc_info%kernelTime
			res_calc_host_time = res_calc_host_time + res_calc_info%hostTime

			res_calc_count = res_calc_count + 1


!      call op_par_loop_8 ( res_calc, edges, &
!                         & p_x,    1, pedge,  OP_READ, &
!                         & p_x,    2, pedge,  OP_READ, &
!                         & p_q,    1, pecell, OP_READ, &
!                         & p_q,    2, pecell, OP_READ, &
!                         & p_adt,  1, pecell, OP_READ, &
!                         & p_adt,  2, pecell, OP_READ, &
!                         & p_res,  1, pecell, OP_INC,  &
!                         & p_res,  2, pecell, OP_INC   &
!											 & )


      bres_calc_info = op_par_loop_bres_calc ( brescalcName, bedges, &
																						 & p_x,      1, pbedge,  OP_READ, &
																						 & p_x,      2, pbedge,  OP_READ, &
																						 & p_q,      1, pbecell, OP_READ, &
																						 & p_adt,    1, pbecell, OP_READ, &
																						 & p_res,    1, pbecell, OP_INC,  &
																						 & p_bound,  -1, OP_ID, OP_READ  &
																					 & )

			bres_calc_kernel_time = bres_calc_kernel_time + bres_calc_info%kernelTime
			bres_calc_host_time = bres_calc_host_time + bres_calc_info%hostTime

			bres_calc_count = bres_calc_count + 1

!      call op_par_loop_6 ( bres_calc, bedges, &
!												 & p_x,      1, pbedge,  OP_READ, &
!												 & p_x,      2, pbedge,  OP_READ, &
!												 & p_q,      1, pbecell, OP_READ, &
!												 & p_adt,    1, pbecell, OP_READ, &
!											   & p_res,    1, pbecell, OP_INC,  &
!                         & p_bound,  -1, OP_ID, OP_READ  &
!											 & )

      ! update flow field
      
      rms(1) = 0.0


      update_info = op_par_loop_update ( updateName, cells, &
																			 & p_qold, -1, OP_ID,  OP_READ,  &
																			 & p_q,    -1, OP_ID,  OP_WRITE, &
																			 & p_res,  -1, OP_ID,  OP_RW,    &
																			 & p_adt,  -1, OP_ID,  OP_READ,  &
																			 & p_rms,  -1, OP_GBL, OP_INC    &
																		 & )

			update_kernel_time = update_kernel_time + update_info%kernelTime
			update_host_time = update_host_time + update_info%hostTime

			update_count = update_count + 1

!      call op_par_loop_5 ( update, cells, &
!                         & p_qold, -1, OP_ID,  OP_READ,  &
!                         & p_q,    -1, OP_ID,  OP_WRITE, &
!                         & p_res,  -1, OP_ID,  OP_RW,    &
!                         & p_adt,  -1, OP_ID,  OP_READ,  &
!                         & p_rms,  -1, OP_GBL, OP_INC    &
!                       & )

    end do ! internal loop

    ncellr = real ( ncell )
    rms(1) = sqrt ( rms(1) / ncellr )

		istat = cudaEventRecord ( iterationEndTime, 0 )

		istat = cudaEventElapsedTime ( iterationPartialTime, iterationStartTime, iterationEndTime )
		
		iterationFullTimes(niter) = iterationPartialTime

		! discard first time in the average
		if ( niter .gt. 1 ) then
		
			iterationTime = iterationTime + iterationPartialTime

		end if
		
		
  end do ! external loop


	istat = cudaEventRecord ( stopEvent, 0 )
	istat = cudaEventSynchronize ( stopEvent )
	istat = cudaEventElapsedTime ( totalExecutiontime, startEvent, stopEvent )

	write(*,*) 'Time total execution (ms): ', totalExecutiontime
	write(*,*) 'Average time per iteration (ms): ', iterationTime / iterationNumber


	write(*,*) 'Number of cells = ', ncell
	write(*,*) 'Number of edges = ', nedge
	write(*,*) 'Number of vertices = ', nnode


	write(*,*) 'total kernel save soln time (ms) = ', save_soln_kernel_time
	write(*,*) 'total kernel adt calc time (ms) = ', adt_calc_kernel_time
	write(*,*) 'total kernel res_calc time (ms) = ', res_calc_kernel_time
	write(*,*) 'total kernel bres_calc time (ms) = ', bres_calc_kernel_time
	write(*,*) 'total kernel update time (ms) = ', update_kernel_time

	write(*,*) 'total host save soln time (ms) = ', save_soln_host_time
	write(*,*) 'total host adt calc time (ms) = ', adt_calc_host_time
	write(*,*) 'total host res_calc time (ms) = ', res_calc_host_time
	write(*,*) 'total host bres_calc time (ms) = ', bres_calc_host_time
	write(*,*) 'total host update time (ms) = ', update_host_time


	write(*,*) 'Average time for kernel save soln (ms): ', (save_soln_kernel_time / real(save_soln_count))
	write(*,*) 'Average time for kernel adt calc (ms): ', (adt_calc_kernel_time / real(adt_calc_count))
	write(*,*) 'Average time for kernel res calc (ms): ', (res_calc_kernel_time / real(res_calc_count))
	write(*,*) 'Average time for kernel bres calc (ms): ', (bres_calc_kernel_time / real(bres_calc_count))
	write(*,*) 'Average time for kernel update (ms): ', (update_kernel_time / real(update_count))

	write(*,*) 'Average time for host save soln (ms): ', (save_soln_host_time / real(save_soln_count))
	write(*,*) 'Average time for host adt calc (ms): ', (adt_calc_host_time / real(adt_calc_count))
	write(*,*) 'Average time for host res calc (ms): ', (res_calc_host_time / real(res_calc_count))
	write(*,*) 'Average time for host bres calc (ms): ', (bres_calc_host_time / real(bres_calc_count))
	write(*,*) 'Average time for host update (ms): ', (update_host_time / real(update_count))



	! uncomment the following statements to write to file the single iteration times

!	retDebug = openfile ( C_CHAR_"/work/cbertoll/airfoil-cuda-final/ittimes.txt"//C_NULL_CHAR )
!
!	do debugiter = 1, iterationNumber
!
!		datad = iterationFullTimes(debugiter)
!		retDebug = writeRealToFile ( datad )
!	end do
!
!	retDebug = closefile ()


	! uncomment the following statements to write to file the airfoil result

!	call op_fetchdata ( p_q )
!
!	retDebug = openfile ( C_CHAR_"/work/cbertoll/airfoil-cuda-final/q.txt"//C_NULL_CHAR )
!
!	do debugiter = 1, 4*ncell
!
!		datad = q(debugiter)
!		retDebug = writeRealToFile ( datad )
!	end do
!
!	retDebug = closefile ()

end program airfoil