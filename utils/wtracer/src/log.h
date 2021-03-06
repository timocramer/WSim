
/**
 *  \file   log.h
 *  \brief  Simulator activity tracer
 *  \author Antoine Fraboulet
 *  \date   2006
 **/

#ifndef LOG_H
#define LOG_H

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void ERROR  (char* fmt, ...); 
void OUTPUT (char* fmt, ...);
void MESSAGE(char* fmt, ...);

#define DMSG(t,x...)            \
do {                            \
  if (t->debug)                 \
    MESSAGE(x);                 \
} while (0)

#define VMSG(t,x...)            \
do {                            \
  if (t->verbose)		\
    MESSAGE(x);                 \
} while (0)

#define DVMSG(t,x...)		\
do {                            \
  if (t->debug || t->verbose)	\
    MESSAGE(x);                 \
} while (0)

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

#endif
