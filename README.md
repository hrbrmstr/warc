
`warc` : Tools to Work with the Web Archive Ecosystem (WARC & CDX files)

The following functions are implemented:

-   `as_warc`: Convert an `httr::respone` object to WARC response objects
-   `create_cdx`: Create a CDX from a WARC file
-   `create_warc`: Use wget to create a WARC archive for a URL list
-   `expand`: Expand a compressed raw buffer
-   `find_sequence`: Find the first occurrence (if any) of a sequence of raw bytes (`pattern`) in `buffer`
-   `read_cdx`: Read a WARC CDX index file
-   `read_warc_entry`: Read a WARC entry from a WARC file
-   `warc_headers`: Extract WARC headers from a WARC response object
-   `create_warc` Use wget to create a WARC archive for a URL list
-   `write_warc_record`: Write a WARC record to a file

### Installation

``` r
devtools::install_git("https://gitlab.com/hrbrmstr/warc.git")
```

### Usage

``` r
library(warc)
library(httr)

# current verison
packageVersion("warc")
```

    ## [1] '0.1.0'

``` r
cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))

i <- 5

p <- file.path(cdx$warc_path[i], cdx$file_name[i])
st <- cdx$compressed_arc_file_offset[i]
sz <- cdx$calc_compressed_warc_rec_size[i]

entry <- read_warc_entry(p, st, sz)

print(entry)
```

    ## Response [https://r-project.org/]
    ##   Date: 2016-09-01 18:03
    ##   Status: 301
    ##   Content-Type: text/html; charset=iso-8859-1
    ##   Size: 314 B
    ## <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
    ## <html><head>
    ## <title>301 Moved Permanently</title>
    ## </head><body>
    ## <h1>Moved Permanently</h1>
    ## <p>The document has moved <a href="https://www.r-project.org/">here</a>.</p>
    ## <hr>
    ## <address>Apache/2.4.10 (Debian) Server at r-project.org Port 443</address>
    ## </body></html>

``` r
print(warc_headers(entry))
```

    ## $`warc-type`
    ## [1] "response"
    ## 
    ## $`warc-record-id`
    ## [1] "<urn:uuid:9EB1B94E-A929-4B5E-AE10-FB54C2E7308B>"
    ## 
    ## $`warc-warcinfo-id`
    ## [1] "<urn:uuid:7D7DC0CA-8FC7-4FFB-9D5A-4187EB50ED74>"
    ## 
    ## $`warc-concurrent-to`
    ## [1] "<urn:uuid:592019F2-C037-4899-8DB2-CC04266B9E29>"
    ## 
    ## $`warc-target-uri`
    ## [1] "https://r-project.org/"
    ## 
    ## $`warc-date`
    ## [1] "2016-09-01 18:03:46 UTC"
    ## 
    ## $`warc-ip-address`
    ## [1] "137.208.57.37"
    ## 
    ## $`warc-block-digest`
    ## [1] "\020"
    ## 
    ## $`warc-payload-digest`
    ## [1] ":\001"
    ## 
    ## $`content-type`
    ## [1] "application/http;msgtype=response"
    ## 
    ## $`content-length`
    ## [1] 578
    ## 
    ## attr(,"class")
    ## [1] "insensitive" "list"

``` r
print(status_code(entry))
```

    ## [1] 301

``` r
print(http_type(entry))
```

    ## [1] "text/html"

### Test Results

``` r
library(warc)
library(testthat)

date()
```

    ## [1] "Mon Sep  5 11:40:08 2016"

``` r
test_dir("tests/")
```

    ## testthat results ========================================================================================================
    ## OK: 0 SKIPPED: 0 FAILED: 0
    ## 
    ## DONE ===================================================================================================================
