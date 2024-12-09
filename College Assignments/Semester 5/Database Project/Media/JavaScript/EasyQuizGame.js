// Check if isUserLoggedIn exists in localStorage
if (localStorage.getItem('isUserLoggedIn') === null) {
  // Set isUserLoggedIn to false for first-time users
  localStorage.setItem('isUserLoggedIn', 'false');
}

imageNumber = 0;
correct = 0;
order = true;
random = false;
trueRandom = false;

function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min) ) + min;
}

async function startPage(){
    // this is our entry point
    rockImages = await createRocks();
    imageNumber = getRndInteger(0, 27);
    createQuestions();
};

function createQuestions(){
  //$('span#answer10').text(rockImages[getRndInteger(0, 27)].alt);
  var j = 1;
  for(let i = 1; i < 36; i = i+4){
    var correctAnswerImage = getRndInteger(0, 27);
    var currentAnswer = "answer"+getRndInteger(i, i+4);
    $('input#'+currentAnswer).val('Correct');
    $('span#'+currentAnswer).text(rockImages[correctAnswerImage].alt);
    var currentQuestion = "quiz"+j;
     
    $('#' + currentQuestion).attr("src", rockImages[correctAnswerImage].src, "alt", rockImages[correctAnswerImage].alt);
    $('#' + currentQuestion).text(rockImages[correctAnswerImage].caption);

    j++
  }

  //Set all none correct answers to random rocks
  for(let i = 1; i <= 36; i++){
    var currentAnswer = "answer"+i;
    var wrongCheck = $('span#' + currentAnswer).text();
    if(wrongCheck == "Wrong"){
      $('span#'+currentAnswer).text(rockImages[getRndInteger(0, 27)].alt);
    }
    $('#answer' + i).text(rockImages[getRndInteger(0, 27)].alt);
  }
}

var submitAnswer = function() {
  for(let i = 0; i < 10; i++){
    var radios = document.getElementsByName('choice'+i);
    var val= "";
    for (var j = 0, length = radios.length; j < length; j++) {
        if (radios[j].checked) {
          val = radios[j].value; 
          break;
        }
    }
    
    if (val == "" ) {
      //alert('please select choice answer');
    } else if ( val == "Correct" ) {
      correct = correct + 1;
    } else {
      //alert('Answer is wrong');
    }
  }
  alert('Your score is ' + correct);

  saveGrade(correct);

  correct = 0;
};

async function saveGrade(correct){
  userID = 1;
  quizName = "Easy Quiz"
  await saveQuizGrade(userID, quizName, correct);
}