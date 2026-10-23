/*
    Project Name: Varad_shell
    Language: C++
    Project Type: Linux terminal application

    Problem Statement: Windows dont directly execute windows commands in its normal command prompt

    solution: Varad_shell is a linux application that canexecute windows commands in its normal command prompt
              and it can gives you option to execute in ubuntu or kali linux terminal distribution
              
    Concepts Used:
1. Header Files
2. Variables and Data Types
3. Input and Output (cin, cout)
4. Conditional Statements (if, else if, else)
5. While Loop
6. String Handling
7. Functions (main, system, getline)
8. Break and Continue Statements
9. Return Statements
10. Command Execution using WSL

Features:
1. Select Ubuntu or Kali Linux
2. Execute basic Linux commands
3. Display command output
4. Built-in help menu
5. Clear the console
6. Exit the application

Limitation:
This is a basic console-based prototype, not a complete
Linux operating system or graphical terminal emulator.
Each command starts a new Bash process, so commands like
cd do not preserve the working directory.          
*/







#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

const char* WSL_EXE = "C:\\Program Files\\WSL\\wsl.exe";

const string READY_MARKER = "__VARADSHELL_READY__";
const string PWD_MARKER = "__VARADSHELL_PWD__";
const string CD_MARKER = "__VARADSHELL_CD_82741__";

// Remove extra spaces and newline characters.
string trim(const string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");

    if (start == string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");

    return s.substr(start, end - start + 1);
}

// Correctly quote an argument for Windows CreateProcess.
string windowsQuote(const string& arg)
{
    string result = "\"";
    size_t backslashes = 0;

    for (char c : arg)
    {
        if (c == '\\')
        {
            backslashes++;
        }
        else if (c == '"')
        {
            result.append(backslashes * 2 + 1, '\\');
            result += '"';
            backslashes = 0;
        }
        else
        {
            result.append(backslashes, '\\');
            backslashes = 0;
            result += c;
        }
    }

    result.append(backslashes * 2, '\\');
    result += '"';

    return result;
}

// Quote a directory path for the Linux Bash shell.
string linuxQuote(const string& path)
{
    string result = "'";

    for (char c : path)
    {
        if (c == '\'')
            result += "'\\''";
        else
            result += c;
    }

    result += "'";
    return result;
}

// Find the text following a marker.
string getMarkerValue(const string& output, const string& marker)
{
    size_t pos = output.rfind(marker);

    if (pos == string::npos)
        return "";

    pos += marker.length();

    size_t end = output.find_first_of("\r\n", pos);

    if (end == string::npos)
        end = output.length();

    return trim(output.substr(pos, end - pos));
}

// Run a Linux command through the selected WSL distribution.
bool runWsl(
    const string& distro,
    const string& linuxCommand,
    string& output,
    DWORD& exitCode)
{
    output.clear();
    exitCode = 0;

    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE readPipe = NULL;
    HANDLE writePipe = NULL;

    if (!CreatePipe(&readPipe, &writePipe, &sa, 0))
    {
        cerr << "Could not create output pipe. Windows error: "
             << GetLastError() << "\n";
        return false;
    }

    // The parent process must not inherit the read end.
    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = writePipe;
    si.hStdError = writePipe;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    string commandLine =
        windowsQuote(WSL_EXE) +
        " --distribution " + distro +
        " --exec /bin/bash --noprofile --norc -c " +
        windowsQuote(linuxCommand);

    // CreateProcess may modify the command-line buffer.
    BOOL created = CreateProcessA(
        WSL_EXE,
        &commandLine[0],
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    CloseHandle(writePipe);
    writePipe = NULL;

    if (!created)
    {
        DWORD error = GetLastError();

        CloseHandle(readPipe);

        cerr << "Could not start WSL. Windows error: "
             << error << "\n";

        return false;
    }

    // Read standard output and standard error.
    char buffer[4096];
    DWORD bytesRead = 0;

    while (ReadFile(
        readPipe,
        buffer,
        sizeof(buffer),
        &bytesRead,
        NULL) && bytesRead > 0)
    {
        output.append(buffer, bytesRead);
    }

    CloseHandle(readPipe);

    WaitForSingleObject(pi.hProcess, INFINITE);

    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return true;
}

// Remove the internal directory marker from displayed output.
void removeDirectoryMarker(string& output, string& currentDirectory)
{
    size_t pos = output.rfind(CD_MARKER);

    if (pos == string::npos)
        return;

    string newDirectory = getMarkerValue(output, CD_MARKER);

    if (!newDirectory.empty())
        currentDirectory = newDirectory;

    output.erase(pos);

    while (!output.empty() &&
           (output.back() == '\n' || output.back() == '\r'))
    {
        output.pop_back();
    }
}

// Display the built-in help.
void showHelp()
{
    cout << "\n========== VARADSHELL HELP ==========\n";
    cout << "help       - Show available commands\n";
    cout << "pwd        - Show current Linux directory\n";
    cout << "ls         - List files and directories\n";
    cout << "cd <path>  - Change current directory\n";
    cout << "mkdir <dir>- Create a directory\n";
    cout << "clear      - Clear the terminal screen\n";
    cout << "exit       - Exit VaradShell\n";
    cout << "=====================================\n\n";

    cout << "You can also run normal Linux commands.\n";
    cout << "Examples: whoami, uname -a, ls -l, cat file.txt\n\n";
}

int main()
{
    string distro;
    string currentDirectory;
    string output;
    DWORD exitCode = 0;

    cout << "=====================================\n";
    cout << "          VARADSHELL v3.1\n";
    cout << "      C++ Linux Command Shell\n";
    cout << "=====================================\n\n";

    cout << "Select your Linux distribution:\n";
    cout << "1. Ubuntu\n";
    cout << "2. Kali Linux\n";
    cout << "\nEnter choice (1 or 2): ";

    string choice;
    getline(cin, choice);

    if (choice == "1")
    {
        distro = "Ubuntu";
    }
    else if (choice == "2")
    {
        distro = "kali-linux";
    }
    else
    {
        cout << "\nInvalid choice. Restart VaradShell and select 1 or 2.\n";
        return 1;
    }

    cout << "\nChecking Linux distribution...\n";

    string testCommand =
        "printf '" + READY_MARKER + "\\n'";

    if (!runWsl(distro, testCommand, output, exitCode))
    {
        cout << "\nCould not launch WSL.\n";
        return 1;
    }

    if (exitCode != 0 ||
        output.find(READY_MARKER) == string::npos)
    {
        cout << "\nCould not start the selected distribution.\n";
        cout << "WSL output:\n" << output << "\n";
        cout << "Exit code: " << exitCode << "\n";
        return 1;
    }

    cout << "Connected to " << distro << " successfully!\n";

    // Discover the initial Linux working directory.
    string pwdCommand =
        "printf '" + PWD_MARKER + "%s\\n' \"$PWD\"";

    if (runWsl(distro, pwdCommand, output, exitCode))
    {
        currentDirectory = getMarkerValue(output, PWD_MARKER);
    }

    if (currentDirectory.empty())
        currentDirectory = "/";

    cout << "Linux directory: " << currentDirectory << "\n";
    cout << "\nType 'help' to see commands.\n\n";

    while (true)
    {
        cout << "VaradShell:" << currentDirectory << "$ ";

        string userCommand;
        if (!getline(cin, userCommand))
            break;

        userCommand = trim(userCommand);

        if (userCommand.empty())
            continue;

        if (userCommand == "exit")
        {
            cout << "Exiting VaradShell. Goodbye!\n";
            break;
        }

        if (userCommand == "help")
        {
            showHelp();
            continue;
        }

        if (userCommand == "clear")
        {
            system("cls");
            continue;
        }

        // Execute the user's command from the remembered directory.
        // The marker reports any directory change back to C++.
        string linuxCommand =
            "cd " + linuxQuote(currentDirectory) +
            " || exit $?; { " + userCommand +
            "; }; status=$?; printf '\\n" +
            CD_MARKER + "%s\\n' \"$PWD\"; exit \"$status\"";

        if (!runWsl(distro, linuxCommand, output, exitCode))
        {
            cout << "Failed to execute the Linux command.\n";
            continue;
        }

        removeDirectoryMarker(output, currentDirectory);

        if (!output.empty())
        {
            cout << output;

            if (output.back() != '\n')
                cout << '\n';
        }

        if (exitCode != 0)
        {
            cout << "[Command exited with code "
                 << exitCode << "]\n";
        }
    }

    return 0;
}
