#include <cassert>

enum ErrorCode_t
{
    kNoErr,
    kFunctionInvalidArgsErr,
};

ErrorCode_t func1 (int *pPtr)
{
    if (pPtr == NULL)
        return kFunctionInvalidArgsErr;
    return kNoErr;
}

void func2 (int *pPtr)
{
    if (pPtr == NULL)
        throw "Exception: func2 argument pPtr is NULL";
}

void func3 (int *pPtr)
{
    assert (pPtr);
}
