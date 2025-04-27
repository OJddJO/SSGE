#ifndef SSGEDECL
    #ifdef DLL_EXPORT
        #define SSGEDECL __declspec(dllexport)
    #else
        #define SSGEDECL
    #endif
#endif // SSGEDECL