Dữ liệu Truyền vào trong hàm main em ví dụ là
data=btopn("AnhViet.dat",0,FALSE); với btopn là hàm trong thư viên btree, thì anh sẽ thay bằng code của a
Như câu lệnh trên thì dữ liệu trả về là kiểu BTA
(extern BTA *btopn(char *,int,int);) 
sẽ có dạng như sau:
struct bt_active {
    FILE *idxunt;
    char idxfid[FIDSZ];
    int fd;                 /* used to hold index file descriptor */
    int lckcnt;             /* re-entrant lock count */
    int shared;
    int wt_threshold;       /* write through threshold */
    CNTRL *cntrl;
    MEMREC *memrec;
    CNTXT *cntxt;
};
typedef struct bt_active BTA;
(em cũng chưa đọc về thư viện này nhiều, e chỉ biết tác dụng của hàm là em dùng thôi =)) )
nó liên quan đến hàm tab auto complete của em ở file SAD.c, nếu dữ liệu anh cấu trúc khác với thư viện btree thì anh báo em, để em viết lại hàm auto complete theo ý tưởng của thư viện btree. 
mà em sợ là em không làm kịp, có gì thì a báo em sớm nhé
