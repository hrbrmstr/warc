#' Read a WARC entry from a WARC file
#'
#' @param path path to WARC file
#' @param start starting offset of WARC record
#' @param size total size of WARC record
#' @param compressed is WARC file compressed? (guessed by file name suffix)
#' @export
#' @examples \dontrun{
#' cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))
#' i <- 1
#' p <- file.path(cdx$warc_path[i], cdx$file_name[i])
#' st <- cdx$compressed_arc_file_offset[i]
#' sz <- cdx$calc_compressed_warc_rec_size[i]
#'
#' (read_warc_entry(p, st, sz))
#' }
read_warc_entry <- function(path, start, size, compressed=grepl("gz$", path)) {

  fil <- file(path, "rb")
  seek(fil, start)
  buffer <- readBin(fil, "raw", size)
  if (compressed) buffer <- expand(buffer);
  close(fil)
  process_entry(buffer)

}

#' Extract WARC headers from a WARC response object
#'
#' @param x WARC response object
#' @return list of WARC headers
#' @export
warc_headers <- function(x) { x$warc_header }

process_entry <- function(buffer) {

  pos <- find_sequence(buffer, charToRaw("\r\n\r\n"))
  warc_info <- buffer[1:(pos-1)]
  warc_info <- rawToChar(warc_info)
  warc_entries <- stri_split_regex(warc_info, "\r\n")[[1]]

  if (substr(warc_entries[1], 1, 5) != "WARC/") {
    warning("Invalid WARC record")
    return(NULL)
  }

  stri_split_fixed(warc_entries[-1], ": ", 2) %>%
    map(~setNames(list(.[2]), .[1])) %>%
    flatten_df() %>%
    type_convert() %>%
    as.list() -> warc_headers

  warc_headers <- httr::insensitive(warc_headers)

  start <- pos+4+1
  size <- warc_headers$`Content-Length`[1]

  response <- buffer[start:length(buffer)]

  cpos <- find_sequence(response, charToRaw("\r\n\r\n"))
  http_headers <- iconv(readBin(response[1:(cpos-1)], character()), to="UTF-8")

  headers <- stri_split_regex(http_headers, "\r?\n")[[1]]

  http_resp <- headers[1]
  status <- as.list(stri_split_regex(http_resp, "[[:space:]]+", 3)[[1]])
  names(status) <- c("version", "status", "message")[seq_along(status)]
  status$status <- as.integer(status$status)

  stri_split_fixed(headers[-1], ": ", 2) %>%
    map(~setNames(list(.[2]), .[1])) %>%
    flatten_df() %>%
    type_convert() %>%
    as.list() -> headers

  headers <- httr::insensitive(headers)

  if (rawToChar(response[(length(response)-3):length(response)]) == "\r\n\r\n") {
    from_end <- 4
  } else {
    from_end <- 0
  }

  content <- response[(cpos+4):(length(response)-from_end)]

  ret <- list(url=warc_headers$`warc-target-uri`,
              status_code=status$status,
              warc_header=warc_headers,
              status=status,
              headers=headers,
              content=content)

  if (length(headers$date) > 0) {
    ret$date <- httr::parse_http_date(headers$date)
  }

  class(ret) <- c("warc_response", "response", class(ret))
  ret

}