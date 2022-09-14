work_dir=build

cmake src -B $work_dir -DCMAKE_BUILD_TYPE=Release
cmake --build $work_dir

cmake src -B $work_dir -DCMAKE_BUILD_TYPE=Debug
cmake --build $work_dir