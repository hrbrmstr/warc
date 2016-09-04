process_request <- function(request, warc_headers) {

  cpos <- find_sequence(request, charToRaw("\r\n\r\n"))
  request_headers <- iconv(readBin(request[1:(cpos-1)], character()), sub="byte", to="UTF-8")

  headers <- stri_split_regex(request_headers, "\r?\n")[[1]]

  http_req <- headers[1]
  req <- as.list(stri_split_regex(http_req, "[[:space:]]+", 3)[[1]])
  names(req) <- c("method", "uri", "version")[seq_along(req)]

  stri_split_fixed(headers[-1], ": ", 2) %>%
    map(~setNames(list(.[2]), .[1])) %>%
    flatten_df() -> headers

  headers <- as.list(headers)
  headers <- httr::insensitive(headers)

  ret <- list(warc_header=warc_headers,
              method=req$method,
              url=warc_headers$`warc-target-uri`,
              request_uri=req$uri,
              http_version=req$version,
              headers=headers,
              fields=NULL,
              options=list(httpget=(req$method=="GET")),
              auth_token=NULL,
              output=NULL)

  if (length(headers[["user-agent"]]) != 0) {
    ret$options$useragent <- headers[["user-agent"]]
  }

  class(ret) <- c("warc", "request", class(ret))
  ret

}