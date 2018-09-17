#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "lt_filenames.h"


int chop_filename(char *fullname,LTFileName *cur)
{
  char tmp_str[FILENAME_LENGTH+5];
  char *ch_ptr;
  int erstat,tmp_int;

  erstat = 0;
  ch_ptr = NULL;

  if( (strcmp(fullname,".") == 0) || (strcmp(fullname,"..") == 0) )
    return 510;

  /* Copy into a temp string because strtok() modifies the input string */
  strcpy(tmp_str,fullname);

  /* strip off pathname, if any */
  ch_ptr = strrchr(fullname,'/');
  if(ch_ptr != NULL) {
    /* Use strncpy to limit the copy to the shorter of either actual filename or the predefined MAX in FI
 * LENAME_PATH_LENGTH */
    strncpy(cur->path, fullname, MIN(((ch_ptr-fullname)+1),(FILENAME_PATH_LENGTH-1)) );
    cur->path[MIN(((ch_ptr-fullname)+1),(FILENAME_PATH_LENGTH-1))] = '\0';
    strcpy(tmp_str,ch_ptr+1);
  }
  else {
    strcpy(cur->path,".");
  }

  if(DEBUG) 
    fprintf(stdout,"chop_filename: path[%s] and filename[%s]\n",cur->path,tmp_str);   


  /* If it sees one of these special filenames it constructs a legal filename which assumes
 *    * the instrument code "c". I don't think this matters. It will be tested as part of 
 *       * hawkcam deployment. This filename is purely a dummy legal construct. 
 *          */
  if(strstr(tmp_str,"telFocus") != NULL) {
    fprintf(stdout,"chop_filename:telFocus detected.\n");
    strcpy(cur->I,"c");
    strcpy(cur->T,"t");
    strcpy(cur->date,"99999999");
    strcpy(cur->n,"0");
    strcpy(cur->m,"0");
    strcpy(cur->w,"0");
    strcpy(cur->p,"0");
    erstat = 500;
  }
  if(strstr(tmp_str,"twilight_calibrate") != NULL) {
    fprintf(stdout,"chop_filename:twilight_calibrate detected.\n");
    strcpy(cur->I,"c");
    strcpy(cur->T,"f");
    strcpy(cur->date,"99999999");
    strcpy(cur->n,"0");
    strcpy(cur->m,"0");
    strcpy(cur->w,"0");
    strcpy(cur->p,"0");
    erstat = 500;
  }

  /* These first jobs are to be done even if this is not a "classical" LT filename. 
 *    * This allows some of the applications to still use this function and operate on 
 *       * non LT files. E.g., mean, imstat etc */

  /* Put everything up until the first . in cur->exposure*/
  tmp_int = strcspn(tmp_str,".");
  strncpy(cur->exposure,tmp_str,(size_t)tmp_int);
  *(cur->exposure+tmp_int) = '\0';
  /* Whatever comes after the first dot is written into cur->ext.
 *    * This may be nothing if there was no . in the original filename */
  if(tmp_int != (int)strlen(tmp_str) ) 
    strcpy(cur->ext,tmp_str+tmp_int+1);
  else
    *(cur->ext) = '\0';
  
  if(DEBUG)
    printf("exposure = [%s] extension = [%s]\n",cur->exposure,cur->ext);   


  /* We already know if it is a focus or flat, so ignore these tests */
  if(erstat != 500){
    if( strcspn(tmp_str,"_")==strlen(tmp_str) )  {
      erstat = 501;
      return erstat;
    }
    /* A slightly more rigourous check is to make sure there are `_' in the correct places    */
    if( (tmp_str[1]!='_') || (tmp_str[3]!='_') || (tmp_str[12]!='_')){
      erstat = 502;
      return erstat; 
    }
  }

  if(erstat != 0) 
    return erstat;

  /* note, do NOT copy path back into tmp_str, just all chars after path */
  if( (strlen(cur->path)==1) && (cur->path[0]=='.'))
    strcpy(tmp_str,fullname);
  else
    strcpy(tmp_str,fullname+strlen(cur->path));

  /* Instrument list 
 * 	b	Blue arm of FRODO
 * 		c	RATCAM
 * 			d	
 * 				e	
 * 					f	
 * 						g	
 * 						        h	IO:O
 * 						                i	
 * 						                	n	NuView
 * 						                		o	Ringo
 * 						                			p	Ringo2
 * 						                				q	Rise
 * 						                					r	Red arm of FRODO
 * 						                						s	SupIRCam 
 * 						                							w	THOR
 * 						                							   */

  strncpy(cur->I,(char *)strtok(tmp_str,"_"),FILENAME_I_LEN);
  if(strspn(cur->I,"abcdefghijklmnopqrstuvwxyz0123456789")==1){                 /* Must be valid LT instr
ument code */
    strncpy(cur->T,(char *)strtok(NULL,"_"),FILENAME_T_LEN);
    if(strspn(cur->T,"abdefqsw")==1){            /* Must be valid LT exposure type code */
      strncpy(cur->date,(char *)strtok(NULL,"_"),FILENAME_DATE_LEN);
      if(sscanf(cur->date,"%d",&tmp_int) == 1){         /* Must be valid integer */
        strncpy(cur->n,(char *)strtok(NULL,"_"),FILENAME_N_LEN);
        if(sscanf(cur->n,"%d",&tmp_int) == 1){          /* Must be valid integer */
          strncpy(cur->m,(char *)strtok(NULL,"_"),FILENAME_M_LEN);
          if(sscanf(cur->m,"%d",&tmp_int) == 1){        /* Must be valid integer */
            strncpy(cur->w,(char *)strtok(NULL,"_"),FILENAME_W_LEN);
            /* if(sscanf(cur->w,"%d",&tmp_int)==1 && (tmp_int>=1) && (tmp_int<=99) ){ }*/       /* Must b
e valid integer (1--99) */
            if(sscanf(cur->w,"%d",&tmp_int) == 1 ){       /* Must be valid integer */
              strncpy(cur->p,(char *)strtok(NULL,"."),FILENAME_P_LEN);
              /* if(! ( (sscanf(cur->p,"%d",&tmp_int)==1) && (tmp_int>=0) && (tmp_int<=9) ) )  { }*/   /*
 Must be valid integer (0--9) */
              if(! (sscanf(cur->p,"%d",&tmp_int)==1) )  {    /* Must be valid integer (0--9) */
                erstat=503;
	      }
            }
            else erstat=504;
          }
          else erstat=505;
        }
        else erstat=506;
      }
      else erstat=507;
    }
    else erstat=508;
  }
  else erstat=509;

  if(DEBUG)
    printf("Chopped: %s %s %s %s %s %s %s\n",cur->I,cur->T,cur->date,cur->n,cur->m,cur->w,cur->p );   

  return erstat;

}





int construct_filename(LTFileName *cur, char *fullname)
{
  strcpy(fullname,cur->path);
  strcat(fullname,"/");
  strcat(fullname,cur->I);
  strcat(fullname,"_");
  strcat(fullname,cur->T);
  strcat(fullname,"_");
  strcat(fullname,cur->date);
  strcat(fullname,"_");
  strcat(fullname,cur->n);
  strcat(fullname,"_");
  strcat(fullname,cur->m);
  strcat(fullname,"_");
  strcat(fullname,cur->w);
  strcat(fullname,"_");
  strcat(fullname,cur->p);
  strcat(fullname,".fits");
  
  /* printf("The filename I have created is %s\n",fullname); */

  return 0;
}



