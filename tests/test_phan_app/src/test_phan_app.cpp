//#define CATCH_CONFIG_RUNNER
//#include <catch2/catch.hpp>




using namespace std;



//SCENARIO( "vectors can be sized and resized", "[vector]" ) {
//
//    GIVEN( "A vector with some items" ) {
//        std::vector<int> v( 5 );
//    }
//}

array <string, TEST_FILE_COUNT> test_files_pre;
array <string, TEST_FILE_COUNT> test_files_post;
array <string, TEST_FILE_COUNT> test_files_facit;


#define PRE(z, n, text) test_files_pre [n] = BOOST_PP_CAT (text, n);
#define POST(z, n, text) test_files_post [n] = BOOST_PP_CAT (text, n);
#define FACIT(z, n, text) test_files_facit [n] = BOOST_PP_CAT (text, n);



int main (int argc, char* argv[])
{
    
    
    BOOST_PP_REPEAT (TEST_FILE_COUNT, PRE, TEST_FILE_PRE_)
    BOOST_PP_REPEAT (TEST_FILE_COUNT, POST, TEST_FILE_POST_)
    
    for (auto i : test_files_pre)
        cout << i << endl;
//    assert(argc == 0);
   
//    cout << TARGET_FILE_0 << endl;

    return 0;
}


