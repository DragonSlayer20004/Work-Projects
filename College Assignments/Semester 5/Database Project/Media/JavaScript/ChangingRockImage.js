// Check if isUserLoggedIn exists in localStorage
if (localStorage.getItem('isUserLoggedIn') === null) {
    // Set isUserLoggedIn to false for first-time users
    localStorage.setItem('isUserLoggedIn', 'false');
}

imageNumber = 0;

async function startPage(){
    rockImages = await createRocks();
    imageNumber = getRndInteger(0, 26);
    timeBasedImage();
    setInterval(timeBasedImage, 20000);
    //Create initial random image
};

function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min) ) + min;
}

function timeBasedImage() {
    temp = imageNumber;
    while(temp == imageNumber){
        imageNumber = getRndInteger(0, 21);
    }
        
    var image = rockImages[imageNumber];

    $("#rotatingImage").attr({"src": image.src, "alt" : image.alt});
    $("#rotatingImageInformation").text(image.caption);
}