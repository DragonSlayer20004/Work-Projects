imageNumber = 0;

async function startPage(){
    rockImages = await createRocks();
    imageNumber = getRndInteger(0, 9);
    timeBasedImage();
    setInterval(timeBasedImage, 20000);
    //Create initial random image
};

function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min) ) + min;
}

function timeBasedImage() {
    imageNumber = (imageNumber + 1)%9;
    var image = rockImages[imageNumber];

    $("#rotatingImage").attr({"src": image.src, "alt" : image.alt});
    $("#rotatingImageInformation").text(image.caption);
}