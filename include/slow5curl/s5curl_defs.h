/*
MIT License

Copyright (c) 2023 Bonson Wong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef S5CURL_DEF
#define S5CURL_DEF

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/* s5curl errno */
int *s5curl_errno_location(void);
#define s5curl_errno (*s5curl_errno_location())

// error codes
#define S5CURL_ERR_OK           (0)     // Successfully completed
#define S5CURL_ERR_EOF          (-1)    // EOF reached
#define S5CURL_ERR_ARG          (-2)    // bad argument
#define S5CURL_ERR_TRUNC        (-3)    // file truncated, or size of header/record in blow5 differs to actual size read
#define S5CURL_ERR_RECPARSE     (-4)    // record parsing error
#define S5CURL_ERR_IO           (-5)    // other file I/O error (check errno for details)
#define S5CURL_ERR_NOIDX        (-6)    // index not loaded
#define S5CURL_ERR_NOTFOUND     (-7)    // read id not found
#define S5CURL_ERR_OTH          (-8)    // other error (big endian, internal error, etc.)
#define S5CURL_ERR_UNK          (-9)    // file format unknown
#define S5CURL_ERR_MEM          (-10)   // memory (re)allocation error
#define S5CURL_ERR_NOAUX        (-11)   // no auxiliary map
#define S5CURL_ERR_NOFLD        (-12)   // field not found
#define S5CURL_ERR_PRESS        (-13)   // (de)compression failure
#define S5CURL_ERR_MAGIC        (-14)   // magic number invalid
#define S5CURL_ERR_VERSION      (-15)   // version incompatible
#define S5CURL_ERR_HDRPARSE     (-16)   // header parsing error
#define S5CURL_ERR_TYPE         (-17)   // error relating to slow5 data type

#define S5CURL_ERR_SLOW5        (-18)    // Error processing s/blow5 data 
#define S5CURL_ERR_FETCH        (-19)    // Fetching data failed
#define S5CURL_ERR_CURL         (-20)    // Invalid CURL connection


// response codes
#define S5CURL_HTTP_OK      (200)   // HTTP successfully completed get
#define S5CURL_HTTP_PARTIAL (206)   // HTTP succesfully completed partial response
#define S5CURL_FTP_OK       (226)   // FTP successfully completed transfer, closing connection
#define S5CURL_FTP_PARTAL   (225)   // FTP successfully completed transfer, connection left open

#ifdef __cplusplus
}
#endif

#endif
