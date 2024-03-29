#include "main.hpp"




















//$(sej$(aaa){AAA}$(bajs$(moa){kmkd}){tej$(haha){kmkm}}){kukens fitta}
//$(bajskmkd){tej$(haha){kmkd}}



//template <bool DO_LOUD = true>
struct Process
{
    vector <pair <string, string>> declaredVariables;
    Context declVar;
//    Context pasteVar;
//    comment::Context commentVal;
//    string str;
    
    Process () : declVar {nullptr, declaredVariables, new STATE ("begin")}
    {
//        declVar.state -> context = &declVar;
//        pasteVar.state -> context = &pasteVar;
//        commentVal.state -> context = &commentVal;
    }
    
    string process (string str)
    {
//        cout << endl << "input: " << endl << str << endl;
        
        for (auto i = str.begin(); i < str.end(); ++i)
        {
            declVar.process (i);
        }
        
        str = declVar.result;
        
        if(STATE ("done")* d = dynamic_cast<STATE ("done")*>(declVar.state))
        {
           
        } else {

            str += declVar.potential;
        }
        

//            cout << endl << "declare: " << endl << str << endl;
//            cout << endl << "variables: " << endl;
//            for (auto& i : declaredVariables)
//            {
//                cout << i.first << " = " << i.second << endl;
//            }

        
        declVar.result.clear ();
        
        return str;
    }
};



void fileApp (Process& p, filesystem::path const& inputPath, filesystem::path const& outputPath) {
    string input = readFileIntoString (inputPath);
    ofstream outputFile (outputPath);
    
    if (!outputFile.is_open ())
        throw runtime_error ("could not open file " + outputPath.string());
    
    outputFile << p.process (input);
    outputFile.close ();
}


void folderApp (Process& p, filesystem::path inputPath)
{
    
    filesystem::rename (inputPath, filesystem::path{inputPath}.replace_filename (p.process (inputPath.filename ())));
    
    inputPath = filesystem::path{inputPath}.replace_filename (p.process (inputPath.filename ()));
    
    set <filesystem::path> all;
    set <filesystem::path> subdirs;
    set <filesystem::path> subfiles;
    
    for (auto& i : filesystem::directory_iterator (inputPath))
    {
        auto renamed = filesystem::path {i.path().parent_path()} /= p.process (i.path().filename());
        all.insert (renamed);
        filesystem::rename (i.path(), renamed);
        
        if (filesystem::is_directory (renamed))
        {
            subdirs.insert (renamed);
            
        } else if (filesystem::is_regular_file (renamed))
        {
            if (renamed.extension() == ".ph")
            {
                p.process (readFileIntoString (renamed));
                filesystem::remove (renamed);
                
            } else
            {
                subfiles.insert (renamed);
            }
        }
    }
    
    for (auto const& filename : subfiles)
    {
        fileApp (p, filename, filename);
    }
    
    for (auto const& dirname : subdirs)
    {
        folderApp (p, dirname);
    }
}

//template <bool DO_LOUD = true>
void app (filesystem::path const& inputPath, filesystem::path outputPath) {
//    filesystem::path p {inputPath};
//    cout << filesystem::exists(inputPath) << endl;
//    cout << filesystem::is_directory (p) << endl;
//    cout << p.extension() << endl;
//    cout << p.stem() << endl;
//    cout << p.filename() << endl;
    
    
    if (not filesystem::exists (inputPath)) {
//        string warn = "file " + inputPath + "does not exists";
        throw runtime_error ("file " + inputPath.string() + "does not exists");
    }
    if (filesystem::exists (outputPath)) {
//        throw runtime_error ("file already exists");
    }
    
    Process p;
    
//    outputPath.replace_filename (p.process (inputPath.filename ()));
//    outputPath += p.process (inputPath.stem());
    
    if (filesystem::is_directory (inputPath))
    {
//        cout << outputPath << endl;
        outputPath/=inputPath.filename ();
        outputPath = filesystem::path{outputPath}.replace_filename (p.process (outputPath.filename ()));
//        cout << outputPath << endl;
//        return;
        filesystem::copy (inputPath, outputPath, std::filesystem::copy_options::recursive);
        folderApp (p, outputPath);

        
    } else if (filesystem::is_regular_file (inputPath))
    {
        fileApp (p, inputPath, outputPath);
        
    } else
    {
        throw runtime_error ("");
    }
    
}

string warning = "";

void assert_folder(string const& inputPath, string const& outputPath, string& warning) {

//    cout << inputPath << endl;
    app (inputPath, outputPath);
    
}

template <bool DO_LOUD = true>
void assert_file(string const& inputPath, string const& outputPath, string const& facitPath, string& warning) {
    
    app (inputPath, outputPath);
    
    string result = readFileIntoString (outputPath);
    string facit = readFileIntoString (facitPath);
    if constexpr (not DO_LOUD) {
        if (result != facit)
        {
            
            string warn = "\n\n\t" + outputPath + "\n\t != " + "\n\t" + facitPath + "\n\n\n";
            throw runtime_error (warn);
        }
    } else {

        if (result != facit)
        {
            warning += "\n\n\t" + outputPath + "\n\t != " + "\n\t" + facitPath + "\n\n\n";
        }
    }
}
#define LOUD(x) x
#define ASSERT_FILE(file, DO_LOUD) assert_file <DO_LOUD> (string (TEST_FILES_PRE_PATH) + string (BOOST_PP_STRINGIZE (file)), string (TEST_FILES_POST_PATH) + string (BOOST_PP_STRINGIZE (file)), string (TEST_FILES_FACIT_PATH) + string (BOOST_PP_STRINGIZE (file)), warning);

#define ASSERT_FOLDER(folder, DO_LOUD) assert_folder (string (TEST_FOLDERS_PRE_PATH) + string (BOOST_PP_STRINGIZE (folder)), string (TEST_FOLDERS_POST_PATH), warning);

#define ASSERT_FILE_SEQ(r, data, file) assert_file (string (TEST_FILES_PRE_PATH) + string (BOOST_PP_STRINGIZE (file)), string (TEST_FILES_POST_PATH) + string (BOOST_PP_STRINGIZE (file)), string (TEST_FILES_FACIT_PATH) + string (BOOST_PP_STRINGIZE (file)), warning);
#define ASSERT_FILES_2(seqFiles) BOOST_PP_SEQ_FOR_EACH(ASSERT_FILE_SEQ, -, seqFiles);
#define ASSERT_FILES(...) ASSERT_FILES_2 (BOOST_PP_TUPLE_TO_SEQ (__VA_ARGS__));


#define PSTR(x) decltype (const_str {x})

template <class T0, class T1>
concept same = std::is_same_v<T0, T1>;









































//byte


namespace input{
struct Context;
struct State
{
    virtual void process (char const* str, Context& ctx) {}
};

struct Context
{
    State* state {nullptr};
    string input;
    vector <string> outputs;
    void process (char const* str);
};

struct Begin : State
{
    virtual void process (char const* str, Context& ctx);
};
struct Input : State
{
    virtual void process (char const* str, Context& ctx)
    {
//        cout << "Input::process" << endl;
        ctx.input = str;
        ctx.state = new Begin;
//        delete this;
    }
};
struct Output : State
{
    virtual void process (char const* str, Context& ctx)
    {
//        cout << "Output::process" << endl;
        if (strcmp (str, "--input") == 0)
        {
            ctx.state = new Input;
//            delete this;
        } else
        {
            ctx.outputs.push_back (string {str});
        }
    }
};

void Context::process (char const* str)
{
//    cout << "Context::process" << endl;
    state -> process (str, *this);
}

void Begin::process (char const* str, Context& ctx)
{
//    cout << "Begin::process" << endl;
    if (strcmp (str, "--input") == 0)
    {
        cout << "--input" << endl;
        ctx.state = new Input;
//        ctx.state = static_cast <Input*> (ctx.state);
//        delete this;
    } else if (strcmp (str, "--output") == 0)
    {
        ctx.state = new Output;
    } else
    {
        throw runtime_error ("");
    }
}
}





#if defined (Debug)
auto main (int,  char**) -> int
{
    int argc = 5;
    char** argv = new char*[argc]{new char[]{}, new char[]{"--input"}, new char[]{"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_pre/1.hpp"}, new char[]{"--output"}, new char[]{"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_post/1.hpp"}};
    input::Context ctx {new input::Begin};
    
#elif defined (Release)
auto main (int argc,  char** argv) -> int
{
#endif
    
    for (char** it = argv + 1; it < argv + argc; ++it)
    {
        ctx.process (*it);
    }
    
    if (ctx.input.empty ())
    {
        throw runtime_error ("must provide an input file");
    } else if (ctx.outputs.empty ())
    {
        throw runtime_error ("must provide one or more output file");
    }
    
    
    app (ctx.input, ctx.outputs.front ());
    
    return 0;
    
    

    
    
    
//    int argc = 2;
//    auto** argv = new char*[argc]{new char*{"bajs"}, new char*{"--input"}, new char*{"då"}, new char*{"--output"}, new char*{"ssss"}};

    
#if defined (Release)


    
    
        

#endif
    
    
    
    
//    string ss;
//    getline(cin, ss);
    
#if defined (Debug)
    cout << "kuk" << endl;
    removeFolderContent (TEST_FOLDERS_POST_PATH);
    ASSERT_FILE (4.hpp, LOUD (1))

    ASSERT_FOLDER (&(root){philips bibliotek}, LOUD(1))
    return 0;
    ASSERT_FOLDER ($(root){philip}, LOUD(1))
    ASSERT_FILE (1.hpp, LOUD (0))
    ASSERT_FILE (declare.hpp, LOUD (0))
    ASSERT_FILE (4.hpp, LOUD (0))
    ASSERT_FILE (paste.hpp, LOUD (0))
#else
    cout << "kiss" << endl;
    app (argv [1], argv [2]);
#endif


    
#ifdef Debug
    
    #define ANTAL TEST_FILE_COUNT
    

    
//    #define TEST_SINGEL_FILE paste.hpp
    
    #ifdef TEST_SINGEL_FILE
    
        string inputPath =  string (TEST_FILES_PRE_PATH) + string (BOOST_PP_STRINGIZE (TEST_SINGEL_FILE));
        string outputPath = string (TEST_FILES_POST_PATH) + string (BOOST_PP_STRINGIZE (TEST_SINGEL_FILE));
        string facitPath = string (TEST_FILES_FACIT_PATH) + string (BOOST_PP_STRINGIZE (TEST_SINGEL_FILE));
    
        app (inputPath, outputPath);
        string result = readFileIntoString (outputPath);
        string facit = readFileIntoString (facitPath);
        
        if (result != facit)
        {
            warning += "\n\n\t" + outputPath + "\n\t != " + "\n\t" + facitPath + "\n\n\n";
        }
    #endif
    #ifdef TEST_ALL_FILES
        array <string, TEST_FILE_COUNT> test_files_pre;
        array <string, TEST_FILE_COUNT> test_files_post;
        array <string, TEST_FILE_COUNT> test_files_facit;


        #define PRE(z, n, text) test_files_pre [n] = BOOST_PP_CAT (text, n);
        #define POST(z, n, text) test_files_post [n] = BOOST_PP_CAT (text, n);
        #define FACIT(z, n, text) test_files_facit [n] = BOOST_PP_CAT (text, n);
    
      
    
        BOOST_PP_REPEAT (TEST_FILE_COUNT, PRE, TEST_FILE_PRE_)
        BOOST_PP_REPEAT (TEST_FILE_COUNT, POST, TEST_FILE_POST_)
        BOOST_PP_REPEAT (TEST_FILE_COUNT, FACIT, TEST_FILE_FACIT_)
    
        for (int i = 0; i < ANTAL; ++i)
        {
            string inputPath = test_files_pre [i];
            string outputPath = test_files_post [i];
            string facitPath = test_files_facit [i];
            
            app (inputPath, outputPath);
            
            string result = readFileIntoString (outputPath);
            string facit = readFileIntoString (facitPath);
        
    //        string post = readFileIntoString (test_files_post[i]);
    //        string facit = readFileIntoString (test_files_facit[i]);
     
            if (result != facit)
            {
                warning += "\n\n\t" + test_files_post[i] + "\n\t != " + "\n\t" + test_files_facit[i] + "\n\n\n";
            }
        }
    
    #endif
    
    if (warning != "") {
//        throw runtime_error (warning);
        cout << warning << endl;
    }
    return 0;
    
#endif
    
#ifdef DEBUGGING
    ifstream infile;
    infile.open ("/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_pre/test0.hpp");
    
    ofstream outfile;
    outfile.open ("/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_post/test0.hpp");
    
#else
    ifstream infile;
    infile.open (argv[1]);
    
    ofstream outfile;
    outfile.open (argv[2]);
#endif
    
    auto remove_beginning_spaces = bind (remove_beginning_chars, _1, ' ');
    auto remove_beginning_newlines = bind (remove_beginning_chars, _1, '\n');
    
    
    string outtext = readFileIntoString(infile);
//    auto extractors = array <extractor, 1> {extractor{"${", "}"}};
//    auto extractors = array <extractor, 1> {extractor{"${", "}"}};
    
    auto stringVariableDeclerationExtractor = extractor {"$(", ")"};
    auto stringValueDeclerationExtractor = extractor {"{", "}"};
    auto stringVariablePasterExtractor = extractor {"${", "}"};
//    auto stringVariablePasteExtractor = extractor {"${", "}"};
    vector <pair <string, string>> declaredVariables;
    
    

        
    
    
    {
//        Process p;
//        p.process (outtext);
    }
    

    outfile << outtext;
    
    

    
   
//    cout << first_signature (str, first_parser) << endl;
    
    

    PROCC ((
        template <int>
        struct gpu;
        
        $(1)
        {
a       # = comment # ?(name)?{explanation}
        
        € = _function (_anonymous/_non-anonymous) # ?[?_scope_var = @(_var)?]?(0 ?i? 0)?{}??(_function_name)?
        $ = _variable (_non-anonymous){_code} # (_name){}
            @ = _paste
            
            template <>
            struct gpu <${0 i 10}> # {_public change all in scope}
            {
    
                $(0 i 4) # {everyting refering to i will clone for times}
//                # int i${i} -> int i0123
@(i)<1 10>
@(i)<1 10> -> {@} 12345678910
@<0 2> -> {ph}  phph
  
                

                     2
@(hej) <-> ${0#}     02  02
                
                    -1
@(hej#) <-> ${0#}    -10  0-1
                
                     2
@(#hej) <-> ${0#}     022  02

                          2
${0#} -> @(#hej)     002  02

kuk @<1 3> &{ph&}               kuk ph1ph2ph3
fitta @<1 3> -> {ph&} kuk       fitta ph123 kuk
@<1 3> -> &{ph}                 phphph
    

                
                

                
                
@(hej) <-> {0}      0 0
@(hej)              ()  0
@(hej) <- {1}       1   ()
@(hej) -> {2@}      ()  21
@(hej)              1   ()
@(hej) <-> {3@}     3   31
                
@(hej@) <-> {kuk}   "snopphejkuk"
                
@(hej) {philip@} philipkuk

                
  @{i} -> {int i} kommer bli int i int i int i int i
  @{i} -> {int@ i@}  int0 i0 int1 i1 int2 i2 int3 i3
 ${i}    0123
                
                $(str){philip}    "philip"
                ${str} -> {int i@} -> {kuk @ hora}   "kuk int iphilip hora"
$(hej){kukens fitta}   kukens fitta
${hej}       kukens fitta
        
                int i${0 10} = 0; #{_private}
                int j€(0 5)(k) = 3; #{public}
                
                €[j = @(i)](0 i 3){int i@(i + 1) = @(j);}(myFun)
                
//                €[j = @(i)](0 i 3){int i@(i) = @(j);} anonymous
//            0 i 3 prio före 0 i 10 -> därför sätter vi om namnet till j
//            0 i 3 is internal dvs endast inne i {} och i refererar inget utanför
                $(_stat_int){static $(con){constexpr} int} fitta = GPU_COUNT;
            ERROR-> $(_stat){kiss}
                @(_stat_int) count = GPU_COUNT;
                
                 ${static} constexpr uint32_t max_image_dimension_1D = GPU_${i}_MAX_IMAGE_DIMENSION_1D;
                 @(st) int i = 3;
            };
        }
           )(0)(string s));
    
//    cout << s << endl;

//    PROCC ((
//        template <int>
//        struct gpu;
//
//        $(1)
//        {
//            € = inline
//            $ = declare variable, only visible to the current scope
//                and to those below
//
//            template <>
//            struct gpu <$(0 i 10)>
//            {
//                int $(0 j 10) = 0;
//
//                €[j = @(i)](0 i 3){int i@(i) = @(j);}(myFun)
//            0 i 3 prio före 0 i 10 -> därför sätter vi om namnet till j
//            0 i 3 is internal dvs endast inne i {} och i refererar inget utanför
//                $(stat){static $(con){constexpr} int} fitta = GPU_COUNT;
//            ERROR-> $(stat){kiss}
//                 @(stat) count = GPU_COUNT;
//                 ${static} constexpr uint32_t max_image_dimension_1D = GPU_${i}_MAX_IMAGE_DIMENSION_1D;
//                 @(st) int i = 3;
//            };
//        }
//           )(0)(string s));
    
//    cout << s << endl;
        
        
        
        
    
    std::vector<double> input = {1.2, 2.3, 3.4, 4.5};
    
//    cout << "hello world" << endl;
    return 0;
}


string first_signature (string const& first, string const& second, string str, auto&& fun)
{
    auto a1 = str.find(first);
    
    while (a1 != string::npos)
    {
        if (int a2 = str.find (second); a2 != string::npos)
        {

            string replac = fun (string (str.begin() + a1, str.begin() + a2 + 1));
                str.replace (str.begin() + a1, str.begin() + a2 + 1, replac);
        }
        else
        {
            break;
        }
        a1 = str.find ("${");
    }
    
//    string res;
    
//    for (auto const& i : str)
//        res += i;
    
    return str;
}
