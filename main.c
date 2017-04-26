#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "btree.h"
#include <string.h>
#include "ADS.h"
#include "SAD.h"
#include "dialog.h"
#define MAXLEN 100

BTA *data;
GtkWidget *textView,*mainwindow;
GtkTextBuffer *buffer;

// Tao button voi icon va label
GtkWidget* gtk_button_with_icon_and_label(char* label,char* fileimage){
  GtkWidget *button,*image;
  image=gtk_image_new();
  gtk_image_set_from_file(GTK_IMAGE(image),fileimage);
  button=gtk_button_new_with_label(label);
  gtk_button_set_image((GtkButton *)button,image);
  gtk_button_set_image_position((GtkButton *)button,GTK_POS_LEFT);
  return button;
}

//Tao icon ghep vao button
GdkPixbuf *create_pixbuf(const gchar *filename){
  GdkPixbuf *pixbuf;
  GError *error=NULL;
  pixbuf=gdk_pixbuf_new_from_file(filename,&error);
  if (!pixbuf){
    fprintf(stderr,"%s\n",error->message);
    g_error_free(error);
  }
  return pixbuf;
}
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
int main(int argc,char *argv[]){
  GtkWidget *window,*mainvbox,*hbox,*vbox;
  GtkWidget *button,*entry,*label,*frame;
  GtkWidget *ins,*del,*gui,*info,*clear;
  // Khoi tao giao dien
  gtk_init(&argc,&argv);
  //neu anh co them data thi them o day nhe!
  char word[MAXLEN], mean[MAXLEN],btree_filename[10]="testBTree",source_text_filename[100]="test.txt";
  FILE *f;  

  btinit();
  //co the truyen file text theo tham so dong lenh:
  if(argc >= 3) {
    strcpy(source_text_filename,argv[1]);
    strcpy(btree_filename,argv[2]);
  }
  data = btopn(btree_filename,0,0);   
  if(data == NULL) data = btcrt(btree_filename,0,FALSE);  
  f=fopen(source_text_filename,"r");
  if(!f) {printf("Cannot open file %s \n",source_text_filename );exit(1);}
  while(!feof(f)) {
      fscanf(f,"%[^#]#%[^\n]\n",word,mean);
      AddWordToDic(data,word,mean);
    }
  fclose(f);

  window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(window,800,600);
  gtk_container_set_border_width(GTK_CONTAINER(window),10);
  gtk_window_set_title(GTK_WINDOW(window),"Từ điển tên - email");
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
  gtk_window_set_icon(GTK_WINDOW(window),create_pixbuf("main.png"));
  g_signal_connect(window,"delete-event",G_CALLBACK(delete_event),NULL);
  mainwindow=window;
  // Ket thuc khoi tao

  //Tao box chinh cho tu dien
  mainvbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_container_add(GTK_CONTAINER(window),mainvbox);

  // Tao box nhap tu va chon tu dien
  // Tao frame nhap tu
  frame=gtk_frame_new("Nhập tên");
  gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_IN);
  //Tao box
  hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

  GtkEntryCompletion *completion=gtk_entry_completion_new();
  gtk_entry_completion_set_text_column(completion,0);
  GtkListStore *liststore=gtk_list_store_new(5,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
  gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(liststore));

  entry=gtk_search_entry_new();
  gtk_entry_set_completion(GTK_ENTRY(entry),completion);
  g_signal_connect(entry,"activate",G_CALLBACK(display),NULL);
  g_signal_connect(entry,"key-press-event",G_CALLBACK(searchword),liststore);
  gtk_box_pack_start(GTK_BOX(hbox),entry,TRUE,TRUE,0);

  GtkSettings *default_settings=gtk_settings_get_default();
  g_object_set(default_settings,"gtk-button-images",TRUE,NULL);

  button=gtk_button_with_icon_and_label("Tra cứu","search.ico");
  g_signal_connect(button,"clicked",G_CALLBACK(search_word),(gpointer)entry);
  gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,0);

  gtk_container_add(GTK_CONTAINER(frame),hbox);
  gtk_box_pack_start(GTK_BOX(mainvbox),frame,FALSE,FALSE,0);
  //

  // Tao box cho nghia va tac dung khac
  hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  vbox=gtk_box_new(TRUE,1);

  ins=gtk_button_with_icon_and_label("Thêm từ","add.ico");
  g_signal_connect(ins,"clicked",G_CALLBACK(insert_word),(gpointer)entry);

  del=gtk_button_with_icon_and_label("Xóa từ","delete.ico");
  g_signal_connect(del,"clicked",G_CALLBACK(delete_word),(gpointer)entry);

  gui=gtk_button_with_icon_and_label("Hướng dẫn","huongdan.ico");
  g_signal_connect(gui,"clicked",G_CALLBACK(show_guide),(gpointer)entry);

  info=gtk_button_with_icon_and_label("Thông tin","thongtin.ico");
  g_signal_connect(info,"clicked",G_CALLBACK(show_info),(gpointer)entry);

  clear=gtk_button_with_icon_and_label("Khôi phục","clear.jpg");
  g_signal_connect(clear,"clicked",G_CALLBACK(clearAll),(gpointer)entry);

  gtk_box_pack_start(GTK_BOX(vbox),ins,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),del,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),gui,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),info,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),clear,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);
  //Tao hop hien nghia
  GtkWidget* scrolledWindow=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_set_border_width(GTK_CONTAINER (scrolledWindow),10);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
  textView=gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD);
  printf("xin chao!");
  buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
  gtk_text_buffer_set_text(buffer,"Xin chào!",-1);
  gtk_container_add(GTK_CONTAINER(scrolledWindow),textView);
  gtk_box_pack_start(GTK_BOX(hbox),scrolledWindow,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(mainvbox),hbox,TRUE,TRUE,0);
  //

  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}
