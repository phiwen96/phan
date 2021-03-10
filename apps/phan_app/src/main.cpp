#include "main.hpp"

#include "extractor.hpp"
#include "trio.hpp"
#include <functional>
















void fun(string& str,vector <pair <string, string>>& declaredVariables, extractor& stringVariableDeclerationExtractor, extractor& stringValueDeclerationExtractor, extractor& stringVariablePasterExtractor){
    
    for (string::iterator it = str.begin(); it != str.end(); ++it) {
        auto declVar = stringVariableDeclerationExtractor.found (*it);
        
        if (declVar) {
            auto [var0, var1, var2, var3] = declVar.value();

            auto [_begin, _end] = pair {it - (var3 - var0 - 1), it + 1};
            string varstring = string (_begin + (var1-var0), _end - (var3 - var2) - 1);
            
            
            
//            auto stringVariableDeclerationExtractor2 = extractor {"$((", ")){{"};
//            auto stringValueDeclerationExtractor2 = extractor {"{{", "}}"};
//            auto stringVariablePasterExtractor2 = extractor {"${{", "}}"};
//            cout << varstring << endl;
//            string tmp = string (_end, str.end());
//            auto a1 = tmp.find_first_of ("}}");
//            if (a1 == string::npos)
////                continue;
//            else
//            {
//                tmp = string (tmp.begin(), tmp.begin() + a1);
//                cout << tmp << endl;
////                fun (tmp, declaredVariables, stringVariableDeclerationExtractor2, stringValueDeclerationExtractor2, stringVariablePasterExtractor2);
//            }
            
//            cout << tmp << endl;
//            return;
//            return;

                for (auto it2 = it + 1; it2 != str.end(); ++it2)
                {
                    auto declVal = stringValueDeclerationExtractor.found (*it2);
                    if (declVal) {
                        auto [val0, val1, val2, val3] = declVal.value();
                        auto [_begin2, _end2] = pair {it2 - (val3 - val0 - 1), it2 + 1};

                        auto valstring = string (_begin2 + (val1-val0), _end2 - (val3 - val2) - 1);
                        
                        auto stringVariableDeclerationExtractor2 = extractor {"$(", ")"};
                        auto stringValueDeclerationExtractor2 = extractor {"{", "}"};
                        auto stringVariablePasterExtractor2 = extractor {"${", "}"};
                        
                        cout << ":" << valstring << endl;
                        fun (valstring, declaredVariables, stringVariableDeclerationExtractor2, stringValueDeclerationExtractor2, stringVariablePasterExtractor2);
                        cout << ":" << valstring << endl;

                        auto declared = declaredVariables.begin();
                        for (; declared != declaredVariables.end(); ++declared)
                        {
                            if (declared -> first == varstring) {
                                break;
                            }
                        }

                        if (declared == declaredVariables.end())
                        {
                            declaredVariables.emplace_back (varstring, valstring);
                        } else
                        {
                            declared -> second = valstring;
                        }

                        cout << varstring << " = " << valstring << endl;

//                        delete stringVariableDeclerationExtractor;
                        stringVariableDeclerationExtractor.reset();

//                        delete stringValueDeclerationExtractor;
                        stringValueDeclerationExtractor.reset();

//                        delete stringVariablePasterExtractor;
                        stringVariablePasterExtractor.reset();

    //                    cout << *it2 << endl;
                        str.replace (it - (var3 - var0) + 1, it2 + 1, valstring);
    //                    cout << valstring << endl;
    //                    cout << (var3 - var2) + (var1 - var0) << endl;
    //                    cout << *(it - (var3 - var2) - (var1 - var0) - 1) << endl;
                        it = it - (var3 - var2) - (var1 - var0) - 1;

                        break;
                    }
                }
            
            continue;
        }
        
            auto pasteVar = stringVariablePasterExtractor.found (*it);
            if (pasteVar)
            {
                auto [var0, var1, var2, var3] = pasteVar.value();
                auto [_begin2, _end2] = pair {it - (var3 - var0 - 1), it + 1};
                auto varstring = string (_begin2 + (var1-var0), _end2 - (var3 - var2) - 1);
                cout << varstring << endl;

                auto declared = declaredVariables.begin();
                for (; declared != declaredVariables.end(); ++declared)
                {
                    if (declared -> first == varstring) {
                        break;
                    }
                }

                if (declared == declaredVariables.end())
                {
                    cout << "ERROR" << endl << "variable " << declared->first << " is not yet defined!" << endl;
                    throw runtime_error ("variable pasted but not yet declared");
                } else
                {
    //                cout << *it << endl;
                    str.replace (_begin2, _end2, declared -> second);
                    it = it - (var3 - var2) - (var1 - var0) - 1;
    //                cout << *(it) << endl;
                }

//                delete stringVariablePasterExtractor;
                stringVariablePasterExtractor.reset();
            }
        
    }
};




auto main(int argc,  char** argv) -> int
{
    
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
    
    
    [&outtext]{
        vector <pair <string, string>> declaredVariables;
        auto* stringVariableDeclerationExtractor = new extractor {"$((", "))"};
        auto* stringValueDeclerationExtractor = new extractor {"{{", "}}"};
        auto* stringVariablePasterExtractor = new extractor {"${{", "}}"};
        for (auto it = outtext.begin(); it != outtext.end(); ++it) {
            auto declVar = stringVariableDeclerationExtractor -> found (*it);
            
            if (declVar) {
                auto [var0, var1, var2, var3] = declVar.value();

                auto [_begin, _end] = pair {it - (var3 - var0 - 1), it + 1};
                auto varstring = string (_begin + (var1-var0), _end - (var3 - var2) - 1);

                for (auto it2 = it + 1; it2 != outtext.end(); ++it2)
                {
                    auto declVal = stringValueDeclerationExtractor -> found (*it2);
                    if (declVal) {
                        auto [val0, val1, val2, val3] = declVal.value();
                        auto [_begin2, _end2] = pair {it2 - (val3 - val0 - 1), it2 + 1};

                        auto valstring = string (_begin2 + (val1-val0), _end2 - (val3 - val2) - 1);
                        
                        auto declared = declaredVariables.begin();
                        for (; declared != declaredVariables.end(); ++declared)
                        {
                            if (declared -> first == varstring) {
                                break;
                            }
                        }
                        
                        if (declared == declaredVariables.end())
                        {
                            declaredVariables.emplace_back (varstring, valstring);
                        } else
                        {
                            declared -> second = valstring;
                        }
                        
                        cout << varstring << " = " << valstring << endl;
                        
                        delete stringVariableDeclerationExtractor;
                        stringVariableDeclerationExtractor = new extractor {"$((", "))"};
                        
                        delete stringValueDeclerationExtractor;
                        stringValueDeclerationExtractor = new extractor {"{{", "}}"};
                        
                        delete stringVariablePasterExtractor;
                        stringVariablePasterExtractor = new extractor {"${{", "}}"};
                        
    //                    cout << *it2 << endl;
                        outtext.replace (it - (var3 - var0) + 1, it2 + 1, valstring);
    //                    cout << valstring << endl;
    //                    cout << (var3 - var2) + (var1 - var0) << endl;
    //                    cout << *(it - (var3 - var2) - (var1 - var0) - 1) << endl;
                        it = it - (var3 - var2) - (var1 - var0) - 1;
                        
                        break;
                    }
                }
                
                continue;
            }
            
            auto pasteVar = stringVariablePasterExtractor -> found (*it);
            if (pasteVar)
            {
                auto [var0, var1, var2, var3] = pasteVar.value();
                auto [_begin2, _end2] = pair {it - (var3 - var0 - 1), it + 1};
                auto varstring = string (_begin2 + (var1-var0), _end2 - (var3 - var2) - 1);
    //            cout << varstring << endl;
                
                auto declared = declaredVariables.begin();
                for (; declared != declaredVariables.end(); ++declared)
                {
                    if (declared -> first == varstring) {
                        break;
                    }
                }
                
                if (declared == declaredVariables.end())
                {
                    throw runtime_error ("variable pasted but not yet declared");
                } else
                {
    //                cout << *it << endl;
                    outtext.replace (_begin2, _end2, declared -> second);
                    it = it - (var3 - var2) - (var1 - var0) - 1;
    //                cout << *(it) << endl;
                }
                
                delete stringVariablePasterExtractor;
                stringVariablePasterExtractor = new extractor {"${{", "}}"};
            }
            
        }
    };
    
    
    
    
    fun (outtext, declaredVariables, stringVariableDeclerationExtractor, stringValueDeclerationExtractor, stringVariablePasterExtractor);
    
    

    
    for(auto& i : declaredVariables)
    {
//        cout << i.first << endl << i.second << endl;
    }
//    if (found) {
//        outfile << "YES\n";
//    }
//    else {
//        outfile << "NO\n";
//    }
//    outtext = first_signature ("${", "}", outtext, first_parser);
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
