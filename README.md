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

You can run my code by using this command 

$ cmake --build .; cd OpenGL/Debug; ./glfwExample.exe

from the build folder. Make sure to move the shaders into the build folder or they will not work.

You can use "w,a,s,d" to do simple maneuvers around the scene. You can also use "x,y,z" to rotate around their respective axes with respect to the origin. 
There are also the typical like FPS through "t"

Difference:
    The biggest difference is that there is no shadows in the rasterized version in comparison to the ray-traced version. For other differences, the spheres don't look as smooth in the rasterized version, which makes sense because we are making them from triangles. Despite being able to increase the steps for the details, if you zoom in enough, you can still see the triangles.
