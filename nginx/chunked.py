import http.client

# Paramètres du serveur
host = "localhost"
port = 4242

# Créer une connexion au serveur
conn = http.client.HTTPConnection(host, port)

# En-têtes de la requête
headers = {
    "Content-Type": "text/plain",
    "Transfer-Encoding": "chunked"
}

# Corps de la requête en chunks
chunks = [
    b"This is the first chunk",
    b"This is the second chunk",
    b"And this is the third chunk",
    b"And this is the 4 chunk",
    b"And this is the 5 chunk",
    b"And this is the 6 chunk"
]

# Ouvrir la connexion et envoyer les en-têtes
conn.connect()
conn.request("POST", "/", headers=headers)

# Envoyer les chunks
for chunk in chunks:
    conn.send(b"%x\r\n%s\r\n" % (len(chunk), chunk))
conn.send(b"0\r\n\r\n")  # Fin des chunks

# Lire la réponse
response = conn.getresponse()

# Afficher la réponse
print("Status:", response.status, response.reason)
print("Response:", response.read().decode())

# Fermer la connexion
conn.close()

