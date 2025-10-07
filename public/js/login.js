async function handleSubmit(event) {
    event.preventDefault(); // Prevent form default submission

    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;

    const data = {
        nama: username,
        password: password,
    };

    // Show loading spinner
    document.getElementById("loginSpinner").classList.remove("hidden");
    document.getElementById("loginText").innerText = "Signing in...";

    try {
        const response = await fetch('http://localhost:9001/api/login', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data), // Send data as JSON
        });

        const result = await response.json();
        console.log(result);

        // Check for successful response and redirect
        if (response.ok && result.redirect) { // Corrected key here
            // Redirect to the URL provided by the server
            window.location.href = result.redirect;
        } else {
            // If login fails, show error message
            alert("Login failed: " + (result.error || "Unknown error"));
        }
    } catch (error) {
        alert("An error occurred: " + error);
    } finally {
        // Hide spinner and reset button text
        document.getElementById("loginSpinner").classList.add("hidden");
        document.getElementById("loginText").innerText = "Sign In";
    }
}

