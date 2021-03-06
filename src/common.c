#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#if defined(_MSC_VER)
  #include <io.h>
#else
  #include <unistd.h>
#endif
#include "defs.h"
#include "mem.h"
#include "common.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

float *logTable;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t FLog2(uint64_t i)
  {
  uint32_t n, m, k = 32, o = (i & (i - 1)) ? 1 : 0;
  static const uint64_t sizes[6] = 
  { 0x0000000000000002ull, 0x000000000000000Cull, 0x00000000000000F0ull, 
    0x000000000000FF00ull, 0x00000000FFFF0000ull, 0xFFFFFFFF00000000ull };

  for(n = 6 ; n-- ; )
    {
    o += (m = (i & *(sizes+n)) ? k : 0);
    i >>= m;
    k >>= 1;
    }

  return o;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Unlink(const char *f)
  {
  if(unlink(f))
    fprintf(stderr, "Warning: file %s was not deleted.\n", f);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint64_t NBytesInFile(FILE *file)
  {
  uint64_t size = 0;
  fseek(file, 0, SEEK_END);
  size = ftell(file);
//  if((size = ftell(file)) < 1000)
//    {
//    fprintf(stderr, "Error: input file is very small!\n");
//    exit(1);
//    }
  size = ftell(file);
  rewind(file);
  return size;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint64_t FopenBytesInFile(const char *fn)
  {
  uint64_t size = 0;
  FILE *file = Fopen(fn, "r");
  
  size = NBytesInFile(file);  
  fclose(file);

  return size;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void FillLogTable(uint32_t nSym, uint32_t alphaDen, uint32_t maxCHigh)
  {
  uint32_t n, maxSize = nSym * maxCHigh * alphaDen;
 
  logTable = (float *) Malloc(maxSize * sizeof(float));
  for(n = 1 ; n != maxSize ; ++n)
    logTable[n] = FLog2(n);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

double SearchLog(uint32_t idx)
  {
  return logTable[idx];
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t DNASymToNum(uint8_t symbol)
  {
  switch(symbol)
    {
    case 'A': return 0;
    case 'T': return 3;
    case 'C': return 1;
    case 'G': return 2;
    default: fprintf(stderr, "Error: unknown numerical symbol\n"); exit(1);
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t GetCompSym(uint8_t symbol)
  {
  switch(symbol)
    {
    case 'A': return 'T';
    case 'C': return 'G';
    case 'G': return 'C';
    case 'T': return 'A';
    default:  return symbol;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t GetCompNum(uint8_t symbol)
  {
  switch(symbol)
    {
    case 0: return 3;
    case 1: return 2;
    case 2: return 1;
    case 3: return 0;
    default:  return symbol;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FILE *Fopen(const char *path, const char *mode)
  {
  FILE *file = fopen(path, mode);

  if(file == NULL)
    {
    fprintf(stderr, "Error opening: %s (mode %s). Does the file exist?\n", 
    path, mode);
    exit(1);
    }

  return file;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

uint8_t *ReverseStr(uint8_t *str, uint32_t end)
  {
  uint32_t start = 0;

  while(start < end)
    {
    str[start] ^= str[end];
    str[end]   ^= str[start];
    str[start] ^= str[end];
    ++start;
    --end;
    }

  return str;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void SortString(char str[])
  {
  char     tmp;
  int32_t  i, j, length = strlen(str);

  for(i = 0 ; i != length-1 ; ++i)
    for (j = i+1 ; j != length ; ++j)
      if(str[i] > str[j])
        {
        tmp    = str[i];
        str[i] = str[j];
        str[j] = tmp;
        }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *CloneString(char *str)
  {
  char *clone;
  if(str == NULL)
    return NULL;
  strcpy((clone = (char*) Malloc((strlen(str) + 1) * sizeof(char))), str);
  return clone;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *concatenate(char *a, char *b)
  {
  char *base;

  base = (char *) Malloc(strlen(a) + strlen(b) + 1);
  strcpy(base, a);
  strcat(base, b);
  return base;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *RepString(const char *str, const char *old, const char *new)
  {
  size_t sLen = strlen(str) + 1;
  char *cout = 0, *p = 0, *tmp; 

  if(!(p = (cout = (char *) Malloc(sLen * sizeof(char)))))
    return 0;
  while(*str)
    if((*str & 0xc0) != 0x80 && !strncmp(str, old, strlen(old)))
      {
      p   -= (intptr_t) cout;
      tmp  = strcpy(p = (cout = (char *) Realloc(cout, sLen += strlen(new) - 
             strlen(old), strlen(new) - strlen(old))) + (intptr_t) p, new);
      p   += strlen(tmp);
      str += strlen(old);
      }
    else
      *p++ = *str++;
  *p = 0;
  return cout;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t ArgsNumber(uint32_t def, char *arg[], uint32_t n, char *str)
  {
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return atol(arg[n+1]);
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

double ArgsDouble(double def, char *arg[], uint32_t n, char *str)
  {
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return atof(arg[n+1]);
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t ArgsState(uint8_t def, char *arg[], uint32_t n, char *str)
  {     
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return def == 0 ? 1 : 0;
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t Args3State(uint8_t def, char *arg[], uint32_t n, char *str)
  {
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return def == 0 ? 2 : 0;
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *ArgsString(char *def, char *arg[], uint32_t n, char *str)
  {
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return arg[n+1];
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *ArgsFiles(char *arg[], uint32_t argc, char *str, char *f)
  {
  int32_t n = argc;

  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return CloneString(arg[n+1]);
  
  return concatenate(concatenate(arg[argc-2], arg[argc-1]), f);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void FAccessWPerm(char *fn)
  {
  if(access(fn, F_OK) != -1)
    {
    fprintf(stderr, "Error: file %s already exists!\n", fn);
    if(access(fn, W_OK) != -1)
      fprintf(stderr, "Note: file %s has write permission.\nTip: to force "
      "writing rerun with \"-f\" option.\nWarning: on forcing, the old (%s) "
      "file will be deleted permanently.\n", fn, fn);
    exit(1);
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void CalcProgress(uint64_t size, uint64_t i)
  {
  if(size > PROGRESS_MIN && i % (size / 100) == 0)
    fprintf(stderr, "Progress:%3d %%\r", (uint8_t) (i / (size / 100)));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Patterns *GetPatterns(char *name)
  {
  FILE      *Reader = Fopen(name, "r");
  Patterns  *Pat;
  uint64_t  v1, v2; 

  Pat    = (Patterns *) Malloc(sizeof(Patterns)); 
  Pat->p = (PatEntry *) Malloc(sizeof(PatEntry)); 
  Pat->nPatterns = 0; 

  while(fscanf(Reader, "%"PRIu64":%"PRIu64"", &v1, &v2) == 2)
    {
    Pat->p[Pat->nPatterns].init = v1;
    Pat->p[Pat->nPatterns].end  = v2;
    Pat->p = (PatEntry *) Realloc(Pat->p, (++Pat->nPatterns + 1) *
    sizeof(PatEntry), sizeof(PatEntry));
    }
  fclose(Reader); 
  unlink(name);

  return Pat;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *ExtractSubSeq(char *name, Parameters *P, uint64_t init, uint64_t end)
  {
  FILE      *Reader = Fopen(name, "r"), *Writter = NULL;
  uint64_t  x;
  char      *fNameOut;

  fNameOut = concatenate(name, ".ext");
  Writter  = Fopen(fNameOut, "w");

  if(P->verbose)
    fprintf(stderr, "Extracting pattern [%"PRIu64";%"PRIu64"]\n", init, end);
  
  // TODO: SHAME ON THIS CODE! IMPROVE THIS WITH BUFFERS AND FREAD/FWRITE...
  fseek(Reader, init, SEEK_SET);
  for(x = init ; x < end ; ++x)
    fprintf(Writter, "%c", fgetc(Reader));

  fclose(Reader);
  fclose(Writter);

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

