# Scanner (задание для Kaspersky)

Данный проект позволяет сканировать директорию на наличие "подозрительных" файлов.

Типы файлов и подозрительные строки для проверки на этих типах файлов заадются в файле `suspicious_files_settings.txt`.

Поиск идёт нерекурсиво, для проверки используются стандартные регулярные выражения С++.

### Сборка проекта:

Для сборки достаточно вспользоваться `cmake`:

```bash
cmake --build ./
```

### Примеры использования:

`./scanner -h` - вывести памятку.

`./scanner ./files-test/` - проверить файлы в дирректории `files-test`.

`./scanner ./files-test/ -f` - вывести краткий отчёт по каждому файлу в дирректории `files-test`.

`./scanner ./files-test/ -v` - вывести подробный отчёт по каждому файлу в дирректории `files-test`.

`./scanner ./files-test/ -j 4` - сканировать файлы в 4-х потоках



Памятка при использовании:

```text
Usage: ./scanner [OPTIONS] [DIRECTORY]

Searches for 'suspicious' files by rules, specified in suspicious_files_settings.txt
Search is done using regex. Search is not recursive.
Available options:
   -h, --help       - print this help and exit
   -f               - print short status for every file
   -v, --verbose    - print per-suspicion-group status for every file
   -j [NUM THREADS] - set the number of threads to run in, by default it's the number of supported threads
   -s [SETTINGS]    - change suspicious files setting file. By default - suspicious_files_settings.txt
```

### Производительность:

#### Тестовая сборка:

- OS: Ubuntu 21.10 x86_64
- CPU: AMD Ryzen 5 3550H with Radeon Vega Mobile Gfx (8) @ 2.100GHz
- Memory: 8GB
- SSD: Western Digital PC SN730 SDBPNTY-512G

#### Результаты:

#### Тест 1 (200'000 файлов, 4Kb каждый)
| Кол-во потоков   | Время работы |
|:----------------:|:------------:|
| 1                | 26640 ms     |
| 2                | 14300 ms     |
| 4                | 12000 ms     |
| 8 (по умолчанию*)| 11800 ms     |
| 16               | 9000 ms      |

#### Тест 2 (100 файлов, 2Mb каждый)
| Кол-во потоков   | Время работы |
|:----------------:|:------------:|
| 1                | 28000 ms      |
| 2                | 15000 ms     |
| 4                | 9000 ms      |
| 8 (по умолчанию*)| 7000 ms      |
| 16               | 6700 ms      |

(*) - Значение по умолчанию зависит от ОС и комплектующего ПК.
