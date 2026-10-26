

import json
import re
import subprocess
from http import HTTPStatus
from wsgiref.simple_server import make_server

WSL_EXE = r"C:\Program Files\WSL\wsl.exe"

ALLOWED_COMMANDS = {
    "pwd", "ls", "whoami", "uname", "mkdir"
}

ALLOWED_DISTROS = {"Ubuntu", "kali-linux"}


def response(start_response, status, payload, extra_headers=None):
    body = (
        json.dumps(payload, ensure_ascii=False) + "\n"
    ).encode("utf-8")

    headers = [
        ("Content-Type", "application/json; charset=utf-8"),
        ("Content-Length", str(len(body))),
        ("Access-Control-Allow-Origin", "*"),
        ("Access-Control-Allow-Headers", "Content-Type"),
        ("Access-Control-Allow-Methods", "POST, OPTIONS"),
    ]

    if extra_headers:
        headers.extend(extra_headers)

    start_response(
        f"{status.value} {status.phrase}",
        headers
    )
    return [body]


def run_linux_command(distro, command_text):
    if not isinstance(distro, str) or distro not in ALLOWED_DISTROS:
        return "Invalid Linux distribution.", 1

    if not isinstance(command_text, str) or len(command_text) > 200:
        return "Invalid command.", 1

    parts = command_text.strip().split()

    if not parts:
        return "Enter a command.", 1

    command = parts[0]
    args = parts[1:]

    if command not in ALLOWED_COMMANDS:
        return f"Command not allowed: {command}", 1

    if command == "mkdir":
        if len(args) != 1:
            return "Usage: mkdir DIRECTORY_NAME", 1

        if not re.fullmatch(r"[A-Za-z0-9_-]+", args[0]):
            return "Use letters, numbers, underscores or hyphens in the folder name.", 1

    elif command == "uname":
        if args not in ([], ["-a"]):
            return "Usage: uname or uname -a", 1

    elif command == "ls":
        if args not in ([], ["-l"], ["-a"], ["-la"], ["-al"]):
            return "Usage: ls, ls -l, ls -a or ls -la", 1

    elif args:
        return f"Usage: {command}", 1

    try:
        result = subprocess.run(
            [
                WSL_EXE,
                "--distribution", distro,
                "--exec", command,
                *args
            ],
            capture_output=True,
            text=True,
            timeout=20,
            shell=False
        )

        output = result.stdout + result.stderr

        if not output.strip():
            output = (
                "(Command completed successfully.)"
                if result.returncode == 0
                else "(No output)"
            )

        return output.rstrip(), result.returncode

    except subprocess.TimeoutExpired:
        return "Command timed out. Try again.", 1

    except Exception as error:
        return f"Could not run Linux command: {error}", 1


def app(environ, start_response):
    method = environ.get("REQUEST_METHOD", "")
    path = environ.get("PATH_INFO", "")

    # Handle the browser's CORS preflight request.
    if method == "OPTIONS" and path == "/api/terminal":
        start_response("204 No Content", [
            ("Access-Control-Allow-Origin", "*"),
            ("Access-Control-Allow-Headers", "Content-Type"),
            ("Access-Control-Allow-Methods", "POST, OPTIONS"),
            ("Content-Length", "0"),
        ])
        return [b""]

    if path != "/api/terminal":
        return response(
            start_response,
            HTTPStatus.NOT_FOUND,
            {"output": "Endpoint not found.", "exitCode": 1}
        )

    if method != "POST":
        return response(
            start_response,
            HTTPStatus.METHOD_NOT_ALLOWED,
            {"output": "Use POST for terminal commands.", "exitCode": 1}
        )

    try:
        content_length = int(environ.get("CONTENT_LENGTH") or 0)

        if content_length <= 0 or content_length > 4096:
            raise ValueError("Invalid request size")

        raw_data = environ["wsgi.input"].read(content_length)
        data = json.loads(raw_data.decode("utf-8"))

        if not isinstance(data, dict):
            raise ValueError("JSON must be an object")

    except (ValueError, UnicodeDecodeError, json.JSONDecodeError):
        return response(
            start_response,
            HTTPStatus.BAD_REQUEST,
            {"output": "Invalid JSON request.", "exitCode": 1}
        )

    command = data.get("command", "")
    distro = data.get("distro", "Ubuntu")

    if not isinstance(command, str) or len(command) > 200:
        return response(
            start_response,
            HTTPStatus.BAD_REQUEST,
            {"output": "Invalid command.", "exitCode": 1}
        )

    output, exit_code = run_linux_command(distro, command)

    return response(
        start_response,
        HTTPStatus.OK,
        {"output": output, "exitCode": exit_code}
    )


if __name__ == "__main__":
    print("VaradShell backend starting...")
    print("API: http://127.0.0.1:5000/api/terminal")
    print("Press Ctrl+C to stop.")

    with make_server("127.0.0.1", 5000, app) as server:
        server.serve_forever()
