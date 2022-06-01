void ee_quadratic_cost_intermediate_jacobian_sparsity(unsigned long const** row,
                                                      unsigned long const** col,
                                                      unsigned long* nnz) {
   static unsigned long const rows[68] = {0,1,2,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13};
   static unsigned long const cols[68] = {14,15,16,17,18,19,20,21,1,2,3,4,5,8,9,10,11,12,1,2,3,4,6,8,9,10,11,12,1,2,3,4,7,8,9,10,11,12,1,2,3,4,8,9,10,11,12,13,1,2,3,4,8,9,10,11,12,13,1,2,3,4,8,9,10,11,12,13};
   *row = rows;
   *col = cols;
   *nnz = 68;
}
