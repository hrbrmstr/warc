
`warc` : ...

The following functions are implemented:

The following data sets are included:

### Installation

``` r
devtools::install_github("hrbrmstr/warc")
```

### Usage

``` r
library(warc)

# current verison
packageVersion("warc")
```

    ## [1] '0.1.0'

### Test Results

``` r
library(warc)
library(testthat)

date()
```

    ## [1] "Thu Sep  1 18:54:35 2016"

``` r
test_dir("tests/")
```

    ## testthat results ========================================================================================================
    ## OK: 0 SKIPPED: 0 FAILED: 0
    ## 
    ## DONE ===================================================================================================================
