Part 1 - �������� REPL
===
�������� | [���w����](Introduction_and_Setting_up_the_REPL.zh-TW.md)

<p>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="https://cstack.github.io/db_tutorial/parts/part2.html">
        Part 2 - ��������򵥵� SQL ������������� &gt;
    </a>
    ����δ���룩
</p>

��Ϊһ�� web �����ߣ���ÿ�춼�ڹ����к͹�ϵ�����ݿ�򽻵����������Ƕ�����˵��һ���ںС��⵼���ҳ�����һЩ���ʣ�

* ���ݱ���ĸ�ʽ��ʲô�������ڴ��Ӳ���У�
* ʲô�Ǻ������ڴ��ƶ���Ӳ���У�
* Ϊʲôһ�����ݱ�ֻ����һ��������
* �ع���������ô��ɵģ�
* ��������θ�ʽ���ģ�
* ��ʱ����ν���ȫ��ɨ�裿
* ׼���õ������ʲô��ʽ���棿

���仰˵��һ�����ݿ������**����**�ģ�

Ϊ�˸������Щ�£������ڴ�ͷ��д���ݿ⡣���ǻ��� sqlite �ܹ��ģ���Ϊ�������С������ MySQL ���� PostgreSQL �����������Ҹ���ϣ����������������ݿⶼ����һ��.db�ļ��У�

Sqlite
===
���ǵ���վ������� [�ڲ� sqlite �ĵ�](https://www.sqlite.org/arch.html)�����⣬�һ���һ�� [SQLite ���ݿ�ϵͳ����ƺ�ʵ��](https://play.google.com/store/books/details?id=9Z6IQQnX1JEC) �ĸ�����

<div align=center>
    <img src="https://cstack.github.io/db_tutorial/assets/images/arch1.gif" alt="sqlite �ܹ���https://www.sqlite.org/zipvfs/doc/trunk/www/howitworks.wiki��"/>
    <hr/>
    sqlite �ܹ�
    <br>
    ��https://www.sqlite.org/zipvfs/doc/trunk/www/howitworks.wiki��
</div>


��ѯͨ��һϵ����������������޸����ݡ�**ǰ��**������
* Tokenizer
* ������
* ����������

���뵽ǰ�˵���һ�� SQL ��ѯ������� sqlite ������ֽ��루�������ǿ��������ݿ������е��ѱ������

*���*������
* �����
* B��
* Pager
* ϵͳ�ӿ�

**�����**��ǰ�����ɵ��ֽ��뵱��ָ�Ȼ�������Զ�һ���������������������ÿ�����洢��һ�ֽ���B�������ݽṹ�С�VM�������ǹ����ֽ���ָ�����͵Ĵ󿪹���䡣

ÿ��**B��**���������ڵ���ɡ�ÿ���ڵ�ĳ���Ϊһҳ��B������ͨ���� Pager ������������Ӳ�̼���ҳ����䱣���Ӳ�̡�

**Pager** ���������Զ�ȡ��д������ҳ�����������ʵ���ƫ���������ݿ��ļ��н��ж�/д���������������ڴ��б����������ҳ��Ļ��棬��ȷ����ʱ��Ҫ����Щҳ��д�ص�Ӳ�̡�

**ϵͳ�ӿ�**�ǲ�ͬ�Ĳ㣬����ȡ����Ϊ�ĸ�����ϵͳ�� sqlite ���б��롣�����ָ���У��Ҳ���ȥ֧�ֶ���ƽ̨��

ǧ��֮��ʼ�����£������Ǵ�һ���򵥵Ķ�����ʼ��**REPL**��

����һ���򵥵� REPL
===
���������������� Sqlite ��ʱ��Sqlite ������һ�� read-execute-print loop��

```
~ sqlite3
SQLite version 3.16.0 2016-11-04 19:09:39
Enter ".help" for usage hints.
Connected to a transient in-memory database.
Use ".open FILENAME" to reopen on a persistent database.
sqlite> create table users (id int, username varchar(255), email varchar(255));
sqlite> .tables
users
sqlite> .exit
~
```

Ϊ�ˣ����ǵ� `main` ����Ӧ��һ������ѭ�������ѭ����ӡ��ʾ����ȡ���룬Ȼ�������룺

```
int main(int argc, char* argv[]) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", input_buffer->buffer);
    }
  }
}
```

���ǽ� `InputBuffer` ����Ϊһ��Χ��״̬�洢��С��װ���Ա�� [getline()](http://man7.org/linux/man-pages/man3/getline.3.html) �������н������Ժ���ϸ���ܣ�

```
typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}
```

���ţ�`print_prompt()` ��ӡһ����ʾ���û��������ڶ�ÿһ������֮ǰ��Ҫ�������������

```
void print_prompt() { printf("db > "); }
```

���Ҫ��ȡһ�����룬�Ǿ�ʹ�� [getline()](http://man7.org/linux/man-pages/man3/getline.3.html)��

```
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

`lineptr`��һ��ָ���ַ�����ָ�룬��������ָ�������ȡ�еĻ������������������Ϊ NULL�������ͽ��� getline ���䣬��˼�ʹ����ʧ�ܣ�ҲӦ���û��ͷš�

`n`��һ��ָ�����ڱ���Ϊ����������Ĵ�С�ı�����ָ�롣

`stream`��������Դ������ʹ�ñ�׼���롣

`����ֵ`����ȡ���ֽ���������С�ڻ������Ĵ�С��

������ `getline` ����ȡ���д洢�� `input_buffer->buffer` �У�������Ļ�������С�洢�� `input_buffer->buffer_length` �С����ǽ�����ֵ�洢�� `input_buffer->input_length` �С�

`buffer` ��ʼΪ�գ����� `getline` �����㹻���ڴ������������У����û�����ָ�������С�

```
void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}
```

���ڶ���һ���ͷ�Ϊ `InputBuffer *` ʵ��������ڴ����Ӧ�ṹ�� `buffer` Ԫ�أ�`getline` �� `read_input` ��Ϊ `input_buffer->buffer` ������ڴ棩

```
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}
```

������ǽ�����ִ����� ����ֻ��һ����ʶ������.exit��������ֹ���� �������ǽ���ʾ������Ϣ������ѭ����

```
if (strcmp(input_buffer->buffer, ".exit") == 0) {
  close_input_buffer(input_buffer);
  exit(EXIT_SUCCESS);
} else {
  printf("Unrecognized command '%s'.\n", input_buffer->buffer);
}
```

�����ԣ�

```
~ ./db
db > .tables
Unrecognized command '.tables'.
db > .exit
~
```

���ˣ�������һ����Ч��REPL�� ����һ�����У����ǽ���ʼ�����������ԡ� ͬʱ�����Ǵ˲����е���������
```
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char* argv[]) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", input_buffer->buffer);
    }
  }
}
```

<p>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="https://cstack.github.io/db_tutorial/parts/part2.html">
        Part 2 - ��������򵥵� SQL ������������� &gt;
    </a>
    ����δ���룩
</p>