/**************************************************************************

This software module was originally developed by

Mikko Suonio (Nokia)

in the course of development of the MPEG-2 NBC/MPEG-4 Audio standard
ISO/IEC 13818-7, 14496-1,2 and 3. This software module is an
implementation of a part of one or more MPEG-2 NBC/MPEG-4 Audio tools
as specified by the MPEG-2 NBC/MPEG-4 Audio standard. ISO/IEC gives
users of the MPEG-2 NBC/MPEG-4 Audio standards free license to this
software module or modifications thereof for use in hardware or
software products claiming conformance to the MPEG-2 NBC/ MPEG-4 Audio
standards. Those intending to use this software module in hardware or
software products are advised that this use may infringe existing
patents. The original developer of this software module and his/her
company, the subsequent editors and their companies, and ISO/IEC have
no liability for use of this software module or modifications thereof
in an implementation. Copyright is not released for non MPEG-2
NBC/MPEG-4 Audio conforming products. The original developer retains
full right to use the code for his/her own purpose, assign or donate
the code to a third party and to inhibit third party from using the
code for non MPEG-2 NBC/MPEG-4 Audio conforming products. This
copyright notice must be included in all copies or derivative works.

Copyright (c) 1997.

***************************************************************************/
/*
 * $Id: nok_ltp_common_internal.h,v 1.5 2002/01/11 00:55:17 wmaycisco Exp $
 */

#ifndef _NOK_LTP_COMMON_INTERNAL_H
#define _NOK_LTP_COMMON_INTERNAL_H


/*
  Purpose:      Number of LTP coefficients. */
#define LPC 1

/*
  Purpose:      Maximum LTP lag.  */
#define MAX_LTP_DELAY 2048

/*
  Purpose:  Length of the bitstream element ltp_data_present.  */
#define LEN_LTP_DATA_PRESENT 1

/*
  Purpose:  Length of the bitstream element ltp_lag.  */
#define LEN_LTP_LAG 11

/*
  Purpose:  Length of the bitstream element ltp_coef.  */
#define LEN_LTP_COEF 3

/*
  Purpose:  Length of the bitstream element ltp_short_used.  */
#define LEN_LTP_SHORT_USED 1

/*
  Purpose:  Length of the bitstream element ltp_short_lag_present.  */
#define LEN_LTP_SHORT_LAG_PRESENT 1

/*
  Purpose:  Length of the bitstream element ltp_short_lag.  */
#define LEN_LTP_SHORT_LAG 5

/*
  Purpose:  Offset of the lags written in the bitstream.  */
#define NOK_LTP_LAG_OFFSET 16

/*
  Purpose:  Length of the bitstream element ltp_long_used.  */
#define LEN_LTP_LONG_USED 1

/*
  Purpose:  Upper limit for the number of scalefactor bands
        which can use lt prediction with long windows.
  Explanation:  Bands 0..NOK_MAX_LT_PRED_SFB-1 can use lt prediction.  */
#define NOK_MAX_LT_PRED_LONG_SFB 40

/*
  Purpose:  Upper limit for the number of scalefactor bands
        which can use lt prediction with short windows.
  Explanation:  Bands 0..NOK_MAX_LT_PRED_SFB-1 can use lt prediction.  */
#define NOK_MAX_LT_PRED_SHORT_SFB 8

/*
   Purpose:      Buffer offset to maintain block alignment.
   Explanation:  This is only used for a short window sequence.  */
#define SHORT_SQ_OFFSET (BLOCK_LEN_LONG-(BLOCK_LEN_SHORT*4+BLOCK_LEN_SHORT/2))

/*
  Purpose:  Number of codes for LTP weight. */
#define CODESIZE 8

/*
   Purpose:      Float type for external data.
   Explanation:  - */
typedef Float float_ext;

/*
  Purpose:  Codebook for LTP weight coefficients.  */
static Float codebook[CODESIZE] =
{
  0.570829f,
  0.696616f,
  0.813004f,
  0.911304f,
  0.984900f,
  1.067894f,
  1.194601f,
  1.369533f
};

#endif /* _NOK_LTP_COMMON_INTERNAL_H */
