#' Convert an \code{httr::respone} object to WARC response objects
#'
#' @param x \code{httr} \code{response} object
#' @export
as_warc <- function(x) {

  if (inherits(x, "response")) {

    ah <- x$all_headers %||% x$headers
    uri <- x$request$url

    purrr::map(ah, function(h) {

      next_uri <- uri

      r_status <- sprintf("%s %s %s\r\n", h$version, h$status, http_statuses[as.character(h$status)])
      r_headers <- h$headers
      names(r_headers) <- stri_trans_totitle(names(h$headers))
      ro <- map2_chr(names(r_headers), r_headers, function(n, v) {
        if (h$status != "200") {
          if (n == "Content-Length") v <- 0
          if (n == "Location") next_uri <<- v
        }
        sprintf("%s: %s\r\n", n, v)
      })
      ro <- c(r_status, ro, "\r\n") # status / http response headers / CRLF
      ro <- paste0(ro, collapse="")

      cl <- nchar(ro)
      cnt <- NULL
      if (h$status == "200") {
        cl <- cl + length(x$content)
        cnt <- x$content
      }

      warc_version <- "WARC/1.0\r\n"
      w_headers <- list(`WARC-Type`="response",
                        `WARC-Target-URI`=uri,
                        `WARC-Date`=strftime(x$date, "%Y-%m-%dT%H:%M:%SZ", td="GMT"),
                        `Content-Type`="application/http;msgtype=response",
                        `WARC-Identified-Payload-Type`=x$headers$`content-type`,
                        `WARC-Record-ID`=sprintf("<urn:uuid:%s>", uuid::UUIDgenerate()),
                        `Content-Length`=cl)

      wo <- map2_chr(names(w_headers), w_headers, function(n, v) {
        sprintf("%s: %s\r\n", n, v)
      })
      wo <- c(warc_version, wo, "\r\n") # version / warc headers / CRLF
      wo <- paste0(wo, collapse="")

      ret <- list(rendered_warc_header=wo, rendered_response_header=ro, content=cnt)
      class(ret) <- c("warc", "warc_record", class(ret))

      uri <<- next_uri

      ret

    })

  }

}
