// imageStream.c: pbc image input stream
// written by apachejuice
// license information in LICENSE
#include "pbc/imageStream.h"
#include "utils/utils.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define magic_bytes 0xBA5EDDAF  // my bytecode is based af!

struct _ImageStream
{
    char* inputname;
    int8_t* input;
    uint32_t length;
};

ImageStream* img_stream_open (char* filename)
{
    FILE* fileptr;
    int8_t* buffer;
    uint32_t filelen;

    fileptr = fopen (filename, "rb");
    if (fileptr == NULL)
    {
        pvm_panicf ("Error reading file %s: %s", filename, strerror (errno));
    }

    fseek (fileptr, 0, SEEK_END);
    filelen = ftell (fileptr);
    rewind (fileptr);

    buffer = checked_malloc (filelen * sizeof (int8_t));
    fread (buffer, filelen, 1, fileptr);
    fclose (fileptr);
    return img_stream_load (buffer, filelen, filename);
}

ImageStream* img_stream_load (int8_t* data, uint32_t length, char* inputname)
{
    ImageStream* stream = checked_malloc (sizeof (ImageStream));
    stream->length      = length;
    stream->input       = data;
    stream->inputname   = inputname;
    return stream;
}
