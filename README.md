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


