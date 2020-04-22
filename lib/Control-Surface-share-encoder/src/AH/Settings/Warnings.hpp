#pragma once

#if defined(__GNUC__) && !defined(__clang__)

#define AH_DIAGNOSTIC_WERROR()                                                 \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic error \"-Wall\"")   \
        _Pragma("GCC diagnostic error \"-Wextra\"")                            \
            _Pragma("GCC diagnostic ignored \"-Wc++0x-compat\"")
#define AH_DIAGNOSTIC_POP() _Pragma("GCC diagnostic pop")
#define AH_DIAGNOSTIC_EXTERNAL_HEADER()                                        \
    _Pragma("GCC diagnostic ignored \"-Wsuggest-override\"")                   \
        _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")

#else

#define AH_DIAGNOSTIC_WERROR()
#define AH_DIAGNOSTIC_POP()
#define AH_DIAGNOSTIC_EXTERNAL_HEADER()

#endif