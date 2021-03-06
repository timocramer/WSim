/**
 *  \file   msp430_flash.h
 *  \brief  MSP430 Flash controller
 *  \author Antoine Fraboulet
 *  \date   2006
 **/

#ifndef MSP430_FLASH_H
#define MSP430_FLASH_H
#if defined(__msp430_have_flash)

#if !defined(FLASHCTL_BASE)
#define FLASHCTL_BASE   0x128
#endif

#define FLASHCTL_START  FLASHCTL_BASE
#define FLASHCTL_END    (FLASHCTL_BASE + 0x04)

enum flash_addr_t {
  FLASH_FCTL1     = FLASHCTL_BASE,
  FLASH_FCTL2     = (FLASHCTL_BASE + 0x02),
  FLASH_FCTL3     = (FLASHCTL_BASE + 0x04)
};

#if defined(WORDS_BIGENDIAN)
struct __attribute__ ((packed)) fctl1_t {
  uint16_t
    fwkey:8,
    blkwrt:1,
    wrt:1,
    res1:1,
    res2:1,
    res3:1,
    MERAS:1,
    ERASE:1,
    res4:1;
};
#else
struct __attribute__ ((packed)) fctl1_t {
  uint16_t
    res4:1,
    ERASE:1,
    MERAS:1,
    res3:1,
    res2:1,
    res1:1,
    wrt:1,
    blkwrt:1,
    fwkey:8;
};
#endif

#if defined(WORDS_BIGENDIAN)
struct __attribute__ ((packed)) fctl2_t {
  uint16_t
    fwkeyx:8,
    fssselx:2,
    fnx:6;
};
#else
struct __attribute__ ((packed)) fctl2_t {
  uint16_t
    fnx:6,
    fsselx:2,
    fwkeyx:8;
};
#endif

#if defined(WORDS_BIGENDIAN)
struct __attribute__ ((packed)) fctl3_t {
  uint16_t
    fwkeyx:8,
    res1:1,
    res2:1,
    EMEX:1,
    lock:1,
    wait:1,
    accvifg:1,
    keyv:1,
    busy:1;
};
#else
struct __attribute__ ((packed)) fctl3_t {
  uint16_t
    busy:1,
    keyv:1,
    accvifg:1,
    wait:1,
    lock:1,
    EMEX:1,
    res2:1,
    res1:1,
    fwkeyx:8;
};
#endif

struct msp430_flash_t {
  union {
    struct fctl1_t      b;
    uint16_t            s;
  } fctl1;
  union {
    struct fctl2_t      b;
    uint16_t            s;
  } fctl2;
  union {
    struct fctl3_t      b;
    uint16_t            s;
  } fctl3;

  uint32_t ticks_divider;
  uint32_t flash_ticks_left;
  uint32_t flash_write_fst;
};

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void    msp430_flash_create      (void);
void    msp430_flash_reset       (void);

extern void (*msp430_flash_update_ptr)(void);
#define msp430_flash_update()			\
  do {						\
    if (msp430_flash_update_ptr != NULL)	\
      msp430_flash_update_ptr();		\
  } while (0)

int16_t msp430_flash_read        (uint16_t addr);
void    msp430_flash_write       (uint16_t addr, int16_t val);
int     msp430_flash_chkifg      (void);
void    msp430_flash_start_erase (uint16_t addr, int size, uint32_t val);

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
#else
#define msp430_flash_create() do { } while (0)
#define msp430_flash_reset()  do { } while (0)
#endif /* have_flash */
#endif
