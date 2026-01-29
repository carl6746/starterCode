#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "components/ray.h"

TEST_CASE("Parametric Line") 
{
    double eps = 0.001;
    point3 orig(0.0,0.0,0.0);
    vec3 dir(1.0,1.0,1.0);

    Ray test_ray(orig,dir);

    for(int t = -1000; t<1000; ++t) {
        REQUIRE_THAT(test_ray.at(t)[0], Catch::Matchers::WithinAbs(t,eps));
        REQUIRE_THAT(test_ray.at(t)[1], Catch::Matchers::WithinAbs(t,eps));
        REQUIRE_THAT(test_ray.at(t)[2], Catch::Matchers::WithinAbs(t,eps));
    }
}

TEST_CASE("Get Origin/ Get Direction")
{
    double eps = 0.001;
    point3 orig(1.0,2.0,3.0);
    vec3 dir(1.2,3.4,5.6);  

    Ray test_ray(orig,dir);

    for(int i = 0; i<3; ++i) {
        REQUIRE_THAT(test_ray.origin()[i], Catch::Matchers::WithinAbs(orig[i],eps));
        REQUIRE_THAT(test_ray.direction()[i], Catch::Matchers::WithinAbs(dir[i],eps));
    }
}
