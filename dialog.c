#include"dialog.h"

gboolean show_info(GtkWidget *widget,gpointer database){
  GdkPixbuf *pixbuf=gdk_pixbuf_new_from_file("thongtin.ico",NULL);
  GtkWidget *dialog=gtk_about_dialog_new();
  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),"Từ điển tên-email");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),"Version 0.0");
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Việt Nhật K58\nNguyen Dinh Hung 20131879\nTran Minh Hai 20131271\nTran Thi Dieu Linh 20132320\nNguyen Dinh Chinh 20111243\nTran Dinh Tai 2015xxxx\n");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),"Chương trình được tham khảo code từ trang zetcode.com\n");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog),pixbuf);
  g_object_unref(pixbuf),pixbuf=NULL;
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

gboolean show_guide(GtkWidget *widget,gpointer database){
  GtkWidget *dialog,*window;
  window=mainwindow;
  dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"1. Để tra cứu, nhập tên sau đó nhấn Enter hoặc click vào nút Tra cứu\n2. Để đưa về màn hình ban đầu, click vào nút Khôi phục\n");
  gtk_window_set_title(GTK_WINDOW(dialog),"Hướng dẫn sử dụng");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
