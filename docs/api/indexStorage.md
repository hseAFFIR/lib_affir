# IndexStorage
## Table of Contents
<!-- TOC -->
* [IndexStorage](#indexstorage)
  * [Table of Contents](#table-of-contents)
  * [Overview](#overview)
  * [Single approach](#single-approach)
    * [Idea](#idea)
    * [Pros and Cons](#pros-and-cons)
    * [](#)
  * [Multi approach](#multi-approach)
<!-- TOC -->
## Overview
_< description >_

## Single approach
*Пока на русском, но в конце перепишу.

### Overview
Суть подхода — хранение индекса в одном файле.

### Pros and Cons
**Плюсы:**
- Работа с одним файлом → нет больших задержек на открытие многих файлов

**Минусы:**
- Сложность сжатия файла. При частом обновлении индекса появляются пустые блоки,
которые нужно как-то отслеживать и сжимать.

### Implementation
Создаём два файла:
1. Метаданные индекса
2. Сам индекс

#### Metadata file
Описывает индекс-файл.
Представляет собой хеш-таблицу `<token_body, (block_bitmask, block_num)>`.
- `block_bitmask`:
Определяет размер блока. Блоки могут иметь разную длину, что сделано для того, чтобы
эффективно хранить токены с разной частотой.
- `block_num`:
Обозначает порядковый номер блока размером, указанного в `block_bitmask`.

#### Index file
Файл в байтовом представлении. Хранит пары `(size_t, size_t)`.
Если при обновлении индекса блок выходит за свои рамки, 
он расширяется до блока следующего размера.

## Multi approach