void stability_constraint_intermediate_jacobian_sparsity(unsigned long const** row,
                                                         unsigned long const** col,
                                                         unsigned long* nnz) {
   static unsigned long const rows[10] = {0,0,0,0,0,0,0,0,0,0};
   static unsigned long const cols[10] = {1,2,3,4,8,9,10,11,12,13};
   *row = rows;
   *col = cols;
   *nnz = 10;
}
