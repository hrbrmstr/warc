#include <Rcpp.h>
using namespace Rcpp;

#include <iostream>
#include <fstream>

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

char *no_space(char *str) {
  int ct=0;
  for (int i=0; str[i]; i++)
    if (str[i] != ' ') str[ct++] = str[i];
    str[ct] = '\0';
    return(str);
}

// [[Rcpp::export]]
void int_create_cdx_from_warc(std::string warc_path,
                              std::string warc_record_types,
                              std::string field_spec,
                              std::string cdx_path) {
}

// [[Rcpp::export]]
void int_create_cdx_from_gzwarc(std::string warc_path,
                                std::string warc_record_types,
                                std::string field_spec,
                                std::string cdx_path) {

  std::string full_path(R_ExpandFileName(warc_path.c_str()));

  Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
  Rcpp::Function basename = baseEnv["basename"];
  std::string base = Rcpp::as<std::string>(basename(full_path));
  const char *warc_file = base.c_str();

  FILE *fp = fopen(full_path.c_str(), "rb");
  gzFile gzf = gzdopen(fileno(fp), "rb");
  int res;
  char buf[BUF_LEN];

  Rcout << "Creating CDX file" << std::endl;
  Rcout << cdx_path << std::endl;

  std::ofstream cdx_file(cdx_path);

  if (gzf) {

    res = gzbuffer(gzf, 32*1024);

    char *line;
    char *key, val[BUF_LEN];
    uintmax_t content_length;
    char warc_type[KEY_VAL_MAX];
    char urn[KEY_VAL_MAX];
    char target_uri[KEY_VAL_MAX];
    char mime_type[KEY_VAL_MAX];
    char redirect[KEY_VAL_MAX];
    z_off_t warc_offset = 0;
    struct tm ts;
    char *cp;
    char warc_time[TIME_MAX];
    char out_buf[BUF_LEN];
    char status[5];
    z_off_t pre, post;

    cdx_file << " CDX a b m s r V g u\n";

    while ((line=gzgets(gzf, buf, BUF_LEN)) != NULL) {

      strcpy(target_uri, "-");
      strcpy(status, "-");
      strcpy(mime_type, "-");
      strcpy(redirect, "-");

      buf[strcspn(buf, "\r\n")] = '\0';

      while(strcmp("\r\n", line=gzgets(gzf, buf, BUF_LEN)) != 0) {

        buf[strcspn(buf, "\r\n")] = '\0';
        char *v = strnstr(buf, ": ", strlen(buf));

        if (v) {

          strncpy(val, (v+2), strlen(buf));
          key = strtok(buf, ":");

          if (strcmp("Content-Length", key) == 0) {
            content_length = strtoumax(val, NULL, 10);
            if (content_length == UINTMAX_MAX && errno == ERANGE)  exit(1);
          } else if (strcmp("WARC-Type", key) == 0) {
            strncpy(warc_type, val, KEY_VAL_MAX);
            strncpy(mime_type ,warc_type, KEY_VAL_MAX);
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

      if (strcmp("response", warc_type) == 0) {

        pre = gztell(gzf);

        line = gzgets(gzf, buf, BUF_LEN);
        buf[strcspn(buf, "\r\n")] = '\0';
        strtok(buf, " ");
        strcpy(status, strtok(NULL, " "));

        while(strcmp("\r\n", line=gzgets(gzf, buf, BUF_LEN)) != 0) {
          buf[strcspn(buf, "\r\n")] = '\0';
          char *v = strnstr(buf, ": ", strlen(buf));
          if (v) {
            strncpy(val, (v+2), strlen(buf));
            key = strtok(buf, ":");
            if (strcmp("Content-Type", key) == 0) {
              strncpy(mime_type, val, KEY_VAL_MAX);
              no_space(mime_type);
            } else if (strcmp("Location", key) == 0) {
              strncpy(redirect, val, KEY_VAL_MAX);
            }
          }
        }

        post = gztell(gzf);

        gzseek(gzf, (content_length - (post-pre+1)), SEEK_CUR);

        res = sprintf(out_buf, "%s %s %s %s %s %ld %s %s\n",
                      target_uri, warc_time, mime_type, status,
                      redirect, warc_offset, warc_file, urn);
        if (res<0) {
          Rcpp::warning("Unable to generate CDX record");
          return;
        }

        cdx_file << out_buf;

      } else {
        gzseek(gzf, content_length, SEEK_CUR);
      }

      warc_offset = gzoffset(gzf);

      line = gzgets(gzf, buf, BUF_LEN);
      line = gzgets(gzf, buf, BUF_LEN);

    }

  }

  cdx_file.close();
  if (gzf) gzclose(gzf);
  if (fp) fclose(fp);

}
