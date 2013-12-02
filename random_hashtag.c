#define PURPLE_PLUGINS

#include <glib.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"

#define PLUGIN_VERSION "0.1"


static void
sending_im_msg_cb(PurpleAccount *account, const char *receiver,
                  char **message) {

}

static gboolean
plugin_load(PurplePlugin *plugin) {
    void *convs_handle;
    convs_handle = purple_conversations_get_handle();

    purple_signal_connect(convs_handle, "sending-im-msg", plugin,
                          PURPLE_CALLBACK(sending_im_msg_cb), NULL);

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
    NULL,
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
