To run my unit test code (This is what I did)
From startcode folder


cmake --preset=default
cd buildVCpkg
cmake --build .

ctest

If you want to run the individual files run one of these. 
$ ./utests/Debug/utest_rays.exe
$ ./utests/Debug/utest_spheres.exe
$ ./utests/debug/utest_vectors.exe

For running the examples/main, I have been using the following 
$ ./examples/Debug/main.exe -w 1000 -h 1000 -d 0.075


