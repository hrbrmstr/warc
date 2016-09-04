#include <Rcpp.h>
using namespace Rcpp;

#include <zlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <xlocale.h>

#define _ISOC99_SOURCE
#include <inttypes.h>

#define BUF_LEN 1024
#define KEY_VAL_MAX 256
#define TIME_MAX 32

#define WARM_START 1024

//"/Users/bob/Desktop/bbb/r-warc-00000.warc.gz"
//

//' @export
// [[Rcpp::export]]
CharacterVector mkcdxgz(std::string path) {

  std::string full_path(R_ExpandFileName(path.c_str()));

  Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
  Rcpp::Function basename = baseEnv["basename"];
  std::string base = Rcpp::as<std::string>(basename(full_path));
  const char *warc_file = base.c_str();

  FILE *fp = fopen(full_path.c_str(), "rb");
  gzFile gzf = gzdopen(fileno(fp), "rb");
  int res;
  char buf[BUF_LEN];

  std::vector<std::string> out;
  out.reserve(WARM_START);

  if (gzf) {

    res = gzbuffer(gzf, 32*1024);

    char *line;
    char *key, val[BUF_LEN];
    uintmax_t content_length;
    char warc_type[KEY_VAL_MAX];
    char urn[KEY_VAL_MAX];
    char target_uri[KEY_VAL_MAX];
    z_off_t warc_offset = 0;
    struct tm ts;
    char *cp;
    char warc_time[TIME_MAX];
    char out_buf[BUF_LEN];

    out.push_back(" CDX a b m V g u\n");

    while ((line=gzgets(gzf, buf, BUF_LEN)) != NULL) {

      strcpy(target_uri, "-");

      buf[strcspn(buf, "\r\n")] = '\0';
      //printf("[%s] [%d]\n", buf, j++);

      while(strcmp("\r\n", line=gzgets(gzf, buf, BUF_LEN)) != 0) {

        buf[strcspn(buf, "\r\n")] = '\0';
        char *v = strnstr(buf, ": ", strlen(buf));

        if (v) {

          strncpy(val, (v+2), strlen(buf));
          key = strtok(buf, ":");
          //printf("[%s] [%s]\n",key, val);

          if (strcmp("Content-Length", key) == 0) {
            content_length = strtoumax(val, NULL, 10);
            if (content_length == UINTMAX_MAX && errno == ERANGE)  exit(1);
          } else if (strcmp("WARC-Type", key) == 0) {
            strncpy(warc_type, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Record-ID", key) == 0) {
            strncpy(urn, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Target-URI", key) == 0) {
            strncpy(target_uri, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Date", key) == 0) {
            cp = strptime(val, "%Y-%m-%dT%H:%M:%S", &ts);
            strcpy(warc_time, "-");
            if (cp != NULL) {
              size_t so = strftime(warc_time, TIME_MAX, "%Y%m%d%H%M%S", &ts);
              if (so == 0) strcpy(warc_time, "-");
            }
          }

        }

      }

      res = sprintf(out_buf, "%s %s warc/%s %ld %s %s\n",
                    target_uri, warc_time, warc_type, warc_offset, warc_file, urn);
      if (res<0) {
        Rcpp::warning("Unable to generate CDX record");
        return(CharacterVector());
      }
      out.push_back(std::string(out_buf));

      warc_offset = gzoffset(gzf);

      gzseek(gzf, content_length, SEEK_CUR);

      line = gzgets(gzf, buf, BUF_LEN);
      line = gzgets(gzf, buf, BUF_LEN);

      //printf("\n");

    }

  }

  if (gzf) gzclose(gzf);
  if (fp) fclose(fp);

  return(wrap(out));

}
