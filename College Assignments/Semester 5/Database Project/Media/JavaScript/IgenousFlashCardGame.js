let rockImages = [];
//The flashcards to be used in the game.

function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min) ) + min;
}
// this is our entry point
async function startPage(){
    // Wait for rockImages to be populated
    rockImages = await createRocks();

    //Set start values
    imageNumber = getRndInteger(0, 8);
    order = true;
    random = false;
    trueRandom = false;

    $("#flashCardText").hide();
    showSlides(imageNumber);
};

// Next/previous controls
function plusSlides(currentSlide) {
  showSlides(imageNumber += currentSlide);
}

// Thumbnail image controls
function currentSlide(currentSlide) {
  showSlides(imageNumber = currentSlide);
}

function toggleImage() {
  $("#flashCard").toggle();
  $("#flashCardText").toggle();
}

function showSlides(currentSlide) {
  // Check to make sure currentSlide is within the wounds
  if (currentSlide > 8) {imageNumber = 0}
  if (currentSlide < 0) {imageNumber = 8}


  $("#flashCard").attr("src", rockImages[imageNumber].src);
  $("#flashCard").attr("alt", rockImages[imageNumber].alt);
  $("#flashCardText").text(rockImages[imageNumber].caption);

  // Toggle flashcard
  $("#flashCardText").hide();
  $("#flashCard").show();
}

function getRndInteger(min, max) {
  return Math.floor(Math.random() * (max - min) ) + min;
}