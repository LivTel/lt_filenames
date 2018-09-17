#ifndef _LT_FILENAMES_H 
#define _LT_FILENAMES_H 

#ifdef CCD_DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

/* Maximum length of filename 
 * ** cjm 21/01/04 Increased length from 80 to 128 - may contain path element.*/
#define FILENAME_LENGTH        128
/**
 *  * The maximum length of the pathname part of the filename.
 *   */
#define FILENAME_PATH_LENGTH   (80)

/* Max length of std LT filename WITHOUT extention */
# define FILENAME_LEN           29

#ifndef MIN
/**
 *  * Macro returning the lesser of two values.
 *   */
#define MIN(x,y)    ((x) < (y) ? (x) : (y))
#endif

typedef struct LTFileName_struct{
  char exposure[FILENAME_LENGTH];
  char path[FILENAME_PATH_LENGTH];
  char ext[12];
  char I[2];
  char T[2];
  char date[9];
  char n[5];
  char m[6];
  char w[3];
  char p[3];
}LTFileName;

/* Some #defines set the string lengths that have been set above. When we come to read the 
 *  * filename we use strncpy to make sure that we never read too much into any of the above strings
 *   * and cause a memory allocation overflow. */
#define FILENAME_EXT_LEN	5
#define FILENAME_I_LEN		2
#define FILENAME_T_LEN		2
#define FILENAME_DATE_LEN	9
#define FILENAME_N_LEN		5
#define FILENAME_M_LEN		6
#define FILENAME_W_LEN		3
#define FILENAME_P_LEN		3

int chop_filename(char *fullname, LTFileName *cur);
int construct_filename(LTFileName *cur, char *fullname);

#endif
