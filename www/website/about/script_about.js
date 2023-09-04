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

window.addEventListener('load', function() {
    var overlayImage = document.querySelector('.overlay-image');
    var gifImage = document.querySelector('#fullscreen-gif img');
  
    // Obtenir les dimensions du GIF
    var gifWidth = gifImage.clientWidth;
    var gifHeight = gifImage.clientHeight;
  
    // Ajuster la position de l'image en fonction des dimensions du GIF
    var offsetX = 552; // Ajustez la valeur selon vos besoins
    var offsetY = -103; // Ajustez la valeur selon vos besoins
  
    overlayImage.style.left = (offsetX + gifWidth / 2) + 'px';
    overlayImage.style.top = (offsetY + gifHeight / 2) + 'px';
  });
  
  // Attendre que la page soit entièrement chargée
window.addEventListener('load', function() {
    // Sélectionner la div contenant le GIF
    var fullscreenDiv = document.getElementById('fullscreen-gif');
  
    // Ajouter une classe spéciale pour l'animation plein écran
    fullscreenDiv.classList.add('fullscreen');
  
    // Supprimer la classe après une durée donnée (par exemple, 3 secondes)
    setTimeout(function() {
      fullscreenDiv.classList.remove('fullscreen');
    }, 2800); // Durée en millisecondes (3 secondes dans cet exemple)
  });

  window.addEventListener('load', function() {
    var fullscreenDiv = document.getElementById('fullscreen-gif');
  
    setTimeout(function() {
      fullscreenDiv.parentNode.removeChild(fullscreenDiv);
    }, 2800);
  });

  window.addEventListener('load', function() {
    var overlayImage = document.querySelector('.overlay-image');
  
    // Ajuster l'opacité progressivement de 0 à 1
    overlayImage.style.opacity = '1';
  });
  
