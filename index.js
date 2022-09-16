"use strict";

/*
Copyright (C) 2019 Damien Nicolet

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

angular.module('nwas', ['ngSanitize', 'pascalprecht.translate']).controller('main', function($scope, $http, apps, $translate) {

  $scope.locale = $translate.use();
  $scope.wallpaper = null;
  $scope.apps = apps;
  $scope.selectedApps = [];
  $scope.customFiles = [];
  $scope.webUSB = typeof navigator.usb !== 'undefined';

  $scope.reload = function reload() {
    document.location.reload();
  }

  $scope.addApplication = function addApplication(app) {
    if($scope.selectedApps.indexOf(app) < 0) {
      $scope.selectedApps.push(app);
    }
  };

  $scope.setWallpaper = function setWallpaper(el) {
    let file = el[0].files[0];
    el.value = null;
    let reader = new FileReader();
    
    reader.addEventListener("load", function() {
        let img = document.createElement('img');
        img.onload = function () { 
          $scope.$apply(function () {
            let cropperDiv = document.getElementById("cropperDiv");
            cropperDiv.innerText = "";
            cropperDiv.appendChild(img);
            $('#imageModal').modal('show');
            img.style = "max-width: 100%;";

            $scope.wallpaper = {
              name: file.name,
              cropper: new Cropper(img, {
                aspectRatio: 320 / 222,
                viewMode: 1
              })
            };
          });
        };
        img.src = reader.result;
    }, false);

    if (file) {
      reader.readAsDataURL(file);
    }
  };

  $scope.saveCroppedWallpaper = function saveCroppedWallpaper() {
    // Resize image
    var canvas = document.createElement('canvas'),
        ctx = canvas.getContext('2d');
    canvas.width = 320;
    canvas.height = 222;

    ctx.drawImage($scope.wallpaper.cropper.getCroppedCanvas(), 0, 0, 320, 222);

    $scope.wallpaper = {
      name: $scope.wallpaper.name, 
      imagesrc: canvas.toDataURL("image/png")
    };
    document.getElementById("wallpaper-name").innerText = $scope.wallpaper.name;
    $('#imageModal').modal('hide');
  }

  $scope.removeWallpaper = function removeWallpaper() {
    $scope.wallpaper = null;
    document.getElementById("wallpaper-file-input").value = null;
    $translate("WALLPAPER_FILE").then(function (translatedValue) {
      document.getElementById("wallpaper-name").innerText = translatedValue;
    });
  };
  
  $scope.removeApplication = function removeApplication(app) {
    let index = $scope.selectedApps.indexOf(app);
    if(index >= 0) {
      $scope.selectedApps.splice(index, 1);
    }
  };

  $scope.removeFile = function removeFile(file) {
    let index = $scope.customFiles.indexOf(file);
    if(index >= 0) {
      $scope.customFiles.splice(index, 1);
    }
  };

  let link = function link(app, linkerScript) {
    let output;
    return new Promise(function (resolve, reject) {
      let linkModule = {
        arguments: ["/input.elf", "-o", "/output.elf", "-T", "/linker.ld"],
        preRun: [ () => {
          linkModule.FS.writeFile("/linker.ld", linkerScript);
          linkModule.FS.createPreloadedFile("/", "input.elf", "apps/" + app.name + "/app.elf", true, true);
        }],
        postRun: [ () => {
          output = linkModule.FS.readFile("/output.elf");
        }]
      };
      ld(linkModule).then( () => {
        let objcopyModule = {
          arguments: ["-O", "binary", "/input.elf", "/output.bin"],
          preRun: [ () => {
              objcopyModule.FS.writeFile("/input.elf", output);
          }],
          postRun: [ () => {
              resolve(objcopyModule.FS.readFile("/output.bin"));
          }]
        };
        objcopy(objcopyModule);
      });
    });
  };

  let loadLinkerScript = function loadLinkerScript() {
    return new Promise(function(resolve, reject) {
      $http.get("apps/external.ld", {responseType: "text"})
        .then(function successCallback(response) {
          resolve(response.data);
        }, function errorCallback(response) {
          reject("Unable to download linker script");
        });
    });
  }
/*
  let inlineImage = function inlineImage(app) {
    let img = new Image();
    img.src = "apps/" + app.name + "/icon.png";

    let canvas = document.createElement('canvas');
    
    canvas.width = img.width;
    canvas.height = img.height;
    
    let context = canvas.getContext('2d');
    context.drawImage(img, 0, 0);
    let imgd = context.getImageData(0, 0, img.width, img.height);
    
    let icon_rgba32 = new Uint32Array(imgd.data.buffer);
    let icon_rgba8888 = new Uint8Array(imgd.data.buffer);
    let icon_rgb565 = new Uint16Array(icon_rgba32.length);
    for(let i = 0; i < icon_rgba32.length; i++) {
      let r = icon_rgba8888[i * 4 + 0] / 255;
      let g = icon_rgba8888[i * 4 + 1] / 255;
      let b = icon_rgba8888[i * 4 + 2] / 255;
      let a = icon_rgba8888[i * 4 + 3] / 255;
      
      let br = r * a + 1 * (1 - a);
      let bg = g * a + 1 * (1 - a);
      let bb = b * a + 1 * (1 - a);
      
      let ir = Math.round(br * 0xFF);
      let ig = Math.round(bg * 0xFF);
      let ib = Math.round(bb * 0xFF);
      
      icon_rgb565[i] = (ir >> 3) << 11 | (ig >> 2) << 5 | (ib >> 3);
    }
    
    let final_data = new Uint8Array(icon_rgb565.buffer, icon_rgb565.byteOffset, icon_rgb565.byteLength);
    let compressed = lz4.makeBlock(final_data);
    
    return compressed;
  }
*/
  let normalizeTextFile = async function normalizeTextFile(file) {

    const replaceAtIndex = (str, index, chr) => {
      if(index > str.length-1) return str;
      return str.substring(0,index) + chr + str.substring(index+1);
    }

    let blob = new Blob([file.binary], {type:'text/plain'});
    return new Promise((resolve, reject) => {
      blob.text().then(text => {
        // TODO: Improve this, it is very inefficient
        text = text.replaceAll("’", "'")
        text = text.replaceAll(" «", '"')
        text = text.replaceAll(" »", '"')
        text = text.replaceAll("«", '"')
        text = text.replaceAll("»", '"')
        text = text.replaceAll("“", '"')
        text = text.replaceAll("”", '"')
        text = text.replaceAll("–", "-")
        text = text.replaceAll("œ", "oe")
        text = text.replaceAll("\r\n", "\n")
        text = text.replaceAll("\r", "\n")
        text = text.normalize('NFKD');

        let t = new TextEncoder("UTF-8").encode(text);
        resolve(t);
      });
    });
  };

  let fromPNGToOBM = function fromPNGToOBM(dataURL) {
    //OBM (Omega Bit Map) is the wallpaper format of Omega
    let img = new Image();
    img.src = dataURL;

    let canvas = document.createElement('canvas');
    
    canvas.width = img.width;
    canvas.height = img.height;

    let img_header32 = new Uint32Array(3);
    img_header32[0] = 466512775; //We use a "random" magic number
    img_header32[1] = canvas.width;
    img_header32[2] = canvas.height;
    
    let context = canvas.getContext('2d');
    context.drawImage(img, 0, 0);
    let imgd = context.getImageData(0, 0, img.width, img.height);
    
    let img_rgba32 = new Uint32Array(imgd.data.buffer);
    let img_rgba8888 = new Uint8Array(imgd.data.buffer);
    let img_rgb565 = new Uint16Array(img_rgba32.length);
    for(let i = 0; i < img_rgba32.length; i++) {
      let r = img_rgba8888[i * 4 + 0] / 255;
      let g = img_rgba8888[i * 4 + 1] / 255;
      let b = img_rgba8888[i * 4 + 2] / 255;
      let a = img_rgba8888[i * 4 + 3] / 255;
      
      let br = r * a + 1 * (1 - a);
      let bg = g * a + 1 * (1 - a);
      let bb = b * a + 1 * (1 - a);
      
      let ir = Math.round(br * 0xFF);
      let ig = Math.round(bg * 0xFF);
      let ib = Math.round(bb * 0xFF);
      
      img_rgb565[i] = (ir >> 3) << 11 | (ig >> 2) << 5 | (ib >> 3);
    }
    

    let img_header8 = new Uint8Array(img_header32.buffer, img_header32.byteOffset, img_header32.byteLength);
    let img_data = new Uint8Array(img_rgb565.buffer, img_rgb565.byteOffset, img_rgb565.byteLength);
    
    let final_data = Uint8Array.from([...img_header8, ...img_data]);

    return final_data;
  }

  let buildArchive = async function buildArchive(applications, wallpaper, files) {
    if(applications.length == 0 && files.length == 0 && wallpaper == null) {
      return new Promise(function(resolve, reject) {
        resolve(new Uint8Array(0x200));
      });
    } else {
      let linkerScript = await loadLinkerScript();
      let address = 0;
      let tar = new tarball.TarWriter();

      for(let i = 0; i < applications.length; i++) {
        let app = applications[i];
        console.log("Processing", app.name, address);
        $scope.$apply(function() {
          $scope.lastAction = $translate.instant("PROCESSING") + " " + app.name;
        });
        let binary = await link(app, linkerScript.replace("(0)", "(" + address + ")"));
        address += 0x200 * Math.floor((binary.length + 1023) / 0x200);
        console.log("Taring", app.name)
        tar.addFileArrayBuffer(app.name, binary, {mode: "775"});
        
        let resp = await $http.get("apps/" + app.name + "/app.icon", {responseType: "arraybuffer"});
        
        files.push({
            name: app.name + ".icon",
            binary: resp.data
        });
        
      }
      if(wallpaper != null) {
        console.log("Inlining wallpaper");
        $scope.$apply(function() {
          $scope.lastAction = $translate.instant("ADDING") + " " + wallpaper.name;
        });
        files.push({
          name: "wallpaper.obm",
          binary: fromPNGToOBM(wallpaper.imagesrc)
        });
      }

      for(let i = 0; i < files.length; i++) {
        let file = files[i];
        console.log("Taring", file.name)
        $scope.$apply(function() {
          $scope.lastAction = $translate.instant("ADDING") + " " + file.name;
        });

        if(file.name.endsWith(".txt") || file.name.endsWith(".urt")) {
          console.log("Normalizing", file.name);
          let normalized = await normalizeTextFile(file);
          file = {
            name: file.name,
            binary: normalized
          };
        }
        tar.addFileArrayBuffer(file.name, file.binary, {mode: "664"});
      }

      console.log("Build archive done");
      return tar.write();
    }
  }

  let uploadFile = async function uploadFile(selectedDevice, dfuDescriptor, file, manifest) {
    console.log("uploading", dfuDescriptor);
    let interfaces = dfu.findDeviceDfuInterfaces(selectedDevice);
    console.log("interfaces", interfaces);
    if (interfaces.length == 0) {
      throw "The selected device does not have any USB DFU interfaces.";
    }
    interfaces[0].name = dfuDescriptor;
    let device = new dfuse.Device(selectedDevice, interfaces[0]);
    console.log("device", device);
    await device.open();
    try {
      let status = await device.getStatus();
      if (status.state == dfu.dfuERROR) {
        await device.clearStatus();
      }
    } catch (error) {
      console.log("Failed to clear status");
    }
    device.logProgress = function(done, total) {
      console.log("progress", done, total);
      $scope.$apply(function() {
        $scope.progress = (done / total) * 100;
      })
    }
    device.logInfo = function(message) {
      console.log(message);
      $scope.$apply(function() {
        if(message.startsWith("Erasing")) {
          $scope.lastAction = $translate.instant("DFU_ERASING");
        } else if (message.startsWith("Copying")) {
          $scope.lastAction = $translate.instant("DFU_COPYING");
        } else if (message.startsWith("Wrote")) {
          $scope.lastAction = $translate.instant("DFU_WROTE");
        } else {
          $scope.lastAction = message;
        }
      })
    }
    await device.do_download(2048, file, manifest).then(
      () => {
        console.log("done", dfuDescriptor);
      },
      error => {
        throw error;
      }
    );
  }

  $scope.upload = function upload() {
    delete $scope.error;
    navigator.usb.requestDevice({
      filters: [{
        vendorId: 0x0483,
        productId: 0xa291
      }]
    }).then(
      async selectedDevice => {
        console.log("Selected device", selectedDevice);
        $scope.$apply(function() {
          $scope.uploading = true;
        });
        let archive = await buildArchive($scope.selectedApps, $scope.wallpaper, $scope.customFiles);
        console.log("Archive", archive);

        // Add error when the destination address is bigger than the flash memory (8MB-2MB=6Mo, 0x90800000)
        if (archive.length > 0x90800000 - 0x90200000) {
          throw Error($translate.instant("TOO_MUCH_DATA"));
        }

        if (archive.length > 0x90400000 - 0x90200000 && dfu.findDeviceDfuInterfaces(selectedDevice).length == 1 && selectedDevice.productName == 'Upsilon Calculator') {
          // It's a version of upsilon with the bootloader, but the dfu is executed from a slot, so a part of the flash memory is locked
          throw Error($translate.instant("TOO_BIG_FILES"))
        }

        await uploadFile(selectedDevice, "@External Flash /0x90200000/32*064Kg,64*064Kg", archive, false);

        $scope.$apply(function() {
          $scope.allDone = true;
        });
      }
    ).catch(error => {
      console.log(error);
      $scope.$apply(function() {
        if(error.message != undefined && error.message.match("Address .*? outside of memory map")) {
          $scope.error = $translate.instant("TOO_MUCH_FILES");
        }
        // Handle Unable to claim interface error
        else if(error.message != undefined && error.message.match("Unable to claim interface")) {
          $scope.error = $translate.instant("UNABLE_TO_CLAIM_INTERFACE");
        }
        else {
          $scope.error = error;
        }
        $scope.allDone = false;
      });
    });
  };

  $scope.getFile = function getFile(el) {
    for (let i = 0; i < el[0].files.length; i++) {
      let file = el[0].files[i];
      let reader = new FileReader();

      reader.addEventListener("load", function() {
        $scope.$apply(function() {
          let found = $scope.customFiles.find(e => e.name == file.name);
          if(found) {
            console.log("replaced", file);
            found.binary = reader.result;
          } else {
            console.log("loaded", file);
            $scope.customFiles.push(
              {name: file.name, binary: reader.result}
            );
          }
          $(file).val("");
        })
      }, false);

      $scope.$apply(function() {
        if (file) {
          console.log("loading", file);
          reader.readAsArrayBuffer(file);
        }
      });
    }
  };

}).directive("ngFileSelect", function() {
  return {
    link: function($scope, el) {
      el.bind("change", function(e) {
        $scope.getFile(el);
      })
    }
  }
  
}).directive("ngWallpaperSelect", function() {
  return {
    link: function($scope, el) {
      el.bind("change", function(e) {
        $scope.setWallpaper(el);
      })
    }
  }

}).config(function ($translateProvider) {
  $translateProvider
    .translations('en', {
      TITLE: 'Tah les applis sur votre calc',
      LEAD: 'Ici le shirAko vous aide à installer des applis sur votre calc.',
      FIRMWARE: 'Pour installer un OS sur lavostre Numworks qui soit digne de Linux, regardez ',
      DISCLAIM: 'Pour voir la leuliste des jeux, observez ',
      HERE: 'ceci.',      
      NO_WEB_USB: 'Votre navigateur ne supporte pas WebUSB, veuillez utiliser',
      SELECTED_APPLICATIONS: 'shirApplications sélectionnées',
      REMOVE: 'Supprimer',
      CUSTOM_FILE: 'Fichier local',
      INSTALL: 'Installer',
      WALLPAPER: 'Fond d\'écran',
      WALLPAPER_FILE: 'Fichier image',
      AVAILABLE_APPLICATIONS: 'Applications disponibles',
      ADD: 'Ajouter',
      ACKNOWLEDGMENTS: 'Remerciements',
      ALL_DONE: 'Terminé, cliquez ici pour recharger la page.',
      ERROR: 'Une erreur est survenue',
      PLEASE_RELOAD: 'Veuillez recharger la page',
      PROCESSING: "Traitement de",
      ADDING: "Ajout de",
      DFU_ERASING: "Effacement",
      DFU_COPYING: "Copie des fichiers",
      DFU_WROTE: "Terminé",
      TOO_MUCH_FILES: "Pas assez de place sur l'appareil",
      OR: "ou",
      CROP_IMAGE_TITLE: "Recadrer le fond d'écran",
      CROP_IMAGE_SAVE: "Sauvegarder",
      CANCEL: "Annuler",
      TOO_BIG_FILES: "Vous écrivez une grande quantité de fichiers sur votre calculatrice. Si vous utilisez une version récente d'Upsilon, vous devez passer par le bootloader (bouton de réinitialisation à l'arrière) pour vous assurer que les fichiers ne seront pas écrits dans du code en cours d'exécution.",
      UNABLE_TO_CLAIM_INTERFACE: "Impossible de réclamer l'interface. Veuillez vous assurer qu'aucun autre onglet ou application n'utilise la calculatrice.",
      TOO_MUCH_DATA: "Vous écrivez trop de données sur votre calculatrice. Veuillez vous assurer que vous n'écrivez pas plus de 6Mo de données. Essayez de supprimer des fichiers ou des applications.",
    
    })
    .translations('fr', {
      TITLE: 'Dépôt d\'application N0110 non officiel',
      LEAD: 'Vous trouverez ici quelques application installables sur une calculatrice N110.',
      FIRMWARE: 'Pour installer un micrologiciel compatible, veuillez vous rendre ',
      DISCLAIM: 'Pour plus d\'informations (ou soumettre un problème) veuillez vous rendre ',
      HERE: 'ici',
      NO_WEB_USB: 'Votre navigateur ne supporte pas WebUSB, veuillez utiliser',
      SELECTED_APPLICATIONS: 'Applications sélectionnées',
      REMOVE: 'Supprimer',
      CUSTOM_FILE: 'Fichier local',
      INSTALL: 'Installer',
      WALLPAPER: 'Fond d\'écran',
      WALLPAPER_FILE: 'Fichier image',
      AVAILABLE_APPLICATIONS: 'shirApplications disponibles',
      ADD: 'Ajouter',
      ACKNOWLEDGMENTS: 'Remerciements',
      ALL_DONE: 'Terminé, cliquez ici pour recharger la page.',
      ERROR: 'Une erreur est survenue',
      PLEASE_RELOAD: 'Veuillez recharger la page',
      PROCESSING: "Traitement de",
      ADDING: "Ajout de",
      DFU_ERASING: "Effacement",
      DFU_COPYING: "Copie des fichiers",
      DFU_WROTE: "Terminé",
      TOO_MUCH_FILES: "Pas assez de place sur l'appareil",
      OR: "ou",
      CROP_IMAGE_TITLE: "Recadrer le fond d'écran",
      CROP_IMAGE_SAVE: "Sauvegarder",
      CANCEL: "Annuler",
      TOO_BIG_FILES: "Vous écrivez une grande quantité de fichiers sur votre calculatrice. Si vous utilisez une version récente d'Upsilon, vous devez passer par le bootloader (bouton de réinitialisation à l'arrière) pour vous assurer que les fichiers ne seront pas écrits dans du code en cours d'exécution.",
      UNABLE_TO_CLAIM_INTERFACE: "Impossible de réclamer l'interface. Veuillez vous assurer qu'aucun autre onglet ou application n'utilise la calculatrice.",
      TOO_MUCH_DATA: "Vous écrivez trop de données sur votre calculatrice. Veuillez vous assurer que vous n'écrivez pas plus de 6Mo de données. Essayez de supprimer des fichiers ou des applications.",
    })
    .registerAvailableLanguageKeys(['en', 'fr'], {
      'en_*': 'en',
      'fr_*': 'fr',
      '*': 'en',
    })
    .determinePreferredLanguage()
    .useSanitizeValueStrategy('sanitizeParameters');
});

