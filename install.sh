#!/bin/bash

echo "Installation en cours..."

mkdir build
cd build
cmake ..
make && echo "Installation terminé avec succès." || echo "Installation ratée."