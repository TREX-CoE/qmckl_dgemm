module qmckl_dgemm
  use, intrinsic :: iso_c_binding
  integer  , parameter :: qmckl_context = c_int64_t
  integer*8, parameter :: QMCKL_NULL_CONTEXT = 0 
  integer  , parameter :: qmckl_exit_code = c_int32_t
  integer(qmckl_exit_code), parameter :: QMCKL_SUCCESS                  = 0
  integer(qmckl_exit_code), parameter :: QMCKL_FAILURE                  = 101

  ! Fortran Interfaces
  interface
     integer (qmckl_context) function qmckl_context_create() bind(C)
       use, intrinsic :: iso_c_binding
       import
     end function qmckl_context_create
  end interface
  
  interface
     integer (qmckl_exit_code) function qmckl_context_destroy(context) bind(C)
       use, intrinsic :: iso_c_binding
       import
       integer (qmckl_context), intent(in), value :: context
     end function qmckl_context_destroy
  end interface

   interface
   integer(c_int32_t) function qmckl_init_pack &
       (context, mType, m, n, k) &
       bind(C)
     use, intrinsic :: iso_c_binding
     import
     implicit none
 
     integer (c_int64_t) , intent(in)  , value :: context
     character           , intent(in)  , value :: mType
     integer (c_int64_t) , intent(in)  , value :: m
     integer (c_int64_t) , intent(in)  , value :: n
     integer (c_int64_t) , intent(in)  , value :: k
 
   end function qmckl_init_pack
 end interface

   interface
   integer(c_int32_t) function qmckl_pack_matrix &
       (context, mType, m, n, A, lda, A_tile) &
       bind(C)
     use, intrinsic :: iso_c_binding
     import
     implicit none
 
     integer (c_int64_t) , intent(in)  , value :: context
     character           , intent(in)  , value :: mType
     integer (c_int64_t) , intent(in)  , value :: m
     integer (c_int64_t) , intent(in)  , value :: n
     integer (c_int64_t) , intent(in)  , value :: lda
     real    (c_double ) , intent(in)          :: A(lda,*)
     real    (c_double ) , intent(in)          :: A_tile(*)
 
   end function qmckl_pack_matrix
 end interface

   interface
   integer(c_int32_t) function qmckl_dgemm_tiled_avx2_nn &
       (context, A, lda, B, ldb, C, ldc) &
       bind(C)
     use, intrinsic :: iso_c_binding
     import
     implicit none
 
     integer (c_int64_t) , intent(in)  , value :: context
     integer (c_int64_t) , intent(in)  , value :: lda
     real    (c_double ) , intent(in)          :: A(lda,*)
     integer (c_int64_t) , intent(in)  , value :: ldb
     real    (c_double ) , intent(in)          :: B(ldb,*)
     integer (c_int64_t) , intent(in)  , value :: ldc
     real    (c_double ) , intent(in)          :: C(ldc,*)
 
   end function qmckl_dgemm_tiled_avx2_nn
 end interface

   interface
   integer(c_int32_t) function qmckl_dgemm_tiled_NN &
       (context, Min, Nin, Kin, A, lda, B, ldb, C, ldc) &
       bind(C)
     use, intrinsic :: iso_c_binding
     import
     implicit none
 
     integer (c_int64_t) , intent(in)  , value :: context
     integer (c_int64_t) , intent(in)  , value :: Min
     integer (c_int64_t) , intent(in)  , value :: Nin
     integer (c_int64_t) , intent(in)  , value :: Kin
     integer (c_int64_t) , intent(in)  , value :: lda
     real    (c_double ) , intent(in)          :: A(lda,*)
     integer (c_int64_t) , intent(in)  , value :: ldb
     real    (c_double ) , intent(in)          :: B(ldb,*)
     integer (c_int64_t) , intent(in)  , value :: ldc
     real    (c_double ) , intent(in)          :: C(ldc,*)
 
   end function qmckl_dgemm_tiled_NN
 end interface

   interface
   integer(c_int32_t) function qmckl_unpack_matrix &
       (context, A, m, n) &
       bind(C)
     use, intrinsic :: iso_c_binding
     import
     implicit none
 
     integer (c_int64_t) , intent(in)  , value :: context
     integer (c_int64_t) , intent(in)  , value :: m
     integer (c_int64_t) , intent(in)  , value :: n
     real    (c_double ) , intent(in)          :: A(n,*)
 
   end function qmckl_unpack_matrix
 end interface

end module qmckl_dgemm