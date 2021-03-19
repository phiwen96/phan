#pragma once
#define DEBUGGING
#include <const_str/const_str.hpp>
#include "declare.hpp"
#include "paste.hpp"
#include "comment.hpp"
#include "extractor.hpp"
#include "phime.hpp"
#include "compiletime.hpp"


#define PROCC2(str, max, var) var = []()->string\
{\
return str;\
}();// = [&s]->string{return "";}();

#define PROCC(x) PROCC2 (BOOST_PP_STRINGIZE (BOOST_PP_SEQ_ELEM (0, x)), BOOST_PP_SEQ_ELEM (1, x), BOOST_PP_SEQ_ELEM (2, x))//BOOST_PP_SEQ_ELEM (2, x) = [](string s){ \

using namespace placeholders;
using namespace std;

string readFileIntoString(ifstream& input_file) {
      return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}






void remove_all_chars (string& str, char c)
{
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), c);
    str.erase(end_pos, str.end());
}

void remove_beginning_chars (string& str, char c)
{
    while (str.front() == c) {
        str.erase (str.begin(), str.begin() + 1);
    }
}



string readFileIntoString(const string& path) {
    ifstream input_file(path);
        if (!input_file.is_open()) {
            throw runtime_error ("Could not open the file - '" + path);
        }
        return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close ();
}


void removeFolderContent (filesystem::path const& p) {
    for (auto& i : filesystem::directory_iterator (p))
        filesystem::remove_all (i.path ());
}


auto allocated = size_t{0};
// Overload operator new and delete to track allocations
void* operator new (size_t size) {
  void* p = std::malloc(size);
  allocated += size;
  return p;
}
void operator delete(void* p) noexcept {
  return std::free(p);
}


template <size_t number_of_bytes, size_t alignment = alignof (max_align_t)>
struct storage_in_bytes
{
      alignas (alignment) byte m_bytes [number_of_bytes];
      
      operator byte * ()
      {
            return m_bytes;
      }
      
      
      
      /**
       @brief number of bytes in storage
       */
      static consteval auto bytes () -> size_t
      {
            return number_of_bytes;
      }
};




#define PH_REFLECT_(...) constexpr auto reflect () const -> decltype (std::tie (__VA_ARGS__)) {return std::tie (__VA_ARGS__);}  //auto reflect2 () -> decltype (std::tie (__VA_ARGS__)) {return std::tie (__VA_ARGS__);}

//return BOOST_PP_STRINGIZE(elem);
#define PH_REFLECT(...) \
\
template <int = -1> \
auto reflect_const () const;\
\
template <> \
auto reflect_const<-1>() const\
{\
return std::tie (__VA_ARGS__);  \
}  \
\
template <> \
auto& reflect ();\
\
BOOST_PP_SEQ_FOR_EACH (__BAJS2, +, BOOST_PP_VARIADIC_TO_SEQ (__VA_ARGS__))

