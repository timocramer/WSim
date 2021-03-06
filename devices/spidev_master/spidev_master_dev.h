
/**
 *  \file   spidev_master_dev.h
 *  \brief  SPI master device example
 *  \author Loic Lemaitre
 *  \date   2009
 **/

#ifndef SPIDEV_MASTER_DEVICES_H
#define SPIDEV_MASTER_DEVICES_H

#define SPIDEV_W_SHIFT  8 /* write protect */
#define SPIDEV_S_SHIFT  9 /* select        */
#define SPIDEV_M_SHIFT 10 /* SPI mode      */
#define SPIDEV_C_SHIFT 11 /* clock         */

#define SPIDEV_D  0x00ff                 /** data 8 bits           **/
#define SPIDEV_W  (1 << SPIDEV_W_SHIFT)  /** write protect negated **/
#define SPIDEV_S  (1 << SPIDEV_S_SHIFT)  /** chip select negated   **/
#define SPIDEV_M  (1 << SPIDEV_M_SHIFT)  /** spi mode              **/
#define SPIDEV_C  (1 << SPIDEV_C_SHIFT)  /** clock                 **/

int  spidev_add_options   (int dev_num, int dev_id, const char *name);
int  spidev_device_size   ();
int  spidev_device_create (int dev_num, int dev_id);

#endif
