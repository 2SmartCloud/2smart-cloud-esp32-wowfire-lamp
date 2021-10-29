# Utils

Библиотека с функциями-утилитами.

***

## API

- std::string RandomString(uint8_t seed, uint8_t length)
- String TimeToStr(uint8_t hours, uint8_t minutes)
- String Sha256(String rec_data)

***

**std::string RandomString(uint8_t seed, uint8_t length)**

Генерирует строку указанной длинны со случайными буквами и числами.

- seed: число которое используется для создания строки.
- length: длинн желаемой строки.

Возвращает строку со случайными буквами и числами.

***

**String TimeToStr(uint8_t hours, uint8_t minutes)**

Соединяет часы и минуты в строку.

Пример: `"15:37"`.

- hours: часы.
- minutes: минуты.

Возвращает строку со временем. 

***

**String Sha256(String data)**

Вычисляет хеш от строки используя алгоритм хеширования SHA256.

- data: строка от которой вычисляется хеш.

Возвращает вычисленный хеш.
