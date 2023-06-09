function getServerURL() {
	var ipAddress = "127.0.0.1";
	var port = "4242";
	var uploadPath = "/upload";

	return "http://" + ipAddress + ":" + port + uploadPath;
}

function uploadFile() {
	var fileInput = document.getElementById('fileInput');
	var file = fileInput.files[0];

	if (file) {
		var formData = new FormData();
		formData.append('file', file);

		var formData = new FormData();
		formData.append('file', file);

		var request = new XMLHttpRequest();
		var url_upload = getServerURL();
		request.open('POST', url_upload, true);
		request.onload = function() {
			if (request.status === 200) {
				alert("Upload sucessfull");
			} else {
				var responseHTML = request.responseText;
				var additionalText = "Erreur lors de l'envoi du fichier."; // Texte supplémentaire
				var errorContainer = document.getElementById('errorContainer');
				errorContainer.innerHTML = additionalText + responseHTML;
			}
		};
		request.onerror = function() {
			var additionalText = "Server connexion error! Can't connect to: ";
			additionalText += url_upload;
			var errorContainer = document.getElementById('errorContainer');
			errorContainer.innerHTML = additionalText;
		};
		request.send(formData);
	}
	else {
		alert("Please select a file before send.");
	}
}
