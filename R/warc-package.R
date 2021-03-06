#' Tools to Work with the Web Archive Ecosystem
#'
#' \href{http://www.digitalpreservation.gov/formats/fdd/fdd000236.shtml}{WARC} files (and
#' the metadata files that usually follow them) are the de facto method of archiving web
#' content.
#'
#' There are tools in Python & Java to work with this data and there are many "big data"
#' tools that make working with large-scale data from sites like
#' \href{http://commoncrawl.org/}{Common Crawl} and
#' \href{https://archive.org/index.php}{The Internet Archive} very straightforward.
#'
#' Now there are tools to create and work with the WARC ecosystem in R.
#'
#' Possible use-cases:
#'
#' \itemize{ \item{If you need to scrape data from many URLs and would like to make the
#' analyses on that data reproducible but are concerned that the sites may change format
#' or may be offline but also don't want to manage individual HTML (etc) files}
#' \item{Analyzing Common Crawl data (etc) natively in R} \item{Saving the entire state of
#' an \code{httr} request (\code{warc} can turn \code{httr} responses into WARC files and
#' turns WARC \code{response} records into \code{httr::response} objects)} }
#'
#' \code{warc} can work with WARC files that are composed of individual gzip streams or on
#' plaintext WARC files and can also read & generate CDX files. Support for more file
#' types (e.g. WET, WAT, etc) are planned.
#'
#' @name warc
#' @docType package
#' @author Bob Rudis (@@hrbrmstr)
#' @import purrr readr
#' @importFrom dplyr glimpse tbl_df data_frame left_join mutate filter select group_by do ungroup
#' @import stringi uuid
#' @importFrom httr insensitive
#' @useDynLib warc
#' @importFrom Rcpp sourceCpp
#' @importFrom scales comma
NULL
