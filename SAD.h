#include "extern.h"
#include <string.h>
#include <gdk/gdkkeysyms.h>

void *showmeaning(char *meaning);
gboolean display(GtkWidget *entryword,gpointer database);
gboolean searchword(GtkWidget *entryword,GdkEvent *event,gpointer listword);
int isPrefix(char *s,char *word);
void
name_insert_after (GtkEditable* edit,
                   gchar* new_text,
                   gint new_length,
                   gpointer position,
                   gpointer data);
void
name_delete_after (GtkEditable* edit,
                   gint start_pos,
                   gint end_pos,
                   gpointer data);