#include <Rcpp.h>
using namespace Rcpp;

#include "warc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <zlib.h>

#include <Rdefines.h>
#include <Rinternals.h>

#include "R_ext/Memory.h"
#include "R_ext/Utils.h"

//' Open a gzip file for reading or writing
//'
//' @param path path to file
//' @param mode "\code{write}", "\code{append}" or "\code{read}"
//' @return handle to the file
//' @export
// [[Rcpp::export]]
XPtrGz gz_open(std::string path, std::string mode) {

  std::string fmode, gmode;

  if (mode == "append") {
    fmode = "a";
    gmode = "a9";
  } else if (mode == "write") {
    fmode = "w";
    gmode = "w9";
  } else if (mode == "read") {
    fmode = "r";
    gmode = "r";
  } else {
    Rcpp::stop("Invalid 'mode' specified");
  }

  std::string full_path(R_ExpandFileName(path.c_str()));

  FILE *wf = fopen(full_path.c_str(), fmode.c_str());
  if (wf) {
    gzFile gzf = gzdopen(dup(fileno(wf)), gmode.c_str());
    gzbuffer(gzf, 64*1024);

    gz_fp *fp = new gz_fp;

    fp->wfp = wf;
    fp->gzf = gzf;

    XPtrGz ptr(fp);

    return(ptr);
  } else {
    Rcpp::stop("Error opening file");
  }

}

//' Return the current raw compressed offset in the file
//'
//' @param gzfile file handle
//' @return offset position (integer)
//' @export
// [[Rcpp::export]]
z_off_t gz_offset(XPtrGz gzfile) {
  if (!gzfile) return(-1);
  return(gzoffset(gzfile->gzf));
}

//' Return the current raw uncompressedf offset in the file
//'
//' @param gzfile file handle
//' @return offset position (integer)
//' @export
// [[Rcpp::export]]
z_off_t gz_tell(XPtrGz gzfile) {
  if (!gzfile) return(-1);
  return(gztell(gzfile->gzf));
}

//' Sets the starting position for the next \code{gz_read()} or \code{gz_write()}
//'
//' @param gzfile file handle
//' @param offset represents a number of bytes in the compressed data stream
//' @param from either "\code{start}", "\code{end}" or "\code{current}"
//' @return \code{TRUE} if successful
//' @export
// [[Rcpp::export]]
bool gz_fseek(XPtrGz gzfile, z_off_t offset, std::string from) {

  if (!gzfile) return(-1);

  int whence = SEEK_SET;

  if (from == "start") whence = SEEK_SET;
  else if (from == "end") whence = SEEK_END;
  else if (from == "current") whence = SEEK_SET;

  return(fseek(gzfile->wfp, offset, whence) == 0);

}


//' Sets the starting position for the next \code{gz_read()} or \code{gz_write()}
//'
//' @param gzfile file handle
//' @param offset represents a number of bytes in the uncompressed data stream
//' @param from either "\code{start}" or "\code{current}"
//' @return the resulting offset location as measured in bytes from the beginning of the
//'   uncompressed stream, or –1 in case of error, in particular if the file is opened
//'   for writing and the new starting position would be before the current position.
//' @export
// [[Rcpp::export]]
z_off_t gz_seek(XPtrGz gzfile, z_off_t offset, std::string from) {
  if (!gzfile) return(-1);
  int whence = (from == "start") ? SEEK_SET : SEEK_CUR;
  return(gzseek(gzfile->gzf, offset, whence));
}

//' Read from a gz file into a raw vector
//'
//' @param gzfile file handle
//' @param len number of of characters
//' @export
// [[Rcpp::export]]
RawVector gz_read_raw(XPtrGz gzfile, unsigned len) {
  if (!gzfile) return("");
  unsigned char *buf = (unsigned char *)malloc(len);
  int res = gzread(gzfile->gzf, buf, len);
  if (res > 0) {
    RawVector ret(buf, buf+res);
    if (buf) free(buf);
    return(ret);
  } else {
    if (buf) free(buf);
    return("");
  }
}


//' Read from a gz file into a character vector
//'
//' @param gzfile file handle
//' @param len number of of characters
//' @export
// [[Rcpp::export]]
std::string gz_read_char(XPtrGz gzfile, unsigned len) {
  if (!gzfile) return("");
  unsigned char *buf = (unsigned char *)malloc(len);
  int res = gzread(gzfile->gzf, buf, len);
  if (res > 0) {
    std::string ret(buf, buf+res);
    if (buf) free(buf);
    return(ret);
  } else {
    if (buf) free(buf);
    return("");
  }
}

//' Test for end of file
//'
//' @export
//' @param gzfile file handle
// [[Rcpp::export]]
bool gz_eof(XPtrGz gzfile) {

  if (!gzfile) return(true);
  return(gzeof(gzfile->gzf));

}

//' Read a line from a gz file
//'
//' @export
//' @param gzfile file handle
//' @note line buffer max is 8,192 characters. The intent of this function is to use it
//'   on well-known formats.
// [[Rcpp::export]]
CharacterVector gz_gets(XPtrGz gzfile) {

  if (!gzfile) return("");
  if (gzeof(gzfile->gzf)) return("");
  char *buf = (char *)malloc(4*64*1024);
  CharacterVector ret(1);
  ret[0] = std::string(gzgets(gzfile->gzf, buf, (4*64*1024)));
  if (buf) free(buf);
  return(ret);

}

//' Read a line from a gz file
//'
//' @export
//' @param gzfile file handle
//' @note line buffer max is 8,192 characters. The intent of this function is to use it
//'   on well-known formats.
// [[Rcpp::export]]
RawVector gz_gets_raw(XPtrGz gzfile) {

  RawVector ret;

  if (!gzfile) return(ret);
  if (gzeof(gzfile->gzf)) return(ret);
  char *buf = (char *)malloc(4*64*1024);
  char *res = gzgets(gzfile->gzf, buf, (4*64*1024));
  if (res != NULL) ret = RawVector(res, res+strnlen(res, 4*64*1024));
  if (buf) free(buf);
  return(ret);

}

//' Write a raw vector to a gz file
//'
//' @param gzfile file handle
//' @param buffer raw vector to write
//' @export
// [[Rcpp::export]]
void gz_write_raw(XPtrGz gzfile, SEXP buffer) {
  if (gzfile) gzwrite(gzfile->gzf, RAW(buffer), GET_LENGTH(buffer));
}

//' Write an atomic character vector to a file
//'
//' @param gzfile file handle
//' @param buffer atomic character vector
//' @export
// [[Rcpp::export]]
void gz_write_char(XPtrGz gzfile, std::string buffer) {
  if (gzfile) gzwrite(gzfile->gzf, buffer.c_str(), buffer.size());
}

//' Flush currenzt gzip stream
//'
//' This will flush all zlib output buffers for the current file
//' and terminate the gzip stream. The next \code{gz_write()} will
//' start a new gzip stream.
//'
//' @param gzfile file handle
//' @export
// [[Rcpp::export]]
void gz_flush(XPtrGz gzfile) {
  int err=0;
  if (gzfile) err = gzflush(gzfile->gzf, Z_FULL_FLUSH);
  if (gzfile) err = gzflush(gzfile->gzf, Z_FINISH);
}

//' Close the gz file
//'
//' @param gzfile file handle
//' @note if you want to properly flush the buffers and correctly terminate a gzip stream
//'   then you \emph{must} call \code{gz_flush()} before closing the file.
//' @export
// [[Rcpp::export]]
void gz_close(XPtrGz gzfile) {
  if (gzfile) {
    gzclose(gzfile->gzf);
    fclose(gzfile->wfp);
  }
}
