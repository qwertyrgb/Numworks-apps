'use strict';

angular.module('nwas').service('apps', function() {
  return [
    { name: "NES TAH", description: {en: "TAHrtiNES emulator", fr: "Émulateur TAHrtiNES"} },
    { name: "Playboy", description: {en: "It's actually GameBoy", fr: "Non, en fait c'est GameBoy"} },
    { name: "Gamesave", description: {en: "TEST: NE PAS UTILISER", fr: "TEST: NE PAS UTILISER"} },
    { name: "Rickroll", description: {en: "The Rickroll", fr: "Le Rickroll"} },
    { name: "Image", description: {en: "An image", fr: "Une image"} },
    { name: "Ball", description: {en: "Haha ballz", fr: "Un objet en forme sphérique qui rebondit sur les bords de votre calculatrice"} },
    { name: "Fractals", description: {en: "Mandelbrot fractal + Burning Ship", fr: "fractale de Mandelbrot + navire vla chaud"} },
    { name: "Golly", description: {en: "Conway's game of life", fr: "Golly"} },
    { name: "Gollyhex", description: {en: "Hex game of life", fr: "Hexlly"} },
    { name: "Gameboy", description: {en: "Better gameboy", fr: "GHameboay"} }
  ];
});
