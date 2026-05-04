# Ключевые модули

### Agent (agent/Agent.cpp)
Центральный модуль:
- run() — главный цикл
- registerAgent() — регистрация на сервере
- pollTasks() — получение и выполнение заданий

### Config (config/Config.cpp)
Работа с config.json:
- Загрузка/сохранение JSON
- Доступ к параметрам

### HttpClient (http/HttpClient.cpp)
HTTP запросы через libcurl:
- POST с JSON
- Поддержка HTTPS

### TaskExecutor (tasks/TaskExecutor.cpp)
Выполнение заданий:
- TIMEOUT — меняет интервал опроса
- CONF — меняет параметры конфига
- FILE — (в разработке)

# Logger (logging/Logger.h)
Логирование через spdlog:
- Запись в файл
- Уровни INFO/ERROR/DEBUG

# Как это работает

1. **main.cpp** → загружает конфиг, запускает Agent
2. Agent::run() → бесконечный цикл:
   - Спрашивает сервер: "есть задание?"
   - Если есть → выполняет через TaskExecutor
   - Отправляет результат
   - Спит poll_interval_sec секунд
   - Повторяет
