void base_arm_kinematics_flow_map_jacobian_sparsity(unsigned long const** row,
                                                    unsigned long const** col,
                                                    unsigned long* nnz) {
   static unsigned long const rows[22] = {0,0,1,1,2,2,3,3,4,4,4,5,5,5,5,5,7,8,9,10,11,12};
   static unsigned long const cols[22] = {2,15,1,15,4,15,3,15,2,3,14,1,2,3,4,14,16,17,18,19,20,21};
   *row = rows;
   *col = cols;
   *nnz = 22;
}
