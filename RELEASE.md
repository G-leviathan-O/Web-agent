## 🚀 Running Release (Linux & Windows)

В этом разделе описано, как запустить готовый релиз Web-Agent на Linux и Windows.

Агент запускается как фоновое приложение и требует наличия файла конфигурации `config.json` в той же директории, что и исполняемый файл.

---

# 🐧 Linux

# 📁 Структура релиза

```text
realeses/linux/
├── Web-agent
└── config.json
```
### 🪟 Windows
#### 📁 Структура релиза
```text
realeses/windows/
├── Web-agent.exe
└── config.json
```
▶️ Запуск

Откройте PowerShell или командную строку:

cd realeses\windows
.\Web-agent.exe
✅ Проверка работы

После запуска:

создаётся файл логов (например agent.log);
агент подключается к серверу;
выполняются задачи (если они есть).
