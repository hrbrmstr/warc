library(stringi)
library(purrr)
library(readr)
library(xml2)
library(httr)

cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))

for (i in 1:nrow(cdx)) {

  cat(sprintf("\n\n===>%d\n\n", i))

  p <- file.path(cdx$warc_path[i], cdx$file_name[i])
  st <- cdx$compressed_arc_file_offset[i]
  sz <- cdx$calc_compressed_warc_rec_size[i]

  entry <- read_warc_entry(p, st, sz)

  print(entry)
  print(warc_headers(entry))
  print(content(entry, as="raw"))
  print(content(entry, as="parsed", encoding="UTF-8"))
  print(content(entry, as="text", encoding="UTF-8"))
  print(headers(entry))
  print(status_code(entry))
  print(http_type(entry))
  print(http_error(entry))

}



library(warc)
library(httr)
library(dplyr)
library(purrr)
library(stringi)
library(readr)
library(xml2)

cdx <- read_cdx("~/Desktop/wg/alexa-20160902.cdx")

for (i in 1:nrow(cdx)) {

  cat(sprintf("\n\n===>%d\n\n", i))

  p <- file.path(cdx$warc_path[i], cdx$file_name[i])
  st <- cdx$compressed_arc_file_offset[i]
  sz <- cdx$calc_compressed_warc_rec_size[i]

  entry <- read_warc_entry(p, st, sz)

  print(entry)

}

extract_headers <- function(x) {

  p <- file.path(x$warc_path, x$file_name)
  st <- x$compressed_arc_file_offset
  sz <- x$calc_compressed_warc_rec_size

  entry <- read_warc_entry(p, st, sz)

  data_frame(original_url=x$original_url,
             header=names(entry$headers),
             value=flatten_chr(entry$headers))

}

filter(cdx, response_code==301) %>%
  rowwise() %>%
  do(extract_headers(.)) %>%
  ungroup() -> df

glimpse(df)

count(df, header) %>%
  arrange(desc(n))

# create_warc(url_list="http://journal.code4lib.org",
#             warc_path="~/data/journal",
#             warc_file="journal",
#             no_output=FALSE,
#             .opts=c('--level=2', "--recursive"))
