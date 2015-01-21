/* 
 * File:   adapters_common.h
 * Author: giulio
 *
 * Created on 11 gennaio 2015, 11.50
 */

#ifndef ADAPTERS_COMMON_H
#define	ADAPTERS_COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        MD_BOTH = -1,
        MD_DEVICE = 0,
        MD_CONTROLLER,
    } ad_mode_t;

    typedef enum {
        EO_CRLF = 0,
        EO_CR,
        EO_LF,
        EO_NONE
    } ad_eos_t;

#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTERS_COMMON_H */

