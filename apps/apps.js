'use strict';

angular.module('nwas').service('apps', function() {
  return [
    { name: "NES TAH", description: {en: "TAHrtiNES emulator", fr: "Émulateur TAHrtiNES"} },
    { name: "Playboy", description: {en: "It's actually GameBoy", fr: "Non, en fait c'est GameBoy"} },
    { name: "Gamesave", description: {en: "TEST: NE PAS UTILISER", fr: "TEST: NE PAS UTILISER"} },
    { name: "Rickroll", description: {en: "The Rickroll", fr: "Le Rickroll"} },
    { name: "Image", description: {en: "An image", fr: "Une image"} },
    { name: "Ball", description: {en: "Haha ballz", fr: "Un objet en forme sphérique qui rebondit sur les bords de votre calculatrice"} },
    { name: "Mandelbrot", description: {en: "The Mandelbrot set fractal", fr: "Visionneur de la fracale de Mandelbrot, zoom jusqu'a 2**31 fois"} },
    { name: "Golly", description: {en: "Conway's game of life", fr: "Golly"} }
  ];
});
