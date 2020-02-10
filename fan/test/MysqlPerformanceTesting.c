#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include<sys/time.h>
#include<unistd.h>
#define MAX_NUM 1024
#define TEST_TIME 10000 //测试次数
int main(int argc, const char *argv[])
{
    MYSQL           mysql;
    MYSQL_RES       *res = NULL;
    MYSQL_ROW       row;
    char            *query_str = NULL;
    int             rc, i, fields;
    int             rows;
    if (NULL == mysql_init(&mysql)) {
        printf("mysql_init(): %s\n", mysql_error(&mysql));
        return -1;
    }
    if (NULL == mysql_real_connect(&mysql,
                "localhost",
                "root",
                "bekl077",
                "test",
                3306,
                NULL,
                0)) {
        printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
        return -1;
    }
    printf("1. Connected MySQL successful! \n");
    //create table 表名(列名1(字段) 字段类型[约束条件],列名2 字段类型,...);
    mysql_query(&mysql,"create table stu(id int(20),name varchar(20),age int(20)) engine=MEMORY");//默认为InnoDB引擎
    struct timeval tv;
    struct timezone tz;
    printf("test time = %d\n",TEST_TIME);
    printf("****************\n\n");
    //insert
    gettimeofday(&tv,&tz);
    int ret;
    long time_sec0 = tv.tv_sec;
    long time_usec0 = tv.tv_usec;
    for(i=0;i<TEST_TIME;i++)
    {
        char spl[MAX_NUM] = {0};
        sprintf(spl,"insert into stu values(%d,'xiaoming',2)",i+1);
        //printf("spl = %s\n",spl);
        mysql_query(&mysql,spl);
    }
    gettimeofday(&tv,&tz);
    long time_sec1 = tv.tv_sec;
    long time_usec1 = tv.tv_usec;
    long time_sec = time_sec1 - time_sec0;
    long time_usec = time_sec *1000000 + time_usec1 - time_usec0;
    printf("time_sec = %ld\n",time_sec);
    printf("tiem_usec = %ld\n",time_usec);
    printf("insert one time = %ld us\n",time_usec/TEST_TIME);
    printf("******************\n\n");

    //update
    gettimeofday(&tv,&tz);
    time_sec0 = tv.tv_sec;
    time_usec0 = tv.tv_usec;
    for(i=0;i<TEST_TIME;i++)
    {
        char spl[MAX_NUM] = {0};
        sprintf(spl,"update stu set name='xiaohong' where id=%d",i+1);
        //printf("spl = %s\n",spl);
        mysql_query(&mysql,spl);
    }
    gettimeofday(&tv,&tz);
    time_sec1 = tv.tv_sec;
    time_usec1 = tv.tv_usec;
    time_sec = time_sec1 - time_sec0;
    time_usec = time_sec *1000000 + time_usec1 - time_usec0;
    printf("time_sec = %ld\n",time_sec);
    printf("tiem_usec = %ld\n",time_usec);
    printf("update one time = %ld us\n",time_usec/TEST_TIME);
    printf("******************\n\n");

    //query
    gettimeofday(&tv,&tz);
    time_sec0 = tv.tv_sec;
    time_usec0 = tv.tv_usec;
    for(i=0;i<TEST_TIME;i++)
    {
        char spl[MAX_NUM] = {0};
        //sprintf(spl,"select from stu where id=%d",i+1);
        sprintf(spl,"select * from stu where id=%d",i+1);
        mysql_query(&mysql,spl);
        res = mysql_store_result(&mysql);
        while (row = mysql_fetch_row(res))
        {
            int t;
            for (t = 0; t < mysql_num_fields(res); t++)
            {
                //printf("%s ", row[t]);
            }
            //printf("\n");
        }
        mysql_free_result(res);     //free result after you get the result
    }
    gettimeofday(&tv,&tz);
    time_sec1 = tv.tv_sec;
    time_usec1 = tv.tv_usec;
    time_sec = time_sec1 - time_sec0;
    time_usec = time_sec *1000000 + time_usec1 - time_usec0;
    printf("time_sec = %ld\n",time_sec);
    printf("tiem_usec = %ld\n",time_usec);
    printf("query one time = %ld us\n",time_usec/TEST_TIME);
    printf("******************\n\n");

    //delete
    gettimeofday(&tv,&tz);
    time_sec0 = tv.tv_sec;
    time_usec0 = tv.tv_usec;
    for(i=0;i<TEST_TIME;i++)
    {
        char spl[MAX_NUM] = {0};
        sprintf(spl,"delete from stu where id=%d",i+1);
        //printf("spl = %s\n",spl);
        mysql_query(&mysql,spl);
    }
    gettimeofday(&tv,&tz);
    time_sec1 = tv.tv_sec;
    time_usec1 = tv.tv_usec;
    time_sec = time_sec1 - time_sec0;
    time_usec = time_sec *1000000 + time_usec1 - time_usec0;
    printf("time_sec = %ld\n",time_sec);
    printf("tiem_usec = %ld\n",time_usec);
    printf("delete one time = %ld us\n",time_usec/TEST_TIME);
    printf("******************\n\n");

    //difference value
    gettimeofday(&tv,&tz);
    time_sec0 = tv.tv_sec;
    time_usec0 = tv.tv_usec;
    for(i=0;i<TEST_TIME;i++)
    {
        char spl[MAX_NUM] = {0};
        sprintf(spl,"delete from stu where id=%d",i+1);
        //printf("spl = %s\n",spl);
    }
    gettimeofday(&tv,&tz);
    time_sec1 = tv.tv_sec;
    time_usec1 = tv.tv_usec;
    time_sec = time_sec1 - time_sec0;
    time_usec = time_sec *1000000 + time_usec1 - time_usec0;
    printf("time_sec = %ld\n",time_sec);
    printf("tiem_usec = %ld\n",time_usec);
    printf("difference value one time = %f us\n",time_usec*1.0/TEST_TIME);
    printf("******************\n\n");

    mysql_query(&mysql,"drop table stu");
    mysql_close(&mysql);
    return 0;
}
