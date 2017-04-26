#include "extern.h"
#include <string.h>
#include <gdk/gdkkeysyms.h>

void *showmeaning(char *meaning);
gboolean display(GtkWidget *entryword,gpointer database);
gboolean searchword(GtkWidget *entryword,GdkEvent *event,gpointer listword);
int isPrefix(char *s,char *word);
