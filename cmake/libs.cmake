
set (boost_preprocessor_pch 
<boost/preprocessor/seq/for_each.hpp>
<boost/preprocessor/variadic/to_seq.hpp>
<boost/preprocessor/facilities/expand.hpp>
<boost/preprocessor/stringize.hpp>
<boost/preprocessor/repetition/enum_params.hpp>
<boost/preprocessor/cat.hpp>
<boost/preprocessor/seq/cat.hpp>
<boost/preprocessor/control/if.hpp>
<boost/preprocessor/facilities/empty.hpp>
<boost/preprocessor/facilities/identity.hpp>
<boost/preprocessor/seq/to_tuple.hpp>
<boost/preprocessor/seq/for_each_i.hpp>
<boost/preprocessor/arithmetic/inc.hpp>
<boost/preprocessor/comparison/not_equal.hpp>
<boost/preprocessor/repetition/for.hpp>
<boost/preprocessor/tuple/elem.hpp>
<boost/preprocessor/repetition.hpp>
<boost/preprocessor/arithmetic/sub.hpp>
<boost/preprocessor/punctuation/comma_if.hpp>
<boost/preprocessor/control/if.hpp>
<boost/preprocessor/facilities/empty.hpp>
<boost/preprocessor/facilities/identity.hpp>
)

set (std_pch
<iostream>
<vector>
<string>
<utility>
<function>
<array>
<tuple>
<type_traits>
<fstream>
<regex>
)



function(ph_boost target)
    cmake_parse_arguments(p "public;private;interface;no_pch" "" "" ${ARGN})
    if (${ARGC} EQUAL "1" OR p_public)
        set (t3 PUBLIC)
    elseif (p_private)
        set (t3 PRIVATE)
    elseif(p_interface)
        set (t3 INTERFACE)
    endif()
    cmake_language (EVAL CODE "
        target_link_libraries(${target} ${t3} ${Boost_LIBRARIES})
        target_include_directories (${target} ${t3} ${Boost_INCLUDE_DIRS})
    ")
    if (NOT p_no_pch)
        target_precompile_headers (${target} 
         PRIVATE 
            ${boost_preprocessor_pch}
        )
    endif ()
    
    
    
endfunction()
