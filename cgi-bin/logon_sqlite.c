#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>

#define DATABASE "./student_logon.db"  //数据库名

int do_insert(sqlite3 *db);

int main()
{
        char *data;
        char username[50],password[20];
        sqlite3 *db ;
        char *errmsg;
        int n;
        printf("Content-type:  text/html;charset=utf-8\n\n");
        printf("<TITlE>注册结果</TITlE>");
        //printf("<H3>注册结果</H3>");
        data= getenv("QUERY_STRING");
        if(data==NULL)
                printf("<p>错误:数据没有被输入或数据传输发生错误</p>");
        else
        {
                sscanf(data,"username=%[^&]&password=%s",username,password);
                /*
                printf("<p>name=%s</p>",username);
                printf("<p>pwd=%s</p>",password);
                printf("%s",data);
                */

                //printf("<p>-------------------------------------------------------</p>");
                //打开数据库
                if (sqlite3_open(DATABASE, &db) != SQLITE_OK)
                {
                        printf("%s\n", sqlite3_errmsg(db));
                        exit(EXIT_FAILURE);
                }
                
                // 创建数据库表
                if (sqlite3_exec(db, "create table if not exists stu( name text, pwd password);", NULL, NULL, &errmsg) != SQLITE_OK)
                {
                        printf("创建数据库失败:%s\n", errmsg);
                        exit(EXIT_FAILURE);
                }
                else
                {
                        printf("<H3>已为用户创建数据库</H3>");
                }
                char sql[256] = {};
                sprintf(sql, "insert into stu values('%s','%s')", username, password);
                if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
                {
                        printf("数据库导入用户数据失败:%s\n", errmsg);
                }
                else
                {
                        printf("<H3>数据库导入用户数据成功，用户注册成功</H3>");
                        printf("<a href=""../login.html"">点此返回登录界面</a>");
                }
                return 0;
        }
}