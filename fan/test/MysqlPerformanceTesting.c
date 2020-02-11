#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include<sys/time.h>
#include<unistd.h>
#define MAX_NUM 1024
#define DATE_VOLUME 1000 //数据量
#define TEST_TIME 10000     //测试次数
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
    if (NULL == mysql_real_connect(&mysql,"localhost","root","bekl077","test",3306,NULL,0))
    {
        printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
        return -1;
    }
    int ret;
    printf("1. Connected MySQL successful! \n");
    //create table 表名(列名1(字段) 字段类型[约束条件],列名2 字段类型,...);
    //mysql_query(&mysql,"create table stu(DateID int(5),creatorName varchar(20),PID int(20),CreateTime datetime,LifeTime int(10),DateType int(1),DateFormat varchar(20),DateVer int(20),DateBuff varchar(255)");
    ret = mysql_query(&mysql,"create table stu(id int(20),\
                                            nameid int(20),\
                                            ageid int(20),\
                                            INDEX XID(id),\
                                            INDEX XAGE(ageid),\
                                            UNIQUE INDEX UNIQUEIDX(nameid))\
                                            engine=MEMORY");//默认为InnoDB引擎 
    printf("engine ret= %d\n",ret);
/*
    ret = mysql_query(&mysql,"ALTER TABLE `stu` ADD INDEX index_id (`id`)");
    printf("index ret = %d\n",ret);
    ret = mysql_query(&mysql,"ALTER TABLE `stu` ADD INDEX index_nameid (`nameid`)");
    printf("index ret = %d\n",ret);
    ret = mysql_query(&mysql,"alter table stu add UNIQUE(`ageid`)"); 
    printf("index ret = %d\n",ret);
*/

    struct timeval tv;
    struct timezone tz;
    printf("****************\n\n");

    //insert
    gettimeofday(&tv,&tz);
    long time_sec0 = tv.tv_sec;
    long time_usec0 = tv.tv_usec;
    for(i=0;i<DATE_VOLUME;i++)
    {
        char spl[MAX_NUM] = {0};
        sprintf(spl,"insert into stu values(%d,%d,%d)",i+1,i+1,i+1);
        //sprintf(spl,"insert into stu values(%d,'xiaoming',2,1990,2,1,'13',13,'huhi')",i+1);
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
    printf("insert one time = %ld us\n",time_usec/DATE_VOLUME);
    printf("******************\n\n");

    //增删改查
    long insert_time = 0;
    long update_time = 0;
    long query_time = 0;
    long delete_time = 0;
    for(i=0;i<TEST_TIME;i++)
    {
        char spl[MAX_NUM] = {0};
        //insert
        gettimeofday(&tv,&tz);
        time_sec0 = tv.tv_sec;
        time_usec0 = tv.tv_usec;
        sprintf(spl,"insert into stu values(%d,%d,%d)",i+9999,i+9999,i+9999);
        //printf("spl = %s\n",spl);
        mysql_query(&mysql,spl);
        gettimeofday(&tv,&tz);
        time_sec1 = tv.tv_sec;
        time_usec1 = tv.tv_usec;
        time_sec = time_sec1 - time_sec0;
        time_usec = time_sec *1000000 + time_usec1 - time_usec0;
        //printf("insert one time = %ld us\n",time_usec);
        insert_time += time_usec;

        //update
        memset(spl,0,MAX_NUM);
        gettimeofday(&tv,&tz);
        time_sec0 = tv.tv_sec;
        time_usec0 = tv.tv_usec;
        sprintf(spl,"update stu set nameid=%d where id=%d",i+10000,i+9999);
        //printf("spl = %s\n",spl);
        mysql_query(&mysql,spl);
        gettimeofday(&tv,&tz);
        time_sec1 = tv.tv_sec;
        time_usec1 = tv.tv_usec;
        time_sec = time_sec1 - time_sec0;
        time_usec = time_sec *1000000 + time_usec1 - time_usec0;
        //printf("update one time = %ld us\n",time_usec);
        update_time += time_usec;

        //query
        memset(spl,0,MAX_NUM);
        gettimeofday(&tv,&tz);
        time_sec0 = tv.tv_sec;
        time_usec0 = tv.tv_usec;
        sprintf(spl,"select * from stu where id=%d",i+9999); 
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
        gettimeofday(&tv,&tz);
        time_sec1 = tv.tv_sec;
        time_usec1 = tv.tv_usec;
        time_sec = time_sec1 - time_sec0;
        time_usec = time_sec *1000000 + time_usec1 - time_usec0;
        //printf("query one time = %ld us\n",time_usec);
        query_time += time_usec;

        //delete
        memset(spl,0,MAX_NUM);
        gettimeofday(&tv,&tz);
        time_sec0 = tv.tv_sec;
        time_usec0 = tv.tv_usec;
        sprintf(spl,"delete from stu where id=%d",i+9999);
        //printf("spl = %s\n",spl);
        mysql_query(&mysql,spl);
        gettimeofday(&tv,&tz);
        time_sec1 = tv.tv_sec;
        time_usec1 = tv.tv_usec;
        time_sec = time_sec1 - time_sec0;
        time_usec = time_sec *1000000 + time_usec1 - time_usec0;
        //printf("delete one time = %ld us\n",time_usec);
        delete_time += time_usec;
    }
    printf("******************\n\n");
    printf("insert one time = %ld us\n",insert_time/TEST_TIME);
    printf("update one time = %ld us\n",update_time/TEST_TIME);
    printf("query one time = %ld us\n",query_time/TEST_TIME);
    printf("delete one time = %ld us\n",delete_time/TEST_TIME);
    printf("******************\n\n");
    mysql_query(&mysql,"drop table stu");
    mysql_close(&mysql);
    return 0;
}
