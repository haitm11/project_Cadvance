#include"ADS.h"
#include<string.h>

gboolean insert_word(GtkWidget *widget,gpointer entry){
  GtkWidget *dialog,*window=mainwindow;
  char word[50],meaning[10000];//bien meaning de luu nghia cua tu
  GtkTextIter start,end;
  strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
  gtk_text_buffer_get_start_iter(buffer,&start);
  gtk_text_buffer_get_end_iter(buffer,&end);
  strcpy(meaning,(char*)gtk_text_buffer_get_text(buffer,&start,&end,0));
  if (strlen(word)==0){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Bạn chưa nhập từ!");
    gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    gtk_dialog_run(GTK_DIALOG(dialog));
  }
  //doan nay la kiem tra xem tu co trung hay khong (em de dieu kien nay de test thoi)
  else if (strlen(word)==1){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Từ đã có trong cơ sở dữ liệu!Bạn muốn cập nhập nghĩa của từ này?");
    gtk_window_set_title(GTK_WINDOW(dialog),"Trùng từ");
    if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES)
        //ham cap nhat tu vao csdl
        printf("Linh!\n");
  }
  else{
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Bạn muốn thêm từ này vào cơ sở dữ liệu?");
    gtk_window_set_title(GTK_WINDOW(dialog),"Xác nhận");
    if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES) {
      //ham them tu vao co so du lieu
    	printf("Linh!\n");
    }
  }
  gtk_widget_destroy(dialog);
}

gboolean delete_word(GtkWidget *widget, gpointer entry){
  GtkWidget *dialog,*window=mainwindow;
  char word[50];
  strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
  if (strlen(word) == 0){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Bạn chưa nhập từ!");
    gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    gtk_dialog_run(GTK_DIALOG(dialog));
  }
  //neu co tu trong csdl, dieu kien trong if chi de test
  else if (strlen(word) == 1){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Bạn muốn xóa từ này khỏi cơ sở dữ liệu?");
    gtk_window_set_title(GTK_WINDOW(dialog),"Xác nhận");
    if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES) {
      //ham xoa tu trong csdl
      gtk_text_buffer_set_text(buffer,"",-1);
      gtk_entry_set_text(GTK_ENTRY(entry),"");
    }
  }
  else{
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Từ không có trong cơ sở dữ liệu!");
    gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    gtk_dialog_run(GTK_DIALOG(dialog));
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
