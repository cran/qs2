qs2
================

[![R-CMD-check](https://github.com/qsbase/qs2/workflows/R-CMD-check/badge.svg)](https://github.com/qsbase/qs2/actions)
[![CRAN-Status-Badge](https://www.r-pkg.org/badges/version/qs2)](https://cran.r-project.org/package=qs2)
[![CRAN-Downloads-Badge](https://cranlogs.r-pkg.org/badges/qs2)](https://cran.r-project.org/package=qs2)
[![CRAN-Downloads-Total-Badge](https://cranlogs.r-pkg.org/badges/grand-total/qs2)](https://cran.r-project.org/package=qs2)

*qs2: a framework for efficient serialization*

`qs2` is the successor to the `qs` package. The goal is to have reliable
and fast performance for saving and loading objects in R.

The `qs2` format directly uses R serialization (via the
`R_Serialize`/`R_Unserialize` C API) while improving underlying
compression and disk IO patterns. If you are familiar with the `qs`
package, the benefits and usage are the same.

``` r
qs_save(data, "myfile.qs2")
data <- qs_read("myfile.qs2")
```

Use the file extension `qs2` to distinguish it from the original `qs`
package. It is not compatible with the original `qs` format.

## Installation

``` r
install.packages("qs2")
```

On x64 Mac or Linux, you can enable multi-threading by compiling from
source. It is enabled by default on Windows.

``` r
remotes::install_cran("qs2", type = "source", configure.args = "--with-TBB --with-simd=AVX2")
```

On non-x64 systems (e.g. Mac ARM) remove the AVX2 flag.

``` r
remotes::install_cran("qs2", type = "source", configure.args = "--with-TBB")
```

Multi-threading in `qs2` uses the `Intel Thread Building Blocks`
framework via the `RcppParallel` package.

## Converting qs2 to RDS

Because the `qs2` format directly uses R serialization, you can convert
it to RDS and vice versa.

``` r
file_qs2 <- tempfile(fileext = ".qs2")
file_rds <- tempfile(fileext = ".RDS")
x <- runif(1e6)

# save `x` with qs_save
qs_save(x, file_qs2)

# convert the file to RDS
qs_to_rds(input_file = file_qs2, output_file = file_rds)

# read `x` back in with `readRDS`
xrds <- readRDS(file_rds)
stopifnot(identical(x, xrds))
```

## Validating file integrity

The `qs2` format saves an internal checksum. This can be used to test
for file corruption before deserialization via the `validate_checksum`
parameter, but has a minor performance penalty.

``` r
qs_save(data, "myfile.qs2")
data <- qs_read("myfile.qs2", validate_checksum = TRUE)
```

# The qdata format

The package also introduces the `qdata` format which has its own
serialization layout and works with only data types (vectors, lists,
data frames, matrices).

It will replace internal types (functions, promises, external pointers,
environments, objects) with NULL. The `qdata` format differs from the
`qs2` format in that it is NOT a general.

The eventual goal of `qdata` is to also have interoperability with other
languages, particularly `Python`.

``` r
qd_save(data, "myfile.qs2")
data <- qd_read("myfile.qs2")
```

## Benchmarks

A summary across 4 datasets is presented below.

#### Single-threaded

| Algorithm       | Compression | Save Time (s) | Read Time (s) |
| --------------- | ----------- | ------------- | ------------- |
| qs2             | 7.96        | 13.4          | 50.4          |
| qdata           | 8.45        | 10.5          | 34.8          |
| base::serialize | 1.1         | 8.87          | 51.4          |
| saveRDS         | 8.68        | 107           | 63.7          |
| fst             | 2.59        | 5.09          | 46.3          |
| parquet         | 8.29        | 20.3          | 38.4          |
| qs (legacy)     | 7.97        | 9.13          | 48.1          |

#### Multi-threaded (8 threads)

| Algorithm   | Compression | Save Time (s) | Read Time (s) |
| ----------- | ----------- | ------------- | ------------- |
| qs2         | 7.96        | 3.79          | 48.1          |
| qdata       | 8.45        | 1.98          | 33.1          |
| fst         | 2.59        | 5.05          | 46.6          |
| parquet     | 8.29        | 20.2          | 37.0          |
| qs (legacy) | 7.97        | 3.21          | 52.0          |

  - `qs2`, `qdata` and `qs` with `compress_level = 3`
  - `parquet` via the `arrow` package using zstd `compression_level = 3`
  - `base::serialize` with `ascii = FALSE` and `xdr = FALSE`

**Datasets used**

  - `1000 genomes non-coding VCF` 1000 genomes non-coding variants (2743
    MB)
  - `B-cell data` B-cell mouse data, Greiff 2017 (1057 MB)
  - `IP location` IPV4 range data with location information (198 MB)
  - `Netflix movie ratings` Netflix ML prediction dataset (571 MB)

These datasets are openly licensed and represent a combination of
numeric and text data across multiple domains. See
`inst/analysis/datasets.R` on Github.

# Usage in C/C++

Serialization functions can be accessed in compiled code. Below is an
example using Rcpp.

``` cpp
// [[Rcpp::depends(qs2)]]
#include <Rcpp.h>
#include "qs2_external.h"
using namespace Rcpp;

// [[Rcpp::export]]
SEXP test_qs_serialize(SEXP x) {
  size_t len = 0;
  unsigned char * buffer = c_qs_serialize(x, &len, 10, true, 4); // object, buffer length, compress_level, shuffle, nthreads
  SEXP y = c_qs_deserialize(buffer, len, false, 4);              // buffer, buffer length, validate_checksum, nthreads
  c_qs_free(buffer);                                             // must manually free buffer
  return y;
}

// [[Rcpp::export]]
SEXP test_qd_serialize(SEXP x) {
  size_t len = 0;
  unsigned char * buffer = c_qd_serialize(x, &len, 10, true, 4); // object, buffer length, compress_level, shuffle, nthreads
  SEXP y = c_qd_deserialize(buffer, len, false, false, 4);       // buffer, buffer length, use_alt_rep, validate_checksum, nthreads
  c_qd_free(buffer);                                             // must manually free buffer
  return y;
}


/*** R
x <- runif(1e7)
stopifnot(test_qs_serialize(x) == x)
stopifnot(test_qd_serialize(x) == x)
*/
```

# Global Options for qs2

The following global options control the behavior of the `qs2`
functions. These global options can be queried or modified using `qopt`
function.

  - **compress\_level**  
    The default compression level used when compressing data.  
    **Default:** `3L`

  - **shuffle**  
    A logical flag indicating whether to allow byte shuffling during
    compression.  
    **Default:** `TRUE`

  - **nthreads**  
    The number of threads used for compression and decompression.  
    **Default:** `1L`

  - **validate\_checksum**  
    A logical flag indicating whether to validate the stored checksum
    when reading data.  
    **Default:** `FALSE`

  - **warn\_unsupported\_types**  
    For `qd_save`, a logical flag indicating whether to warn when saving
    an object with unsupported types.  
    **Default:** `TRUE`

  - **use\_alt\_rep**  
    For `qd_read`, a logical flag indicating whether to use ALTREP when
    reading in string data.  
    **Default:** `FALSE`

-----
