#define CATCH_CONFIG_MAIN
// #include <catch2/catch.hpp>
//#include <template_library/template_library.hpp>
using namespace std;

struct Base{};


//int main(int argv, char** argv)
//{
//    assert(argv == 3);
//}


// TEST_CASE ( "Quick check", "[main]" )
// {
//     std::vector<double> values {1, 2., 3.};
// //    auto [mean, moment] = accumulate_vector(values);
//     SECTION ( "resizing bigger changes size and capacity" )
//     {
//         values.resize( 10 );

//             REQUIRE ( values.size() == 10 );
//             REQUIRE ( values.capacity() >= 10 );
//     }
    
// //    REQUIRE( mean == 2.0 );
// //    REQUIRE( moment == Approx(4.666666) );
// }

// SCENARIO( "vectors can be sized and resized", "[vector]" ) {

//     GIVEN( "A vector with some items" ) {
//         std::vector<int> v( 5 );

//         REQUIRE( v.size() == 5 );
//         REQUIRE( v.capacity() >= 5 );

//         WHEN( "the size is increased" ) {
//             v.resize( 10 );

//             THEN( "the size and capacity change" ) {
//                 REQUIRE( v.size() == 10 );
//                 REQUIRE( v.capacity() >= 10 );
//             }
//         }
//         WHEN( "the size is reduced" ) {
//             v.resize( 0 );

//             THEN( "the size changes but not capacity" ) {
//                 REQUIRE( v.size() == 0 );
//                 REQUIRE( v.capacity() >= 5 );
//             }
//         }
//         WHEN( "more capacity is reserved" ) {
//             v.reserve( 10 );

//             THEN( "the capacity changes but not the size" ) {
//                 REQUIRE( v.size() == 5 );
//                 REQUIRE( v.capacity() >= 10 );
//             }
//         }
//         WHEN( "less capacity is reserved" ) {
//             v.reserve( 0 );

//             THEN( "neither size nor capacity are changed" ) {
//                 REQUIRE( v.size() == 5 );
//                 REQUIRE( v.capacity() >= 5 );
//             }
//         }
//     }
// }


//$<ph>(libname = KUK)


int main(){return 0;}
