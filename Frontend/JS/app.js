

document.addEventListener("DOMContentLoaded", () => {
    const terminalForm = document.getElementById("terminalForm");
    const commandInput = document.getElementById("commandInput");
    const terminalOutput = document.getElementById("terminalOutput");
    const distroSelect = document.getElementById("distroSelect");
    const clearBtn = document.getElementById("clearBtn");
    const fullscreenBtn = document.getElementById("fullscreenBtn");
    const terminalSection = document.querySelector(".terminal-section");
    const logoutBtn = document.getElementById("logoutBtn");

    // Show a message if an expected HTML element is missing.
    if (
        !terminalForm ||
        !commandInput ||
        !terminalOutput ||
        !distroSelect
    ) {
        console.error(
            "VaradShell: Check the terminal element IDs in dashboard.html."
        );
        return;
    }

    const API_URL = "http://127.0.0.1:5000/api/terminal";

    // Add output to the terminal safely.
    function addOutput(text, className = "terminal-result") {
        const element = document.createElement("pre");
        element.className = className;
        element.textContent = text;
        terminalOutput.appendChild(element);
        terminalOutput.scrollTop = terminalOutput.scrollHeight;
        return element;
    }

    // Display the command entered by the user.
    function addCommand(command) {
        const line = document.createElement("p");
        line.className = "terminal-line";

        const prompt = document.createElement("span");
        prompt.className = "prompt";
        prompt.textContent = `varad@VaradShell:~$ ${command}`;

        line.appendChild(prompt);
        terminalOutput.appendChild(line);
    }

    // Send the command to the Flask backend.
    terminalForm.addEventListener("submit", async (event) => {
        event.preventDefault();

        const command = commandInput.value.trim();

        if (!command) {
            return;
        }

        // Clear the terminal without sending a Linux command.
        if (command === "clear") {
            terminalOutput.replaceChildren();
            commandInput.value = "";
            return;
        }

        const distro = distroSelect.value;

        addCommand(command);
        commandInput.value = "";
        commandInput.disabled = true;

        const loading = addOutput("Running command...", "terminal-result");

        try {
            const response = await fetch(API_URL, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    command: command,
                    distro: distro
                })
            });

            const data = await response.json();
            loading.remove();

            if (data.output) {
                const result = addOutput(data.output);

                if (!response.ok || data.exitCode !== 0) {
                    result.style.color = "#ff626c";
                }
            } else {
                addOutput("The server returned no output.");
            }
        } catch (error) {
            loading.remove();

            addOutput(
                "Cannot connect to Flask.\n" +
                "1. Check that the backend is running.\n" +
                "2. Confirm it uses http://127.0.0.1:5000.\n" +
                "3. Check the VS Code terminal for errors."
            ).style.color = "#ff626c";

            console.error("VaradShell backend error:", error);
        } finally {
            commandInput.disabled = false;
            commandInput.focus();
            terminalOutput.scrollTop = terminalOutput.scrollHeight;
        }
    });

    // Clear terminal button.
    if (clearBtn) {
        clearBtn.addEventListener("click", () => {
            terminalOutput.replaceChildren();
            commandInput.focus();
        });
    }

    // Fullscreen terminal button.
    if (fullscreenBtn && terminalSection) {
        fullscreenBtn.addEventListener("click", async () => {
            try {
                if (!document.fullscreenElement) {
                    await terminalSection.requestFullscreen();
                } else {
                    await document.exitFullscreen();
                }
            } catch (error) {
                console.error("Fullscreen error:", error);
                addOutput("Fullscreen is not available in this browser.");
            }
        });
    }

    // Switch between Ubuntu and Kali.
    distroSelect.addEventListener("change", () => {
        addOutput(
            `Selected Linux distribution: ${distroSelect.value}`,
            "terminal-result"
        );
    });

    // Let the user press Escape to leave fullscreen.
    document.addEventListener("fullscreenchange", () => {
        if (fullscreenBtn) {
            fullscreenBtn.textContent =
                document.fullscreenElement
                    ? "Exit Fullscreen"
                    : "Fullscreen";
        }
    });

    // Logout placeholder until login/session authentication is implemented.
    if (logoutBtn) {
        logoutBtn.addEventListener("click", () => {
            const confirmLogout = window.confirm(
                "Do you want to leave the dashboard?"
            );

            if (confirmLogout) {
                window.location.href = "login.html";
            }
        });
    }

    // Initial message.
    addOutput(
        "VaradShell v3.1 ready.\n" +
        "Connect to the Flask backend to run supported Linux commands."
    );

    commandInput.focus();
});
