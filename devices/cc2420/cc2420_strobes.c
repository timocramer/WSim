
/**
 *  \file   cc2420_strobes.c
 *  \brief  CC2420 strobe commands
 *  \author Nicolas Boulicault
 *  \date   2007
 **/

/*
 *  cc2420_strobes.c
 *  
 *
 *  Created by Nicolas Boulicault on 05/06/07.
 *  Copyright 2007 __WorldSens__. All rights reserved.
 *
 */

#include "cc2420_internals.h"
#include "cc2420_macros.h"
#include "cc2420_strobes.h"
#include "cc2420_debug.h"
#include "cc2420_rx.h"
#include "cc2420_registers.h"
#include "cc2420_mux.h"


/***************************************************/
/***************************************************/
/***************************************************/

/** 
 * for debug, strobe command names
 */
/*
char * CC2420_STROBE_NAMES[] = 
{
  "SNOP", 
  "SXOSCON", 
  "STXCAL",
  "SRXON",
  "STXON",
  "STXONCCA",
  "SRFOFF",
  "SXOSCOFF",
  "SFLUSHRX",
  "SFLUSHTX",
  "SACK",
  "SACKPEND",
  "SRXDEC",
  "STXENC",
  "SAES"
};
*/

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in power off mode [0]
 */

void cc2420_strobe_state_power_down(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCON :
      CC2420_DBG_STROBE("cc2420:strobe:power_down: SXOSCON\n");
      CC2420_XOSC_STARTING_ENTER(cc2420);
      break;

    default :
      CC2420_DBG_STROBE("cc2420:strobe:power_down: invalid strobe command %d in power down state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in idle state [1]
 */

void cc2420_strobe_state_idle(struct _cc2420_t * cc2420) 
{
    switch (cc2420->SPI_addr) 
      {
      case CC2420_STROBE_SXOSCOFF :
	CC2420_DBG_STROBE("cc2420:strobe:idle: SXOSCOFF\n");
	CC2420_POWER_DOWN_ENTER(cc2420);
	break;
      case CC2420_STROBE_STXON :
	CC2420_DBG_STROBE("cc2420:strobe:idle: STXON\n");
	CC2420_TX_CALIBRATE_ENTER(cc2420);
	break;
      case CC2420_STROBE_SRXON :
	CC2420_DBG_STROBE("cc2420:strobe:idle: SRXON\n");
	CC2420_RX_CALIBRATE_ENTER(cc2420);
	break;
      case CC2420_STROBE_SFLUSHTX :
	CC2420_DBG_STROBE("cc2420:strobe:idle: SFLUSHTX\n");
	cc2420->tx_underflow = 0;
	cc2420->tx_fifo_len = 0;
	cc2420->tx_available_bytes = 0;
	cc2420->tx_frame_completed = 0;
	break;
      case CC2420_STROBE_SFLUSHRX :
	CC2420_DBG_STROBE("cc2420:strobe:idle: SFLUSHRX\n");
	cc2420->rx_overflow = 0;
	cc2420->rx_data_bytes = 0;
	cc2420->rx_len = 0;
	cc2420->rx_fifo_read = 0;	
	cc2420->rx_fifo_write = 0;
	cc2420->rx_frame_start = 0;
	cc2420->rx_frame_end = 0;
	cc2420->rx_first_data_byte = -1;
	cc2420->nb_rx_frames = 0;
	cc2420->FIFOP_pin = 0x00;
	cc2420->FIFOP_set = 1;
	cc2420->FIFO_pin  = 0x00;
	cc2420->FIFO_set  = 1;
	break;
      case CC2420_STROBE_SRFOFF :
	CC2420_DBG_STROBE("cc2420:strobe:idle: SRFOFF, allowed in idle mode but not usefull!\n");
	break;

      default :
	CC2420_DBG_STROBE("cc2420:strobe:idle: invalid strobe command %d in idle state\n",
		     cc2420->SPI_addr);
	return;
      }
}


/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in tx_calibrate state [32]
 */

void cc2420_strobe_state_tx_calibrate(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      CC2420_DBG_STROBE("cc2420:strobe:calibrate: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      CC2420_DBG_STROBE("cc2420:strobe:calibrate: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      CC2420_DBG_STROBE("cc2420:strobe:calibrate: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      CC2420_DBG_STROBE("cc2420:strobe:calibrate: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:calibrate: invalid strobe command %d in tx_calibrate state\n",
		   cc2420->SPI_addr);
      return;
    }
}


/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in tx_preamble state [34-35-36]
 */

void cc2420_strobe_state_tx_preamble(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SXOSCOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_preamble: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRFOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_preamble: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHTX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_preamble: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHRX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_preamble: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      CC2420_RX_SFD_SEARCH_ENTER(cc2420);
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:tx_preamble: invalid strobe command %d in tx_preamble state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in tx_frame state [34-35-36]
 */

void cc2420_strobe_state_tx_frame(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SXOSCOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_frame: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRFOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_frame: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHTX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_frame: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHRX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_frame: SRFORX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:tx_frame: invalid strobe command %d in tx_frame state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in tx_underflow state [34-35-36]
 */

void cc2420_strobe_state_tx_underflow(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      CC2420_DBG_STROBE("cc2420:strobe:tx_underflow: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      CC2420_DBG_STROBE("cc2420:strobe:tx_underflow: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      CC2420_DBG_STROBE("cc2420:strobe:tx_underflow: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      CC2420_DBG_STROBE("cc2420:strobe:tx_underflow: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:tx_underflow: invalid strobe command %d in tx_underflow state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in rx_calibrate state [2 - 40]
 */

void cc2420_strobe_state_rx_calibrate(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      CC2420_DBG_STROBE("cc2420:strobe:rx_calibrate: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      CC2420_DBG_STROBE("cc2420:strobe:rx_calibrate: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_STXONCCA :
      CC2420_DBG_STROBE("cc2420:strobe:rx_calibrate: STXONCCA\n");
      if (!cc2420_check_cca(cc2420))
	break;
      CC2420_TX_CALIBRATE_ENTER(cc2420);
      break;
    case CC2420_STROBE_STXON :
      CC2420_DBG_STROBE("cc2420:strobe:rx_calibrate: STXON\n");
      CC2420_TX_CALIBRATE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      CC2420_DBG_STROBE("cc2420:strobe:rx_calibrate: STFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      CC2420_DBG_STROBE("cc2420:strobe:rx_calibrate: STFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:rx_calibrate: invalid strobe command %d in rx_calibrate state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in rx_sfd_search state [3-4-5-6]
 */

void cc2420_strobe_state_rx_sfd_search(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SXOSCOFF");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRFOFF");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHTX");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHRX");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    case CC2420_STROBE_STXONCCA :
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: STXONCCA\n");
      if (!cc2420_check_cca(cc2420))
	break;
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe STXONCCA");
      CC2420_TX_CALIBRATE_ENTER(cc2420);
      break;
    case CC2420_STROBE_STXON :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe STXON");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: STXON\n");
      CC2420_TX_CALIBRATE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRXON :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRXON");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: SRXON\n");
      CC2420_RX_SFD_SEARCH_ENTER(cc2420);
      break;
    case CC2420_STROBE_SACK :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SACK");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: SACK\n");
      cc2420->tx_frame_pending = 0;
      CC2420_TX_ACK_CALIBRATE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SACKPEND :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SACKPEND");
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: SACKPEND\n");
      cc2420->tx_frame_pending = 1;
      CC2420_TX_ACK_CALIBRATE_ENTER(cc2420);
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:rx_sfd_search: invalid strobe command %d in rx_sfd_search state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in rx_frame state [3-4-5-6]
 */

void cc2420_strobe_state_rx_frame(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SXOSCOFF");
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRFOFF");
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHTX");
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHRX");
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      CC2420_RX_SFD_SEARCH_ENTER(cc2420);  /* noticed on hardware */
      break;
    case CC2420_STROBE_STXONCCA :
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: STXONCCA\n");
      if (!cc2420_check_cca(cc2420))
	break;
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe STXONCCA");
      CC2420_TX_CALIBRATE_ENTER(cc2420);
    case CC2420_STROBE_STXON :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe STXON");
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: STXON\n");
      CC2420_TX_CALIBRATE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRXON :
      logpkt_rx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRXON");
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: SRXON\n");
      CC2420_RX_SFD_SEARCH_ENTER(cc2420); /* verified on hardware */
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:rx_frame: invalid strobe command %d in rx_frame state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in tx_ack_calibrate state [48]
 */

void cc2420_strobe_state_tx_ack_calibrate(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_calibrate: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_calibrate: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_calibrate: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_calibrate: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    case CC2420_STROBE_STXONCCA :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_calibrate: STXONCCA\n");
      if (!cc2420_check_cca(cc2420))
	break;
      CC2420_TX_CALIBRATE_ENTER(cc2420);
    case CC2420_STROBE_STXON :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_calibrate: STXON\n");
      CC2420_TX_CALIBRATE_ENTER(cc2420);
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_calibrate: invalid strobe command %d in tx_ack_calibrate state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in tx_ack_preamble state [49-50-51]
 */

void cc2420_strobe_state_tx_ack_preamble(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SXOSCOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_preamble: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRFOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_preamble: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHTX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_preamble: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHRX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_preamble: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack_preamble: invalid strobe command %d in tx_ack_preamble state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in tx_ack state [49-50-51]
 */

void cc2420_strobe_state_tx_ack(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SXOSCOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SRFOFF");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHTX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      logpkt_tx_abort_pkt(cc2420->worldsens_radio_id, "strobe SFLUSHRX");
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:tx_ack: invalid strobe command %d in tx_ack state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in rx_wait state [14]
 */

void cc2420_strobe_state_rx_wait(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      CC2420_DBG_STROBE("cc2420:strobe:rx_wait: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      CC2420_DBG_STROBE("cc2420:strobe:rx_wait: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      CC2420_DBG_STROBE("cc2420:strobe:rx_wait: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      CC2420_DBG_STROBE("cc2420:strobe:rx_wait: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:rx_wait: invalid strobe command %d in rx_wait state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * deal with strobes in rx_overflow state [14]
 */

void cc2420_strobe_state_rx_overflow(struct _cc2420_t * cc2420) 
{
  switch (cc2420->SPI_addr) 
    {
    case CC2420_STROBE_SXOSCOFF :
      CC2420_DBG_STROBE("cc2420:strobe:rx_overflow: SXOSCOFF\n");
      CC2420_POWER_DOWN_ENTER(cc2420);
      break;
    case CC2420_STROBE_SRFOFF :
      CC2420_DBG_STROBE("cc2420:strobe:rx_overflow: SRFOFF\n");
      CC2420_IDLE_ENTER(cc2420);
      break;
    case CC2420_STROBE_SFLUSHTX :
      CC2420_DBG_STROBE("cc2420:strobe:rx_overflow: SFLUSHTX\n");
      cc2420->tx_underflow = 0;
      cc2420->tx_fifo_len = 0;
      cc2420->tx_available_bytes = 0;
      cc2420->tx_frame_completed = 0;
      break;
    case CC2420_STROBE_SFLUSHRX :
      CC2420_DBG_STROBE("cc2420:strobe:rx_overflow: SFLUSHRX\n");
      cc2420->rx_overflow = 0;
      cc2420->rx_data_bytes = 0;
      cc2420->rx_len = 0;
      cc2420->rx_fifo_read = 0;	
      cc2420->rx_fifo_write = 0;
      cc2420->rx_frame_start = 0;
      cc2420->rx_frame_end = 0;
      cc2420->rx_first_data_byte = -1;
      cc2420->nb_rx_frames = 0;
      cc2420->FIFOP_pin = 0x00;
      cc2420->FIFOP_set = 1;
      cc2420->FIFO_pin  = 0x00;
      cc2420->FIFO_set  = 1;
      CC2420_RX_SFD_SEARCH_ENTER(cc2420);
      break;
    default :
      CC2420_DBG_STROBE("cc2420:strobe:rx_overflow: invalid strobe command %d in rx_overflow state\n",
		   cc2420->SPI_addr);
      return;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/

/**
 * issue a command strobe
 */

char* cc2420_strobe_str(int strobe)
{
  switch (strobe) 
    {
    case CC2420_STROBE_SNOP    : return "SNOP";     /* no op                                        */
    case CC2420_STROBE_SXOSCON : return "SXOSCON";  /* turn on oscillator                           */
    case CC2420_STROBE_STXCAL  : return "STXCAL";   /* enable and calibrate freq synthetizer for TX */
    case CC2420_STROBE_SRXON   : return "SRXON";    /* enable RX                                    */
    case CC2420_STROBE_STXON   : return "STXON";    /* enable TX                                    */
    case CC2420_STROBE_STXONCCA: return "STXONCCA"; /* if channel clear, enable cal and TX          */
    case CC2420_STROBE_SRFOFF  : return "SRFOFF";   /* disable RT/TX and freq synth                 */
    case CC2420_STROBE_SXOSCOFF: return "SXOSCOFF"; /* turn off crystal osc and RF                  */
    case CC2420_STROBE_SFLUSHRX: return "SFLUSHRX"; /* flush RX FIFO                                */
    case CC2420_STROBE_SFLUSHTX: return "SFLUSHTX"; /* flush TX FIFO                                */
    case CC2420_STROBE_SACK    : return "SACK";     /* send ACK with pending field clear            */
    case CC2420_STROBE_SACKPEND: return "SACKPEND"; /* send ACK with pending field set              */
    case CC2420_STROBE_SRXDEC  : return "SRXDEC";   /* start RX FIFO inline decryption              */
    case CC2420_STROBE_STXENC  : return "STXENC";   /* start TX FIFO inline encryption              */
    case CC2420_STROBE_SAES    : return "SAES";     /* AES standalone encryption                    */
    default: break;
    }
  return "Unknown";
}

void cc2420_strobe_command(struct _cc2420_t * cc2420) 
{
  tracer_event_record(TRACER_CC2420_STROBE, cc2420->SPI_addr);

  CC2420_DBG_STROBE("cc2420:strobe: starting Strobe %02d - %s\n", cc2420->SPI_addr, 
                    cc2420_strobe_str(cc2420->SPI_addr));
  if (cc2420->SPI_addr == CC2420_STROBE_SNOP) 
    {
      /* even for a noop do nothing in those states */
      if ( (cc2420->fsm_state == CC2420_STATE_VREG_OFF) ||
	   (cc2420->fsm_state == CC2420_STATE_VREG_STARTING) ||
	   (cc2420->fsm_state == CC2420_STATE_RESET) ) 
	{
	  CC2420_DBG_STROBE("cc2420:strobe:noop: NOOP not possible in state %d\n", cc2420->fsm_state);
	  return ;
	}
      return ;
    }

  switch (cc2420->fsm_state) 
    {
    case CC2420_STATE_VREG_OFF :
    case CC2420_STATE_VREG_STARTING :
    case CC2420_STATE_RESET :
      CC2420_DBG_STROBE("cc2420:strobe:command: VREG_OFF, VREG_STARTING, RESET: invalid states for strobe commands\n");
      break;
    case CC2420_STATE_POWER_DOWN :
      cc2420_strobe_state_power_down(cc2420);
      break;
    case CC2420_STATE_IDLE :
      cc2420_strobe_state_idle(cc2420);
      break;
    case CC2420_STATE_TX_CALIBRATE :
      cc2420_strobe_state_tx_calibrate(cc2420);
      break;
    case CC2420_STATE_TX_PREAMBLE :
      cc2420_strobe_state_tx_preamble(cc2420);
      break;
    case CC2420_STATE_TX_FRAME :
      cc2420_strobe_state_tx_frame(cc2420);
      break;
    case CC2420_STATE_TX_UNDERFLOW :
      cc2420_strobe_state_tx_underflow(cc2420);
      break;
    case CC2420_STATE_RX_SFD_SEARCH :
      cc2420_strobe_state_rx_sfd_search(cc2420);
      break;
    case CC2420_STATE_RX_FRAME :
      cc2420_strobe_state_rx_frame(cc2420);
      break;
    case CC2420_STATE_TX_ACK_CALIBRATE :
      cc2420_strobe_state_tx_ack_calibrate(cc2420);
      break;
    case CC2420_STATE_TX_ACK_PREAMBLE :
      cc2420_strobe_state_tx_ack_preamble(cc2420);
      break;
    case CC2420_STATE_TX_ACK :
      cc2420_strobe_state_tx_ack(cc2420);
      break;
    case CC2420_STATE_RX_WAIT :
      cc2420_strobe_state_rx_wait(cc2420);
      break;
    case CC2420_STATE_RX_OVERFLOW :
      cc2420_strobe_state_rx_overflow(cc2420);
      break;
    case CC2420_STATE_RX_CALIBRATE :
      cc2420_strobe_state_rx_calibrate(cc2420);
      break;
    case CC2420_STATE_XOSC_STARTING:
      CC2420_DBG_STROBE("cc2420:strobe: XOSC not yet stable, strobe %d ignored\n");
      break;
    default:
      CC2420_DBG_STROBE("cc2420:strobe: state %d, strobe %d not implemented yet\n",
                        cc2420->fsm_state,cc2420->SPI_addr);
      break;
    }
}

/***************************************************/
/***************************************************/
/***************************************************/
