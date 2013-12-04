#define PURPLE_PLUGINS

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"
#include "debug.h"

#define PLUGIN_VERSION "0.1"

static const int MAX_LINELENGTH = 80;
static const char *DEBUG_NAME = "random_hashtag";
static const char *TAGS_FILENAME = ".random_hashtags";
static char **tag_list;
static size_t tag_list_size;

static char *
get_tags_filepath(const char *tags_filename) {
    const char *homedir = getenv("HOME");
    size_t filepath_length = strlen(homedir) + strlen(tags_filename) + 2;
    char *filepath = malloc(sizeof(char) * filepath_length);
    int written = snprintf(filepath, filepath_length, "%s/%s", homedir, tags_filename);
    if (written < 0) {
        free(filepath);
        return NULL;
    }

    return filepath;
}

static char **
expand_list(char **list, size_t old_size, size_t new_size) {
    list = realloc(list, sizeof(char *) * (new_size + 1));
    for (size_t i = old_size; i <= new_size; ++i) {
        list[i] = NULL;
    }
    return list;
}

static char **
read_tags_from_file(FILE *fp, size_t *out_count) {
    size_t max_linecount = 1;
    char **lines = calloc(max_linecount + 1, sizeof(char *));

    size_t linecount = 0;
    while (1) {
        char *line = malloc(sizeof(char) * MAX_LINELENGTH);
        if (fgets(line, MAX_LINELENGTH, fp) == NULL) {
            free(line);
            *out_count = linecount;
            return lines;
        }

        if (linecount == max_linecount) {
            size_t new_size = max_linecount * 2;
            lines = expand_list(lines, max_linecount, new_size);
            max_linecount = new_size;
        }

        lines[linecount] = line;
        ++linecount;
    }

    *out_count = linecount;
    return lines;
}

static void
free_tags_list(char **tags) {
    if (tags == NULL) {
        return;
    }

    for (int i = 0;; ++i) {
        if (tags[i] == NULL) {
            break;
        }
        free(tags[i]);
    }
    free(tags);
}

static char **
get_tag_list(const char *tags_filename, size_t *out_list_size) {
    char *filepath = get_tags_filepath(TAGS_FILENAME);
    purple_debug_info(DEBUG_NAME, "Tags filepath: '%s'\n", filepath);
    FILE *fp = fopen(filepath, "r");
    free(filepath);

    if (fp == NULL) {
        return NULL;
    }

    char **tags = read_tags_from_file(fp, out_list_size);
    fclose(fp);
    return tags;
}

static const char *
get_random_hashtag(void) {
    int index = rand() % tag_list_size;
    return tag_list[index];
}

static char *
add_random_hashtag_to_message(char *old_message) {
    const char *hashtag = get_random_hashtag();
    size_t hashtag_size = strlen(hashtag);
    size_t old_size = strlen(old_message);
    char *new_message = realloc(old_message, old_size + hashtag_size + 1);
    return strncat(new_message, hashtag, hashtag_size + 1);
}

static gboolean
writing_im_msg_cb(PurpleAccount *account, const char *who,
                  char **message, PurpleConversation *conv,
		  PurpleMessageFlags flags) {
    char *msg = *message;
    size_t length = strlen(msg);
    if (msg[length - 1] == '#' && tag_list != NULL) {
        *message = add_random_hashtag_to_message(msg);
    }

    return FALSE;
}

static gboolean
plugin_load(PurplePlugin *plugin) {
    void *convs_handle;
    purple_debug_info(DEBUG_NAME, "Loading plugin\n");
    convs_handle = purple_conversations_get_handle();

    purple_signal_connect(convs_handle, "writing-im-msg", plugin,
                          PURPLE_CALLBACK(writing_im_msg_cb), NULL);

    tag_list = get_tag_list(TAGS_FILENAME, &tag_list_size);
    purple_debug_info(DEBUG_NAME, "Read %lu tags from file\n", tag_list_size);

    return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin) {
    purple_debug_info(DEBUG_NAME, "Unloading plugin\n");
    free_tags_list(tag_list);
    return TRUE;
}

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

    "core-nablaa-random_hashtag",
    "Random hashtag",
    PLUGIN_VERSION,

    "Replaces # with a random hashtag",
    "When a message has #-character as the last character, it is replaced by a random hashtag",
    "Miika-Petteri Matikainen <miikapetteri@gmail.com>",
    "http://github.com/nablaa/pidgin-random-hashtag-plugin",

    plugin_load,
    plugin_unload,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static void
init_plugin(PurplePlugin *plugin) {
}

PURPLE_INIT_PLUGIN(leftify_tabs, init_plugin, info)
