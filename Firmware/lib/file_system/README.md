# file_system

Библиотека для работы с файловой системой.

***

## API

- bool InitFiles()
- bool LoadConfig()
- bool SaveConfig()
- bool EraseFlash()
- bool WriteSettings(const char *file_path, uint8_t *data, size_t - size_of_data)
- bool ReadSettings(const char *file_path, uint8_t *data, size_t - size_of_data)

***

**bool InitFiles()**

Инициализирует файловую систему.

***

**bool LoadConfig()**

Читает конфигурационный файл с параметрами сети и пользователя.

***

**bool SaveConfig()**

Записывает конфигурационный файл с параметрами сети и пользователя.

***

**bool EraseFlash()**

Перезаписывает конфигурационный файл с параметрами сети и пользователя.

***

**bool WriteSettings(const char \*file_path, uint8_t \*data, size_t - size_of_data)**

Записывает данные в память.

- file_path: имя файла.
- data: данные.
- size_of_data: размер данных.

Возвращает статус записи.

***

**bool ReadSettings(const char \*file_path, uint8_t \*data, size_t - size_of_data)**

Считывает данные из памяти.

- file_path: имя файла.
- data: данные.
- size_of_data: размер данных.

Возвращает статус считывания.

***
