# Collatz Project — Docker

## Запуск локально в Docker

Після клонування репозиторія виконайте:

```bash
docker build -t collatz-project .
docker run --rm -v "$(pwd):/app" collatz-project
```

Після запуску програма виведе результат у консоль і створить/оновить файл `results.txt` у корені репозиторія.

## Альтернативний запуск без збереження results.txt на хості

```bash
docker run --rm collatz-project
```
