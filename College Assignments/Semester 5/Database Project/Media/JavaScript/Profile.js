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

async function startPage(){
    let users = await createUsers();
    localStorage.setItem('isUserLoggedIn', 'true');
    // Call the function to display quizzes for the user
    displayQuizzes(users);
    let questions = await CreateQuestions();
};

function displayQuizzes(user) {
    // Proceed if user and user.grades exist
    const container = document.getElementById('quizContainer');
    container.innerHTML = '';

    // Calculate the average grade
    const grades = user.grades.map(quiz => quiz.grade);
    const totalGrades = grades.reduce((sum, grade) => sum + grade, 0);
    const averageGrade = grades.length > 0 ? (totalGrades / grades.length).toFixed(2) : 0;

    // Display each quiz and grade
    user.grades.forEach((quiz, index) => {
        const quizDiv = document.createElement('div');
        quizDiv.classList.add('quiz-information');

        const questionDiv = document.createElement('div');
        questionDiv.classList.add('question');
        questionDiv.innerHTML = `Quiz ${index + 1}: ${quiz.quizName}`;

        const answersDiv = document.createElement('div');
        answersDiv.classList.add('answers');
        answersDiv.innerHTML = `Grade: ${quiz.grade}`;

        // Create a delete button
        const deleteButton = document.createElement('button');
        deleteButton.classList.add('delete-button');
        deleteButton.textContent = 'Delete';
        deleteButton.onclick = async () => {
            if (confirm(`Are you sure you want to delete Quiz ${index + 1}: ${quiz.quizName}?`)) {
                console.log(quiz);
                console.log(quiz.gradeID);
                await deleteGrade(quiz.gradeID); // Call the deleteGrade function
            }
        };

        // Append elements to the quizDiv
        quizDiv.appendChild(questionDiv);
        quizDiv.appendChild(answersDiv);
        quizDiv.appendChild(deleteButton);

        // Append the quizDiv to the container
        container.appendChild(quizDiv);
    });

    // Display the average grade
    const averageDiv = document.createElement('div');
    averageDiv.classList.add('average-grade');
    averageDiv.innerHTML = `<strong>Average Grade:</strong> ${averageGrade}`;

    container.appendChild(averageDiv);
}


