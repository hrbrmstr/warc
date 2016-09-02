library(stringi)
library(purrr)
library(readr)
library(xml2)
library(httr)

cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))

# for (i in 1:nrow(cdx)) {

  i <- 7
  cat(sprintf("===>%d\n", i))

  p <- file.path(cdx$warc_path[i], cdx$file_name[i])
  st <- cdx$compressed_arc_file_offset[i]
  sz <- cdx$calc_compressed_warc_rec_size[i]

  entry <- read_warc_entry(p, st, sz)

# }


# str(entry)
#
# warc_headers(entry)
#
# text_content(entry)
# parsed_content(entry)
# content(entry, as="raw")
# headers(entry)
# status_code(entry)
# http_type(entry)
# http_error(entry)
