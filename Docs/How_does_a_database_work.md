数据库是如何工作的？
===
简体中文 | [繁體中文](How_does_a_database_work.zh-TW.md)
* 内存和硬盘里的数据是用什么格式保存的？
* 它什么时候从内存移动到硬盘？
* 为什么一个数据表只能有一个主键？
* 回滚操作是怎么完成的？
* 索引是如何格式化的？
* 什么时候进行、怎么进行全表扫描？
* 准备好的语句使用什么格式保存的？

短一些，一个数据库是如何**工作**的？

我正在用 C 从头开始创建一个 [sqlite](https://www.sqlite.org/arch.html) 的副本，以了解数据库是如何工作的，并且记录我的流程。

目录
===
* [Part 1 - 介绍并设置 REPL](Introduction_and_Setting_up_the_REPL.md)
* [Part 2 - 世界上最简单的 SQL 编译器和虚拟机](https://cstack.github.io/db_tutorial/parts/part2.html)（尚未翻译）
* [Part 3 - 一个只能添加数据的、只有一个表的内存数据库](https://cstack.github.io/db_tutorial/parts/part3.html)（尚未翻译）
* [Part 4 - 我们的第一次测试（以及 Bugs）](https://cstack.github.io/db_tutorial/parts/part4.html)（尚未翻译）
* [Part 5 - 持久化到硬盘](https://cstack.github.io/db_tutorial/parts/part5.html)（尚未翻译）
* [Part 6 - 游标抽象](https://cstack.github.io/db_tutorial/parts/part6.html)（尚未翻译）
* [Part 7 - 介绍 B-树](https://cstack.github.io/db_tutorial/parts/part7.html)（尚未翻译）
* [Part 8 - B-树叶子结点的格式](https://cstack.github.io/db_tutorial/parts/part8.html)（尚未翻译）
* [Part 9 - 二分搜索和重复键](https://cstack.github.io/db_tutorial/parts/part9.html)（尚未翻译）
* [Part 10 - 分割叶节点](https://cstack.github.io/db_tutorial/parts/part10.html)（尚未翻译）
* [Part 11 - 递归搜索 B-树](https://cstack.github.io/db_tutorial/parts/part11.html)（尚未翻译）
* [Part 12 - 扫描多级 B-树](https://cstack.github.io/db_tutorial/parts/part12.html)（尚未翻译）
* [Part 13 - 分割后更新树节点](https://cstack.github.io/db_tutorial/parts/part13.html)（尚未翻译）

> *"What I cannot create, I do not understand." - [理查德·费曼](https://en.m.wikiquote.org/wiki/Richard_Feynman)*

<div align=center>
    <img src="https://cstack.github.io/db_tutorial/assets/images/arch2.gif" alt="sqlite 架构（https://www.sqlite.org/arch.html）"/>
    <hr/>
    sqlite 架构（https://www.sqlite.org/arch.html）
</div>
