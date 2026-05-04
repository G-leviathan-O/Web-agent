# Документация Tasks API

## Общая информация

Агент получает задачи с сервера через эндпоинт:


/wa_task/


Пример ответа сервера:

```json
{
  "session_id": "123456",
  "task_code": "TIMEOUT",
  "options": "30"
}
Поля ответа
Поле	Тип	Описание
session_id	string	Идентификатор сессии
task_code	string	Тип задачи
options	string	Параметры задачи
Поле options

Поле options содержит параметры, необходимые для выполнения задачи.
Формат зависит от значения task_code.

Доступные задачи
TIMEOUT

Изменяет интервал опроса сервера агентом.

Что делает:

Изменяет параметр:

poll_interval_sec

Формат options:

<число_секунд>

Пример:

{
  "task_code": "TIMEOUT",
  "options": "60"
}
CONF

Изменяет параметр в файле config.json.

Формат options:

ключ=значение

Примеры:

{
  "task_code": "CONF",
  "options": "descr=Office PC"
}
{
  "task_code": "CONF",
  "options": "server_url=https://example.com/api"
}
FILE

Отправляет файл на сервер.

Что делает:

Агент ищет файл в директории:

result_directory

и отправляет его на сервер.

Формат options:

имя_файла

Пример:

{
  "task_code": "FILE",
  "options": "report.txt"
}
TASK

Запускает внешнюю программу.

Что делает:

Агент запускает программу, указанную в конфигурации:

"task_program": "program.exe"

Формат options:

В данный момент не используется.

Пример:

{
  "task_code": "TASK",
  "options": ""
}
