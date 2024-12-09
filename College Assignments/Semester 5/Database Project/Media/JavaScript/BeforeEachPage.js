// Check if isUserLoggedIn exists in localStorage
if (localStorage.getItem('isUserLoggedIn') === null) {
    // Set isUserLoggedIn to false for first-time users
    localStorage.setItem('isUserLoggedIn', 'false');
  }
  
// Check if rockImages in localStorage is too small and reset if necessary
if (localStorage.getItem('rockImages') !== null && JSON.parse(localStorage.getItem('rockImages')).length < 25) {
  localStorage.removeItem('rockImages');
}

// Check if quizQuestions in localStorage is too small and reset if necessary
if (localStorage.getItem('quizQuestions') !== null && JSON.parse(localStorage.getItem('quizQuestions')).length < 50) {
  localStorage.removeItem('quizQuestions');
}

// Check if users in localStorage is too small and reset if necessary
if (localStorage.getItem('users') !== null && JSON.parse(localStorage.getItem('users')).length < 2) {
  localStorage.removeItem('users');
}

async function createRocks() {
  // Check if rocks have been set on the site yet.
  if (localStorage.getItem('rockImages') === null) {
    let rockImages = [];
    // Create Rocks
    for (let i = 0; i < 27; i++) {
      const rockName = await fetchRockDataName(i);
      const rockImageSource = await fetchRockImageData(i);
      const rockDescription = await fetchRockDescriptionData(i);
      rockImages.push(createImage(rockName, rockImageSource, rockDescription));
    }

    console.log(rockImages); // Log the array of rocks

    // Save the created rocks to localStorage
    localStorage.setItem('rockImages', JSON.stringify(rockImages));
    return rockImages;
  } else {
    let rockImages = JSON.parse(localStorage.getItem('rockImages'));
    return rockImages;
  }
}
  

  
  // Create image
  function createImage(rockName, rockImageSource, rockDescription) {
    return { src: rockImageSource, alt: rockName, caption: rockDescription };
  }
  
  async function fetchRockDataName(location) {
    const xmlData = `
      <request>
          <rockID>${location}</rockID>
      </request>
    `;
    try {
      const response = await fetch("http://127.0.0.1:12345/rocksName", {
        method: "POST",
        headers: {
          "Content-Type": "application/xml",
        },
        body: xmlData,
        cache: "no-cache"
      });
      const data = await response.text();
      //console.log("Response from server:", data);
      return data; // return the fetched rock name
    } catch (error) {
      console.error("Error:", error);
      return "Error: " + error;
    }
  }
  
  async function fetchRockImageData(location) {
    const xmlData = `
      <request>
          <rockID>${location}</rockID>
      </request>
    `;
    try {
      const response = await fetch("http://127.0.0.1:12345/rocksImage", {
        method: "POST",
        headers: {
          "Content-Type": "application/xml",
        },
        body: xmlData,
        cache: "no-cache"
      });
      const data = await response.text();
      //console.log("Response from server:", data);
      return data; // return the fetched image source
    } catch (error) {
      console.error("Error:", error);
      return "Error: " + error;
    }
  }
  
  async function fetchRockDescriptionData(location) {
    const xmlData = `
      <request>
          <rockID>${location}</rockID>
      </request>
    `;
    try {
      const response = await fetch("http://127.0.0.1:12345/rocksDescription", {
        method: "POST",
        headers: {
          "Content-Type": "application/xml",
        },
        body: xmlData,
        cache: "no-cache"
      });
      const data = await response.text();
      //console.log("Response from server:", data);
      return data; // return the fetched rock description
    } catch (error) {
      console.error("Error:", error);
      return "Error: " + error;
    }
  }

// Call the createRocks function
createRocks();
  
// Function to create quiz questions
async function CreateQuestions() {
  // Check if quizQuestions are already set in localStorage
  if (localStorage.getItem('quizQuestions') === null) {
    let quizQuestions = [];

    // Fetch all questions from the database
    const questions = await fetchQuestions();

    // Loop through each question and fetch additional data based on type
    for (let question of questions) {
      const questionDetails = await fetchQuestionDetails(question);
      quizQuestions.push(questionDetails);
    }

    console.log(quizQuestions); // Log the array of quiz questions

    // Save the quiz questions to localStorage as a JSON string
    localStorage.setItem('quizQuestions', JSON.stringify(quizQuestions));
    return quizQuestions;
  } else {
    // Return the quiz questions from localStorage
    return JSON.parse(localStorage.getItem('quizQuestions'));
  }
}

// Fetch all questions from the database
async function fetchQuestions() {
  try {
    const response = await fetch("http://127.0.0.1:12345/getQuestions", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      cache: "no-cache"
    });

    if (!response.ok) { // Check if the response is not 200 OK
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    const data = await response.json();
    return data;
  } catch (error) {
    console.error("Error fetching questions:", error);
    return [];
  }
}

// Fetch question details based on its type
async function fetchQuestionDetails(question) {
  switch (question.type) {
    case 'multichoice':
      return await fetchMultiChoiceQuestion(question.questionID);
    case 'fillintheblank':
      // Not implemented yet
      console.error("Not implemented yet:", question.questionType);
      return null;
    case 'draganddrop':
      // Not implemented yet
      console.error("Not implemented yet:", question.questionType);
      return null;
    default:
      console.error("Unknown question type:", question.questionType);
      return null;
  }
}

// Fetch multiple-choice question details
async function fetchMultiChoiceQuestion(questionID) {
  const xmlData = `
    <request>
        <questionID>${questionID}</questionID>
    </request>
  `;
  try {
    const response = await fetch(`http://127.0.0.1:12345/getMultiChoiceQuestion/`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: xmlData,
      cache: "no-cache"
    });

    // Log the raw response text before parsing
    const rawData = await response.text();
    //console.log("Raw server response:", rawData);

    // Parse the response if it's JSON
    const data = JSON.parse(rawData);
    //console.log("Parsed response:", data);

    return formatMultiChoiceQuestion(data);
  } catch (error) {
    console.error("Error fetching multiple-choice question:", error);
    return null;
  }
}


// Formatting functions for each question type

function formatMultiChoiceQuestion(questionData) {
  // Check if 'answers' exists and is an array before calling .map()
  const answers = Array.isArray(questionData.answers)
    ? questionData.answers.map((answer) => ({
        text: answer.answerDescription,
        isCorrect: answer.answerID === questionData.correctAnswer,
      }))
    : []; // Default to an empty array if answers are undefined or not an array

  //console.log("Fetched question data:", questionData);

  return questionData
}


function formatFillInTheBlankQuestion(questionData) {
  return {
    type: 'fillintheblank',
    question: questionData.questionDescription,
    correctAnswer: questionData.correctAnswer
  };
}

function formatDragAndDropQuestion(questionData) {
  return {
    type: 'draganddrop',
    question: questionData.questionDescription,
    correctAnswer: questionData.correctAnswer,
    options: [
      questionData.correctAnswer,
      questionData.WrongAnswerOne,
      questionData.WrongAnswerTwo,
      questionData.WrongAnswerThree
    ]
  };
}


// Call the CreateQuestions function to load quiz questions
CreateQuestions();

async function createUsers() {
  // Check if users have already been set in localStorage
  if (localStorage.getItem('users') === null) {
    try {
      // Fetch user data
      let userFetch = await fetchUser(1);
      let users = [];

      // Ensure userFetch contains the necessary user information before pushing
      if (userFetch) {
        users.push(userFetch);
      }
      //console.log('Fetched and stored users:', users); // Log users

      // Save the created user to localStorage
      localStorage.setItem('users', JSON.stringify(users));
    } catch (error) {
      console.error('Error fetching user:', error);
    }
  }

  // Parse the users from localStorage
  let storedUsers = JSON.parse(localStorage.getItem('users'));

  // If the first item is a stringified JSON object, parse it
  if (typeof storedUsers[0] === 'string') {
    storedUsers[0] = JSON.parse(storedUsers[0]);
  }

  // Return the first user in the array (assuming single-user context)
  return storedUsers[0];
}

async function fetchUser(location) {
  const xmlData = `
    <request>
        <userID>${location}</userID>
    </request>
  `;
  try {
    const response = await fetch("http://127.0.0.1:12345/getUser", {
      method: "POST",
      headers: {
        "Content-Type": "application/xml",
      },
      body: xmlData,
      cache: "no-cache"
    });

    if (!response.ok) {
      throw new Error(`Server returned ${response.status}: ${response.statusText}`);
    }

    const data = await response.text();
    //console.log("Response from server:", data);
    return data; // return the fetched rock name
  } catch (error) {
    console.error("Error:", error);
    return "Error: " + error;
  }
}

//Save Quiz Grade
async function saveQuizGrade(userID, quizName, grade) {
  const xmlData = `
      <QuizGrade>
          <userID>${userID}</userID>
          <quizName>${quizName}</quizName>
          <grade>${grade}</grade>
      </QuizGrade>
  `;

  try {
      const response = await fetch("http://127.0.0.1:12345/SaveQuizGrade", {
          method: "POST",
          headers: {
              "Content-Type": "application/xml",
          },
          body: xmlData,
          cache: "no-cache"
      });

      if (!response.ok) {
          throw new Error(`Server returned ${response.status}: ${response.statusText}`);
      }

      const responseText = await response.text();
      console.log("Response from server:", responseText);

      // Handle success or error response
      if (responseText.includes("successfully saved")) {
          alert("Quiz grade saved successfully!");
      } else {
          alert("Failed to save quiz grade: " + responseText);
      }
  } catch (error) {
      console.error("Error saving quiz grade:", error);
      alert("Error saving quiz grade: " + error.message);
  }
}

// Delete statements
// Delete a grade
async function deleteGrade(gradeID) {
  try {
    const xmlData = `
      <request>
        <gradeID>${gradeID}</gradeID>
      </request>
    `;

    const response = await fetch("http://127.0.0.1:12345/DeleteQuizGrade", {
      method: "POST",
      headers: {
        "Content-Type": "application/xml",
      },
      body: xmlData,
    });

    if (!response.ok) {
      throw new Error(`Failed to delete grade: ${response.status}`);
    }

    alert("Grade deleted successfully.");
    fetchUser(1); // Refresh the grades list (CHANGE TO NOT BE 1 in the future)
  } catch (error) {
    console.error("Error deleting grade:", error);
    alert("Failed to delete grade.");
  }
}

// Delete a question
async function deleteQuestion(questionID) {
  try {
    const xmlData = `
      <request>
        <questionID>${questionID}</questionID>
      </request>
    `;

    const response = await fetch("http://127.0.0.1:12345/DeleteQuestion", {
      method: "POST",
      headers: {
        "Content-Type": "application/xml",
      },
      body: xmlData,
    });

    if (!response.ok) {
      throw new Error(`Failed to delete question: ${response.status}`);
    }

    alert("Question deleted successfully.");
    loadQuestions(); // Refresh the questions list
  } catch (error) {
    console.error("Error deleting question:", error);
    alert("Failed to delete question.");
  }
}
