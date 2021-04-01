// common.h: common constants
// license information in LICENSE
#ifndef __PVM_COMMON_H__
#define __PVM_COMMON_H__
#define PVM_0_1_MAJOR 0x00
#define PVM_0_1_MINOR 0x00
#define PVM_0_1_PATCH 0x00

#define PVM_MAJOR PVM_0_1_MAJOR
#define PVM_MINOR PVM_0_1_MINOR
#define PVM_PATCH PVM_0_1_PATCH

static inline int pvm_get_patch ()
{
    return PVM_PATCH;
}

static inline int pvm_get_minor ()
{
    return PVM_MINOR;
}

static inline int pvm_get_major ()
{
    return PVM_MAJOR;
}

#endif /* __PVM_COMMON_H__ */