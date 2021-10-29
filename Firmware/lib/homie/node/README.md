# Node 

Класс для работы с нодами.

***
## API

- Node(const char* name, const char* id, Device* device)
- virtual bool Init(Homie* homie)
- void AddProperty(Property* property)
- String GetId() const
- Device* GetDevice() const
- Property* GetProperty(String id)
- virtual void HandleCurrentState()


***

**Node(const char\* name, const char\* id, Device\* device)**

Создает объект ноды.

- name: имя ноды.
- id: идентификатор ноды.
- device: указатель на объект [устройства](../device/README.md).

***

**virtual bool Init(Homie\* homie)**

Инициализирует ноду опубликовав необходимые аттрибуты. Вызывает инициализацию у [свойств](../property/README.md). Сохраняет указатель на объект [homie](../README.md).

- homie: указатель на объект [homie](../README.md).

Возвращает статус инициализации.

***

**void AddProperty(Property\* property)**

Добавляет указатель на свойство в список свойств для дальнейшего обращения.

- property: указатель на [свойство](../property/README.md).

***

**String GetId() const**

Возвращает идентификатор ноды.

***

**Device\* GetDevice() const**

Возвращает указатель на объект [устройства](../device/README.md).

***

**Property\* GetProperty(String id)**

Возвращает указатель на свойство по идентификатору.

- id: идентификатор [свойствa](../property/README.md).

***

**virtual void HandleCurrentState()**

Обрабатывает текущее состяние ноды. Вызывает обработку текущих состояний у [свойств](../property/README.md).
