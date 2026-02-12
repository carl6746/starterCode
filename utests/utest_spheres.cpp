#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../src/components/sphere.h"

TEST_CASE("Missed sphere") 
{
    HitStruct rec;
    Sphere s({0,0,-10},1,{0,0,0});
    double tmax=1000;
    REQUIRE(s.intersect(ray({0,0,0},{2,2,2}),1,tmax,rec)==false);
}

TEST_CASE("Hit sphere straight on")
{
    HitStruct rec;
    Sphere s({0,0,-10},1,{0,0,0});
    double tmax=1000;
    REQUIRE(s.intersect(ray({0,0,0},{0,0,-1}),1,tmax,rec)==true);
}

TEST_CASE("Hit sphere from different perspective") {
    HitStruct rec;
    Sphere s({3,3,-3},1,{0,0,0});
    double tmax=1000;
    REQUIRE(s.intersect(ray({1,1,1},{1,1,-2}),1,tmax,rec)==true);
}

TEST_CASE("Hit sphere at an angle from origin") {
    HitStruct rec;
    Sphere s({5,5,-5},1,{0,0,0});
    double tmax=1000;
    REQUIRE(s.intersect(ray({0,0,0},{1,1,-1}),1,tmax,rec)==true);
}
