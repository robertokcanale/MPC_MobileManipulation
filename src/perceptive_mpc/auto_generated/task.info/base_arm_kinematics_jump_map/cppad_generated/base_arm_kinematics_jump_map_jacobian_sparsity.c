void base_arm_kinematics_jump_map_jacobian_sparsity(unsigned long const** row,
                                                    unsigned long const** col,
                                                    unsigned long* nnz) {
   static unsigned long const rows[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
   static unsigned long const cols[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
   *row = rows;
   *col = cols;
   *nnz = 13;
}
