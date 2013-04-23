/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is MPEG4IP.
 * 
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2000, 2001.  All Rights Reserved.
 * 
 * Contributor(s): 
 *              Bill May        wmay@cisco.com
 */
#define SEC_PER_DAY 86400
#define SEC_PER_HOUR 3600
#define SEC_PER_MINUTE 60

struct sdp_decode_info_ {
  int isSet;
  int isMem;
  const char *memptr;
  const char *filename;
  FILE *ifile;
};

void sdp_debug(int loglevel, const char *fmt, ...)

#ifndef _WIN32
     __attribute__((format(__printf__, 2, 3)))
#endif
     ;
