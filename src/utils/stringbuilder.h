// stringbuilder.h: a simple string builder from
// <https://nachtimwald.com/2017/02/26/efficient-c-string-builder/>
// licensed under the MIT license <https://opensource.org/licenses/MIT>
#ifndef __PVM_STRING_BUILDER_H__
#define __PVM_STRING_BUILDER_H__

#include <stddef.h>

struct _StringBuilder;
typedef struct _StringBuilder StringBuilder;

StringBuilder *str_builder_create ();
void str_builder_destroy (StringBuilder *sb);
void str_builder_add_str (StringBuilder *sb, const char *str, size_t len);
void str_builder_add_char (StringBuilder *sb, char c);
void str_builder_add_int (StringBuilder *sb, int val);
void str_builder_clear (StringBuilder *sb);
void str_builder_truncate (StringBuilder *sb, size_t len);
void str_builder_drop (StringBuilder *sb, size_t len);
size_t str_builder_len (const StringBuilder *sb);
const char *str_builder_peek (const StringBuilder *sb);
char *str_builder_dump (const StringBuilder *sb, size_t *len);

#endif /* __PVM_STRING_BUILDER_H__ */
