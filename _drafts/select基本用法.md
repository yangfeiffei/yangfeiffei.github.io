





# 1. select基本用法

1. 显示表中有的列属性

```sql
HR@ test> desc EMPLOYEES                            
```

2. 基本select语句

```sql
HR@ test> select FIRST_NAME, SALARY from EMPLOYEES;
```

- select和from组成；
- 列之间用逗号分隔；

3. 查看四则运算后的列

```sql
HR@ test> select FIRST_NAME, SALARY, SALARY+100 from EMPLOYEES;
```

- 先乘除后加减

  ```sql
  HR@ test> select FIRST_NAME, SALARY, (SALARY+100)*12 from EMPLOYEES;   
  ```

4. null值：未定义的值，不是0，不是空格

```sql
HR@ test> select FIRST_NAME, SALARY, COMMISSION_PCT from EMPLOYEES;
```

- 必要的时候对空值进行转换；

5. 定义别名

```sql
#别名自动转化为大写
HR@ test> select FIRST_NAME as name, SALARY from EMPLOYEES;
#强制定义为小写
HR@ test> select FIRST_NAME as "name", SALARY from EMPLOYEES;
# as可以省略
HR@ test> select FIRST_NAME "name", SALARY from EMPLOYEES;
# 为长表达式定义别名
HR@ test> select FIRST_NAME, SALARY*12 as annual_sal from EMPLOYEES;
# 使用了如空格等特殊字符的别名，要用引号
HR@ test> select FIRST_NAME, SALARY*12 as "annual sal" from EMPLOYEES;
```

6. 连接列

```sql
HR@ test> select FIRST_NAME||EMAIL from employees;
# 将空格连接在两列之间
HR@ test> select FIRST_NAME||' '||EMAIL from employees;
# 或者
HR@ test> select FIRST_NAME||'@'||EMAIL from employees;
```

7. 去除重复列

```sql
HR@ test> select distinct EMPLOYEE_ID from employees;
```

# 2.select限制和排列数据

1. 简单过滤

```sql
HR@ test> select FIRST_NAME,SALARY from EMPLOYEES where SALARY>10000;
HR@ test> select FIRST_NAME,SALARY from EMPLOYEES where DEPARTMENT_ID=90;
# 字符串为条件的时候大小写敏感
HR@ test> select FIRST_NAME,SALARY from EMPLOYEES where LAST_NAME='Whalen';
# 日期做条件：格式敏感，要与系统日期格式一致
HR@ test> select FIRST_NAME,SALARY from EMPLOYEES where HIRE_DATE='14-JUN-05';
HR@ test> select FIRST_NAME,SALARY from EMPLOYEES where HIRE_DATE='14-jun-05';
# 查看系统日期格式
HR@ test> select sysdate from dual;
SYSDATE
---------
27-MAY-18
```

2. 复杂一点

```sql
HR@ test> select FIRST_NAME,SALARY from EMPLOYEES where SALARY>=4000 and SALARY<=5000;
```













