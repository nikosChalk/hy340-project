

#ifndef HY340_PROJECT_VIRTUAL_MACHINE_UTILITIES_H
#define HY340_PROJECT_VIRTUAL_MACHINE_UTILITIES_H

/**
 * Macro for calling member function pointers
 * @param object The object or object reference whose member function pointer will be called
 * @param ptrToMember Pointer to the member function to call
 *
 * example:
 * class A {
 *      void foo(int x);
 * };
 * A a;
 * call_member_func_ptr(a, &A::foo) (5);
 */
#define call_member_func_ptr(object, ptrToMember)  ((object).*(ptrToMember))

#endif //HY340_PROJECT_VIRTUAL_MACHINE_UTILITIES_H
