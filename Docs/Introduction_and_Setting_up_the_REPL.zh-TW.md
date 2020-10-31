Part 1 - ������O�� REPL
===
[��������](Introduction_and_Setting_up_the_REPL.md) | ���w����

<p>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="https://cstack.github.io/db_tutorial/parts/part2.html">
        Part 2 - ��������ε� SQL ���g����̓�M�C &gt;
    </a>
    ����δ���g��
</p>

����һ�� web �_�l�ߣ���ÿ�춼�ڹ����к��P�S�͔�����򽻵��������������ҁ��f��һ���ںС��@�����ҳ��F��һЩ�Ɇ���

* ��������ĸ�ʽ��ʲ�N�� ���ڃȴ��Ӳ�P�У�
* ʲ�N�Ǻ����ăȴ��Ƅӵ�Ӳ�P�У�
* ��ʲ�Nһ��������ֻ����һ�����I��
* �؝L���������N��ɵģ�
* ��������θ�ʽ���ģ�
* �Εr������M��ȫ����裿
* �ʂ�õ��Z����ʲ�N��ʽ���棿

�Q��Ԓ�f��һ�������������**����**�ģ�

���˸�����@Щ�£������ڏ��^���������졣���ǻ�� sqlite �ܘ��ģ���������w�eС������ MySQL ���� PostgreSQL �ǘӣ������Ҹ���ϣ������������������춼����һ��.db�ļ��У�

Sqlite
===
�����ľWվ�����S�� [�Ȳ� sqlite �ęn](https://www.sqlite.org/arch.html)�����⣬��߀��һ�� [SQLite ������ϵ�y���OӋ�͌��F](https://play.google.com/store/books/details?id=9Z6IQQnX1JEC) �ĸ�����

<div align=center>
    <img src="https://cstack.github.io/db_tutorial/assets/images/arch1.gif" alt="sqlite �ܘ���https://www.sqlite.org/zipvfs/doc/trunk/www/howitworks .wiki��"/>
    <hr/>
    sqlite �ܘ�
    <br>
    ��https://www.sqlite.org/zipvfs/doc/trunk/www/howitworks.wiki��
</div>

��ԃͨ�^һϵ�нM����z�������޸Ĕ����� **ǰ��**������
* Tokenizer
* ������
* ���a������

ݔ�뵽ǰ�˵���һ�� SQL ��ԃ��ݔ���� sqlite ̓�M�C�ֹ��a�����|���ǿ����ڔ��������\�е��Ѿ��g����

*���*������
* ̓�M�C
* B��
* Pager
* ϵ�y�ӿ�

**̓�M�C**��ǰ�����ɵ��ֹ��a����ָ�Ȼ�ᣬ�����Ԍ�һ����������M������������ÿ�����惦��һ�N����B��Ĕ����Y���С� VM���|�����P��ֹ��aָ����͵Ĵ��_�P�Z�䡣

ÿ��**B��**�����S�������c�M�ɡ�ÿ�����c���L�Ȟ�һ퓡� B������ͨ�^�� Pager �l��������Ӳ�P�z�������䱣���Ӳ�P��

**Pager** �����������xȡ���딵��퓡���ؓ؟���m����ƫ�����ڔ������ļ����M���x/����������߀���ڃȴ��б�������L�����ľ��棬�K�_���Εr��Ҫ���@Щ��挑�ص�Ӳ�P��

**ϵ�y�ӿ�**�ǲ�ͬ�Čӣ����wȡ�Q춞��Ă�����ϵ�y�� sqlite �M�о��g�����@��ָ���У��Ҳ���ȥ֧�ֶ�Nƽ̨��

ǧ��֮��ʼ����£�׌�҂���һ�����εĖ|���_ʼ��**REPL**��

�u��һ�����ε� REPL
===
�����������І��� Sqlite �ĕr��Sqlite ������һ�� read-execute-print loop��

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

��ˣ��҂��� `main` ��������һ���o��ѭ�h���@��ѭ�h��ӡ��ʾ���@ȡݔ�룬Ȼ��̎��ݔ�룺

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

�҂���`InputBuffer` ���x��һ�����@��B�惦��С���b���Ա��[getline()](http://man7.org/linux/man-pages/man3/getline.3.html) �����M�н���������Ԕ����B��

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

������`print_prompt()` ��ӡһ����ʾ�o�Ñ����҂����xÿһ��ݔ��֮ǰ��Ҫ�M���@��������

```
void print_prompt() { printf("db > "); }
```

���Ҫ�xȡһ��ݔ�룬�Ǿ�ʹ�� [getline()](http://man7.org/linux/man-pages/man3/getline.3.html)��

```
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

`lineptr`��һ��ָ���ַ�����ָᘣ��҂��Á�ָ������xȡ�еľ��n�^����������O�Þ� NULL�������͌��� getline ���䣬��˼�ʹ����ʧ����Ҳ�����Ñ�ጷš�

`n`��һ��ָ����춱���龏�n�^����Ĵ�С��׃����ָᘡ�

`stream`��ݔ���Դ���҂�ʹ�Ø˜�ݔ�롣

`����ֵ`���xȡ���ֹ���������С춾��n�^�Ĵ�С��

�҂�׌ `getline` ���xȡ���д惦�� `input_buffer->buffer` �У�������ľ��n�^��С�惦�� `input_buffer->buffer_length` �С��҂�������ֵ�惦�� `input_buffer->input_length` �С�

`buffer` �_ʼ��գ����� `getline` �������ăȴ���ݼ{ݔ���У��K׌���n�^ָ��ݔ���С�

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

�F�ڶ��xһ��ጷŞ� `InputBuffer *` ��������ăȴ��푑��Y���� `buffer` Ԫ�أ�`getline` �� `read_input` �О� `input_buffer->buffer` ����ăȴ棩

```
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}
```

���ᣬ�҂������K��������F��ֻ��һ�����R�e�����.exit�������Kֹ���򡣷�t���҂����@ʾ�e�`��Ϣ�K�^�mѭ�h��

```
if (strcmp(input_buffer->buffer, ".exit") == 0) {
  close_input_buffer(input_buffer);
  exit(EXIT_SUCCESS);
} else {
  printf("Unrecognized command '%s'.\n", input_buffer->buffer);
}
```

��ԇԇ��

```
~ ./db
db > .tables
Unrecognized command '.tables'.
db > .exit
~
```

���ˣ��҂���һ����Ч��REPL������һ�����У��҂����_ʼ�_�l�����Z�ԡ�ͬ�r���@�Ǵ˲����е���������

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
        Part 2 - ��������ε� SQL ���g����̓�M�C &gt;
    </a>
    ����δ���g��
</p>