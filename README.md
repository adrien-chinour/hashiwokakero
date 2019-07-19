# HASHI-TM2H - PROJET TECHNOLOGIQUE L2 UNIVERSITE DE BORDEAUX

## Installation

* __Installation off-source :__ `mkdir Build ; cd Build ; cmake .. -DENABLE_DEBUG=OFF ; make ; make install ; cd ..`
* __Installation in-source :__ `cmake . -DENABLE_DEBUG=OFF ; make ; make install`

__Remarque :__ Pour le debugage "-DENABLE_DEBUG=ON"

## Utilisation

-./hashi_text (Jouer au jeu en mode CLI sur le terminal)
   * Aucun paramètre necessaire

- ./hashi_gui <nom_du_fichier> (Jouer au jeu en mode graphique avec la library SDL2)
   * <nom_du_fichier> : pour jouer à une partie en particulier (paramètre non obligatoire)

- ./hashi_solve <nom_du_fichier> <nom_de_la_save> (Résoudre une partie)
   * <nom_du_fichier> : [OBLIGATOIRE] nom du fichier a resoudre
   * <nom_de_la_save> : nom de la sauvegarde, si non renseigné elle est par defaut <nom_du_fichier>.solved

## Debugage (utilisation de Dart)
    EDIT : test non pris en charge car inutile...
    * make test : lance les tests des fonctions du core
    * make ExperimentalCoverage : lance les tests de couvertures; résultats sauvegardés dans Testing/CoverageInfo
