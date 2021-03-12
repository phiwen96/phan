//#define CATCH_CONFIG_RUNNER
//#include <catch2/catch.hpp>




using namespace std;

string readFileIntoString(const string& path) {
    ifstream input_file(path);
        if (!input_file.is_open()) {
            cerr << "Could not open the file - '"
                 << path << "'" << endl;
            exit(EXIT_FAILURE);
        }
        return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}



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
    BOOST_PP_REPEAT (TEST_FILE_COUNT, FACIT, TEST_FILE_FACIT_)
    
    string warning = "";
    
    for (int i = 0; i < TEST_FILE_COUNT; ++i)
    {
        string post = readFileIntoString (test_files_post[i]);
        string facit = readFileIntoString (test_files_facit[i]);
 
        if (post != facit)
        {
            warning += "\n\n\t" + test_files_post[i] + "\n\t != " + "\n\t" + test_files_facit[i] + "\n\n\n";
        }
    }
    if (warning != "") {
        throw runtime_error (warning);
    }
       
//    assert(argc == 0);
   
//    cout << TARGET_FILE_0 << endl;

    return 0;
}


