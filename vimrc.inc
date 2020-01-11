let &path = "@VIMRC_INCLUDE_DIRS@,/home/olivier/include*," . &path 
let &makeprg="@CMAKE_MAKE_PROGRAM@ -C @PROJECT_BINARY_DIR@ 2>&1"

"syn keyword cppExceptions        do_try or_terminate or_rethrow or_return throw_error rethrow_error value_or_throw value_or_return define_or_throw define_or_return assign_or_throw assign_or_return
"syn keyword cppType     int128_t uint128_t float128_t
"syn keyword cppStructure        SUBSUMED_BY REQUIRES_RET 
"syn keyword cppStructure        REQUIRES_T REQUIRES_T1 REQUIRES_T2 REQUIRES_T3 REQUIRES_T4 REQUIRES_T5 REQUIRES_T6 REQUIRES_T7 
"syn keyword cppStructure        DECL_REQUIRES_T DECL_REQUIRES_T1 DECL_REQUIRES_T2 DECL_REQUIRES_T3 DECL_REQUIRES_T4 DECL_REQUIRES_T5 DECL_REQUIRES_T6 DECL_REQUIRES_T7 
"syn keyword cppStructure        DECL_REQUIRES_PA RDECL_REQUIRES_PA REQUIRES_C REQUIRES_PA 
"syn keyword cppStructure        REQUIRES_T_M REQUIRES_T1_M REQUIRES_T2_M REQUIRES_T3_M REQUIRES_T4_M REQUIRES_T5_M REQUIRES_T6_M REQUIRES_T7_M 
"syn keyword cppStructure        DECL_REQUIRES_T_M DECL_REQUIRES_T1_M DECL_REQUIRES_T2_M DECL_REQUIRES_T3_M DECL_REQUIRES_T4_M DECL_REQUIRES_T5_M DECL_REQUIRES_T6_M DECL_REQUIRES_T7_M 
"syn keyword cppStructure        DECL_REQUIRES_PA_M RDECL_REQUIRES_PA_M REQUIRES_C_M REQUIRES_PA_M
"
"syn keyword cppStructure        DECLARE_ABSTRACTS 

let &errorformat=&errorformat . ",%EE:%m[%.%./%f:%l],%EF:%m[%.%./%f:%l],%IS:%m[%.%./%f:%l],%IA:%m[%.%./%f:%l],%WW:%m[%.%./%f:%l],%I%m[%.%./%f:%l]"
let &errorformat="%E%f:%l:%c: error:%m,%W%f:%l:%c: warning:%m," . &errorformat
