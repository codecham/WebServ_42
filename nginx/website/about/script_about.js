var images = [
    "../../images/fry.webp",
    "../../images/jojos.gif",
    "../../images/Shaquille.gif",
    "../../images/etchebest.gif",
    "../../images/john_sena.gif",
    "../../images/ramsay.gif",
];

document.addEventListener('DOMContentLoaded', function() {
var personImages = document.querySelectorAll('.person img');
console.log(personImages);
console.log(images);

personImages.forEach(function(img) {
    img.addEventListener('mouseover', function() {
        var randomIndex = Math.floor(Math.random() * images.length);
        img.setAttribute('src', images[randomIndex]);
    });
    console.log("oui");
    img.addEventListener('mouseout', function() {
        var fullPath = window.location.pathname; 
        var filename = fullPath.split('/').pop(); 
        if (filename == "dcorenti.html")
            img.setAttribute('src', "../../images/dcorenti.jpeg");
        else
            img.setAttribute('src', "../../images/dduvivie.jpeg");
    });
});
});
