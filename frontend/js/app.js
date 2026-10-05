async function loadProblems() {
    const container = document.getElementById("problems-container");
    const count = document.getElementById("problem-count");

    try {
        const response = await fetch("/api/problems");

        if (!response.ok) {
            throw new Error("Failed to fetch problems");
        }

        const problems = await response.json();

        count.textContent = `${problems.length} problems`;

        container.innerHTML = "";

        problems.forEach(problem => {
            const card = document.createElement("div");
        
            card.className = "problem-card";
        
            card.innerHTML = `
                <h3>${problem.problem_id}. ${problem.title}</h3>
                <p>${problem.description}</p>
                <span class="difficulty">${problem.difficulty}</span>
            `;
        
            card.addEventListener("click", () => {
                window.location.href = `/problem.html?id=${problem.problem_id}`;
            });
        
            container.appendChild(card);
        });

        if (problems.length === 0) {
            container.innerHTML = "<p>No problems available.</p>";
        }

    } catch (error) {
        console.error(error);

        count.textContent = "";

        container.innerHTML =
            "<p>Unable to load problems from the server.</p>";
    }
}

async function updateAuthNavigation() {
    const authLink = document.getElementById("auth-link");

    if (!authLink) {
        return;
    }

    try {
        const response = await fetch("/api/session");

        if (response.ok) {
            authLink.textContent = "Logout";
            authLink.href = "#";

            authLink.onclick = async function (event) {
                event.preventDefault();

                await fetch("/api/logout", {
                    method: "POST"
                });

                window.location.href = "/";
            };
        } else {
            authLink.textContent = "Login";
            authLink.href = "/login.html";
            authLink.onclick = null;
        }
    } catch (error) {
        authLink.textContent = "Login";
        authLink.href = "/login.html";
    }
}

document.addEventListener("DOMContentLoaded", updateAuthNavigation);

loadProblems();
