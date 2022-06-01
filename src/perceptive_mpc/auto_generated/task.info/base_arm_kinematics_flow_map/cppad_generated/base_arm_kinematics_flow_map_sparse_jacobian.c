#include <math.h>
#include <stdio.h>

typedef struct Array {
    void* data;
    unsigned long size;
    int sparse;
    const unsigned long* idx;
    unsigned long nnz;
} Array;

struct LangCAtomicFun {
    void* libModel;
    int (*forward)(void* libModel,
                   int atomicIndex,
                   int q,
                   int p,
                   const Array tx[],
                   Array* ty);
    int (*reverse)(void* libModel,
                   int atomicIndex,
                   int p,
                   const Array tx[],
                   Array* px,
                   const Array py[]);
};

void base_arm_kinematics_flow_map_sparse_jacobian(double const *const * in,
                                                  double*const * out,
                                                  struct LangCAtomicFun atomicFun) {
   //independent variables
   const double* x = in[0];

   //dependent variables
   double* jac = out[0];

   // auxiliary variables
   double v[5];

   jac[0] = x[15] / 2.;
   jac[1] = x[2] * 0.5;
   jac[2] = - jac[0];
   jac[3] = - x[1] * 0.5;
   jac[5] = x[4] * 0.5;
   jac[6] = - jac[0];
   jac[7] = - x[3] * 0.5;
   v[0] = - x[14];
   v[0] = v[0] + v[0];
   v[1] = 0 - x[2] * x[14];
   v[1] = v[1] + v[1];
   jac[8] = x[2] * v[0] + v[1];
   v[2] = x[14] + x[14];
   v[3] = x[3] * x[14];
   jac[14] = v[3] + v[3];
   jac[9] = 0 - (x[3] * v[2] + jac[14]);
   v[3] = - x[2];
   v[3] = v[3] + v[3];
   v[4] = x[3] + x[3];
   jac[10] = x[2] * v[3] + 1 - x[3] * v[4];
   jac[11] = 0 - v[1];
   jac[12] = 0 - x[1] * v[0];
   jac[13] = x[4] * v[2];
   jac[15] = x[4] * v[4] - x[1] * v[3];
   // variable duplicates: 1
   jac[4] = jac[0];
   // dependent variables without operations
   jac[16] = 1;
   jac[17] = 1;
   jac[18] = 1;
   jac[19] = 1;
   jac[20] = 1;
   jac[21] = 1;
}

