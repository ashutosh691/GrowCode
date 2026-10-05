async function pollSubmissionStatus(submissionId) {
    const message = document.getElementById("submission-message");

    const finalStatuses = new Set([
        "ACCEPTED",
        "WRONG_ANSWER",
        "COMPILATION_ERROR",
        "RUNTIME_ERROR",
        "TIME_LIMIT_EXCEEDED"
    ]);

    while (true) {
        try {
            const response = await fetch(
                `/api/submissions/${submissionId}`,
                {
                    credentials: "include"
                }
            );

            if (!response.ok) {
                throw new Error("Unable to get submission status");
            }

            const result = await response.json();

            message.textContent =
                `Submission #${submissionId}: ${result.status}`;

            if (finalStatuses.has(result.status)) {
                break;
            }

            await new Promise(resolve =>
                setTimeout(resolve, 1000)
            );
        } catch (error) {
            console.error(error);

            message.textContent =
                "Submission received, but status could not be checked.";

            break;
        }
    }
}

async function submitSolution(problemId) {
    const code = document.getElementById("code-editor").value;
    const message = document.getElementById("submission-message");
    const button = document.getElementById("submit-button");

    if (!code.trim()) {
        message.textContent = "Please write some code first.";
        return;
    }

    button.disabled = true;
    message.textContent = "Submitting...";

    try {
        const response = await fetch("/api/submissions", {
            method: "POST",
            credentials: "include",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                problem_id: Number(problemId),
                language_id: 1,
                code: code
            })
        });

        const result = await response.json();

        if (!response.ok) {
            message.textContent =
                result.error || "Submission failed.";
            button.disabled = false;
            return;
        }

        message.textContent =
            `Submission received. Submission ID: ${result.submission_id}`;

        await pollSubmissionStatus(result.submission_id);

        button.disabled = false;

    } catch (error) {
        console.error(error);

        message.textContent =
            "Unable to submit solution.";

        button.disabled = false;
    }
}

async function loadProblem() {
    const container = document.getElementById("problem-container");

    const params = new URLSearchParams(window.location.search);
    const problemId = params.get("id");

    if (!problemId) {
        container.innerHTML = "<p>Problem ID is missing.</p>";
        return;
    }

    try {
        const response = await fetch(
            `/api/problems/${problemId}`
        );

        if (!response.ok) {
            throw new Error("Problem not found");
        }

        const problem = await response.json();

        container.innerHTML = `
            <div class="problem-detail">

                <div class="problem-header">
                    <div>
                        <h1>${problem.problem_id}. ${problem.title}</h1>

                        <span class="difficulty">
                            ${problem.difficulty}
                        </span>
                    </div>
                </div>

                <section class="problem-section">
                    <h2>Description</h2>
                    <p>${problem.description}</p>
                </section>

                <section class="problem-section">
                    <h2>Constraints</h2>
                    <p>${problem.constraints}</p>
                </section>

                <section class="problem-section">
                    <h2>Sample Input / Output</h2>

                    <div id="sample-test-cases">
                        ${
                            problem.sample_test_cases &&
                            problem.sample_test_cases.length > 0
                            ? problem.sample_test_cases.map((sample, index) => `
                                <div class="sample-test-case">

                                    <h3>Sample ${index + 1}</h3>

                                    <p><strong>Input</strong></p>

                                    <pre>${sample.input || "(empty)"}</pre>

                                    <p><strong>Output</strong></p>

                                    <pre>${sample.expected_output}</pre>

                                </div>
                            `).join("")
                            : "<p>No sample test cases available.</p>"
                        }
                    </div>
                </section>

                <section class="editor-section">

                    <h2>Your Solution</h2>

                    <textarea
                        id="code-editor"
                        placeholder="// Write your C++ solution here..."
                    ></textarea>

                    <button id="submit-button">
                        Submit Solution
                    </button>

                    <p id="submission-message"></p>

                </section>

            </div>
        `;

        document
            .getElementById("submit-button")
            .addEventListener("click", () => {
                submitSolution(problemId);
            });

    } catch (error) {
        console.error(error);

        container.innerHTML =
            "<p>Unable to load this problem.</p>";
    }
}

loadProblem();