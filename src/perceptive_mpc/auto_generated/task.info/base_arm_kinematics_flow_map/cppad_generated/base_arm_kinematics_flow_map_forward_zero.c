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

void base_arm_kinematics_flow_map_forward_zero(double const *const * in,
                                               double*const * out,
                                               struct LangCAtomicFun atomicFun) {
   //independent variables
   const double* x = in[0];

   //dependent variables
   double* y = out[0];

   // auxiliary variables
   double v[2];

   v[0] = x[15] / 2.;
   y[0] = x[2] * v[0];
   y[1] = 0 - x[1] * v[0];
   y[2] = x[4] * v[0];
   y[3] = 0 - x[3] * v[0];
   v[0] = 0 - x[2] * x[14];
   v[0] = v[0] + v[0];
   v[1] = x[3] * x[14];
   v[1] = v[1] + v[1];
   y[4] = x[2] * v[0] + x[14] - x[3] * v[1];
   y[5] = x[4] * v[1] - x[1] * v[0];
   // dependent variables without operations
   y[6] = 0;
   y[7] = x[16];
   y[8] = x[17];
   y[9] = x[18];
   y[10] = x[19];
   y[11] = x[20];
   y[12] = x[21];
}

