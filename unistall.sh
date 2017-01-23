#! /bin/bash

echo "Désinstallation en cours..."

cd build
make clean
cd ..
rm -r build/

echo "Désinstallation terminé."