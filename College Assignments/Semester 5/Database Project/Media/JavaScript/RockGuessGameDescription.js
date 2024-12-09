// Check if isUserLoggedIn exists in localStorage
if (localStorage.getItem('isUserLoggedIn') === null) {
  // Set isUserLoggedIn to false for first-time users
  localStorage.setItem('isUserLoggedIn', 'false');
}

function changeAnswer(answer) {
    document.getElementById("rockGuessAnswer").innerText = answer;
}
function test() {
    document.getElementById("rockGuessAnswer").innerText = "heelo";
}

async function startPage(){
  // this is our entry point
  rockImages = await createRocks();
  $("#flashCard").attr("src", "../Media/Images/tableRocks.1.jpg");

  imageNumber = 0;
  order = true;
  random = false;
  trueRandom = false;

  showSlides(imageNumber);

  $("#clickme").click(function() {
      var useranswer = $("#answer").val();
      $("#status").html(useranswer);
      
      if (rockImages[imageNumber].alt == useranswer) {
        alert("Great Job! Your Correct!");
        ClearText();
        plusSlides(1); // GET the next card
      }
      else {
        alert("Try Again!");
        ClearText();
      }
    });
};


function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min) ) + min;
}

// Next/previous controls
function plusSlides(currentSlide) {
    showSlides(imageNumber += currentSlide);
  }
  
  // Thumbnail image controls
  function currentSlide(currentSlide) {
    showSlides(imageNumber = currentSlide);
  }
  
  function showSlides(currentSlide) {
    if (currentSlide > rockImages.length) {imageNumber = 0}
    if (currentSlide < 0) {imageNumber = rockImages.length}
    $("#flashCard").attr("src", rockImages[imageNumber].src, "alt", rockImages[imageNumber].alt);
    //ReplaceText
    let string = rockImages[imageNumber].caption;
    let regex = new RegExp(rockImages[imageNumber].alt, "ig"); 
    let newString = string.replace(regex, "______")
    $("#flashCardText").text(newString);
    $("#flashCardText").show();
  }



function ClearText() {
    $("#answer").val("");
    $("#status").html("Make a guess!");
}

startPage();