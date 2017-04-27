#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "inc/btree.h"
#include <time.h>

#define MAXLEN 100
void FileDeleteRow(char* filename,char* word);
  
BTA *dic;
int rsize;
FILE *fout,*f;
long lines,row;
clock_t begin,end;
char word[MAXLEN],mean[MAXLEN],s[300];

void test(char* filename,int num){
  printf("%s...\n",filename );
  fprintf(fout, "%d,",num);
  //Loading time:
  lines=0;
  begin=clock();
    btinit();   
    dic = btcrt("test",0,FALSE);  
    f=fopen(filename,"r");
      while(!feof(f)) {
        fscanf(f,"%[^#]#%[^\n]\n",word,mean);lines++;     
        btins(dic,word,mean,MAXLEN*sizeof(char));   
      }
    fclose(f);    
  end=clock();
  double loadtime=(double)(end-begin)/CLOCKS_PER_SEC;
  //printf(" Loading time: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);

  //Insert time:  
  strcpy(word,"abcdefghijk");
  strcpy(mean,"ABCDEFGHIJK");
  begin=clock();
    if(btins(dic,word,mean,MAXLEN*sizeof(char))==0){
      f= fopen(filename,"r+");
      fseek(f, 0, SEEK_END);
      fprintf(f,"%s#%s\n",word,mean);
      fclose(f);
    }             
  end=clock();
  fprintf(fout, "%lf,",(double)(end-begin)/CLOCKS_PER_SEC); 
  //printf(" Insert time: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);

  //Search time:
  double tmp=0;
  for(int i=1;i<=(num/2);i++){
    row=rand()%lines+1;
    f=fopen(filename,"r");
    while(row>0) {
      fgets(s,300,f);row--;        
    }
    fscanf(f,"%[^#]#%[^\n]\n",word,mean);
    fclose(f);
    //printf(" Rand word %s \n",word);
    begin=clock();
      btsel(dic,word,mean,MAXLEN*sizeof(char),&rsize);        
    end=clock();
    tmp+=(double)(end-begin);
  }
  fprintf(fout, "%lf,",tmp/CLOCKS_PER_SEC/(num/2));
  //printf(" Mean %s\n",mean);
  //printf(" Search time: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);

  //Delete time:
  row=rand()%lines+1;
  f=fopen(filename,"r");
  while(row>0) {
    fgets(s,300,f);row--;        
  }
  fscanf(f,"%[^#]#%[^\n]\n",word,mean);
  fclose(f);
  //printf(" Rand word: %s \n",word);
  begin=clock();
    btdel(dic,word);
    FileDeleteRow(filename,word);       
  end=clock(); 
  fprintf(fout, "%lf,",(double)(end-begin)/CLOCKS_PER_SEC);
  fprintf(fout, "%lf\n",loadtime); 
  //printf(" Delete time: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);

  btcls(dic);
  remove("test");
}
int main() {
  srand(time(NULL));
  fout=fopen("result.txt","w");
  fprintf(fout, "Size(lines),Insert(s),Search(s),Delete(s),Load(s)\n");
  test("../sinh/sinh10.txt",10);
  test("../sinh/sinh20.txt",20);
  test("../sinh/sinh40.txt",40);
  test("../sinh/sinh100.txt",100);
  test("../sinh/sinh200.txt",200);
  test("../sinh/sinh400.txt",400);
  test("../sinh/sinh1000.txt",1000);
  test("../sinh/sinh2000.txt",2000);
  test("../sinh/sinh4000.txt",4000);
  test("../sinh/sinh10000.txt",10000);
  test("../sinh/sinh20000.txt",20000);
  test("../sinh/sinh40000.txt",40000);
  test("../sinh/sinh100000.txt",100000);
  test("../sinh/sinh200000.txt",200000);
  test("../sinh/sinh400000.txt",400000);
  test("../sinh/sinh1000000.txt",1000000);
  test("../sinh/sinh2000000.txt",2000000); 
  fclose(fout); 
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
  fclose(fp1);
  fclose(fp2);  
  remove(filename);  
  rename("copy.c", filename);
}