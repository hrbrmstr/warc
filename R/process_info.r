process_info <- function(info, warc_headers) {

  cpos <- find_sequence(info, charToRaw("\r\n\r\n"))
  warc_info <- iconv(readBin(info[1:(cpos-1)], character()), sub="byte", to="UTF-8")


  ret <- list(warc_header=warc_headers,
              content=warc_info)

  class(ret) <- c("warc", "info", class(ret))
  ret

}