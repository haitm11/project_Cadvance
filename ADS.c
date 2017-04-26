#include "ADS.h"
#include "SAD.h"
#include <string.h>
#define MAXLEN 100
extern char source_text_filename[MAXLEN];
int AddWordToDic(BTA *Dic,char *w,char *m)
{
    int rsize,k=0,i;
    if(btsel(Dic,w,m,MAXLEN*sizeof(char),&rsize)==0) {
        return 0;
    } else {
        btins(Dic,w,m,MAXLEN*sizeof(char)); 
    }
    return 1;
}
void FileDeleteRow(char* filename,char* word){
  FILE *fp1,*fp2;
  char c,buffer[201];
  int i=0,position;  
  fp1 = fopen(filename, "r");  
  fp2 = fopen("copy.c", "w");  
  c = getc(fp1);
  while (c != EOF) {    
    if(c=='#') position=i;
    buffer[i++]=c;    
    if (c == '\n'){
      buffer[i]='\0';      
      buffer[position]='\0';
      if (strcmp(word,buffer)!=0){
        buffer[position]='#';        
        fputs(buffer,fp2);
      }      
      i=0;
    }      
    c = getc(fp1);
  }
  //close both the files.
  fclose(fp1);
  fclose(fp2);
  //remove original file
  remove(filename);
  //rename the file copy.c to original name
  rename("copy.c", filename);
}
gboolean insert_word(GtkWidget *widget,gpointer entry){
  GtkWidget *dialog,*window=mainwindow;
  char word[MAXLEN],meaning[MAXLEN];//bien meaning de luu nghia cua tu
  GtkTextIter start,end;
  strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
  gtk_text_buffer_get_start_iter(buffer,&start);
  gtk_text_buffer_get_end_iter(buffer,&end);
  strcpy(meaning,(char*)gtk_text_buffer_get_text(buffer,&start,&end,0));
  if (strlen(word)==0){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Bạn chưa nhập tên!");
    gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    gtk_dialog_run(GTK_DIALOG(dialog));
  }  
  else{
  	int rsize;
  	if(btsel(data,word,meaning,MAXLEN*sizeof(char),&rsize)==0) {
      	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Tên đã có trong cơ sở dữ liệu!");
    	gtk_window_set_title(GTK_WINDOW(dialog),"Trùng tên");
    } 
    else{
    	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Bạn muốn thêm tên này vào cơ sở dữ liệu?");
	    gtk_window_set_title(GTK_WINDOW(dialog),"Xác nhận");
	    if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES) {
	      	btins(data,word,meaning,MAXLEN*sizeof(char));
	      	FILE *file = fopen(source_text_filename,"r+");
	        fseek(file, 0, SEEK_END);
	        fprintf(file,"%s#%s\n",word,meaning);
	        fclose(file);
	    }
    }  
  }
  gtk_widget_destroy(dialog);
}

gboolean delete_word(GtkWidget *widget, gpointer entry){
  GtkWidget *dialog,*window=mainwindow;
  char word[MAXLEN],meaning[MAXLEN];
  strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
  if (strlen(word) == 0){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Bạn chưa nhập tên!");
    gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    gtk_dialog_run(GTK_DIALOG(dialog));
  }  
  else{
  	int rsize;
  	if(btsel(data,word,meaning,MAXLEN*sizeof(char),&rsize)==0) {
      	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Bạn muốn xóa tên này khỏi cơ sở dữ liệu?");
    	gtk_window_set_title(GTK_WINDOW(dialog),"Xác nhận");
    	if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES) {
	      	if(btdel(data, word) == 0) {	          
	          FileDeleteRow(source_text_filename,word);
	        }	        
	        gtk_text_buffer_set_text(buffer,"",-1);
      		gtk_entry_set_text(GTK_ENTRY(entry),"");
	    }
    } 
    else{	    
	    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Tên không có trong cơ sở dữ liệu!");
    	gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    	gtk_dialog_run(GTK_DIALOG(dialog));
    }
  }
  gtk_widget_destroy(dialog);
  return FALSE;
}
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
