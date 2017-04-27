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
int AddWordToSou(BTA *Dic,char *w,char *m)
{
    int rsize,k=0,i;
    char om[MAXLEN];
    if(btsel(Dic,w,om,MAXLEN*sizeof(char),&rsize)==0) {
        
        if(strstr(om,m)==NULL && (strlen(om)+strlen(m))<MAXLEN){
          strcat(om,"\n");
          strcat(om,m);
          btupd(Dic,w,om,MAXLEN*sizeof(char));
        }
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
char* createBtreeFilename(){
  char *str=strdup(source_text_filename);
  char *p=str;
  do{
    if( *p=='.' && !(*(p+1)=='/' || *(p+1)=='.' && *(p+2)=='/') ) break;
    p++;
  }while(*p!='\0');
  *p='\0';
  return str;
}
char* createBtreeFilenameSou(){
  char *str=strdup(source_text_filename);
  char *p=str;
  do{
    if( *p=='.' && !(*(p+1)=='/' || *(p+1)=='.' && *(p+2)=='/') ) break;
    p++;
  }while(*p!='\0');
  *p='\0';
  return strcat(str,"sou");
}
gboolean insert_word(GtkWidget *widget,gpointer entry){
  GtkWidget *dialog,*d,*window=mainwindow;
  char word[MAXLEN],meaning[MAXLEN],omeaning[MAXLEN],am[50];
  GtkTextIter start,end;
  strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
  gtk_text_buffer_get_start_iter(buffer,&start);
  gtk_text_buffer_get_end_iter(buffer,&end);
  strcpy(meaning,(char*)gtk_text_buffer_get_text(buffer,&start,&end,0));
  if (strlen(word)==0){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Bạn chưa nhập tên!");
    gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }  
  else{
  	int rsize;
  	if(btsel(data,word,omeaning,MAXLEN*sizeof(char),&rsize)==0) {
      dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Tên đã có trong cơ sở dữ liệu! Cập nhật ?");
    	gtk_window_set_title(GTK_WINDOW(dialog),"Trùng tên");
      if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES) {
          btupd(data,word,meaning,MAXLEN*sizeof(char));
          FileDeleteRow(source_text_filename,word);         
          FILE *file = fopen(source_text_filename,"r+");
          fseek(file, 0, SEEK_END);
          fprintf(file,"%s#%s\n",word,meaning);
          fclose(file);
          gtk_widget_destroy(dialog);
          d=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Tên đã được cập nhật !");
          gtk_window_set_title(GTK_WINDOW(d),"Success");
          gtk_dialog_run(GTK_DIALOG(d)); gtk_widget_destroy(d);
      }
      else gtk_widget_destroy(dialog);
    } 
    else{
    	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Bạn muốn thêm tên này vào cơ sở dữ liệu?");
	    gtk_window_set_title(GTK_WINDOW(dialog),"Xác nhận");
	    if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES) {
	      	btins(data,word,meaning,MAXLEN*sizeof(char));
          soundEx(am,word,50,1);
          AddWordToSou(sou,am,word);          
	      	FILE *file = fopen(source_text_filename,"r+");
	        fseek(file, 0, SEEK_END);
	        fprintf(file,"%s#%s\n",word,meaning);
	        fclose(file);
          gtk_widget_destroy(dialog);
          d=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Tên đã được thêm!");
          gtk_window_set_title(GTK_WINDOW(d),"Success");
          gtk_dialog_run(GTK_DIALOG(d)); gtk_widget_destroy(d);
	    }
      else gtk_widget_destroy(dialog);
    }  
  }
  
}

gboolean delete_word(GtkWidget *widget, gpointer entry){
  GtkWidget *dialog,*d,*window=mainwindow;
  char word[MAXLEN],meaning[MAXLEN],am[50],s[100];
  strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
  if (strlen(word) == 0){
    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Bạn chưa nhập tên!");
    gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }  
  else{
  	int rsize;
  	if(btsel(data,word,meaning,MAXLEN*sizeof(char),&rsize)==0) {
      dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Bạn muốn xóa tên này khỏi cơ sở dữ liệu?");
    	gtk_window_set_title(GTK_WINDOW(dialog),"Xác nhận");
    	if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES) {
	      	gtk_widget_destroy(dialog);
          if(btdel(data, word) == 0) {
	          FileDeleteRow(source_text_filename,word);
            soundEx(am,word,50,1); 

            char *p,*q;
            if(btsel(sou,am,s,MAXLEN*sizeof(char),&rsize)==0) {
              p=strstr(s,word);
              q=p+strlen(word);
              if(p!=s)p--;
              while(*q!='\0') {
                *p=*q;
                p++;q++;
              }
              *p='\0';
              if(strlen(s)==0)
                btdel(sou, am);
              else
                btupd(sou,am,s,MAXLEN*sizeof(char));
            }
            
            d=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Tên đã được xóa !");
            gtk_window_set_title(GTK_WINDOW(d),"Success");
            gtk_dialog_run(GTK_DIALOG(d)); gtk_widget_destroy(d);
    	    }        
	        gtk_text_buffer_set_text(buffer,"",-1);
      		gtk_entry_set_text(GTK_ENTRY(entry),"");
	    }
      else gtk_widget_destroy(dialog);
    } 
    else{	    
	    dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Tên không có trong cơ sở dữ liệu!");
    	gtk_window_set_title(GTK_WINDOW(dialog),"Lỗi!");
    	gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
  }  
  return FALSE;
}
gboolean search_word(GtkWidget *button,gpointer database){
  display(GTK_WIDGET(database),NULL);
  return TRUE;
}

gboolean delete_event(GtkWidget *widget,GdkEvent *event,gpointer database){
    gtk_main_quit();
    btcls(data);
    btcls(sou);
    printf("End!\n");
    return FALSE;
}

gboolean clearAll(GtkWidget *widget,gpointer database){
    gtk_text_buffer_set_text(buffer,"Xin chào!", -1);
    gtk_entry_set_text(GTK_ENTRY(database),"");
    return FALSE;
}
int soundEx(char *SoundEx,
              char *WordString,
              int   LengthOption,
              int   CensusOption)
  {
      int  InSz = 31;
      char WordStr[32];     /* one bigger than InSz */
      int  SoundExLen, WSLen, i;
      char FirstLetter, *p, *p2;

      SoundExLen = WSLen = 0;
      SoundEx[0] = 0;

      if(CensusOption) {
        LengthOption = 4;
      }

      if(LengthOption) {
          SoundExLen = LengthOption;
      }
      if(SoundExLen > 10) {
          SoundExLen = 10;
      }
      if(SoundExLen < 4) {
          SoundExLen = 4;
      }

      if(!WordString) {
          return(0);
      }

      /* Copy WordString to WordStr
       * without using funcs from other
       * libraries.
      */
      for(p = WordString,p2 = WordStr,i = 0;(*p);p++,p2++,i++) {
        if(i >= InSz) break;
        (*p2) = (*p);
      }
      (*p2) = 0;



      /* Convert WordStr to
       * upper-case, without using funcs
       * from other libraries
      */
      for(p = WordStr;(*p);p++) {
        if( (*p) >= 'a' && (*p) <= 'z' ) {
            (*p) -= 0x20;
        }
      }


      /* convert all non-alpha
       * chars to spaces
      */
      for(p = WordStr;(*p);p++) {
        if( (*p) < 'A' || (*p) > 'Z' ) {
            (*p) = ' ';
        }
      }

      /* Remove leading spaces
      */
      for(i = 0, p = p2 = WordStr;(*p);p++) {
          if(!i) {
              if( (*p) != ' ' ) {
                  (*p2) = (*p);
                  p2++;
                  i++;
              }
          }
          else {
              (*p2) = (*p);
              p2++;
          }
      }
      (*p2) = 0;

      /* Get length of WordStr
      */
      for(i = 0,p = WordStr;(*p);p++) i++;


      /* Remove trailing spaces
      */
      for(;i;i--) {
          if(WordStr[i] == ' ') {
            WordStr[i] = 0;
          }
          else {
              break;
          }
      }

      /* Get length of WordStr
      */
      for(WSLen = 0,p = WordStr;(*p);p++) WSLen++;

      if(!WSLen) {
        return(0);
      }



      /* Perform our own multi-letter
       * improvements
       *
       * underscore placeholders (_) will be
       * removed below.
      */
      if(!CensusOption) {
          if(WordStr[0] == 'P' && WordStr[1] == 'S') {
              WordStr[0] = '_';
          }

          if(WordStr[0] == 'P' && WordStr[1] == 'F') {
              WordStr[0] = '_';
          }

          /* v1.0e gh is G-sound at start of word
          */
          if(WordStr[0] == 'G' && WordStr[1] == 'H') {
              WordStr[1] = '_';
          }

          for(i = 0;i < WSLen;i++) {
              if(WordStr[i] == 'D' && WordStr[i+1] == 'G') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if( WordStr[i] == 'G' && WordStr[i+1] == 'H') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i] == 'K' && WordStr[i+1] == 'N') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i] == 'G' && WordStr[i+1] == 'N') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i] == 'M' && WordStr[i+1] == 'B') {
                  WordStr[i+1] = '_';
                  i++;
                  continue;
              }

              if(WordStr[i] == 'P' && WordStr[i+1] == 'H') {
                  WordStr[i] = 'F';
                  WordStr[i+1] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i]  ==  'T'  &&
                 WordStr[i+1] == 'C' &&
                 WordStr[i+2] == 'H'
                ) {

                  WordStr[i] = '_';
                  i++; i++;
                  continue;
              }
              if(WordStr[i] == 'M' && WordStr[i+1] == 'P'
                 && (WordStr[i+2] == 'S' ||
                     WordStr[i+2] == 'T' ||
                     WordStr[i+2] == 'Z')
                ) {
                  WordStr[i+1] = '_';
                  i++;
              }
          }
      } /* end if(!CensusOption) */



      /* squeeze out underscore characters
       * added as a byproduct of above process
       * (only needed in c styled replace)
      */
      for(p = p2 = WordStr;(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != '_' ) {
            p2++;
        }
      }
      (*p2) = 0;





      /* This must be done AFTER our
       * multi-letter replacements
       * since they could change
       * the first letter
      */
      FirstLetter = WordStr[0];


      /* In case we're in CensusOption
       * 1 and the word starts with
       * an 'H' or 'W'
       *  (v1.0c djr: add test for H or W)
      */
      if(FirstLetter == 'H' || FirstLetter == 'W') {
          WordStr[0] = '-';
      }



      /* In properly done census
       * SoundEx, the H and W will
       * be squeazed out before
       * performing the test
       * for adjacent digits
       * (this differs from how
       * 'real' vowels are handled)
      */
      if(CensusOption == 1) {
          for(p = &(WordStr[1]);(*p);p++) {
             if((*p) == 'H' || (*p) == 'W') {
                 (*p) = '.';
             }
          }
      }





      /* Perform classic SoundEx
       * replacements.
      */
      for(p = WordStr;(*p);p++) {
          if( (*p) == 'A'   ||
              (*p) == 'E'   ||
              (*p) == 'I'   ||
              (*p) == 'O'   ||
              (*p) == 'U'   ||
              (*p) == 'Y'   ||
              (*p) == 'H'   ||
              (*p) == 'W'
            ){
              (*p) = '0';   /* zero */
          }
          if( (*p) == 'B'   ||
              (*p) == 'P'   ||
              (*p) == 'F'   ||
              (*p) == 'V'
            ){
              (*p) = '1';
          }
          if( (*p) == 'C'   ||
              (*p) == 'S'   ||
              (*p) == 'G'   ||
              (*p) == 'J'   ||
              (*p) == 'K'   ||
              (*p) == 'Q'   ||
              (*p) == 'X'   ||
              (*p) == 'Z'
            ){
              (*p) = '2';
          }
          if( (*p) == 'D'   ||
              (*p) == 'T'
            ){
              (*p) = '3';
          }
          if( (*p) == 'L' ) {
              (*p) = '4';
          }

          if( (*p) == 'M'   ||
              (*p) == 'N'
            ){
              (*p) = '5';
          }
          if( (*p) == 'R' ) {
              (*p) = '6';
          }
      }
      /* soundex replacement loop done  */




      /* In properly done census
       * SoundEx, the H and W will
       * be squezed out before
       * performing the test
       * for adjacent digits
       * (this differs from how
       * 'real' vowels are handled)
      */
      if(CensusOption == 1) {
          /* squeeze out dots
          */
          for(p = p2 = &WordStr[1];(*p);p++) {
            (*p2) = (*p);
            if( (*p2) != '.' ) {
                p2++;
            }
          }
          (*p2) = 0;
      }



      /* squeeze out extra equal adjacent digits
       * (don't include first letter)
       * v1.0c djr (now includes first letter)
      */
      for(p = p2 = &(WordStr[0]);(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != p[1] ) {
            p2++;
        }
      }
      (*p2) = 0;



      /* squeeze out spaces and zeros
       * Leave the first letter code
       * to be replaced below.
       * (In case it made a zero)
      */
      for(p = p2 = &WordStr[1];(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != ' ' && (*p2) != '0' ) {
            p2++;
        }
      }
      (*p2) = 0;



      /* Get length of WordStr
      */
      for(WSLen = 0,p = WordStr;(*p);p++) WSLen++;


      /* Right pad with zero characters
      */
      for(i = WSLen;i < SoundExLen;i++ ) {
          WordStr[i] = '0';
      }

      /* Size to taste
      */
      WordStr[SoundExLen] = 0;


      /* Replace first digit with
       * first letter.
      */
      WordStr[0] = FirstLetter;


      /* Copy WordStr to SoundEx
      */
      for(p2 = SoundEx,p = WordStr;(*p);p++,p2++) {
          (*p2) = (*p);
      }
      (*p2) = 0;
      
      return(SoundExLen);
  }