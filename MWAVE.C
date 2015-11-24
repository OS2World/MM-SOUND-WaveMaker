/* Output from p2c, the Pascal-to-C translator */
/* From input file "mwave.pas" */
/* p2c: \emx\p2c/system.imp, line 10: Warning: Symbol 'SYSTEM' was already defined [220]
 * Warning: Symbol 'SYSTEM' was already defined [220] */


#include <p2c.h>


/*
  ***************************************************************************
  *                                                                         *
  *            Wave maker for SoundBlaster 1.5 by HaJott                    *
  *                                                                         *
  ***************************************************************************
    Discription of data types :

    type
       chunk = record
         main               : string[4];             = RIFF
         main_length        : array[0..3] of byte;   = length of file
         chunk_type         : string[4];             = WAVE
         sub_chunk          : string[4];             = fmt_ :_=space=ASCII 32
         sub_chunk_length   : array[0..3] of byte;   = always 16 bytes
                                                       don't know why !
         format             : word;                  = always 1 for PCM mode
                              every SoundBlaster works with format = 1
         modus              : word;                  1 = mono, 2 = stereo
         sample_freq        : array[0..3] of byte;   11.025 kHz = $2B11
                                                     22.050 kHz = $5622
                                                     44.100 kHz = $AC44
         bytes_per_sec      : array[0..3] of byte;   as it is
         bytes_per_sample   : word;                  1 = 8 Bit, 2 = 16 Bit
         bits_per_sample    : word;                  only 8, 12 or 16
         data_chunk         : string[4];             = data
         data_length        : array[0..3] of byte;   length of data block
       end;

*/

/*$N+*/

typedef unsigned short w;

typedef uchar dw[4];
/*       w                  = array[0..1] of byte;*/
/*       st                 = string[4]; */
typedef uchar dat[16384];

typedef struct recordType {
  /*st*/
  dw main, main_length;
  /*st*/
  dw chunk_type;
  /*st*/
  dw sub_chunk, sub_chunk_length;
  w format, modus;
  dw sample_freq, bytes_per_sec;
  w bytes_per_sample, bits_per_sample;
  /*st*/
  dw data_chunk, data_length;
  dat rf;
} recordType;

typedef recordType datafile;


Static short i, j, k, maxdat;
Static FILE *datei1;
Static uchar ri;
Static double x, stepx;
Static unsigned short code;
Static Anyptr p;
Static short lenght;
Static recordType chunk;
Static short step;
Static boolean bol;
Static unsigned short wo;
Static Char sinortri;
Static Char datei1_NAME[_FNSIZE];



Static Void initall()
{
  x = 0.0;
  code = 0;
  i = 0;
  k = 0;
  j = 0;
  bol = true;
  chunk.main[0] = 'R';
  chunk.main[1] = 'I';
  chunk.main[2] = 'F';
  chunk.main[3] = 'F';
  chunk.main_length[0] = 45;
  chunk.main_length[1] = 64;
  chunk.main_length[2] = 0;
  chunk.main_length[3] = 0;
  chunk.chunk_type[0] = 'W';
  chunk.chunk_type[1] = 'A';
  chunk.chunk_type[2] = 'V';
  chunk.chunk_type[3] = 'E';
  chunk.sub_chunk[0] = 'f';
  chunk.sub_chunk[1] = 'm';
  chunk.sub_chunk[2] = 't';
  chunk.sub_chunk[3] = ' ';
  chunk.sub_chunk_length[0] = 16;
  chunk.sub_chunk_length[1] = 0;
  chunk.sub_chunk_length[2] = 0;
  chunk.sub_chunk_length[3] = 0;
  chunk.format = 1;
  chunk.modus = 1;
  chunk.sample_freq[0] = 17;
  chunk.sample_freq[1] = 43;
  chunk.sample_freq[2] = 0;
  chunk.sample_freq[3] = 0;
  chunk.bytes_per_sec[0] = 68;
  chunk.bytes_per_sec[1] = 172;
  chunk.bytes_per_sec[2] = 0;
  chunk.bytes_per_sec[3] = 0;
  chunk.bytes_per_sample = 4;
  chunk.bits_per_sample = 8;
  chunk.data_chunk[0] = 'd';
  chunk.data_chunk[1] = 'a';
  chunk.data_chunk[2] = 't';
  chunk.data_chunk[3] = 'a';
  chunk.data_length[0] = 0;
  chunk.data_length[1] = 40;
  chunk.data_length[2] = 0;
  chunk.data_length[3] = 0;
  for (j = 1; j <= 16384; j++)
    chunk.rf[i - 1] = 0;
}


main(argc, argv)
int argc;
Char *argv[];
{
  short FORLIM, FORLIM1;

  PASCAL_MAIN(argc, argv);
  datei1 = NULL;
  initall();
  printf("\n Wave maker for SoundBlaster 1.5 by HaJott \n\n");
  printf(" Enter number in () to create wave \n");
  printf("Pulse wave (2), Sinus wave (1), triangle wave (0) : ");
  scanf("%c%*[^\n]", &sinortri);
  getchar();
  switch (sinortri) {

  case '1':
    stepx = 0.4;
    /**** stepx makes the frequency, 0.4=low 0.8=high ****/
    printf("\nMake Sinus \n");
    for (i = 1; i <= 16384; i++) {
      chunk.rf[i - 1] = labs((long)floor(127 + 127 * sin(x) + 0.5));
      /*this makes a sinus wave between 0 and 254 (8bit)*/
      x += stepx;
    }  /*for i:=1 to 16384 do*/
    break;

  case '2':   /*makes a triangle wave between 0 an 254 (8bit)*/
    i = 0;
    step = 16;
    /**** step makes the frequency, step=64 means low step=16 high*/
    maxdat = 16384 / (step * 2);
    printf("\nMake Pulse \n");
    FORLIM = maxdat;
    for (k = 1; k <= FORLIM; k++) {
      FORLIM1 = step;
      for (j = 1; j <= FORLIM1; j++) {
	i++;
	chunk.rf[i - 1] = 0;
      }
      FORLIM1 = step;
      for (j = 1; j <= FORLIM1; j++) {
	i++;
	chunk.rf[i - 1] = 255;
      }
      /**** makes a pulse wave between 0 and 255 (8bit) ****/
    }  /*for k:=1 to 256 do*/
    break;

  default:
    printf("\nMake Triangle \n");
    step = 8;   /*step makes the frequency, step=8 means low step=16 high*/
    while (i <= 16384) {
      if (chunk.rf[i - 1] >= 255 - step) {
	step = -step;
	bol = false;
      }
      if (chunk.rf[i - 1] <= step) {
	step = abs(step);
	bol = true;
      }
      i++;
      chunk.rf[i - 1] = chunk.rf[i - 2] + step;
    }
    break;
  }/*case*/

  printf("Size of complete Datafile : %ld\n", sizeof(recordType));
  printf("Size of wave data : %ld\n", sizeof(dat));
  chunk.main_length[0] = (sizeof(recordType)) & 255;
  chunk.main_length[1] = ((unsigned long)(sizeof(recordType))) >> 8;
  chunk.data_length[0] = (sizeof(dat)) & 255;
  chunk.data_length[1] = ((unsigned long)(sizeof(dat))) >> 8;
  strcpy(datei1_NAME, "c:\\mmos2\\sounds\\aaa.wav");
  if (datei1 != NULL)
    datei1 = freopen(datei1_NAME, "w+b", datei1);
  else
    datei1 = fopen(datei1_NAME, "w+b");
  if (datei1 == NULL)
    _EscIO(FileNotFound);
  fwrite(&chunk, sizeof(recordType), 1, datei1);
  if (datei1 != NULL)
    fclose(datei1);
  datei1 = NULL;
  if (datei1 != NULL)
    fclose(datei1);
  exit(EXIT_SUCCESS);
}



/* End. */
