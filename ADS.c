#include"ADS.h"
#include<string.h>

gboolean search_word(GtkWidget *button,gpointer database){
  display(GTK_WIDGET(database),NULL);
  return TRUE;
}

gboolean delete_event(GtkWidget *widget,GdkEvent *event,gpointer database){
    gtk_main_quit();
    return FALSE;
}

gboolean clearAll(GtkWidget *widget,gpointer database){
    gtk_text_buffer_set_text(buffer,"Xin chào!", -1);
    gtk_entry_set_text(GTK_ENTRY(database),"");
    return FALSE;
}
