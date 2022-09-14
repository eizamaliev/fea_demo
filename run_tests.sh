work_dir=test_dir

cmake src -B $work_dir -DENABLE_TESTS=ON
cmake --build $work_dir 

./$work_dir/bin/Tests

# ./$work_dir/bin/fem_demo data/mesh_coarse.k

rm -rf $work_dir