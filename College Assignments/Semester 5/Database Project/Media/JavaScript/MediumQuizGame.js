// Global Variables
let quizContainer, resultsContainer, submitButton, previousButton, nextButton, currentSlide, myQuestions = 0;
let slides;

// Check if isUserLoggedIn exists in localStorage
if (localStorage.getItem('isUserLoggedIn') === null) {
  // Set isUserLoggedIn to false for first-time users
  localStorage.setItem('isUserLoggedIn', 'false');
}

// this is our entry point
async function startPage() {
  // Wait for rockImages to be populated
  rockImages = await createRocks();

  myQuestions = await CreateQuestions();
  buildQuiz();
  slides = document.querySelectorAll(".slide");
  currentSlide = 0;
  showSlide(0);
}

document.addEventListener("DOMContentLoaded", function() {
  // Initialize DOM Variables
  quizContainer = document.getElementById('quiz');
  resultsContainer = document.getElementById('results');
  submitButton = document.getElementById('submit');
  previousButton = document.getElementById("previous");
  nextButton = document.getElementById("next");

  // Initialize quiz
  startPage();

  // Event listeners
  submitButton.addEventListener('click', showResults);
  previousButton.addEventListener("click", showPreviousSlide);
  nextButton.addEventListener("click", showNextSlide);
});

// Functions
function buildQuiz() {
  // variable to store the HTML output
  const output = [];

  // Set image attributes if applicable
  $("h1").attr({"src": rockImages.src, "alt": rockImages.alt});
  $("h1").text(rockImages.caption);

  // For each question...
  myQuestions.forEach((questionGroup, questionGroupNumber) => {
    // Check if questionGroup is not null
    if (questionGroup != null) {
      questionGroup.forEach((currentQuestion, questionNumber) => {
        // Check if currentQuestion is not null and has answers
        if (currentQuestion != null && currentQuestion.answers != null) {
          const answers = [];

          // Iterate over the answers, assuming currentQuestion.answers is an array of objects
          currentQuestion.answers.forEach(answer => {
            // Check if the answer description is valid (non-null and non-empty)
            if (answer.answerDescription != null && answer.answerDescription.trim() !== "") {
              answers.push(
                `<label>
                  <input type="radio" name="question${questionNumber}" value="${answer.answerID}">
                  ${answer.answerDescription}
                </label>`
              );
            }
          });

          // Only push question and answers to output if there are valid answers
          if (answers.length > 0) {
            output.push(
              `<div class="slide">
                <div class="question"> ${currentQuestion.description} </div>
                <div class="answers"> ${answers.join("")} </div>
              </div>`
            );
          }
        }
      });
    }
  });

  // Finally, combine our output list into one string of HTML and put it on the page
  quizContainer.innerHTML = output.join('');
}

function showResults() {
  // gather answer containers from our quiz
  const answerContainers = quizContainer.querySelectorAll('.answers');
  let numCorrect = 0;

  // For each question...
  myQuestions.forEach((questionGroup, questionGroupNumber) => {
    if (questionGroup != null) {
      questionGroup.forEach((currentQuestion, questionNumber) => {
        const answerContainer = answerContainers[questionNumber];
        const selector = `input[name=question${questionNumber}]:checked`;
        const userAnswer = (answerContainer.querySelector(selector) || {}).value;

        // Check if the user answer is correct
        if (userAnswer === String(currentQuestion.correctAnswer)) {
          numCorrect++;
          answerContainers[questionNumber].style.color = 'lightgreen'; // Highlight correct answer in green
        } else {
          answerContainers[questionNumber].style.color = 'red'; // Highlight incorrect answer in red
        }

        // Display feedback for each answer
        // Loop through all answers and mark if the answer is correct or incorrect
        const allAnswers = answerContainer.querySelectorAll('label');
        allAnswers.forEach(label => {
          const answerValue = label.querySelector('input').value;
          if (answerValue === String(currentQuestion.correctAnswer)) {
            label.style.color = 'green'; // Correct answer highlighted in green
          } else if (answerValue === userAnswer) {
            label.style.color = 'red'; // Incorrectly selected answer highlighted in red
          }
        });
      });
    }
  });

  // Show number of correct answers out of total
  saveGrade(numCorrect);
  resultsContainer.innerHTML = `${numCorrect} out of ${myQuestions.length}`;
}

function showSlide(n) {
  slides[currentSlide].classList.remove('active-slide');
  slides[n].classList.add('active-slide');
  currentSlide = n;

  previousButton.style.display = currentSlide === 0 ? 'none' : 'inline-block';
  nextButton.style.display = currentSlide === slides.length - 1 ? 'none' : 'inline-block';
  submitButton.style.display = currentSlide === slides.length - 1 ? 'inline-block' : 'none';
}

function showNextSlide() {
  showSlide(currentSlide + 1);
}

function showPreviousSlide() {
  showSlide(currentSlide - 1);
}

async function saveGrade(correct){
  userID = 1;
  quizName = "Medium Quiz"
  await saveQuizGrade(userID, quizName, correct);
}