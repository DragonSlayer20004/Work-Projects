// Check if isUserLoggedIn exists in localStorage
if (localStorage.getItem('isUserLoggedIn') === null) {
    // Set isUserLoggedIn to false for first-time users
    localStorage.setItem('isUserLoggedIn', 'false');
}

// Get the variable on other pages
const isUserLoggedIn = localStorage.getItem('isUserLoggedIn') === 'true';

document.addEventListener("DOMContentLoaded", function() {
    // Select the div elements
    const logInRequest = document.getElementById('logInRequest');
    const profileInformation = document.getElementById('profileInformation');

    // Check the login status
    if (localStorage.getItem('isUserLoggedIn') === 'false') {
        // Show log-in request and hide profile information
        logInRequest.style.display = "block";
        profileInformation.style.display = "none";
    } else {
        // Show profile information and hide log-in request
        logInRequest.style.display = "none";
        profileInformation.style.display = "block";
    }
});