// sander@cedsys.nl 13-2-2016
// Extracts IP addresses from a scanio FTP results file
// https://scans.io/series/21-ftp-banner-full_ipv4
//
// usage: ./scansio_ftp_parse <path_to_extracted_file>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// The max number of lines for the output file(s)
int max_linecount = 100000;

// The output directory it should write to. if this is
// defined, end with a slash. if empty, files will end up
// in the current directory.
const char outfile_dir[] = "";

// do not touch -- mark as static?
int bufSize = 2048;
int outfiles = 1;
char scanio_date[15];


FILE* rtn_write_fp(FILE* fp_write) {
  /*
   * Function:  rtn_write_fp
   * --------------------
   * returns a FILE pointer. The filename is dynamic as we want to
   * write to several files instead of one big file, as defined by
   * max_linecount.
   *
   *  fp_write (optional): the previous FILE pointer to close.
   *
   *  returns: the new FILE pointer.
   */

  FILE* fp_tmp;

  if(fp_write){
    fclose(fp_write);
  }

  /* I don't _really_ know if we need to manually allocate a new
   * buffer each time; could just make it static and reuse the same
   * allocation, or honestly just stack allocate a fixed buffer.
   */
  size_t outfile_len = strlen(outfile_dir) + strlen(scanio_date) + (floor(log10(abs(outfiles))) + 1) + 5;
  char *outfile_tmp = malloc(outfile_len);

  /* Honestly, you could just as easily use snprintf here.
   * also, whilst not critical, using the *n* variants of
   * strcpy, would be nice
   */
  strcpy(outfile_tmp, outfile_dir);
  sprintf(outfile_tmp, "%s%s_%d.txt", outfile_dir, scanio_date, outfiles);
  outfile_tmp[outfile_len+1] = '\0'; // did you really mean "\0"? that's a string with two nulls...

  outfiles += 1;

  printf("writing: %s\n", outfile_tmp);

  /* probably should check the returned value here as well... */
  fp_tmp = fopen(outfile_tmp, "w");
  free(outfile_tmp);
  return fp_tmp;    
}

void set_outfile_name(char *infile){
  /*
   * Function:  set_outfile_name
   * --------------------
   * parses the date from the scan.io list and stores it in a global variable
   * so that the output filenames can be linked to their input file.
   *
   *  infile: full path to the input file
   *
   *  returns:
   */

  if(strstr(infile, "-21-ftp-banner-full_ipv4-") != NULL) {
    char *token = strtok(infile+41, "-");

    strncpy(scanio_date, token, 15);
  } else {
    strncpy(scanio_date, "unknown", 15); // you don't need an explicit '\0'
  }
}

int main(int argc, char *argv[])
{
  if (argc <= 1) {
    printf("usage: %s $FILE\n", argv[0]);
    return 0;
  }

  char infile[strlen(argv[1])+1];
  strcpy(infile, argv[1]);

  set_outfile_name(argv[1]);

  FILE* fp_read;
  FILE* fp_write;

  if ((fp_read = fopen(infile, "r")) == NULL)
  {
    perror("fopen failed");
    return 1;
  }

  int linecount;
  char buf[bufSize];

  linecount = 0;
  fp_write = rtn_write_fp(fp_write);

  while (fgets(buf, sizeof(buf), fp_read) != NULL)
  {
    if((linecount % max_linecount) == 0 && linecount != 0){
      fp_write = rtn_write_fp(fp_write);
    }

    buf[21] = '\0';
    char *token = strtok(buf+6, "\"");

    fputs(token, fp_write);
    fputs("\n", fp_write);

    linecount += 1;
  }

  fclose(fp_read);
  fclose(fp_write);

  return 0;
}
