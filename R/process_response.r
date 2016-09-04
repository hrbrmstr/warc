process_response <- function(response, warc_headers) {

  cpos <- find_sequence(response, charToRaw("\r\n\r\n"))
  http_headers <- iconv(readBin(response[1:(cpos-1)], character()), sub="byte", to="UTF-8")

  headers <- stri_split_regex(http_headers, "\r?\n")[[1]]

  http_resp <- headers[1]
  status <- as.list(stri_split_regex(http_resp, "[[:space:]]+", 3)[[1]])
  names(status) <- c("version", "status", "message")[seq_along(status)]
  status$status <- as.integer(status$status)

  stri_split_fixed(headers[-1], ": ", 2) %>%
    map(~setNames(list(.[2]), .[1])) %>%
    flatten_df() -> headers

  headers <- as.list(headers)
  headers <- httr::insensitive(headers)

  if ((length(headers$date) > 0) && (!is.na(headers$date))) {
    headers$date <- httr::parse_http_date(headers$date)
  }

  if ((length(headers[["content-type"]]) > 0) && (headers[["content-type"]] == "")) {
    headers["content-type"] <- "application/http"
  }

  if ((length(headers[["content-length"]]) > 0) && (headers[["content-length"]] == 0)) {
    content <- raw()
  } else {

    if (rawToChar(response[(length(response)-3):length(response)]) == "\r\n\r\n") {
      from_end <- 4
    } else {
      from_end <- 0
    }

    content <- response[(cpos+4):(length(response)-from_end)]

  }

  ret <- list(url=warc_headers$`warc-target-uri`,
              status_code=status$status,
              warc_header=warc_headers,
              status=status,
              headers=headers,
              content=content)

  if ((length(headers$date) > 0) && (!is.na(headers$date))) {
    ret$date <- headers$date
  } else {
    ret$date <- as.POSIXct(NA)
  }

  class(ret) <- c("warc", "response", class(ret))
  ret

}