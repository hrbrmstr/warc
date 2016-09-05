#include <Rcpp.h>
using namespace Rcpp;

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cassert>
#include <iterator>

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

  Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
  Rcpp::Function basename = baseEnv["basename"];
  std::string base = Rcpp::as<std::string>(basename(warc_path));
  const char *warc_file = base.c_str();

  std::map<char, std::string> warc_fields;
  std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  for (char c : alphabet) warc_fields[c] = "-";

  warc_fields['g'] = std::string(warc_file);

  std::ifstream wf(warc_path);
  std::ofstream cdx_file(cdx_path);

  cdx_file.close();
  wf.close();

}

// [[Rcpp::export]]
void int_create_cdx_from_gzwarc(std::string warc_path,
                                std::string warc_record_types,
                                std::string field_spec,
                                std::string cdx_path) {

  Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
  Rcpp::Function basename = baseEnv["basename"];
  std::string base = Rcpp::as<std::string>(basename(warc_path));
  const char *warc_file = base.c_str();

  FILE *fp = fopen(warc_path.c_str(), "rb");
  gzFile gzf = gzdopen(fileno(fp), "rb");
  int res;
  char buf[BUF_LEN];

  std::string::size_type len = field_spec.length();
  std::string::size_type cur = 0;

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
    char warc_concurrent_to[KEY_VAL_MAX];
    char warc_block_digest[KEY_VAL_MAX];
    char warc_payload_digest[KEY_VAL_MAX];
    char warc_ip_address[KEY_VAL_MAX];
    char warc_refers_to[KEY_VAL_MAX];
    char warc_truncated[KEY_VAL_MAX];
    char warc_warcinfo_id[KEY_VAL_MAX];
    char warc_filename[KEY_VAL_MAX];
    char warc_profile[KEY_VAL_MAX];
    char warc_identified_payload_type[KEY_VAL_MAX];
    char warc_segment_number[KEY_VAL_MAX];
    char warc_segment_origin_id[KEY_VAL_MAX];
    char warc_segment_total_length[KEY_VAL_MAX];
    z_off_t warc_offset = 0;
    struct tm ts;
    char *cp;
    char warc_time[TIME_MAX];
    char status[5];
    z_off_t pre, post;

    std::map<char, std::string> warc_fields;
    std::string warc_keys = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (char c : warc_keys) warc_fields[c] = "-";
    warc_fields['g'] = std::string(warc_file);

    cdx_file << " CDX ";
    for (char c : field_spec) {
      cur++;
      cdx_file << c;
      if (cur < len) {
        cdx_file << " ";
      } else {
        cdx_file << std::endl;
      }
    }

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

          if (strcmp("WARC-Record-ID", key) == 0) {
            strncpy(urn, val, KEY_VAL_MAX);
            warc_fields['u'] = std::string(urn);
          } else if (strcmp("Content-Length", key) == 0) {
            content_length = strtoumax(val, NULL, 10);
            if (content_length == UINTMAX_MAX && errno == ERANGE)  exit(1);
          } else if (strcmp("WARC-Date", key) == 0) {
            cp = strptime(val, "%Y-%m-%dT%H:%M:%S", &ts);
            strcpy(warc_time, "-");
            if (cp != NULL) {
              size_t so = strftime(warc_time, TIME_MAX, "%Y%m%d%H%M%S", &ts);
              if (so == 0) strcpy(warc_time, "-");
              warc_fields['b'] = std::string(warc_time);
            }
          } else if (strcmp("WARC-Type", key) == 0) {
            strncpy(warc_type, val, KEY_VAL_MAX);
            strncpy(mime_type, warc_type, KEY_VAL_MAX);
            warc_fields['m'] = std::string(warc_type);
          } else if (strcmp("WARC-Concurrent-To", key) == 0) {
            strncpy(warc_concurrent_to, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Block-Digest", key) == 0) {
            strncpy(warc_block_digest, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Payload-Digest", key) == 0) {
            strncpy(warc_payload_digest, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-IP-Address", key) == 0) {
            strncpy(warc_ip_address, val, KEY_VAL_MAX);
            warc_fields['e'] = std::string(warc_ip_address);
          } else if (strcmp("WARC-Refers-To", key) == 0) {
            strncpy(warc_refers_to, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Target-URI", key) == 0) {
            strncpy(target_uri, val, KEY_VAL_MAX);
            warc_fields['a'] = std::string(target_uri);
          } else if (strcmp("WARC-Truncated", key) == 0) {
            strncpy(warc_truncated, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Warcinfo-ID", key) == 0) {
            strncpy(warc_warcinfo_id, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Filename", key) == 0) {
            strncpy(warc_filename, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Profile", key) == 0) {
            strncpy(warc_profile, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Identified-Payload-Type", key) == 0) {
            strncpy(warc_identified_payload_type, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Segment-Number", key) == 0) {
            strncpy(warc_segment_number, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Segment-Origin-ID", key) == 0) {
            strncpy(warc_segment_origin_id, val, KEY_VAL_MAX);
          } else if (strcmp("WARC-Segment-Total-Length", key) == 0) {
            strncpy(warc_segment_total_length, val, KEY_VAL_MAX);
          }

        }

      }

      if (strcmp("response", warc_type) == 0) {

        pre = gztell(gzf);

        line = gzgets(gzf, buf, BUF_LEN);
        buf[strcspn(buf, "\r\n")] = '\0';
        strtok(buf, " ");
        strcpy(status, strtok(NULL, " "));
        warc_fields['s'] = std::string(status);

        while(strcmp("\r\n", line=gzgets(gzf, buf, BUF_LEN)) != 0) {
          buf[strcspn(buf, "\r\n")] = '\0';
          char *v = strnstr(buf, ": ", strlen(buf));
          if (v) {
            strncpy(val, (v+2), strlen(buf));
            key = strtok(buf, ":");
            if (strcmp("Content-Type", key) == 0) {
              strncpy(mime_type, val, KEY_VAL_MAX);
              no_space(mime_type);
              warc_fields['m'] = std::string(warc_type);
            } else if (strcmp("Location", key) == 0) {
              strncpy(redirect, val, KEY_VAL_MAX);
              warc_fields['r'] = std::string(redirect);
            }
          }
        }

        post = gztell(gzf);

        gzseek(gzf, (content_length - (post-pre+1)), SEEK_CUR);

        cur = 0;
        for (char c : field_spec) {
          cur++;
          cdx_file << warc_fields[c];
          if (cur < len) {
            cdx_file << " ";
          } else {
            cdx_file << std::endl;
          }
        }

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
