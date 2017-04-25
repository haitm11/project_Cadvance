#include"SAD.h"

void *show_meaning(char *meaning){
  FILE *stream;
  GtkTextIter iter;
  char line[2000];
  static int created=0;
  // Ham chuyen 1 string tuong duong thanh 1 file
  stream=fmemopen(meaning,strlen(meaning)+1,"r");
  gtk_text_buffer_set_text(buffer,"",-1);
  gtk_text_buffer_get_start_iter(buffer,&iter);
  //Tag kieu chu
  if (!created){
    gtk_text_buffer_create_tag(buffer,"lmarg15","left_margin",15,NULL);
    gtk_text_buffer_create_tag(buffer,"lmarg30","left_margin",30,NULL);
    gtk_text_buffer_create_tag(buffer,"lmarg35","left_margin",35,NULL);
    gtk_text_buffer_create_tag(buffer,"bold","weight",PANGO_WEIGHT_BOLD,NULL);
    created=1;
  }
  //In nghia
  while (fgets(line,2000,stream)!=NULL){
    switch(line[0]){
    case '@':
      line[0]=' ';
      gtk_text_buffer_insert(buffer,&iter,"\n",-1);
      gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,line,-1,"bold",NULL);
      gtk_text_buffer_insert(buffer,&iter,"\n",-1);
      break;
    case '*':
      line[0]=' ';
      gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,line,-1,"lmarg15",NULL);
      break;
    case '!':
      line[0]=' ';
      gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,line,-1,"lmarg15",NULL);
      break;
    case '-':
      line[0]=' ';
      gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,line,-1,"lmarg30",NULL);
      break;
    case '=':
      line[0]=' ';
      gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,line,-1,"lmarg35",NULL);
      break;
    default:
      gtk_text_buffer_insert(buffer,&iter,line,-1);
      break;
    }
  }
}

gboolean display(GtkWidget *entryword,gpointer database){
  GtkWidget *dialog,*window=mainwindow;
  char word[50],mean[10000];
  int value;
  strcpy(word,(char *)gtk_entry_get_text(GTK_ENTRY(entryword)));
  if (strlen(word) == 0) {
    strcpy(mean,"Bạn chưa nhập từ vào ô tìm kiếm!");
    }
    else{
  if (strcmp(word,"linh")==0){ //doan nay la doan anh tim tu trong csdl
    strcpy(mean,"Xin chao Linh\n");
  }else{
    strcpy(mean,"Xin lỗi! Không tìm thấy từ này trong dữ liệu\n");
  }
  }
  show_meaning(mean);
}

int isPrefix(char *s,char *word){
  int len1,len2,i;
  len1=strlen(s);
  len2=strlen(word);
  if (len1 > len2) return 0;
  for (i=0;i<len1;i++){
    if (s[i]!=word[i])
      return 0;
    }
  return 1;
}

gboolean searchword(GtkWidget *entryword,GdkEvent *event,gpointer listword){
  GdkEventKey *key=(GdkEventKey *)event;
  GtkListStore *liststore=(GtkListStore *)listword;
  GtkTreeIter iter;
  int count=0,len;
  BTint value;//bien phuc vu cho ham xu ly tab(su dung thu vien btree)
  char text[50],near_word[50];
  //ham xu ly dau Tab
  if (key->keyval==GDK_KEY_Tab){
    strcpy(text,gtk_entry_get_text(GTK_ENTRY(entryword)));
    if (bfndky(data,text,&value)==QNOKEY){
      // ham bnxtky tim key gan nhat voi key muon tim (em dung thu vien libbt)
      bnxtky(data,near_word,&value);
      if (isPrefix(text,near_word)){
        gtk_entry_set_text(GTK_ENTRY(entryword),near_word);
        gtk_editable_set_position(GTK_EDITABLE(entryword),strlen(near_word));
      }
      else return TRUE;
    }
    display(entryword,NULL);
    return TRUE;
  }
//end xu ly tab

  else{
    strcpy(text,gtk_entry_get_text(GTK_ENTRY(entryword)));
    //ham xu ly cua anh, cai text la cai lay dk

    return FALSE;
  }
}