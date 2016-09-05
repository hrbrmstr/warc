#' Convert an httr::resspone object to WARC request/response objects
#'
#' @export
as_warc <- function(x) {
  if (inherits(x, "request") || inherits(x, "response")) {
    if (inherits(x, "response")) {

      ah <- x$all_headers

      purrr::map_chr(ah, function(h) {

        r_status <- sprintf("%s %s %s\r\n", h$version, h$status, http_statuses[as.character(h$status)])
        r_headers <- h$headers
        names(r_headers) <- stri_trans_totitle(names(h$headers))
        o <- map2_chr(names(r_headers), r_headers, function(n, v) {
          sprintf("%s: %s\r\n", n, v)
        })
        o <- c(r_status, o, "\r\n")
        o <- paste0(o, collapse="")
        o

        cl <- nchar(o)
        if (h$status == 200) cl <- cl + length(x$content)

        # WARC/1.0
        wh <- list(`WARC-Tye`="response",
                   `WARC-Date`=strftime(x$date, "%Y-%m-%dT-%H:%M:%sZ", td="GMT"),
                   `Content-Type`="application/http;msgtype=response",
                   `WARC-Identified-Payload-Type`=x$headers$`content-type`,
                   `WARC-Record-ID`=sprintf("urn:uuid:%s", uuid::UUIDgenerate()),
                   `Content-Length`=)

      }) -> ho

    }
  }
}