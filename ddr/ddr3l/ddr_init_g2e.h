#pragma once
#include <stdint-gcc.h>

#ifndef	__DDR_INIT_G2E_
#define	__DDR_INIT_G2E_

#define RZG2E_DDR_VERSION    "rev.0.12"

#ifdef ddr_qos_init_setting
   #define REFRESH_RATE  3900               // Average periodic refresh interval[ns]. Support 3900,7800
#else
   #if RZG2_REF_INT == 0
      #define REFRESH_RATE  3900
   #elif RZG2_REF_INT == 1
      #define REFRESH_RATE  7800
   #else
      #define REFRESH_RATE  3900
   #endif
#endif

extern int32_t InitDram(void);
#define INITDRAM_OK (0)
#define INITDRAM_NG (0xffffffff)
#define INITDRAM_ERR_I (0xffffffff)
#define INITDRAM_ERR_O (0xfffffffe)
#define INITDRAM_ERR_T (0xfffffff0)

#endif /* __DDR_INIT_G2E_ */
