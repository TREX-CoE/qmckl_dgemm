#include <stdio.h>
#include "config.h"

#if defined(HAVE_MKL)
#include <mkl.h>
#else
#include <cblas.h>
#endif

#include "utils.h"
#include "qmckl_dgemm.h"
#include "qmckl_dgemm_private.h"

//int main(int argc, char *argv[]) {
int main() {

  double *A;
  double *B;
  double *C;
  double *CUnpack;
  double *ABlas;
  double *BBlas;
  double *DBlas;
  int64_t DIM_M, DIM_N, DIM_K;
  int64_t M, N, K;
  int64_t MBlas, NBlas, KBlas;
  int64_t incColA = 1;
  int64_t incColB = 1;
  int64_t incColC = 1;
  int64_t iterM, iterN, iterK;
  int64_t rep = 0;

  int64_t DIM_M_MAX =MR2*4;
  int64_t DIM_N_MAX =NR2*4;
  int64_t DIM_K_MAX =50;

  for(iterM = 1;iterM <= DIM_M_MAX; ++iterM){
    DIM_M = iterM;
    for(iterN = 1;iterN <= DIM_N_MAX; ++iterN){
      DIM_N = iterN;
      for(iterK = 1;iterK <= DIM_K_MAX; ++iterK){
	DIM_K = iterK;


	//qmckl_context context = qmckl_context_create();
	//qmckl_context_struct* const ctx = (qmckl_context_struct* const) context;
  qmckl_packed_matrix packed_matrix_A = qmckl_packed_matrix_create();
  qmckl_packed_matrix packed_matrix_B = qmckl_packed_matrix_create();
  qmckl_packed_matrix packed_matrix_C = qmckl_packed_matrix_create();
  qmckl_packed_struct* const pmatA = (qmckl_packed_struct* const) packed_matrix_A;
  qmckl_packed_struct* const pmatB = (qmckl_packed_struct* const) packed_matrix_B;
  qmckl_packed_struct* const pmatC = (qmckl_packed_struct* const) packed_matrix_C;

  //init_dims_avx2_input(context, DIM_M, DIM_N, DIM_K);
  qmckl_init_pack(packed_matrix_A, 'A', DIM_M, DIM_N, DIM_K);
  qmckl_init_pack(packed_matrix_B, 'B', DIM_M, DIM_N, DIM_K);
  qmckl_init_pack(packed_matrix_C, 'C', DIM_M, DIM_N, DIM_K);

  M = DIM_M;
  N = DIM_N;
  K = DIM_K;
  MBlas = M;
  NBlas = N;
  KBlas = K;
  printf("\n----------------------------------\n");
  printf("(%ld, %ld, %ld) | M=%ld K=%ld N=%ld | ",iterM, iterN, iterK, (long)M,(long)K,(long)N);

  int64_t incRowA = K;
  int64_t incRowB = N;
  int64_t incRowC = pmatC->Nt;

  A = (double *)malloc( M * K * sizeof(double));
  B = (double *)malloc( K * N * sizeof(double));
  C = (double *)aligned_alloc( 64, pmatC->Mt * pmatC->Nt * sizeof(double));
  CUnpack = (double *)aligned_alloc( 64, pmatC->Mt * pmatC->Nt * sizeof(double));

  ABlas = (double *)malloc( MBlas * KBlas * sizeof(double));
  BBlas = (double *)malloc( KBlas * NBlas * sizeof(double));
  DBlas = (double *)malloc( MBlas * NBlas * sizeof(double));

  fill_matrix_random(A, M,K);
  fill_matrix_random(B, K, N);
  fill_matrix_zeros  (C, M*N);

  copy_matrix(ABlas, A, MBlas,KBlas);
  copy_matrix(BBlas, B, KBlas,NBlas);
  fill_matrix_zeros  (DBlas, MBlas*NBlas);

  int i,j=rep;

  qmckl_pack_matrix(packed_matrix_A, 'A', M, K, A, incRowA);
  qmckl_pack_matrix(packed_matrix_B, 'B', K, N, B, incRowB);
  qmckl_pack_matrix(packed_matrix_C, 'C', M, N, C, incRowC);

  qmckl_dgemm_tiled_avx2_nn(packed_matrix_A, incRowA,
			   packed_matrix_B, incRowB,
			   packed_matrix_C, incRowC);

  const int MB = MBlas;
  const int NB = NBlas;
  const int KB = KBlas;
  const double alpha=1.0;
  const double beta=0.0;

#if defined(HAVE_MKL)

  double *ABlasp;
  double *BBlasp;


  // Get size of packed A
  size_t ABlasp_size = dgemm_pack_get_size("A",&MB,&NB,&KB);
  ABlasp = (double *)mkl_malloc(ABlasp_size,64);

  // Pack
  dgemm_pack("A","T",&MB,&NB,&KB,&alpha,ABlas,&KB,ABlasp);

  // Get size of packed B
  size_t BBlasp_size = dgemm_pack_get_size("B",&MB,&NB,&KB);
  BBlasp = (double *)mkl_malloc(BBlasp_size,64);

  // Pack
  dgemm_pack("B","T",&MB,&NB,&KB,&alpha,BBlas,&NB,BBlasp);

  dgemm_compute("P","P",&MB,&NB,&KB,ABlasp,&KB,BBlasp,&NB,&beta,DBlas,&MB);

  //print_matrix(DBlas, M, N);
  //printf("\n-------------diff-----------------\n");
  //print_diff_matrix_AT_B(C,D, M, N);
  //print_diff_matrix_ASer_BT(context, C,DBlas, M, N);
  qmckl_unpack_matrix(packed_matrix_C, CUnpack, M, N);
  //print_diff_matrix(CUnpack,DBlas, M, N);
  qmckl_exit_code rc = get_diff_matrix_ABT(CUnpack,DBlas, M, N);
  if(rc == QMCKL_FAILURE){
    printf(" QMCKL_FAILURE !\n");
    return QMCKL_FAILURE;
  }
  else{
    printf(" QMCKL_SUCCESS !\n");
  }
  //print_matrix(CUnpack, M, N);
  printf("\n----------------------------------\n");

  mkl_free(ABlasp);
  mkl_free(BBlasp);

#else

   CBLAS_LAYOUT order = CblasColMajor;
   CBLAS_TRANSPOSE transA = CblasTrans;
   CBLAS_TRANSPOSE transB = CblasTrans;

  qmckl_unpack_matrix(packed_matrix_C, CUnpack, M, N);

  cblas_dgemm(order, transA, transB, MB,NB,KB,alpha,ABlas,KB,BBlas,NB,beta,DBlas,MB);

  qmckl_exit_code rc = get_diff_matrix_ABT(CUnpack,DBlas, M, N);

  if(rc == QMCKL_FAILURE){
    printf(" QMCKL_FAILURE !\n");
    return QMCKL_FAILURE;
  }
  else{
    printf(" QMCKL_SUCCESS !\n");
  }
  printf("\n----------------------------------\n");

#endif

  qmckl_packed_matrix_destroy(packed_matrix_A);
  qmckl_packed_matrix_destroy(packed_matrix_B);
  qmckl_packed_matrix_destroy(packed_matrix_C);
  //qmckl_context_destroy(context);
  free(A);
  free(B);
  free(C);
  free(CUnpack);
  free(ABlas);
  free(BBlas);
  free(DBlas);
      }
    }
  }

  return QMCKL_SUCCESS;
}
