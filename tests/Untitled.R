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


