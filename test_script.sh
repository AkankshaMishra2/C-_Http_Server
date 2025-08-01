#!/bin/bash

# Lancer le serveur en arrière-plan
./webserv confs/default.conf &
SERVER_PID=$!

# Attendre que le serveur démarre
sleep 2

echo "=== Test GET index.html ==="
curl -s http://127.0.0.1:8082/index.html | head -10

echo -e "\n=== Test GET fichier inexistant ==="
curl -s -o /dev/null -w "%{http_code}" http://127.0.0.1:8082/inexistant.html

echo -e "\n=== Test GET répertoire avec autoindex ==="
curl -s http://127.0.0.1:8082/static/ | head -5

echo -e "\n=== Test POST upload ==="
echo "test content" > test_file.txt
curl -s -X POST -F "file=@test_file.txt" http://127.0.0.1:8082/upload
ls -la www/uploads/

echo -e "\n=== Test DELETE ==="
curl -s -X DELETE http://127.0.0.1:8082/uploads/test_file.txt
ls -la www/uploads/

# Arrêter le serveur
kill $SERVER_PID
wait $SERVER_PID 2>/dev/null

echo "Tests terminés."
