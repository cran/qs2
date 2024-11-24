#ifndef QS2_EXTERNAL_H
#define QS2_EXTERNAL_H

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <Rconfig.h>
#include <R_ext/Rdynload.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned char * c_qs_serialize(SEXP object, size_t * len, const int compress_level = 3, const bool shuffle = true, const int nthreads = 1) {
  static unsigned char *(*fun)(SEXP, size_t *, const int, const bool, const int) = (unsigned char *(*)(SEXP, size_t *, const int, const bool, const int)) R_GetCCallable("qs2", "c_qs_serialize");
  return fun(object, len, compress_level, shuffle, nthreads);
}
bool c_qs_free(void *buffer) {
  static bool (*fun)(void *) = (bool (*)(void *)) R_GetCCallable("qs2", "c_qs_free");
  return fun(buffer);
}
SEXP c_qs_deserialize(const unsigned char * buffer, const size_t len, const bool validate_checksum = false, const int nthreads = 1) {
  static SEXP(*fun)(const unsigned char *, const size_t, const bool, const int) = (SEXP(*)(const unsigned char *, const size_t, const bool, const int)) R_GetCCallable("qs2", "c_qs_deserialize");
  return fun(buffer, len, validate_checksum, nthreads);
}
unsigned char * c_qd_serialize(SEXP object, size_t * len, const int compress_level = 3, const bool shuffle = true, const bool warn_unsupported_types = true, const int nthreads = 1) {
  static unsigned char *(*fun)(SEXP, size_t *, const int, const bool, const bool, const int) = (unsigned char *(*)(SEXP, size_t *, const int, const bool, const bool, const int)) R_GetCCallable("qs2", "c_qd_serialize");
  return fun(object, len, compress_level, shuffle, warn_unsupported_types, nthreads);
}
bool c_qd_free(void *buffer) {
  static bool (*fun)(void *) = (bool (*)(void *)) R_GetCCallable("qs2", "c_qd_free");
  return fun(buffer);
}
SEXP c_qd_deserialize(const unsigned char * buffer, const size_t len, const bool use_alt_rep = false, const bool validate_checksum = false, const int nthreads = 1) {
  static SEXP(*fun)(const unsigned char *, const size_t, const bool, const bool, const int) = (SEXP(*)(const unsigned char *, const size_t, const bool, const bool, const int)) R_GetCCallable("qs2", "c_qd_deserialize");
  return fun(buffer, len, use_alt_rep, validate_checksum, nthreads);
}

#ifdef __cplusplus
}
#endif

#endif // include guard