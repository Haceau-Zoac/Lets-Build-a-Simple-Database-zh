一個數據庫是如何工作的？
===
[简体中文](How_does_a_database_work.md) | 繁體中文
* 數據保存的格式是什麼？(在內存和硬盤中)
* 什麼是候將它從內存移動到硬盤中？
* 為什麼一個數據表只能有一個主鍵？
* 回滾操作是怎麼完成的？
* 索引是如何格式化的？
* 何時及如何進行全表掃描？
* 準備好的語句以什麼格式保存？

短一些，一個數據庫是如何**工作**的？

我正在用 C 從頭開始創建一個 sqlite 的副本，以了解數據庫是如何工作的，並記錄我的流程。

目錄
===
* [Part 1 - 簡介和設置 REPL](https://cstack.github.io/db_tutorial/parts/part1.html)（尚未翻譯）
* [Part 2 - 世界上最簡單的SQL編譯器和虛擬機](https://cstack.github.io/db_tutorial/parts/part2.html)（尚未翻譯）
* [Part 3 - 一個存儲在內存的，只能追加數據的，只有一個表的數據庫](https://cstack.github.io/db_tutorial/parts/part3.html)（尚未翻譯）
* [Part 4 - 我們的第一個測試(以及Bug)](https://cstack.github.io/db_tutorial/parts/part4.html)（尚未翻譯）
* [Part 5 - 持久化到硬盤](https://cstack.github.io/db_tutorial/parts/part5.html)（尚未翻譯）
* [Part 6 - 游標抽象](https://cstack.github.io/db_tutorial/parts/part6.html)（尚未翻譯）
* [Part 7 - 介紹B樹](https://cstack.github.io/db_tutorial/parts/part7.html)（尚未翻譯）
* [Part 8 - B樹葉子結點的格式](https://cstack.github.io/db_tutorial/parts/part8.html)（尚未翻譯）
* [Part 9 - 二分搜索和重複鍵](https://cstack.github.io/db_tutorial/parts/part9.html)（尚未翻譯）
* [Part 10 - 分割一個葉節點](https://cstack.github.io/db_tutorial/parts/part10.html)（尚未翻譯）
* [Part 11 - 遞歸搜索B樹](https://cstack.github.io/db_tutorial/parts/part11.html)（尚未翻譯）
* [Part 12 - 掃描一個多級B樹](https://cstack.github.io/db_tutorial/parts/part12.html)（尚未翻譯）
* [Part 13 - 在分割後更新樹節點](https://cstack.github.io/db_tutorial/parts/part13.html)（尚未翻譯）

> “What I cannot create, I do not understand.” - [理查德·費曼](https://en.m.wikiquote.org/wiki/Richard_Feynman)