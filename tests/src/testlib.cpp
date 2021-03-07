#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <template_library/template_library.hpp>
#include <iostream>
using namespace std;

struct Base{};



TEST_CASE( "Quick check", "[main]" ) {
    std::vector<double> values {1, 2., 3.};
//    auto [mean, moment] = accumulate_vector(values);

    
//    REQUIRE( mean == 2.0 );
//    REQUIRE( moment == Approx(4.666666) );
}


//$<ph>(libname = KUK)
