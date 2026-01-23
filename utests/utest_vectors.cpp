#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../src/vec3.h"

TEST_CASE(" Dot Product") 
{
    double eps = 0.001;
    vec3 v1(0.0,1.0,0.0);
    vec3 v2(0.0,0.0,1.0);
    vec3 v3(1.0,1.0,1.0);
    vec3 v4(2.0,2.0,2.0);

    double dot_orth = dot(v1,v2);
    double dot_parallel = dot(v3,v4);
    double answer_dot_orth = 0.0;
    double answer_dot_parallel = 6.0;
    
    REQUIRE_THAT(dot_orth, Catch::Matchers::WithinAbs(answer_dot_orth,eps));
    REQUIRE_THAT(dot_parallel, Catch::Matchers::WithinAbs(answer_dot_parallel,eps));
}

TEST_CASE(" Cross Product ")
{
    double eps = 0.001;
    vec3 v1(0.0,1.0,0.0);
    vec3 v2(0.0,0.0,1.0);
    vec3 v3(1.0,1.0,1.0);
    vec3 v4(2.0,2.0,2.0);

    vec3 cross_orth = cross(v1,v2);
    vec3 cross_parallel = cross(v3,v4);
    vec3 answer_cross_orth(1.0,0.0,0.0);
    vec3 answer_cross_parallel(0.0,0.0,0.0);

    for(int i=0; i<3; ++i) {
        REQUIRE_THAT(cross_orth[i], Catch::Matchers::WithinAbs(answer_cross_orth[i],eps));
        REQUIRE_THAT(cross_parallel[i], Catch::Matchers::WithinAbs(answer_cross_parallel[i],eps));
    }
}

TEST_CASE( " Vector Addition ")
{
    vec3 v1(1.0,1.0,1.0);
    vec3 v2(1.0,2.0,1.0);
    double eps = 0.001;
    vec3 answer_sum(2.0,3.0,2.0);
    vec3 answer_diff(0.0,-1.0,0.0);

    vec3 sum = v1+v2;
    vec3 diff = v1-v2;

   for(int i=0; i<3; ++i) {
        REQUIRE_THAT(sum[i], Catch::Matchers::WithinAbs(answer_sum[i],eps));
        REQUIRE_THAT(diff[i], Catch::Matchers::WithinAbs(answer_diff[i],eps));
    }
}

TEST_CASE(" Scalar Multiplication ")
{
    double eps = 0.001;

    vec3 v1(1.0,1.0,1.0);
    vec3 v2(1.0,2.0,1.0);
    
    vec3 answer_v1(2.0,2.0,2.0);
    vec3 answer_v2(0.5,1.0,0.5);
    vec3 answer_negative(-1.0,-1.0,-1.0);

    vec3 twice_scalar = 2*v1;
    vec3 scalar_twice = v1*2;
    vec3 half_scalar = v2/2;
    vec3 negative_v1 = -v1;

    for(int i=0; i<3; ++i) {
        REQUIRE_THAT(twice_scalar[i], Catch::Matchers::WithinAbs(answer_v1[i],eps));
        REQUIRE_THAT(negative_v1[i], Catch::Matchers::WithinAbs(answer_negative[i],eps));
        REQUIRE_THAT(scalar_twice[i], Catch::Matchers::WithinAbs(answer_v1[i],eps));
        REQUIRE_THAT(half_scalar[i], Catch::Matchers::WithinAbs(answer_v2[i],eps));
    }
    
}

TEST_CASE( "Unit Vector" )
{
    double eps = 0.001;
    vec3 v1(2.0,-4.0,1.0);

    vec3 unit_v1 = unit_vector(v1);
    vec3 answer_v1(0.436,-0.872,0.218);

    for(int i=0; i<3; ++i) {
        REQUIRE_THAT(unit_v1[i], Catch::Matchers::WithinAbs(answer_v1[i],eps));
    }
    
    
   
}