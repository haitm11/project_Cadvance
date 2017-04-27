#include "extern.h"
int AddWordToDic(BTA *Dic,char *w,char *m);
void FileDeleteRow(char* filename,char* word);
gboolean insert_word(GtkWidget *widget,gpointer entry);
gboolean delete_word(GtkWidget *widget,gpointer entry);
gboolean search_word(GtkWidget *button,gpointer database);
gboolean delete_event(GtkWidget *widget,GdkEvent *event,gpointer database);
gboolean clearAll(GtkWidget *widget,gpointer database);
int soundEx(char *SoundEx,
              char *WordString,
              int   LengthOption,
              int   CensusOption);
