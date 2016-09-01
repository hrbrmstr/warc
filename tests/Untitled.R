cdx <- read_cdx("~/Desktop/warc/20160901.cdx")

i <- 3
(p <- file.path(cdx$warc_path[i], cdx$file_name[i]))
(st <- cdx$compressed_arc_file_offset[i])
(sz <- cdx$calc_compressed_warc_rec_size[i])

buffer <- read_warc_entry(p, st, sz)


library(stringi)
library(purrr)

pos <- stri_locate_first_regex(buffer, "\r\n\r\n")
warc_info <- substr(buffer, 1, pos-1)
warc_entries <- stri_split_regex(warc_info, "\r\n")[[1]]

if (substr(warc_entries[1], 1, 5) != "WARC/") { message("blargh") }

stri_split_fixed(warc_entries[-1], ": ", 2) %>%
  map(~setNames(list(.[2]), .[1])) %>%
  flatten_df() -> warc_header

start <- pos[,2]+1
size <- warc_header$`Content-Length`[1]

content <- substr(buffer, start, size)

