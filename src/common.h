// common.h: common constants
// license information in LICENSE
#ifndef __PVM_COMMON_H__
#define __PVM_COMMON_H__
#define PVM_0_1_MAJOR 0x27
#define PVM_0_1_MINOR 0x03
#define PVM_0_1_PATCH 0x10

#define PVM_MAJOR PVM_0_1_MAJOR
#define PVM_MINOR PVM_0_1_MINOR
#define PVM_PATCH PVM_0_1_PATCH

inline int pvm_get_patch ()
{
    return PVM_PATCH;
}

inline int pvm_get_minor ()
{
    return PVM_MINOR;
}

inline int pvm_get_major ()
{
    return PVM_MAJOR;
}

#endif /* __PVM_COMMON_H__ */