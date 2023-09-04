function getServerURL() {
    var ipAddress = "localhost";
    var port = "4242";
    var uploadPath = "/upload/";

    return "http://" + ipAddress + ":" + port + uploadPath;
}

function uploadFile() {
    var fileInput = document.getElementById('fileInput');
    var file = fileInput.files[0];

    if (file) {
        var formData = new FormData();
        formData.append('file', file);

        var url_upload = getServerURL();
        fetch(url_upload, {
            method: 'POST',
            body: formData
        })
        .then(response => {
            if (response.status === 200) {
                alert("Upload successful");
            }
            else {
                alert(response.status + " " + response.statusText)
            }
        })
        .catch(error => {
            console.error("Error");
        });
    } else {
        alert("Please select a file before sending.");
    }
}
