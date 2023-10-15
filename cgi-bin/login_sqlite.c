#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<string.h>

#define DATABASE "./student_logon.db"  //用户注册数据库文件名

int do_insert(sqlite3 *db);
int (*callback)(void * arg, int ncolum, char ** f_value, char ** f_name);  //回调函数

int main()
{
        char *data;
        char username[50],password[20];
        sqlite3 *db ;
        char *errmsg;
        int n;
        printf("Content-type:  text/html;charset=utf-8\n\n");
        printf("<TITlE>登陆结果</TITlE>");
        printf("<H3>姓名或密码错误!</H3>");
        printf("<a href=""../login.html"">点此返回登录界面</a>");
        printf("<H3>你也可以创建一个新账号:</H3>");
        printf("<a href=""../logon.html"">点此快速注册</a>");
        data= getenv("QUERY_STRING");
        if(data==NULL)
                printf("<p>错误:数据没有被输入或数据传输发生错误</p>");
        else
        {
                sscanf(data,"username=%[^&]&password=%s",username,password);
                //printf("<p>name=%s</p>",username);
                //printf("<p>pwd=%s</p>",password);
                //printf("%s",data);

                //printf("<p>-------------------------------------------------------</p>");
                //打开用户注册数据库
                if (sqlite3_open(DATABASE, &db) != SQLITE_OK)
                {
                        printf("%s\n", sqlite3_errmsg(db));
                        exit(EXIT_FAILURE);
                }
                else
                {
                        char sql[128]="select * from stu where name='";
                        strcat(sql,username);
                        strcat(sql,"'");
                        strcat(sql," and pwd='");
                        strcat(sql,password);
                        strcat(sql,"';");
                        //printf("<p>sql=%s</p>",sql);
                        
                        char **resultp;
                        int nrow;
                        int ncolumn;
                        if (sqlite3_get_table(db, sql, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
                        {
                                printf("%s\n", errmsg);
                                exit(EXIT_FAILURE);
                        }
                        else
                        {
                                //printf("query done.\n");
                        }

                        char usr_data[64];  //用户输入的数据
                        strcat(usr_data,username);  //用户输入数据拼接
                        strcat(usr_data,password);
                        //printf("<p>usr_data=%s</p>",usr_data);
                        char* col_data;  //数据库中一行中的一个字段数据
                        char sql_data[64];  //数据库中一行中的数据

                        int i;// 第i行
                        int j;// 第i行的第j个字段
                        int index = 0; // 用于计数，表示整个表格的第index个字段（包括表头）
                        for (i = 0; i < nrow + 1; ++i)
                        {
                                for (j = 0; j < ncolumn; ++j)
                                {
                                        col_data=resultp[index++];
                                        if(i>0)
                                        {
                                                if(j==0)
                                                {
                                                        //printf("<p>%s</p>", col_data);
                                                        strcpy(sql_data,col_data);
                                                        //printf("<p>%s</p>", sql_data);
                                                }
                                                else
                                                {
                                                        //printf("<p>%s</p>", col_data);
                                                        strcat(sql_data,col_data);
                                                        //printf("<p>%s</p>", sql_data);
                                                }
                                        }
                                }
                                putchar(10);
                        }

                        if(strcmp(usr_data,sql_data)==0)
                        {
                                printf("<script>window.location.href='../firstpage.html';</script>");
                        }

                        // 释放sqlite3_get_table()函数返回的结果表
                        sqlite3_free_table(resultp);
                }
                return 0;
        }
}