async function loadProgress() {
    const message =
        document.getElementById("progress-message");

    try {
        const response = await fetch(
            "/api/progress",
            {
                credentials: "include"
            }
        );

        const result = await response.json();

        if (!response.ok) {
            message.textContent =
                result.error || "Unable to load progress.";
            return;
        }

        document.getElementById("problems-solved").textContent =
            result.problems_solved;

        document.getElementById("total-attempts").textContent =
            result.total_attempts;

        document.getElementById("accepted").textContent =
            result.accepted;

        document.getElementById("wrong-answer").textContent =
            result.wrong_answer;

        document.getElementById("compilation-error").textContent =
            result.compilation_error;

        document.getElementById("runtime-error").textContent =
            result.runtime_error;

        document.getElementById("time-limit-exceeded").textContent =
            result.time_limit_exceeded;

        message.textContent = "";

    } catch (error) {
        console.error(error);

        message.textContent =
            "Unable to connect to the server.";
    }
}

loadProgress();
