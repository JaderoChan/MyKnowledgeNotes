# [Git] Commit hash 值的计算

Git 的 Commit hash 是根据提交时的元数据通过 SHA-1 进行哈希运算，得到的 40 位十六进制字符串。

实际使用中常取 Commit hash 的前 7 位作为缩写。（起到唯一标识的作用即可）

## 元数据

Commit hash 计算的元数据包括：

- 提交的树对象哈希（代表当前目录结构）
- 父提交哈希（上一次 Commit 的 hash）
- 作者信息（名称 + 邮箱）
- 提交者信息（名称 + 邮箱）
- 提交时间戳（秒数 + 时区）
- 提交信息（Commit message）

通过这些元数据，Git 会构造一个 **Commit Object**，再通过这个对象的文本进行哈希。Commit Object 的格式如下：

```plaintext
commit {content_length}\0
tree {tree_hash}
parent {parent_hash}
author {name} <{email}> {timestamp} {tz}
committer {name} <{email}> {timestamp} {tz}

{commit message}
```

其中，第一行的 `commit {content_length}\0` 是 Git 内部隐藏的头部，它会由 Git 自动添加，并且使用 `git cat-file -p` 也不会打印它。`{content_length}` 将会由 Git 进行计算，表示后面所有内容的字节长度。`\0` 是一个二进制 0 值，作为分隔符。

## 树对象哈希（对应元数据的 `tree {tree_hash}` 这一部分）

树对象的哈希计算方式与 Commit Object 相同，但进行哈希计算的对象并非上面的 Commit Object，而是一个 **目录结构清单**，其格式大致如下：

```plaintext
tree {content_lenth}\0
[file_permission] [filename]\0[SHA-1 hash as 20 bytes binary]
...
```

其中，`file_permission` 记录文件的权限，例如 `100644` 为普通文件、`100755` 为可执行文件，`040000` 为子目录。

`\0` 用于分隔文件名与哈希值（这里的哈希值不同于 Commit hash，它使用二进制进行存储，而非十六进制字符串）。

最后，所有文件条目都按文件名字典序排序。

## 哈希计算

当 Commit Object 对象构造完成后，Git 会将 Commit Object 文本通过 SHA-1 进行哈希计算并得到 Commit hash。
