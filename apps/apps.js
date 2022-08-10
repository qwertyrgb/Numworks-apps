'use strict';

angular.module('nwas').service('apps', function() {
  return [
    { name: "KhiCAS", description: {en: "Computer algebra system", fr: "Système de calcul formel"} },
    { name: "Periodic", description: {en: "Periodic table of elements", fr: "Tableau périodique des éléments"} },
    { name: "Nofrendo", description: {en: "NES emulator", fr: "Émulateur NES"} },
    { name: "Peanut-GB", description: {en: "GameBoy emulator", fr: "Émulateur GameBoy"} },
    { name: "HexEdit", description: {en: "Hexadecimal editor", fr: "Éditeur hexadécimal"} },
    { name: "BadApple", description: {en: "Demo Bad Apple", fr: "Démo Bad Apple"} },
    { name: "Rickroll", description: {en: "The Rickroll", fr: "Le Rickroll"} },
    { name: "Image", description: {en: "An image", fr: "Une image"} },
    { name: "Example", description: {en: "Example", fr: "Exemple"} },
    { name: "Example-Cpp", description: {en: "Example cpp", fr: "Exemple cpp"} },
    { name: "Ball", description: {en: "Haha ballz", fr: "Un objet en forme sphérique qui rebondit sur les bords de votre calculatrice"} },
    { name: "UnitCircle", description: {en: "Unit circle", fr: "Cercle trigonométrique"} },
    { name: "CHIP-8", description: {en: "CHIP-8 interpreter", fr: "Interpréteur CHIP-8"} },
  ];
});
