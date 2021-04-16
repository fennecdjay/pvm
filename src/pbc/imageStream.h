// imageStream.h: pbc image input stream
// written by apachejuice
// license information in LICENSE
#ifndef __pvm_imgstream_h__
#define __pvm_imgstream_h__
#include <inttypes.h>

typedef struct _ImageStream ImageStream;

ImageStream* img_stream_open (char* filename);
ImageStream* img_stream_load (int8_t* data, uint32_t length, char* inputname);
void img_stream_free (ImageStream* img_stream);

#endif
