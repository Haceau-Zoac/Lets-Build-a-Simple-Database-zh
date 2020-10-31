Part 1 - 簡介和設置 REPL
===
[简体中文](Introduction_and_Setting_up_the_REPL.md) | 繁體中文

<p>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="https://cstack.github.io/db_tutorial/parts/part2.html">
        Part 2 - 世界上最簡單的 SQL 編譯器和虛擬機 &gt;
    </a>
    （尚未翻譯）
</p>

作為一個 web 開發者，我每天都在工作中和關係型數據庫打交道，但是他們對我來說是一個黑盒。這導致我出現了一些疑問：

* 數據保存的格式是什麼？ （在內存和硬盤中）
* 什麼是候將它從內存移動到硬盤中？
* 為什麼一個數據表只能有一個主鍵？
* 回滾操作是怎麼完成的？
* 索引是如何格式化的？
* 何時及如何進行全表掃描？
* 準備好的語句以什麼格式保存？

換句話說，一個數據庫是如何**工作**的？

為了搞清楚這些事，我正在從頭編寫數據庫。它是基於 sqlite 架構的，因為它的體積小，不像 MySQL 或是 PostgreSQL 那樣，所以我更有希望理解它。整個數據庫都存在一個.db文件中！

Sqlite
===
他們的網站上有許多 [內部 sqlite 文檔](https://www.sqlite.org/arch.html)，另外，我還有一個 [SQLite 數據庫系統：設計和實現](https://play.google.com/store/books/details?id=9Z6IQQnX1JEC) 的副本。

<div align=center>
    <img src="https://cstack.github.io/db_tutorial/assets/images/arch1.gif" alt="sqlite 架構（https://www.sqlite.org/zipvfs/doc/trunk/www/howitworks .wiki）"/>
    <hr/>
    sqlite 架構
    <br>
    （https://www.sqlite.org/zipvfs/doc/trunk/www/howitworks.wiki）
</div>

查詢通過一系列組件來檢索或是修改數據。 **前端**包括：
* Tokenizer
* 解析器
* 代碼生成器

輸入到前端的是一個 SQL 查詢。輸出是 sqlite 虛擬機字節碼（本質上是可以在數據庫上運行的已編譯程序）

*後端*包括：
* 虛擬機
* B樹
* Pager
* 系統接口

**虛擬機**將前端生成的字節碼當成指令。然後，他可以對一個或多個表進行索引操作，每個表都存儲在一種叫做B樹的數據結構中。 VM本質上是關於字節碼指令類型的大開關語句。

每個**B樹**都由許多個節點組成。每個節點的長度為一頁。 B數可以通過向 Pager 發出命令來從硬盤檢索頁面或將其保存回硬盤。

**Pager** 接收命令以讀取或寫入數據頁。它負責以適當的偏移量在數據庫文件中進行讀/寫操作。它還會在內存中保留最近訪問頁面的緩存，並確定何時需要將這些頁面寫回到硬盤。

**系統接口**是不同的層，具體取決於為哪個操作系統的 sqlite 進行編譯。在這個指南中，我不會去支持多種平台。

千里之行始於足下，讓我們從一個簡單的東西開始：**REPL**。

製作一個簡單的 REPL
===
當你在命令行啟動 Sqlite 的時候，Sqlite 會啟動一個 read-execute-print loop：

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

為此，我們的 `main` 函數應有一個無限循環，這個循環打印提示，獲取輸入，然後處理輸入：

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

我們將`InputBuffer` 定義為一個圍繞狀態存儲的小包裝，以便和[getline()](http://man7.org/linux/man-pages/man3/getline.3.html) 函數進行交互（稍後詳細介紹）

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

接著，`print_prompt()` 打印一個提示給用戶。我們在讀每一行輸入之前都要進行這個操作。

```
void print_prompt() { printf("db > "); }
```

如果要讀取一行輸入，那就使用 [getline()](http://man7.org/linux/man-pages/man3/getline.3.html)：

```
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

`lineptr`：一個指向字符串的指針，我們用來指向包含讀取行的緩衝區。如果將它設置為 NULL，那它就將由 getline 分配，因此即使命令失敗，也應由用戶釋放。

`n`：一個指向用於保存為緩衝區分配的大小的變量的指針。

`stream`：輸入來源。我們使用標準輸入。

`返回值`：讀取的字節數，可能小於緩衝區的大小。

我們讓 `getline` 將讀取的行存儲在 `input_buffer->buffer` 中，將分配的緩衝區大小存儲在 `input_buffer->buffer_length` 中。我們將返回值存儲在 `input_buffer->input_length` 中。

`buffer` 開始為空，所以 `getline` 分配足夠的內存來容納輸入行，並讓緩衝區指向輸入行。

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

現在定義一個釋放為 `InputBuffer *` 實例分配的內存和響應結構的 `buffer` 元素（`getline` 在 `read_input` 中為 `input_buffer->buffer` 分配的內存）

```
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}
```

最後，我們解析並執行命令。現在只有一個可識別的命令：.exit，它將終止程序。否則，我們將顯示錯誤消息並繼續循環。

```
if (strcmp(input_buffer->buffer, ".exit") == 0) {
  close_input_buffer(input_buffer);
  exit(EXIT_SUCCESS);
} else {
  printf("Unrecognized command '%s'.\n", input_buffer->buffer);
}
```

來試試！

```
~ ./db
db > .tables
Unrecognized command '.tables'.
db > .exit
~
```

好了，我們有一個有效的REPL。在下一部分中，我們將開始開發命令語言。同時，這是此部分中的整個程序：

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
        Part 2 - 世界上最簡單的 SQL 編譯器和虛擬機 &gt;
    </a>
    （尚未翻譯）
</p>