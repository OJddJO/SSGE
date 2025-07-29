// This file should not be included
// It is used internally by the SSGE library

#ifndef __SSGE_ERROR_H__
#define __SSGE_ERROR_H__

#include <stdio.h>

#define SSGE_Error(msg) {\
    fprintf(stderr, "[SSGE][%s] " msg "\n", __func__);\
    exit(1);\
}

#define SSGE_ErrorEx(fmsg, arg) {\
    fprintf(stderr, "[SSGE][%s] " fmsg "\n", __func__, arg);\
    exit(1);\
}

#define SSGE_ErrorEx2(fmsg, arg1, arg2) {\
    fprintf(stderr, "[SSGE][%s] " fmsg "\n", __func__, arg1, arg2);\
    exit(1);\
}
#endif // __SSGE_ERROR_H__