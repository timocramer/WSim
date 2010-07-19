
/**
 *  \file   atmega128_digiIO.c
 *  \brief  Atmega128 MCU Digital IO ports
 *  \author Antoine Fraboulet
 *  \date   2007
 **/

#include <stdio.h>
#include <ctype.h>

#include "arch/common/hardware.h"
#include "atmega128.h"


/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void atmega128_digiIO_reset(void)
{
  /* after a reset the pin IO are switched to input mode */
  int i;
  for(i=0 ; i<7 ; i++)
    {
      //      DIGIIO_DIR(i) = 0;
      //      DIGIIO_SEL(i) = 0;
    }
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

int8_t atmega128_digiIO_mcu_read (uint16_t addr)
{
    uint8_t res = 0;
    uint8_t index = IO_ADDRESS_TO_IDX(addr);
    
    HW_DMSG_DIGI_IO("atmega128:dio: reading from MCU [%s:0x%02x]\n",
                    atmega128_debug_portname(addr),addr);

    res = DIGIIO_REGS(index);
    
    HW_DMSG_DIGI_IO("atmega128:dio: read from MCU [%s:0x%02x] = 0x%02x\n",
                    atmega128_debug_portname(addr),addr,res);
    
    return res;
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void atmega128_digiIO_mcu_write(uint16_t addr, int8_t UNUSED val)
{
    uint8_t index = IO_ADDRESS_TO_IDX(addr);
    
    HW_DMSG_DIGI_IO("atmega128:dio: write from MCU [%s:0x%02x] = 0x%02x\n",
                    atmega128_debug_portname(addr),addr,val & 0xff); 

    
    if (DIGIIO_IS_INPUT) // all PINX index addresses
    {
        ERROR("atmega128:dio: write on %s input register (read only)\n",
                atmega128_debug_portname(addr));
    }
    else
    {
        return; // TODO
    }
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

int atmega128_digiIO_dev_read (int UNUSED port_number, uint8_t UNUSED *val)
{
  return 0;
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void atmega128_digiIO_dev_write(int UNUSED port_number, uint8_t UNUSED val, uint8_t UNUSED bitmask)
{
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

int atmega128_digiIO_internal_dev_read (int UNUSED port_number, uint8_t UNUSED *val)
{
  return 0;
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void atmega128_digiIO_internal_dev_write(int UNUSED port_number, uint8_t UNUSED val, uint8_t UNUSED bitmask)
{
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void atmega128_digiIO_update_done(void)
{
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

int atmega128_digiIO_chkifg(void)
{
  return 0;
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

