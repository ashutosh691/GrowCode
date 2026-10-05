async function loadSubmissionHistory() {
    const message = document.getElementById("history-message");
    const container = document.getElementById("submission-history");

    try {
        const response = await fetch(
            "/api/submissions",
            {
                credentials: "include"
            }
        );

        const result = await response.json();

        if (!response.ok) {
            message.textContent =
                result.error || "Unable to load submissions.";
            return;
        }

        if (result.length === 0) {
            message.textContent =
                "You have not made any submissions yet.";
            return;
        }

        message.textContent = "";

        container.innerHTML = `
            <table class="submission-table">

                <thead>
                    <tr>
                        <th>Submission ID</th>
                        <th>Problem</th>
                        <th>Language</th>
                        <th>Status</th>
                        <th>Submitted At</th>
                    </tr>
                </thead>

                <tbody>
                    ${result.map(submission => `
                        <tr>
                            <td>${submission.submission_id}</td>

                            <td>
                                ${submission.problem_title}
                            </td>

                            <td>
                                ${submission.language}
                            </td>

                            <td>
                                <span class="status ${submission.status.toLowerCase()}">
                                    ${submission.status}
                                </span>
                            </td>

                            <td>
                                ${submission.submitted_at}
                            </td>
                        </tr>
                    `).join("")}
                </tbody>

            </table>
        `;

    } catch (error) {
        console.error(error);

        message.textContent =
            "Unable to connect to the server.";
    }
}

loadSubmissionHistory();