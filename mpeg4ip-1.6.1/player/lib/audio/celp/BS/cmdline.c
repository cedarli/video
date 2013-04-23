/**********************************************************************
MPEG-4 Audio VM
Command line module



This software module was originally developed by

Charalampos Ferkidis (University of Hannover / ACTS-MoMuSys)
Heiko Purnhagen (University of Hannover / ACTS-MoMuSys)
partially based on a concept by FhG IIS, Erlangen

and edited by

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

Copyright (c) 1996.



Source file: cmdline.c

$Id: cmdline.c,v 1.1 2002/05/13 18:57:42 wmaycisco Exp $

Required modules:
common.o		common module

Authors:
      partially based on a concept by FHG <iis.fhg.de>
CF    Charalampos Ferekidis, Uni Hannover <ferekidi@tnt.uni-hannover.de>
HP    Heiko Purnhagen, Uni Hannover <purnhage@tnt.uni-hannover.de>

Changes:
28-may-96   CF    added functions for parsing of init files and strings
05-jun-96   HP    minor changes
                  moved ErrorMsg() to seperat module
06-jun-96   HP    setDefault added to CmdLineEval()
07-jun-96   HP    use CommonProgName(), CommonWarning(), CommonExit()
10-jun-96   HP    ...
13-jun-96   HP    changed ComposeFileName()
14-jun-96   HP    fixed bug in ComposeFileName()
19-jun-96   HP    changed ComposeFileName()
                  fixed bug in CmdLineParseString()
20-jun-96   HP    added NOTE re CmdLineParseString()
08-aug-96   HP    changed string handling in CmdLineParseString()
                  changed handling of variable length argument list
                  added CmdLineEvalFree(), CmdLineParseFree()
09-aug-96   HP    ...
14-aug-96   HP    fixed minor bug in CmdLineHelp(), added debug code
26-aug-96   HP    CVS
27-aug-96   HP    fixed bug in CmdLineEvalFree()
10-dec-96   HP    changed dflt. extension concat in ComposeFileName()
07-apr-97   HP    "-" support in CmdLineEval() and ComposeFileName()
04-may-98   HP/BT unsigned in ComposeFileName()
**********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline.h"		/* command line module */
#include "common_m4a.h"		/* common module */


/* ---------- declarations ---------- */

#define MAX_HELP_SIZE 4096	/* max length of parameter/switch help */
#define MAX_TOKEN_NUM 4096	/* max num of tokens generated by parser */
#define MAX_LINE_SIZE 1024	/* max length of line in parsed init file */
#define MAX_FILE_SIZE 65536	/* max length of parsed init file */


/* ---------- variables ---------- */

static int CLdebugLevel = 0;	/* debug level */


/* ---------- internal functions ---------- */


/* StripPath() */
/* Strip path from file name. */

static char *StripPath (
  char *fileName)	/* in: file name */
			/* returns: file name without path */
{
  char *tmpName;

  do {
    tmpName = strchr(fileName,':');
    if (tmpName==NULL)
      tmpName=strchr(fileName,'\\');
    if (tmpName==NULL)
      tmpName=strchr(fileName,'/');
    if (tmpName!=NULL)
      fileName = tmpName+1;
  } while (tmpName!=NULL);
  return fileName;
}


/* ---------- functions ---------- */


/* CmdLineInit() */
/* Init command line module. */

void CmdLineInit (
  int debugLevel)		/* in: debug level */
				/*     0=off  1=basic  2=full */
{
  CLdebugLevel = debugLevel;
  if (CLdebugLevel >= 1)
    printf("CmdLineInit: debugLevel=%d\n",CLdebugLevel);
}


/* CmdLineEval() */
/* Evaluate parameters and switches in argv[]. */
/* Command line mode (progNamePtr!=NULL): */
/*   Evaluate command line in argv[] from main() and extract program name */
/*   from argv[0]. Switches are identified by preceding '-' in the */
/*   command line. */
/* Token list mode (progNamePtr==NULL): */
/*   Evaluate token list in argv[] (as generated by CmdLineParseString() or */
/*   CmdLineParseFile()). Switches are identified by preceding '-' if */
/*   paraList!=NULL. Switches don't have a preceding '-' if paraList==NULL. */

int CmdLineEval (
  int argc,			/* in: num command line args */
  char *argv[],			/* in: command line args */
  CmdLinePara *paraList,	/* in: parameter info list */
				/*     or NULL */
  CmdLineSwitch *switchList,	/* in: switch info list */
				/*     or NULL */
  int setDefault,		/* in: 0 = leave switch used flags and args */
				/*         unchanged */
				/*     1 = init switch used flags and args */
				/*         with defaultValue */
  char **progNamePtr)		/* out: program name */
				/*      or NULL */
				/* returns: */
				/*  0=OK  1=help switch  2=error */
{
  char *progName;
  char *tmpProgName;
  int i;
  int minusFlag;
  char *minusChar;
  CmdLinePara *paraPtr;
  CmdLineSwitch *switchPtr;
  int tmpVarArgIdx[MAX_TOKEN_NUM];
  int count;  
  int *varArgIdx;

  /* extract program name from argv[0] if command line mode */
  if (progNamePtr != NULL) {
    progName = StripPath(argv[0]);
    if ((tmpProgName=strchr(progName,'.'))!=NULL)
      *tmpProgName = '\0';
    *progNamePtr = progName;
    CommonProgName(progName);
  }
  else	/* progNamePtr==NULL */
    progName = NULL;

  /* set minusFlag if switches are preceded by '-' */
  minusFlag = (progNamePtr!=NULL || paraList!=NULL);
  minusChar = (minusFlag) ? "-" : "";
    
  if (CLdebugLevel >= 1)
    printf("CmdLineEval: argc=%d  mode=%s  minusChar=\"%s\"\n",
	   argc,(progNamePtr!=NULL)?"cmd line":"token list",minusChar);

  if (setDefault) {
    /* reset switch used flags and evaluate default values */
    switchPtr = switchList;
    while (switchPtr != NULL && switchPtr->switchName != NULL) {
      if (switchPtr->argument != NULL) {
	if (switchPtr->usedFlag != NULL)
	  *((int*)switchPtr->usedFlag) = 0;
	if (switchPtr->format == NULL)
	  *((int*)switchPtr->argument) = 0;
	else
	  if (switchPtr->defaultValue != NULL) {
	    if (strcmp(switchPtr->format,"%s") == 0)
	      *((char**)switchPtr->argument) = switchPtr->defaultValue;
	    else
	      if (sscanf(switchPtr->defaultValue,switchPtr->format,
			 switchPtr->argument) != 1) {
		CommonWarning("CmdLineEval: "
			      "switch %s%s default argument format error",
			      minusChar,switchPtr->switchName);
		return 2;
	      }
	  }
      }
      switchPtr++;
    }
  }

  /* scan arguments  */
  i = (progNamePtr==NULL) ? 0 : 1;	/* skip program name */
					/* if command line mode */
  paraPtr = paraList;
  count = 0;

  while (i < argc) {
    if ((*argv[i] == '-' && *(argv[i]+1) != '\0') ||
	(minusFlag == 0 && *argv[i] != '\0')) {
      /* evaluate cmdline switch */
      switchPtr = switchList;
      while (switchPtr != NULL && switchPtr->switchName != NULL) {
	if (strcmp(argv[i]+minusFlag,switchPtr->switchName) == 0) {
	  /* switchList entry found */
	  if (switchPtr->argument == NULL) {
	    /* help switch found */
	    return 1;
	  }
	  if (switchPtr->format != NULL) {
	    /* read switch argument */
	    if (++i >= argc) {
	      CommonWarning("CmdLineEval: switch %s%s has no argument",
			    minusChar,switchPtr->switchName);
	      return 2;
	    }
	    else
	      if (strcmp(switchPtr->format,"%s") == 0)
		*((char**)switchPtr->argument) = argv[i];
	      else
		if (sscanf(argv[i],switchPtr->format,switchPtr->argument)
		    != 1) {
		  CommonWarning("CmdLineEval: "
				"switch %s%s argument format error",
				minusChar,switchPtr->switchName);
		  return 2;
		}
	  }
	  else
	    /* switch without argument */
	    *((int*)switchPtr->argument) = 1;
	  /* set switch used flag */
	  if (switchPtr->usedFlag != NULL)
	    *((int*)switchPtr->usedFlag) = 1;
	  break;	/* while (switchPtr ...) */
	}
	switchPtr++;
      }		/* while (switchPtr ...) */

      if (switchPtr == NULL || switchPtr->switchName == NULL) {
	CommonWarning("CmdLineEval: switch %s unknown",argv[i]);
	return 2;
      }
    }

    else {	/* if (*argv[i] == '-' && ...) */
      /* evaluate cmdline argument */
      if (paraPtr == NULL || paraPtr->argument == NULL) {
	CommonWarning("CmdLineEval: too many arguments",argv[i]);
	return 2;
      }
      if (paraPtr->format == NULL) {
	/* variable length argument list */
	if (count+1 >= MAX_TOKEN_NUM) {
	  CommonWarning("CmdLineEval: argument list %s too long",
			paraPtr->help);
	  return 2;
	}
	tmpVarArgIdx[count++] = i;
      }
      else {
	if (strcmp(paraPtr->format,"%s") == 0)
	  *((char**)paraPtr->argument) = argv[i];
	else
	  if (sscanf(argv[i],paraPtr->format,paraPtr->argument) != 1) {
	    CommonWarning("CmdLineEval: argument %s format error",
			  paraPtr->help);
	    return 2;
	  }
	paraPtr++;
      }
    }
    
    i++;
  }		/* while (i < argc) */
    
  if (paraPtr != NULL && paraPtr->argument != NULL &&
      paraPtr->format == NULL) {
    /* variable length argument list */
    if ((varArgIdx = (int*)malloc((count+1)*sizeof(int))) == NULL)
      CommonExit(1,"CmdLineEval: memory allocation error (varArgIdx)");
    for (i=0; i<count; i++)
      varArgIdx[i] = tmpVarArgIdx[i];
    varArgIdx[count] = -1;
    *((int**)paraPtr->argument) = varArgIdx;
    paraPtr++;
  }

  if (paraPtr != NULL && paraPtr->argument != NULL) {
    CommonWarning("CmdLineEval: argument %s is missing",
		  paraPtr->help);
    return 2;
  }

  return 0;
}


/* CmdLineEvalFree() */
/* Free memory allocated by CmdLineEval() for variable length */
/* argument list. */

void CmdLineEvalFree (
  CmdLinePara *paraList)	/* in: parameter info list */
				/*     or NULL */
{
  CmdLinePara *paraPtr;

  paraPtr = paraList;
  while (paraPtr != NULL && paraPtr->argument != NULL)
    if (paraPtr->format == NULL) {
      free(*((void**)paraPtr->argument));
      paraPtr = NULL;
    }
    else
      paraPtr++;

  if (CLdebugLevel >= 1)
    printf("CmdLineEvalFree: %s\n",
	   (paraList && !paraPtr) ? "free varArgIdx" : "no varArgIdx");
}


/* CmdLineHelp() */
/* Print help text about program usage including description of */
/* command line parameters and switches. */

void CmdLineHelp (
  char *progName,		/* in: program name */
				/*     or NULL */
  CmdLinePara *paraList,	/* in: parameter info list */
				/*     or NULL */
  CmdLineSwitch *switchList,	/* in: switch info list */
				/*     or NULL */
  FILE *outStream)		/* in: output stream */
				/*     (e.g. stdout) */
{
  int minusFlag;
  char *minusChar;
  char help[MAX_HELP_SIZE];	/* copy of help for strtok() */
  char *token;
  int maxSwitchLen,tmp;
  CmdLinePara *paraPtr;
  CmdLineSwitch *switchPtr;

  /* set minusFlag if switches are preceded by '-' */
  minusFlag = (progName!=NULL || paraList!=NULL);
  minusChar = (minusFlag) ? "-" : "";
    
  fprintf(outStream,"\n");

  maxSwitchLen = 0;
  switchPtr = switchList;
  while (switchPtr != NULL && switchPtr->switchName != NULL) {
    if ((tmp=strlen(switchPtr->switchName)+
	 ((switchPtr->argument!=NULL && switchPtr->format!=NULL)?4:0))
	> maxSwitchLen)
      maxSwitchLen = tmp;
    switchPtr++;
  }
  
  if (progName != NULL)
    fprintf(outStream,"usage: %s",progName);
  else
    fprintf(outStream,"token list format:");
  fprintf(outStream," %sswitches",minusChar);
  paraPtr = paraList;
  while (paraPtr != NULL && paraPtr->argument != NULL) {
    fprintf(outStream," %s",paraPtr->help);
    if (paraPtr->format==NULL)
      /* remaining args are not processed !!! */
      break;	/* while (paraPtr ...) */
    paraPtr++;
  }
  fprintf(outStream,"\n");
  
  /* display help for every switch  */
  switchPtr=switchList;
  while (switchPtr != NULL && switchPtr->switchName != NULL) {
    fprintf(outStream,"%9s %s%s%-*s   ",
	    (switchPtr==switchList)?"switches:":"",
	    minusChar,switchPtr->switchName,
	    maxSwitchLen-strlen(switchPtr->switchName),
	    (switchPtr->format==NULL)?"":" <x>");
    if (strchr(switchPtr->help,'\n')==NULL)
      fprintf(outStream,"%s",switchPtr->help);
    else {
      /* multiple lines separated by '\n' */
      strncpy(help,switchPtr->help,MAX_HELP_SIZE-1);
      help[MAX_HELP_SIZE-1] = '\0';
      token = strtok(help,"\n");
      fprintf(outStream,"%s",token);
      while (token!=NULL) {
	token = strtok(NULL,"\n");
	if (token!=NULL) {
	  fprintf(outStream,"\n");
	  if (strcmp(token,"\b")==0)
	    fprintf(outStream,"%9s %*s  ",
		    "",maxSwitchLen+minusFlag,"");
	  else
	    fprintf(outStream,"%9s %*s   %s",
		    "",maxSwitchLen+minusFlag,"",token);
	}
      }
    }
    if (switchPtr->argument!=NULL && switchPtr->defaultValue!=NULL)
      fprintf(outStream," (dflt: %s)",switchPtr->defaultValue);
    fprintf(outStream,"\n");
    switchPtr++;
  }
  fprintf(outStream,"\n");
}


/* CmdLineParseString() */
/* Parse a copy of string into tokens separated by sepaChar. */
/* Resulting token list can be evaluated by CmdLineEval(). */

char **CmdLineParseString (
  char *string,			/* in: string to be parsed */
				/*     NOTE: string is not modified */
  char *sepaChar,		/* in: token separator characters */
  int *count)			/* out: number of tokens generated by parser */
				/*      (corresponds to argc) */
				/* returns: */
				/*  list of tokens generated by parser */
				/*  (corresponds to argv[]) */
{
  char *tmpTokenList[MAX_TOKEN_NUM];
  char *firstChar;
  char *stringBuf;
  char **tokenList;
  int  i,size;

  if (CLdebugLevel >= 1)
    printf("CmdLineParseString: sepa=\"%s\"\n",sepaChar);

  if (string == NULL)
    stringBuf = NULL;
  else {
    /* find first non-sepaChar */
    firstChar = string;
    while (*firstChar!='\0' && strchr(sepaChar,*firstChar)!=NULL)
      firstChar++;
    size = strlen(firstChar);
    
    /* copy string content */ 
    if ((stringBuf = (char*)malloc((size+1)*sizeof(char))) == NULL)
      CommonExit(1,"CmdLineParseString: memory allocation error (stringBuf)");
    strcpy(stringBuf,firstChar);
  }

  /* parse string */
  i = 0;
  tmpTokenList[i] = (stringBuf==NULL)?(char*)NULL:strtok(stringBuf,sepaChar);
  if (tmpTokenList[0]!=NULL && tmpTokenList[0]!=stringBuf)
    CommonExit(1,"CmdLineParseString: internal error");
  while (tmpTokenList[i]!=NULL) {
    if (CLdebugLevel >= 2)
      printf("%4d: \"%s\"\n",i,tmpTokenList[i]);
    if (++i >= MAX_TOKEN_NUM)
      CommonExit(1,"CmdLineParseString: too many tokens");
    tmpTokenList[i] = strtok(NULL,sepaChar);
  }
  *count = i;

  /* copy token list */
  if ((tokenList = (char**)malloc((*count+1)*sizeof(char*))) == NULL)
    CommonExit(1,"CmdLineParseString: memory allocation error (tokenList)");
  for (i=0; i<*count; i++)
    tokenList[i] = tmpTokenList[i];
  tokenList[*count] = NULL;
  
  if (CLdebugLevel >= 1)
    printf("CmdLineParseString: tokenCount=%d\n",*count);

  return tokenList;
}


/* CmdLineParseFile() */
/* Parse init file into tokens separated by sepaChar. */
/* Comments preceded by a commentSepaChar are ingnored. */
/* Resulting token list can be evaluated by CmdLineEval(). */

char **CmdLineParseFile (
  char *fileName,		/* in: file name of init file */
  char *sepaChar,		/* in: token separator characters */
  char *commentSepaChar,	/* in: comment separator characters */
  int *count)			/* out: number of tokens generated by parser */
				/*      (corresponds to argc) */
				/* returns: */
				/*  list of tokens generated by parser */
				/*  (corresponds to argv[]) */
				/*  or NULL if file error */
{
  FILE *initFile;
  char lineBuf[MAX_LINE_SIZE];
  char *comment;
  char tmpFileBuf[MAX_FILE_SIZE];
  char *firstChar;
  char *fileBuf;
  char *tmpTokenList[MAX_TOKEN_NUM];
  char **tokenList;
  int  i,size,line;

  if (CLdebugLevel >= 1)
    printf("CmdLineParseFile: file=\"%s\"  sepa=\"%s\"  com=\"%s\"\n",
	   fileName,sepaChar,commentSepaChar);

  /* open init file */
  if ((initFile = fopen(fileName,"r")) == NULL) {
    CommonWarning("CmdLineParseFile: error opening init file %s",fileName);
    return NULL;
  }
  
  /* read init file */
  line = 0;
  size = 0;
  tmpFileBuf[0] = '\0';
  while (fgets(lineBuf,MAX_LINE_SIZE,initFile) != NULL) {
    line++;
    comment = strpbrk(lineBuf,commentSepaChar);
    if (comment != NULL)
      i = comment-lineBuf;
    else {
      i = strlen(lineBuf)-1;
      if (lineBuf[i] != '\n')
	CommonExit(1,"CmdLineParseFile: line %d too long",line);
    }
    if (size+i+1 >= MAX_FILE_SIZE)
      CommonExit(1,"CmdLineParseFile: file too long");
    strncat(tmpFileBuf+size,lineBuf,i);
    strncat(tmpFileBuf+size+i,sepaChar,1);
    size += i+1;
  }
  
  /* close init file */
  if (fclose(initFile)) {
    CommonWarning("CmdLineParseFile: error closing init file");
    return NULL;
  }
  
  if (CLdebugLevel >= 1)
    printf("CmdLineParseFile: initFileLineNum=%d\n",line);

  /* find first non-sepaChar */
  firstChar = tmpFileBuf;
  while (*firstChar!='\0' && strchr(sepaChar,*firstChar)!=NULL)
    firstChar++;
  size -= firstChar-tmpFileBuf;

  /* copy file content */ 
  if ((fileBuf = (char*)malloc((size+1)*sizeof(char))) == NULL)
    CommonExit(1,"CmdLineParseFile: memory allocation error (fileBuf)");
  strcpy(fileBuf,firstChar);

  /* parse string */
  i = 0;
  tmpTokenList[i] = strtok(fileBuf,sepaChar);
  if (tmpTokenList[0]!=NULL && tmpTokenList[0]!=fileBuf)
    CommonExit(1,"CmdLineParseFile: internal error");
  while (tmpTokenList[i]!=NULL) {
    if (CLdebugLevel >= 2)
      printf("%4d: \"%s\"\n",i,tmpTokenList[i]);
    if (++i >= MAX_TOKEN_NUM)
      CommonExit(1,"CmdLineParseFile: too many tokens");
    tmpTokenList[i] = strtok(NULL,sepaChar);
  }
  *count = i;

  /* copy token list */
  if ((tokenList = (char**)malloc((*count+1)*sizeof(char*))) == NULL)
    CommonExit(1,"CmdLineParseFile: memory allocation error (tokenList)");
  for (i=0; i<*count; i++)
    tokenList[i] = tmpTokenList[i];
  tokenList[*count] = NULL;
  
  if (CLdebugLevel >= 1)
    printf("CmdLineParseFile: tokenCount=%d\n",*count);

  return tokenList;
}


/* CmdLineParseFree() */
/* Free memory allocated by CmdLineParseString() or CmdLineParseFile(). */

void CmdLineParseFree (
  char **tokenList)		/* in: token list returned by */
				/*     CmdLineParseString() or */
				/*     CmdLineParseFile() */
{
  if (tokenList != NULL) {
    if (*tokenList != NULL)
      free(*tokenList);
    free(tokenList);
  }

  if (CLdebugLevel >= 1)
    printf("CmdLineParseFree: %s\n",
	   (tokenList) ? "free tokenList" : "no tokenList");
}


/* ComposeFileName() */
/* Compose filename using default path and extension if required. */
/* Handles Unix & DOS paths. "-" is passed through directly. */

int ComposeFileName (
  char *inName,			/* in: input filename */
  int forceDefault,		/* in: 0=keep input path and/or extension if */
				/*       available, otherwise use default(s) */
				/*     1=force usage of default */
				/*       path and extension */
  char *defaultPath,		/* in: default path */
				/*     or NULL */
  char *defaultExt,		/* in: default extension */
				/*     or NULL */
  char *fileName,		/* out: composed filename */
  unsigned int fileNameMaxLen)	/* in: fileName max length */
				/* returns: */
				/*  0=OK  1=result too long */
{
  char *name,*dot,*tmp;
  char pathChar;

  if (CLdebugLevel >= 1)
    printf("ComposeFileName: in=\"%s\"  forceDef=%d  path=\"%s\"  ext=\"%s\""
	   "  len=%d\n",
	   inName,forceDefault,
	   (defaultPath!=NULL)?defaultPath:"(NULL)",
	   (defaultExt!=NULL)?defaultExt:"(NULL)",
	   fileNameMaxLen);

  if (strcmp(inName,"-")==0) {
    if (fileNameMaxLen<2)
      return 1;
    strcpy(fileName,inName);
    return 0;
  }

  /* compose path */
  name = StripPath(inName);
  if (name==inName || forceDefault) {
    /* use default path */
    if (defaultPath==NULL || *defaultPath=='\0')
      *fileName = '\0';
    else
      if (strlen(defaultPath)+1 >= fileNameMaxLen)
	return 1;
      else {
	strcpy(fileName,defaultPath);
	tmp = fileName+strlen(fileName)-1;
	if (strchr(fileName,'/')!=NULL || strchr(inName,'/')!=NULL)
	  pathChar = '/';
	else
	  if (strchr(fileName,'\\')!=NULL || strchr(inName,'\\')!=NULL)
	    pathChar = '\\';
	  else
	    pathChar = '/';
	if (*tmp!=pathChar) {
	  /* append pathChar to default path */
	  *(++tmp) = pathChar;
	  *(++tmp) = '\0';
	}
      }
    if (strlen(fileName)+strlen(name) >= fileNameMaxLen)
      return 1;
    else
      strcat(fileName,name);
  }
  else {
    /* use input path */
    if (strlen(inName) >= fileNameMaxLen)
      return 1;
    else
      strcpy(fileName,inName);
  }

  /* compose extension */
  dot = strchr(StripPath(fileName),'.');
  if (dot!=NULL && forceDefault) {
    /* remove input extension */
    *dot = '\0';
    dot = NULL;
  }
  if (dot==NULL && defaultExt!=NULL && *defaultExt!='\0') {
    /* use default extension */
    if (strlen(fileName)+strlen(defaultExt)+1 >= fileNameMaxLen)
      return 1;
    else {
      if (strchr(defaultExt,'.') == NULL)
	/* insert '.' before extension */
	strcat(fileName,".");
      strcat(fileName,defaultExt);
    }
  }
  
  if (CLdebugLevel >= 1)
    printf("ComposeFileName: fileName=\"%s\"\n",fileName);

  return 0;
}


/* end of cmdline.c */

