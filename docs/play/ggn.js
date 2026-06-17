// include: shell.js
// include: minimum_runtime_check.js
(function() {
  // "30.0.0" -> 300000
  function humanReadableVersionToPacked(str) {
    str = str.split('-')[0]; // Remove any trailing part from e.g. "12.53.3-alpha"
    var vers = str.split('.').slice(0, 3);
    while(vers.length < 3) vers.push('00');
    vers = vers.map((n, i, arr) => n.padStart(2, '0'));
    return vers.join('');
  }
  // 300000 -> "30.0.0"
  var packedVersionToHumanReadable = n => [n / 10000 | 0, (n / 100 | 0) % 100, n % 100].join('.');

  var TARGET_NOT_SUPPORTED = 2147483647;

  // Note: We use a typeof check here instead of optional chaining using
  // globalThis because older browsers might not have globalThis defined.
  var currentNodeVersion = typeof process !== 'undefined' && process.versions?.node ? humanReadableVersionToPacked(process.versions.node) : TARGET_NOT_SUPPORTED;
  if (currentNodeVersion < 180300) {
    throw new Error(`This emscripten-generated code requires node v${ packedVersionToHumanReadable(180300) } (detected v${packedVersionToHumanReadable(currentNodeVersion)})`);
  }

  var userAgent = typeof navigator !== 'undefined' && navigator.userAgent;
  if (!userAgent) {
    return;
  }

  var currentSafariVersion = userAgent.includes("Safari/") && !userAgent.includes("Chrome/") && userAgent.match(/Version\/(\d+\.?\d*\.?\d*)/) ? humanReadableVersionToPacked(userAgent.match(/Version\/(\d+\.?\d*\.?\d*)/)[1]) : TARGET_NOT_SUPPORTED;
  if (currentSafariVersion < 150000) {
    throw new Error(`This emscripten-generated code requires Safari v${ packedVersionToHumanReadable(150000) } (detected v${currentSafariVersion})`);
  }

  var currentFirefoxVersion = userAgent.match(/Firefox\/(\d+(?:\.\d+)?)/) ? parseFloat(userAgent.match(/Firefox\/(\d+(?:\.\d+)?)/)[1]) : TARGET_NOT_SUPPORTED;
  if (currentFirefoxVersion < 79) {
    throw new Error(`This emscripten-generated code requires Firefox v79 (detected v${currentFirefoxVersion})`);
  }

  var currentChromeVersion = userAgent.match(/Chrome\/(\d+(?:\.\d+)?)/) ? parseFloat(userAgent.match(/Chrome\/(\d+(?:\.\d+)?)/)[1]) : TARGET_NOT_SUPPORTED;
  if (currentChromeVersion < 85) {
    throw new Error(`This emscripten-generated code requires Chrome v85 (detected v${currentChromeVersion})`);
  }
})();

// end include: minimum_runtime_check.js
// The Module object: Our interface to the outside world. We import
// and export values on it. There are various ways Module can be used:
// 1. Not defined. We create it here
// 2. A function parameter, function(moduleArg) => Promise<Module>
// 3. pre-run appended it, var Module = {}; ..generated code..
// 4. External script tag defines var Module.
// We need to check if Module already exists (e.g. case 3 above).
// Substitution will be replaced with actual code on later stage of the build,
// this way Closure Compiler will not mangle it (e.g. case 4. above).
// Note that if you want to run closure, and also to use Module
// after the generated code, you will need to define   var Module = {};
// before the code. Then that object will be used in the code, and you
// can continue to use Module afterwards as well.
var Module = typeof Module != 'undefined' ? Module : {};

// Determine the runtime environment we are in. You can customize this by
// setting the ENVIRONMENT setting at compile time (see settings.js).

// Attempt to auto-detect the environment
var ENVIRONMENT_IS_WEB = !!globalThis.window;
var ENVIRONMENT_IS_WORKER = !!globalThis.WorkerGlobalScope;
// N.b. Electron.js environment is simultaneously a NODE-environment, but
// also a web environment.
var ENVIRONMENT_IS_NODE = globalThis.process?.versions?.node && globalThis.process?.type != 'renderer';
var ENVIRONMENT_IS_SHELL = !ENVIRONMENT_IS_WEB && !ENVIRONMENT_IS_NODE && !ENVIRONMENT_IS_WORKER;

// --pre-jses are emitted after the Module integration code, so that they can
// refer to Module (if they choose; they can also define Module)
// include: C:\Users\youfo\AppData\Local\Temp\tmpxijnp56n.js

  if (!Module['expectedDataFileDownloads']) Module['expectedDataFileDownloads'] = 0;
  Module['expectedDataFileDownloads']++;
  (() => {
    // Do not attempt to redownload the virtual filesystem data when in a pthread or a Wasm Worker context.
    var isPthread = typeof ENVIRONMENT_IS_PTHREAD != 'undefined' && ENVIRONMENT_IS_PTHREAD;
    var isWasmWorker = typeof ENVIRONMENT_IS_WASM_WORKER != 'undefined' && ENVIRONMENT_IS_WASM_WORKER;
    if (isPthread || isWasmWorker) return;
    var isNode = globalThis.process && globalThis.process.versions && globalThis.process.versions.node && globalThis.process.type != 'renderer';
    async function loadPackage(metadata) {

      var PACKAGE_PATH = '';
      if (typeof window === 'object') {
        PACKAGE_PATH = window['encodeURIComponent'](window.location.pathname.substring(0, window.location.pathname.lastIndexOf('/')) + '/');
      } else if (typeof process === 'undefined' && typeof location !== 'undefined') {
        // web worker
        PACKAGE_PATH = encodeURIComponent(location.pathname.substring(0, location.pathname.lastIndexOf('/')) + '/');
      }
      var PACKAGE_NAME = 'E:/old4/Bitbucket/source/ggn/laug_th/web-build/ggn.data';
      var REMOTE_PACKAGE_BASE = 'ggn.data';
      Module['dataFileCompressedChunks'] = Module['dataFileCompressedChunks'] || {};
      Module['dataFileCompressedChunks'][REMOTE_PACKAGE_BASE] = ["ggn.data.gz"];
      Module['dataFileCompressedSize'] = Module['dataFileCompressedSize'] || {};
      Module['dataFileCompressedSize'][REMOTE_PACKAGE_BASE] = 7874762;
      var REMOTE_PACKAGE_NAME = Module['locateFile'] ? Module['locateFile'](REMOTE_PACKAGE_BASE, '') : REMOTE_PACKAGE_BASE;
      var REMOTE_PACKAGE_SIZE = metadata['remote_package_size'];

      async function fetchRemotePackage(packageName, packageSize) {
        if (isNode) {
          var contents = require('fs').readFileSync(packageName);
          return new Uint8Array(contents).buffer;
        }
        var compressedChunks = Module['dataFileCompressedChunks'] && (Module['dataFileCompressedChunks'][packageName] || Module['dataFileCompressedChunks'][packageName.split('/').pop()]);
        if (compressedChunks && compressedChunks.length) {
          if (typeof DecompressionStream === 'undefined') {
            throw new Error('This browser cannot decompress the compressed game data. Please use a current Chrome, Edge, Firefox, or Safari.');
          }
          if (!Module['dataFileDownloads']) Module['dataFileDownloads'] = {};
          var compressedSize = Module['dataFileCompressedSize'] && (Module['dataFileCompressedSize'][packageName] || Module['dataFileCompressedSize'][packageName.split('/').pop()]);
          var compressedLoaded = 0;
          var compressedParts = [];
          var basePath = packageName.lastIndexOf('/') >= 0 ? packageName.substring(0, packageName.lastIndexOf('/') + 1) : '';
          for (var chunkIndex = 0; chunkIndex < compressedChunks.length; ++chunkIndex) {
            var chunkName = compressedChunks[chunkIndex];
            var chunkUrl = basePath + chunkName;
            var chunkResponse = await fetch(chunkUrl);
            if (!chunkResponse.ok) {
              throw new Error(`${chunkResponse.status}: ${chunkResponse.url}`);
            }
            var chunkBytes = new Uint8Array(await chunkResponse.arrayBuffer());
            compressedParts.push(chunkBytes);
            compressedLoaded += chunkBytes.length;
            Module['dataFileDownloads'][chunkUrl] = { loaded: chunkBytes.length, total: chunkBytes.length };
            Module['setStatus'] && Module['setStatus'](`Downloading compressed data... (${compressedLoaded}/${compressedSize || compressedLoaded})`);
          }
          var compressedData = new Uint8Array(compressedLoaded);
          var compressedOffset = 0;
          for (var compressedPart of compressedParts) {
            compressedData.set(compressedPart, compressedOffset);
            compressedOffset += compressedPart.length;
          }
          Module['setStatus'] && Module['setStatus']('Decompressing data...');
          var decompressedResponse = new Response(new Blob([compressedData]).stream().pipeThrough(new DecompressionStream('gzip')));
          var decompressedBuffer = await decompressedResponse.arrayBuffer();
          if (packageSize && decompressedBuffer.byteLength !== packageSize) {
            throw new Error(`Decompressed data size mismatch: ${decompressedBuffer.byteLength}/${packageSize}`);
          }
          return decompressedBuffer;
        }
        if (!Module['dataFileDownloads']) Module['dataFileDownloads'] = {};
        try {
          var response = await fetch(packageName);
        } catch (e) {
          throw new Error(`Network Error: ${packageName}`, {e});
        }
        if (!response.ok) {
          throw new Error(`${response.status}: ${response.url}`);
        }

        const chunks = [];
        const headers = response.headers;
        const total = Number(headers.get('Content-Length') || packageSize);
        let loaded = 0;

        Module['setStatus'] && Module['setStatus']('Downloading data...');
        const reader = response.body.getReader();

        while (1) {
          var {done, value} = await reader.read();
          if (done) break;
          chunks.push(value);
          loaded += value.length;
          Module['dataFileDownloads'][packageName] = {loaded, total};

          let totalLoaded = 0;
          let totalSize = 0;

          for (const download of Object.values(Module['dataFileDownloads'])) {
            totalLoaded += download.loaded;
            totalSize += download.total;
          }

          Module['setStatus'] && Module['setStatus'](`Downloading data... (${totalLoaded}/${totalSize})`);
        }

        const packageData = new Uint8Array(chunks.map((c) => c.length).reduce((a, b) => a + b, 0));
        let offset = 0;
        for (const chunk of chunks) {
          packageData.set(chunk, offset);
          offset += chunk.length;
        }
        return packageData.buffer;
      }

      var fetchPromise;
      var fetched = Module['getPreloadedPackage'] && Module['getPreloadedPackage'](REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE);

      if (!fetched) {
        // Note that we don't use await here because we want to execute the
        // the rest of this function immediately.
        fetchPromise = fetchRemotePackage(REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE);
      }

    async function runWithFS(Module) {

      function assert(check, msg) {
        if (!check) throw new Error(msg);
      }
Module['FS_createPath']("/", "Language", true, true);
Module['FS_createPath']("/Language", "Default", true, true);
Module['FS_createPath']("/Language/Default", "String", true, true);
Module['FS_createPath']("/Language/Default/String", "CharacterName", true, true);
Module['FS_createPath']("/Language/Default/String", "Command", true, true);
Module['FS_createPath']("/Language/Default/String", "Dungeon", true, true);
Module['FS_createPath']("/Language/Default/String", "ItemExpressions", true, true);
Module['FS_createPath']("/Language/Default/String", "Story", true, true);
Module['FS_createPath']("/Language/Default/String", "UnknownName", true, true);
Module['FS_createPath']("/Language/Default/String", "letter", true, true);
Module['FS_createPath']("/Language/Default/String", "messages", true, true);
Module['FS_createPath']("/Language/Default/String/messages", "CharaMessages", true, true);
Module['FS_createPath']("/Language/Default/String/messages", "GameMessages", true, true);
Module['FS_createPath']("/Language/Default/String/messages/GameMessages", "ShopMessage", true, true);
Module['FS_createPath']("/Language/Default/String/messages", "ItemMessages", true, true);
Module['FS_createPath']("/Language/Default/String", "tips", true, true);
Module['FS_createPath']("/Language", "Shiyou", true, true);
Module['FS_createPath']("/Language/Shiyou", "String", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "CharacterName", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "Command", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "Dungeon", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "ItemExpressions", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "Story", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "UnknownName", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "letter", true, true);
Module['FS_createPath']("/Language/Shiyou/String", "messages", true, true);
Module['FS_createPath']("/Language/Shiyou/String/messages", "GameMessages", true, true);
Module['FS_createPath']("/Language/Shiyou/String/messages/GameMessages", "ShopMessage", true, true);
Module['FS_createPath']("/Language/Shiyou/String/messages", "ItemMessages", true, true);
Module['FS_createPath']("/", "assets", true, true);
Module['FS_createPath']("/assets", "graphic", true, true);
Module['FS_createPath']("/assets/graphic", "chara", true, true);
Module['FS_createPath']("/assets/graphic/chara", "common", true, true);
Module['FS_createPath']("/assets/graphic/chara", "dot", true, true);
Module['FS_createPath']("/assets/graphic/chara", "op", true, true);
Module['FS_createPath']("/assets/graphic", "effect", true, true);
Module['FS_createPath']("/assets/graphic/effect", "basic", true, true);
Module['FS_createPath']("/assets/graphic/effect", "equip", true, true);
Module['FS_createPath']("/assets/graphic/effect", "fire", true, true);
Module['FS_createPath']("/assets/graphic/effect", "mob", true, true);
Module['FS_createPath']("/assets/graphic/effect", "spell", true, true);
Module['FS_createPath']("/assets/graphic/effect", "trap", true, true);
Module['FS_createPath']("/assets/graphic", "event", true, true);
Module['FS_createPath']("/assets/graphic/event", "staff", true, true);
Module['FS_createPath']("/assets/graphic", "interface", true, true);
Module['FS_createPath']("/assets/graphic", "item", true, true);
Module['FS_createPath']("/assets/graphic", "map", true, true);
Module['FS_createPath']("/assets/graphic", "skin", true, true);
Module['FS_createPath']("/", "data", true, true);
Module['FS_createPath']("/data", "Boss", true, true);
Module['FS_createPath']("/data/Boss", "kanako", true, true);
Module['FS_createPath']("/data/Boss/kanako", "spell", true, true);
Module['FS_createPath']("/data/Boss", "kanakoEx", true, true);
Module['FS_createPath']("/data/Boss/kanakoEx", "spell", true, true);
Module['FS_createPath']("/data/Boss", "kyuualice", true, true);
Module['FS_createPath']("/data/Boss/kyuualice", "spell", true, true);
Module['FS_createPath']("/data/Boss", "kyuualiceEx", true, true);
Module['FS_createPath']("/data/Boss/kyuualiceEx", "spell", true, true);
Module['FS_createPath']("/data/Boss", "remilia", true, true);
Module['FS_createPath']("/data/Boss/remilia", "spell", true, true);
Module['FS_createPath']("/data/Boss", "remiliaEx", true, true);
Module['FS_createPath']("/data/Boss/remiliaEx", "spell", true, true);
Module['FS_createPath']("/data/Boss", "tewi", true, true);
Module['FS_createPath']("/data/Boss/tewi", "spell", true, true);
Module['FS_createPath']("/data/Boss", "tewiEx", true, true);
Module['FS_createPath']("/data/Boss/tewiEx", "spell", true, true);
Module['FS_createPath']("/data/Boss", "youmu", true, true);
Module['FS_createPath']("/data/Boss/youmu", "spell", true, true);
Module['FS_createPath']("/data/Boss", "youmuEx", true, true);
Module['FS_createPath']("/data/Boss/youmuEx", "spell", true, true);
Module['FS_createPath']("/data", "DropScripts", true, true);
Module['FS_createPath']("/data/DropScripts", "2000", true, true);
Module['FS_createPath']("/data", "Dungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "99FDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "99FUraDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "R10_99FDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "_kgm_testDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "_old_testDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "blueDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "blueExtraDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "eventDreamDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "eventEndingDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "firstHome", true, true);
Module['FS_createPath']("/data/Dungeon", "globalScript", true, true);
Module['FS_createPath']("/data/Dungeon", "greenDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "greenExtraDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "hellDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "introDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "lastDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "noEquipDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "noFoodDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "purpleDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "purpleExtraDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "redDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "redExtraDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "tenMatchDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "testDungeon", true, true);
Module['FS_createPath']("/data/Dungeon", "tutorialDungeon", true, true);
Module['FS_createPath']("/data", "Effect", true, true);
Module['FS_createPath']("/data/Effect", "Chara", true, true);
Module['FS_createPath']("/data/Effect/Chara", "Attack", true, true);
Module['FS_createPath']("/data/Effect/Chara", "Special", true, true);
Module['FS_createPath']("/data/Effect", "Equipment", true, true);
Module['FS_createPath']("/data/Effect/Equipment", "Attack", true, true);
Module['FS_createPath']("/data/Effect/Equipment", "Damage", true, true);
Module['FS_createPath']("/data/Effect", "Other", true, true);
Module['FS_createPath']("/data/Effect", "Spell", true, true);
Module['FS_createPath']("/data/Effect", "Trap", true, true);
Module['FS_createPath']("/data", "Enemy", true, true);
Module['FS_createPath']("/data", "HomeDataBase", true, true);
Module['FS_createPath']("/data/HomeDataBase", "HomeEvents", true, true);
Module['FS_createPath']("/data", "Interface", true, true);
Module['FS_createPath']("/data", "Item", true, true);
Module['FS_createPath']("/data", "Mapchip", true, true);
Module['FS_createPath']("/data", "OtherChara", true, true);
Module['FS_createPath']("/data", "PlayerChara", true, true);

    for (var file of metadata['files']) {
      var name = file['filename']
      Module['addRunDependency'](`fp ${name}`);
    }

      async function processPackageData(arrayBuffer) {
        assert(arrayBuffer, 'Loading data file failed.');
        assert(arrayBuffer.constructor.name === ArrayBuffer.name, 'bad input to processPackageData ' + arrayBuffer.constructor.name);
        var byteArray = new Uint8Array(arrayBuffer);
        var curr;
        // Reuse the bytearray from the XHR as the source for file reads.
          for (var file of metadata['files']) {
            var name = file['filename'];
            var data = byteArray.subarray(file['start'], file['end']);
            // canOwn this data in the filesystem, it is a slice into the heap that will never change
        Module['FS_createDataFile'](name, null, data, true, true, true);
        Module['removeRunDependency'](`fp ${name}`);
          }
          Module['removeRunDependency']('datafile_E:/old4/Bitbucket/source/ggn/laug_th/web-build/ggn.data');
      }
      Module['addRunDependency']('datafile_E:/old4/Bitbucket/source/ggn/laug_th/web-build/ggn.data');

      if (!Module['preloadResults']) Module['preloadResults'] = {};

      Module['preloadResults'][PACKAGE_NAME] = {fromCache: false};
      if (!fetched) {
        fetched = await fetchPromise;
      }
      processPackageData(fetched);

    }
    if (Module['calledRun']) {
      runWithFS(Module);
    } else {
      if (!Module['preRun']) Module['preRun'] = [];
      Module['preRun'].push(runWithFS); // FS is not initialized yet, wait for it
    }

    }
    loadPackage({"files": [{"filename": "/Language/Default/CharaLanguage.id", "start": 0, "end": 338}, {"filename": "/Language/Default/ItemLanguage.id", "start": 338, "end": 752}, {"filename": "/Language/Default/Name.id", "start": 752, "end": 1000}, {"filename": "/Language/Default/String/CharacterName/CharaName1000.id", "start": 1000, "end": 2842}, {"filename": "/Language/Default/String/CharacterName/CharaName2000.id", "start": 2842, "end": 22110}, {"filename": "/Language/Default/String/CharacterName/CharaName2000table.csv", "start": 22110, "end": 30868}, {"filename": "/Language/Default/String/CharacterName/CharaName3000.id", "start": 30868, "end": 31788}, {"filename": "/Language/Default/String/CharacterName/CharaName3000table.csv", "start": 31788, "end": 32418}, {"filename": "/Language/Default/String/CharacterName/CharaName4000.id", "start": 32418, "end": 32492}, {"filename": "/Language/Default/String/CharacterName/CharaName4000table.csv", "start": 32492, "end": 33416}, {"filename": "/Language/Default/String/CharacterName/CharaName5000.id", "start": 33416, "end": 40494}, {"filename": "/Language/Default/String/CharacterName/CharaName_header.id", "start": 40494, "end": 40970}, {"filename": "/Language/Default/String/Command/BaseCommand.id", "start": 40970, "end": 43130}, {"filename": "/Language/Default/String/Command/Command_header.id", "start": 43130, "end": 43638}, {"filename": "/Language/Default/String/Command/GameCommand.id", "start": 43638, "end": 49358}, {"filename": "/Language/Default/String/Command/HomeResultWindow.id", "start": 49358, "end": 50294}, {"filename": "/Language/Default/String/Command/OtherCharaCommand.id", "start": 50294, "end": 51798}, {"filename": "/Language/Default/String/Command/ResultWindow.id", "start": 51798, "end": 58722}, {"filename": "/Language/Default/String/Command/Titlecommand.id", "start": 58722, "end": 61786}, {"filename": "/Language/Default/String/Dungeon/DungeonExplanation.id", "start": 61786, "end": 64014}, {"filename": "/Language/Default/String/Dungeon/DungeonName.id", "start": 64014, "end": 65188}, {"filename": "/Language/Default/String/Dungeon/DungeonRegulation.id", "start": 65188, "end": 66440}, {"filename": "/Language/Default/String/Dungeon/Dungeon_header.id", "start": 66440, "end": 66686}, {"filename": "/Language/Default/String/ItemExpressions/BookExpressions.id", "start": 66686, "end": 84154}, {"filename": "/Language/Default/String/ItemExpressions/Box7027longExplanationBase.lua", "start": 84154, "end": 84364}, {"filename": "/Language/Default/String/ItemExpressions/Box7027shortExplanationBase.lua", "start": 84364, "end": 84574}, {"filename": "/Language/Default/String/ItemExpressions/BoxExpressions.id", "start": 84574, "end": 112744}, {"filename": "/Language/Default/String/ItemExpressions/BulletExpressions.id", "start": 112744, "end": 136034}, {"filename": "/Language/Default/String/ItemExpressions/DrinkExpressions.id", "start": 136034, "end": 164474}, {"filename": "/Language/Default/String/ItemExpressions/EquipmentExpressions.id", "start": 164474, "end": 229240}, {"filename": "/Language/Default/String/ItemExpressions/Expressions_header.id", "start": 229240, "end": 230450}, {"filename": "/Language/Default/String/ItemExpressions/FoodExpressions.id", "start": 230450, "end": 242218}, {"filename": "/Language/Default/String/ItemExpressions/MoneyExpressions.id", "start": 242218, "end": 243858}, {"filename": "/Language/Default/String/ItemExpressions/OtherExpressions.id", "start": 243858, "end": 245650}, {"filename": "/Language/Default/String/ItemExpressions/SpellExpressions.id", "start": 245650, "end": 304510}, {"filename": "/Language/Default/String/ItemExpressions/TrapExpressions.id", "start": 304510, "end": 324952}, {"filename": "/Language/Default/String/Story/CharacterColorMacros.id", "start": 324952, "end": 326804}, {"filename": "/Language/Default/String/Story/ED.id", "start": 326804, "end": 333362}, {"filename": "/Language/Default/String/Story/ED2.id", "start": 333362, "end": 341102}, {"filename": "/Language/Default/String/Story/Story_header.id", "start": 341102, "end": 343030}, {"filename": "/Language/Default/String/Story/blueDungeon.id", "start": 343030, "end": 349738}, {"filename": "/Language/Default/String/Story/blueDungeonAfterBoss.id", "start": 349738, "end": 360880}, {"filename": "/Language/Default/String/Story/blueDungeonAfterEvent.id", "start": 360880, "end": 369048}, {"filename": "/Language/Default/String/Story/blueDungeonBeforeEvent.id", "start": 369048, "end": 372858}, {"filename": "/Language/Default/String/Story/blueExtraDungeonBeforeEvent.id", "start": 372858, "end": 373292}, {"filename": "/Language/Default/String/Story/firstEvent.id", "start": 373292, "end": 379266}, {"filename": "/Language/Default/String/Story/greenDungeon.id", "start": 379266, "end": 385590}, {"filename": "/Language/Default/String/Story/greenDungeonAfterEvent.id", "start": 385590, "end": 389656}, {"filename": "/Language/Default/String/Story/greenDungeonCloseEvent.id", "start": 389656, "end": 392854}, {"filename": "/Language/Default/String/Story/greenExtraDungeonBeforeEvent.id", "start": 392854, "end": 393294}, {"filename": "/Language/Default/String/Story/homeAkyuAppearEvent.id", "start": 393294, "end": 394872}, {"filename": "/Language/Default/String/Story/homeHinaAppearEvent.id", "start": 394872, "end": 397246}, {"filename": "/Language/Default/String/Story/homeMarisaAppearEvent.id", "start": 397246, "end": 399344}, {"filename": "/Language/Default/String/Story/homeNitoriAppearEvent.id", "start": 399344, "end": 401234}, {"filename": "/Language/Default/String/Story/homeOtherEvent.id", "start": 401234, "end": 401484}, {"filename": "/Language/Default/String/Story/homeResetEvent.id", "start": 401484, "end": 402080}, {"filename": "/Language/Default/String/Story/homeYoumuAppearEvent.id", "start": 402080, "end": 404798}, {"filename": "/Language/Default/String/Story/intro.id", "start": 404798, "end": 412320}, {"filename": "/Language/Default/String/Story/lastDungeon.id", "start": 412320, "end": 420056}, {"filename": "/Language/Default/String/Story/lastDungeonOpenEvent.id", "start": 420056, "end": 428670}, {"filename": "/Language/Default/String/Story/mottoOpenEvent.id", "start": 428670, "end": 434666}, {"filename": "/Language/Default/String/Story/noEquipDungeonBeforeEvent.id", "start": 434666, "end": 435080}, {"filename": "/Language/Default/String/Story/noFoodDungeonBeforeEvent.id", "start": 435080, "end": 435490}, {"filename": "/Language/Default/String/Story/playerLevelExplanation.id", "start": 435490, "end": 436042}, {"filename": "/Language/Default/String/Story/purpleDungeon.id", "start": 436042, "end": 441524}, {"filename": "/Language/Default/String/Story/purpleDungeonAfterEvent.id", "start": 441524, "end": 449092}, {"filename": "/Language/Default/String/Story/purpleDungeonCloseEvent.id", "start": 449092, "end": 451350}, {"filename": "/Language/Default/String/Story/purpleExtraDungeonBeforeEvent.id", "start": 451350, "end": 451792}, {"filename": "/Language/Default/String/Story/redDungeon.id", "start": 451792, "end": 456504}, {"filename": "/Language/Default/String/Story/redDungeonAfter.id", "start": 456504, "end": 463022}, {"filename": "/Language/Default/String/Story/redDungeonAfterEvent.id", "start": 463022, "end": 464078}, {"filename": "/Language/Default/String/Story/redExtraDungeonBeforeEvent.id", "start": 464078, "end": 464490}, {"filename": "/Language/Default/String/Story/tenMatchDungeonBeforeEvent.id", "start": 464490, "end": 465100}, {"filename": "/Language/Default/String/Story/tutorial.id", "start": 465100, "end": 472158}, {"filename": "/Language/Default/String/Story/tutorialDungeon.id", "start": 472158, "end": 476244}, {"filename": "/Language/Default/String/Style.id", "start": 476244, "end": 476298}, {"filename": "/Language/Default/String/UnknownName/UnknownBook.id", "start": 476298, "end": 477096}, {"filename": "/Language/Default/String/UnknownName/UnknownBox.id", "start": 477096, "end": 477656}, {"filename": "/Language/Default/String/UnknownName/UnknownBullet.id", "start": 477656, "end": 478392}, {"filename": "/Language/Default/String/UnknownName/UnknownDrink.id", "start": 478392, "end": 482772}, {"filename": "/Language/Default/String/UnknownName/UnknownSpell.id", "start": 482772, "end": 484450}, {"filename": "/Language/Default/String/UnknownName/Unknown_header.id", "start": 484450, "end": 484638}, {"filename": "/Language/Default/String/letter/StringWindowData.id", "start": 484638, "end": 491130}, {"filename": "/Language/Default/String/letter/letter - \u30b3\u30d4\u30fc.id", "start": 491130, "end": 492948}, {"filename": "/Language/Default/String/letter/letter.id", "start": 492948, "end": 494766}, {"filename": "/Language/Default/String/messages/CharaMessages/CharaMessages_header.id", "start": 494766, "end": 494802}, {"filename": "/Language/Default/String/messages/CharaMessages/YoumuMessage.id", "start": 494802, "end": 496960}, {"filename": "/Language/Default/String/messages/GameMessages/AttackMessages.id", "start": 496960, "end": 498152}, {"filename": "/Language/Default/String/messages/GameMessages/BossMessage.id", "start": 498152, "end": 498598}, {"filename": "/Language/Default/String/messages/GameMessages/CharaStatesMessage.id", "start": 498598, "end": 503468}, {"filename": "/Language/Default/String/messages/GameMessages/ConditionExpression.id", "start": 503468, "end": 513484}, {"filename": "/Language/Default/String/messages/GameMessages/ConditionMessage.id", "start": 513484, "end": 516280}, {"filename": "/Language/Default/String/messages/GameMessages/DropingMsMessage.id", "start": 516280, "end": 518678}, {"filename": "/Language/Default/String/messages/GameMessages/DropingStateMessage.id", "start": 518678, "end": 519976}, {"filename": "/Language/Default/String/messages/GameMessages/EnemyMessage.id", "start": 519976, "end": 524948}, {"filename": "/Language/Default/String/messages/GameMessages/GameMessages_header.id", "start": 524948, "end": 525872}, {"filename": "/Language/Default/String/messages/GameMessages/MapMessage.id", "start": 525872, "end": 526638}, {"filename": "/Language/Default/String/messages/GameMessages/OtherMessage.id", "start": 526638, "end": 527404}, {"filename": "/Language/Default/String/messages/GameMessages/ShopMessage.id", "start": 527404, "end": 536262}, {"filename": "/Language/Default/String/messages/GameMessages/ShopMessage.lua", "start": 536262, "end": 536418}, {"filename": "/Language/Default/String/messages/GameMessages/ShopMessage/MarisaShopBuy.lua", "start": 536418, "end": 536574}, {"filename": "/Language/Default/String/messages/GameMessages/ShopMessage/MarisaShopSell.lua", "start": 536574, "end": 536730}, {"filename": "/Language/Default/String/messages/ItemMessages/AttributeExpressions.id", "start": 536730, "end": 539722}, {"filename": "/Language/Default/String/messages/ItemMessages/BookMessages.id", "start": 539722, "end": 541438}, {"filename": "/Language/Default/String/messages/ItemMessages/BoxMessages.id", "start": 541438, "end": 545722}, {"filename": "/Language/Default/String/messages/ItemMessages/BulletMessages.id", "start": 545722, "end": 546774}, {"filename": "/Language/Default/String/messages/ItemMessages/DrinkMessages.id", "start": 546774, "end": 548914}, {"filename": "/Language/Default/String/messages/ItemMessages/EquipMessages.id", "start": 548914, "end": 552858}, {"filename": "/Language/Default/String/messages/ItemMessages/FoodMessages.id", "start": 552858, "end": 553198}, {"filename": "/Language/Default/String/messages/ItemMessages/ItemMessages.id", "start": 553198, "end": 553950}, {"filename": "/Language/Default/String/messages/ItemMessages/ItemMessages_header.id", "start": 553950, "end": 555088}, {"filename": "/Language/Default/String/messages/ItemMessages/MoneyMessages.id", "start": 555088, "end": 555466}, {"filename": "/Language/Default/String/messages/ItemMessages/OtherMessages.id", "start": 555466, "end": 556840}, {"filename": "/Language/Default/String/messages/ItemMessages/SpellMessages.id", "start": 556840, "end": 561584}, {"filename": "/Language/Default/String/messages/ItemMessages/TrapMessages.id", "start": 561584, "end": 563268}, {"filename": "/Language/Default/String/messages/Message_header.id", "start": 563268, "end": 563692}, {"filename": "/Language/Default/String/tips/tipsCommand.id", "start": 563692, "end": 563860}, {"filename": "/Language/Default/String/tips/tipsContents.id", "start": 563860, "end": 635476}, {"filename": "/Language/Default/String/tips/tips_header.id", "start": 635476, "end": 635552}, {"filename": "/Language/Default/Unknown.id", "start": 635552, "end": 635728}, {"filename": "/Language/Default/systems.id", "start": 635728, "end": 636734}, {"filename": "/Language/Language\u306b\u3064\u3044\u3066.txt", "start": 636734, "end": 640918}, {"filename": "/Language/Shiyou/CharaLanguage.id", "start": 640918, "end": 641256}, {"filename": "/Language/Shiyou/ItemLanguage.id", "start": 641256, "end": 641670}, {"filename": "/Language/Shiyou/Name.id", "start": 641670, "end": 641918}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName1000.id", "start": 641918, "end": 643046}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName2000.id", "start": 643046, "end": 662300}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName2000table.csv", "start": 662300, "end": 668972}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName3000.id", "start": 668972, "end": 669892}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName3000table.csv", "start": 669892, "end": 670522}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName4000.id", "start": 670522, "end": 672770}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName5000.id", "start": 672770, "end": 674218}, {"filename": "/Language/Shiyou/String/CharacterName/CharaName_header.id", "start": 674218, "end": 674694}, {"filename": "/Language/Shiyou/String/Command/BaseCommand.id", "start": 674694, "end": 676560}, {"filename": "/Language/Shiyou/String/Command/Command_header.id", "start": 676560, "end": 677020}, {"filename": "/Language/Shiyou/String/Command/GameCommand.id", "start": 677020, "end": 681770}, {"filename": "/Language/Shiyou/String/Command/HomeResultWindow.id", "start": 681770, "end": 682706}, {"filename": "/Language/Shiyou/String/Command/ResultWindow.id", "start": 682706, "end": 689630}, {"filename": "/Language/Shiyou/String/Command/Titlecommand.id", "start": 689630, "end": 692402}, {"filename": "/Language/Shiyou/String/Dungeon/DungeonExplanation.id", "start": 692402, "end": 694630}, {"filename": "/Language/Shiyou/String/Dungeon/DungeonName.id", "start": 694630, "end": 695418}, {"filename": "/Language/Shiyou/String/Dungeon/Dungeon_header.id", "start": 695418, "end": 695616}, {"filename": "/Language/Shiyou/String/ItemExpressions/BookExpressions.id", "start": 695616, "end": 713852}, {"filename": "/Language/Shiyou/String/ItemExpressions/BoxExpressions.id", "start": 713852, "end": 739752}, {"filename": "/Language/Shiyou/String/ItemExpressions/BulletExpressions.id", "start": 739752, "end": 763356}, {"filename": "/Language/Shiyou/String/ItemExpressions/DrinkExpressions.id", "start": 763356, "end": 789772}, {"filename": "/Language/Shiyou/String/ItemExpressions/EquipmentExpressions.id", "start": 789772, "end": 859852}, {"filename": "/Language/Shiyou/String/ItemExpressions/Expressions_header.id", "start": 859852, "end": 861062}, {"filename": "/Language/Shiyou/String/ItemExpressions/FoodExpressions.id", "start": 861062, "end": 871714}, {"filename": "/Language/Shiyou/String/ItemExpressions/MoneyExpressions.id", "start": 871714, "end": 874350}, {"filename": "/Language/Shiyou/String/ItemExpressions/OtherExpressions.id", "start": 874350, "end": 875978}, {"filename": "/Language/Shiyou/String/ItemExpressions/SpellExpressions.id", "start": 875978, "end": 916304}, {"filename": "/Language/Shiyou/String/ItemExpressions/TrapExpressions.id", "start": 916304, "end": 937618}, {"filename": "/Language/Shiyou/String/ItemExpressions/test.lua", "start": 937618, "end": 937953}, {"filename": "/Language/Shiyou/String/Story/Story_header.id", "start": 937953, "end": 938563}, {"filename": "/Language/Shiyou/String/Story/blueDungeon.id", "start": 938563, "end": 944017}, {"filename": "/Language/Shiyou/String/Story/blueDungeonAfterEvent.id", "start": 944017, "end": 945023}, {"filename": "/Language/Shiyou/String/Story/blueDungeonBeforeEvent.id", "start": 945023, "end": 945533}, {"filename": "/Language/Shiyou/String/Story/firstEvent.id", "start": 945533, "end": 954353}, {"filename": "/Language/Shiyou/String/Story/homeHinaAppearEvent.id", "start": 954353, "end": 956375}, {"filename": "/Language/Shiyou/String/Story/homeMarisaAppearEvent.id", "start": 956375, "end": 957913}, {"filename": "/Language/Shiyou/String/Story/homeNitoriAppearEvent.id", "start": 957913, "end": 959537}, {"filename": "/Language/Shiyou/String/Story/homeOtherEvent.id", "start": 959537, "end": 959719}, {"filename": "/Language/Shiyou/String/Story/intro.id", "start": 959719, "end": 963709}, {"filename": "/Language/Shiyou/String/Story/lastDungeon.id", "start": 963709, "end": 965427}, {"filename": "/Language/Shiyou/String/Story/lastDungeonBeforeEvent.id", "start": 965427, "end": 967461}, {"filename": "/Language/Shiyou/String/Story/redDungeon.id", "start": 967461, "end": 977733}, {"filename": "/Language/Shiyou/String/Story/redDungeonAfterEvent.id", "start": 977733, "end": 978765}, {"filename": "/Language/Shiyou/String/Story/tutorial.id", "start": 978765, "end": 985865}, {"filename": "/Language/Shiyou/String/Story/tutorialDungeon.id", "start": 985865, "end": 990013}, {"filename": "/Language/Shiyou/String/Story/tutorialDungeonBeforeEvent.id", "start": 990013, "end": 990405}, {"filename": "/Language/Shiyou/String/Style.id", "start": 990405, "end": 990459}, {"filename": "/Language/Shiyou/String/UnknownName/UnknownBook.id", "start": 990459, "end": 991257}, {"filename": "/Language/Shiyou/String/UnknownName/UnknownBox.id", "start": 991257, "end": 991817}, {"filename": "/Language/Shiyou/String/UnknownName/UnknownBullet.id", "start": 991817, "end": 992553}, {"filename": "/Language/Shiyou/String/UnknownName/UnknownDrink.id", "start": 992553, "end": 996933}, {"filename": "/Language/Shiyou/String/UnknownName/UnknownSpell.id", "start": 996933, "end": 998611}, {"filename": "/Language/Shiyou/String/UnknownName/Unknown_header.id", "start": 998611, "end": 998799}, {"filename": "/Language/Shiyou/String/letter/StringWindowData.id", "start": 998799, "end": 1005291}, {"filename": "/Language/Shiyou/String/letter/letter - \u30b3\u30d4\u30fc.id", "start": 1005291, "end": 1007109}, {"filename": "/Language/Shiyou/String/letter/letter.id", "start": 1007109, "end": 1008927}, {"filename": "/Language/Shiyou/String/messages/GameMessages/AttackMessages.id", "start": 1008927, "end": 1010119}, {"filename": "/Language/Shiyou/String/messages/GameMessages/CharaStatesMessage.id", "start": 1010119, "end": 1014815}, {"filename": "/Language/Shiyou/String/messages/GameMessages/ConditionExpression.id", "start": 1014815, "end": 1024439}, {"filename": "/Language/Shiyou/String/messages/GameMessages/ConditionMessage.id", "start": 1024439, "end": 1027065}, {"filename": "/Language/Shiyou/String/messages/GameMessages/DropingMsMessage.id", "start": 1027065, "end": 1028975}, {"filename": "/Language/Shiyou/String/messages/GameMessages/DropingStateMessage.id", "start": 1028975, "end": 1030273}, {"filename": "/Language/Shiyou/String/messages/GameMessages/EnemyMessage.id", "start": 1030273, "end": 1033691}, {"filename": "/Language/Shiyou/String/messages/GameMessages/GameMessages_header.id", "start": 1033691, "end": 1034579}, {"filename": "/Language/Shiyou/String/messages/GameMessages/MapMessage.id", "start": 1034579, "end": 1035345}, {"filename": "/Language/Shiyou/String/messages/GameMessages/OtherMessage.id", "start": 1035345, "end": 1036111}, {"filename": "/Language/Shiyou/String/messages/GameMessages/ShopMessage.id", "start": 1036111, "end": 1044449}, {"filename": "/Language/Shiyou/String/messages/GameMessages/ShopMessage.lua", "start": 1044449, "end": 1044605}, {"filename": "/Language/Shiyou/String/messages/GameMessages/ShopMessage/MarisaShopBuy.lua", "start": 1044605, "end": 1044761}, {"filename": "/Language/Shiyou/String/messages/GameMessages/ShopMessage/MarisaShopSell.lua", "start": 1044761, "end": 1044917}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/AttributeExpressions.id", "start": 1044917, "end": 1047727}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/BookMessages.id", "start": 1047727, "end": 1049331}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/BoxMessages.id", "start": 1049331, "end": 1053507}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/BulletMessages.id", "start": 1053507, "end": 1054559}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/DrinkMessages.id", "start": 1054559, "end": 1056585}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/EquipMessages.id", "start": 1056585, "end": 1060529}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/FoodMessages.id", "start": 1060529, "end": 1060869}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/ItemMessages.id", "start": 1060869, "end": 1061621}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/ItemMessages_header.id", "start": 1061621, "end": 1062759}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/MoneyMessages.id", "start": 1062759, "end": 1063137}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/OtherMessages.id", "start": 1063137, "end": 1064511}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/SpellMessages.id", "start": 1064511, "end": 1068623}, {"filename": "/Language/Shiyou/String/messages/ItemMessages/TrapMessages.id", "start": 1068623, "end": 1070415}, {"filename": "/Language/Shiyou/String/messages/Message_header.id", "start": 1070415, "end": 1070757}, {"filename": "/Language/Shiyou/Unknown.id", "start": 1070757, "end": 1070933}, {"filename": "/Language/Shiyou/systems.id", "start": 1070933, "end": 1071939}, {"filename": "/Language/select.id", "start": 1071939, "end": 1072013}, {"filename": "/assets/graphic/chara/common/emotion.png.ggntex", "start": 1072013, "end": 1596317}, {"filename": "/assets/graphic/chara/common/graphicpass.id", "start": 1596317, "end": 1596365}, {"filename": "/assets/graphic/chara/common/shadow.png.ggntex", "start": 1596365, "end": 1600477}, {"filename": "/assets/graphic/chara/dot/filelist.csv", "start": 1600477, "end": 1602657}, {"filename": "/assets/graphic/chara/dot/graphicpass.id", "start": 1602657, "end": 1605523}, {"filename": "/assets/graphic/chara/dot/\u30a2\u30ad\u30e5\u30a6dot.png.ggntex", "start": 1605523, "end": 1867683}, {"filename": "/assets/graphic/chara/dot/\u30a2\u30e4dot.png.ggntex", "start": 1867683, "end": 2391987}, {"filename": "/assets/graphic/chara/dot/\u30a2\u30ea\u30b9dot.png.ggntex", "start": 2391987, "end": 2654147}, {"filename": "/assets/graphic/chara/dot/\u30a4\u30afdot.png.ggntex", "start": 2654147, "end": 3178451}, {"filename": "/assets/graphic/chara/dot/\u30a4\u30c1\u30ea\u30f3dot.png.ggntex", "start": 3178451, "end": 3702755}, {"filename": "/assets/graphic/chara/dot/\u30a4\u30c1\u30ea\u30f3\u30ce\u30df dot.png.ggntex", "start": 3702755, "end": 3964915}, {"filename": "/assets/graphic/chara/dot/\u30a4\u30c1\u30ea\u30f3\u30ce\u30dfdot.png.ggntex", "start": 3964915, "end": 4227075}, {"filename": "/assets/graphic/chara/dot/\u30a6\u30c4\u30dbdot - \u30b3\u30d4\u30fc.png.ggntex", "start": 4227075, "end": 4751379}, {"filename": "/assets/graphic/chara/dot/\u30a6\u30c4\u30dbdot.png.ggntex", "start": 4751379, "end": 5275683}, {"filename": "/assets/graphic/chara/dot/\u30a6\u30c9\u30f3\u30b2dot.png.ggntex", "start": 5275683, "end": 5537843}, {"filename": "/assets/graphic/chara/dot/\u30a6\u30f3\u30b6\u30f3dot.png.ggntex", "start": 5537843, "end": 6062147}, {"filename": "/assets/graphic/chara/dot/\u30a8\u30a4\u30addot.png.ggntex", "start": 6062147, "end": 6324307}, {"filename": "/assets/graphic/chara/dot/\u30a8\u30a4\u30ea\u30f3dot.png.ggntex", "start": 6324307, "end": 6586467}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30a2\u30aadot.png.ggntex", "start": 6586467, "end": 7110771}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30a2\u30abdot.png.ggntex", "start": 7110771, "end": 7635075}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30ad\u30a4\u30eddot.png.ggntex", "start": 7635075, "end": 8159379}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30af\u30eddot.png.ggntex", "start": 8159379, "end": 8683683}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30b7\u30a2\u30f3dot.png.ggntex", "start": 8683683, "end": 9207987}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30c0\u30a4\u30c0\u30a4dot.png.ggntex", "start": 9207987, "end": 9732291}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30a2\u30aadot.png.ggntex", "start": 9732291, "end": 10256595}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30a2\u30abdot.png.ggntex", "start": 10256595, "end": 10780899}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30ad\u30a4\u30eddot.png.ggntex", "start": 10780899, "end": 11305203}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30af\u30eddot.png.ggntex", "start": 11305203, "end": 11829507}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30b7\u30a2\u30f3dot.png.ggntex", "start": 11829507, "end": 12353811}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30c0\u30a4\u30c0\u30a4dot.png.ggntex", "start": 12353811, "end": 12878115}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30de\u30bc\u30f3\u30bfdot.png.ggntex", "start": 12878115, "end": 13402419}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30d5\u30e9\u30c4\u30ad\u30df\u30c9\u30eadot.png.ggntex", "start": 13402419, "end": 13926723}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30de\u30bc\u30f3\u30bfdot.png.ggntex", "start": 13926723, "end": 14451027}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30c0\u30de\u30df\u30c9\u30eadot.png.ggntex", "start": 14451027, "end": 14975331}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30aa\u30ef\u30b7dot.png.ggntex", "start": 14975331, "end": 15499635}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30ea\u30f3dot.png.ggntex", "start": 15499635, "end": 16023939}, {"filename": "/assets/graphic/chara/dot/\u30aa\u30f3\u30ea\u30e7\u30a6dot.png.ggntex", "start": 16023939, "end": 16548243}, {"filename": "/assets/graphic/chara/dot/\u30ab\u30b0\u30e4dot.png.ggntex", "start": 16548243, "end": 16810403}, {"filename": "/assets/graphic/chara/dot/\u30ab\u30bb\u30f3dot.png.ggntex", "start": 16810403, "end": 17072563}, {"filename": "/assets/graphic/chara/dot/\u30ab\u30ca\u30b3dot.png.ggntex", "start": 17072563, "end": 17596867}, {"filename": "/assets/graphic/chara/dot/\u30ad\u30b9\u30e1dot.png.ggntex", "start": 17596867, "end": 18121171}, {"filename": "/assets/graphic/chara/dot/\u30ad\u30e2\u30b1\u30a4\u30cddot.png.ggntex", "start": 18121171, "end": 18383331}, {"filename": "/assets/graphic/chara/dot/\u30ad\u30e5\u30a6\u30a2\u30ea\u30b9dot.png.ggntex", "start": 18383331, "end": 18645491}, {"filename": "/assets/graphic/chara/dot/\u30b1\u30a4\u30cddot.png.ggntex", "start": 18645491, "end": 18907651}, {"filename": "/assets/graphic/chara/dot/\u30b3\u30a2\u30af\u30dedot.png.ggntex", "start": 18907651, "end": 19431955}, {"filename": "/assets/graphic/chara/dot/\u30b3\u30a4\u30b7dot.png.ggntex", "start": 19431955, "end": 19694115}, {"filename": "/assets/graphic/chara/dot/\u30b3\u30ac\u30b5dot.png.ggntex", "start": 19694115, "end": 20218419}, {"filename": "/assets/graphic/chara/dot/\u30b3\u30de\u30c1dot.png.ggntex", "start": 20218419, "end": 20742723}, {"filename": "/assets/graphic/chara/dot/\u30b5\u30af\u30e4dot.png.ggntex", "start": 20742723, "end": 21004883}, {"filename": "/assets/graphic/chara/dot/\u30b5\u30c8\u30eadot.png.ggntex", "start": 21004883, "end": 21267043}, {"filename": "/assets/graphic/chara/dot/\u30b5\u30ca\u30a8dot.png.ggntex", "start": 21267043, "end": 21529203}, {"filename": "/assets/graphic/chara/dot/\u30b5\u30cb\u30fc\u30df\u30eb\u30afdot.png.ggntex", "start": 21529203, "end": 22053507}, {"filename": "/assets/graphic/chara/dot/\u30b6\u30b3\u30a6\u30b5\u30aedot.png.ggntex", "start": 22053507, "end": 22315667}, {"filename": "/assets/graphic/chara/dot/\u30b6\u30b3\u30e8\u30a6\u30bb\u30a4dot.png.ggntex", "start": 22315667, "end": 22839971}, {"filename": "/assets/graphic/chara/dot/\u30b7\u30ba\u30cfdot.png.ggntex", "start": 22839971, "end": 23102131}, {"filename": "/assets/graphic/chara/dot/\u30b8\u30d0\u30af\u30cb\u30f3\u30ae\u30e7\u30a6dot.png.ggntex", "start": 23102131, "end": 23626435}, {"filename": "/assets/graphic/chara/dot/\u30b9\u30a4\u30abdot.png.ggntex", "start": 23626435, "end": 24150739}, {"filename": "/assets/graphic/chara/dot/\u30b9\u30bf\u30fc\u30b5\u30d5\u30a1\u30a4\u30a2dot.png.ggntex", "start": 24150739, "end": 24675043}, {"filename": "/assets/graphic/chara/dot/\u30b9\u30ef\u30b3dot.png.ggntex", "start": 24675043, "end": 24937203}, {"filename": "/assets/graphic/chara/dot/\u30be\u30f3\u30d3\u30d5\u30a7\u30a2\u30ea\u30fcdot.png.ggntex", "start": 24937203, "end": 25199363}, {"filename": "/assets/graphic/chara/dot/\u30c0\u30a4\u30e8\u30a6\u30bb\u30a4dot.png.ggntex", "start": 25199363, "end": 25723667}, {"filename": "/assets/graphic/chara/dot/\u30c1\u30a7\u30f3dot.png.ggntex", "start": 25723667, "end": 25985827}, {"filename": "/assets/graphic/chara/dot/\u30c1\u30eb\u30cedot.png.ggntex", "start": 25985827, "end": 26510131}, {"filename": "/assets/graphic/chara/dot/\u30c6\u30a4dot.png.ggntex", "start": 26510131, "end": 26772291}, {"filename": "/assets/graphic/chara/dot/\u30c6\u30f3\u30b7dot.png.ggntex", "start": 26772291, "end": 27034451}, {"filename": "/assets/graphic/chara/dot/\u30c8\u30ad\u30b3dot.png.ggntex", "start": 27034451, "end": 27558755}, {"filename": "/assets/graphic/chara/dot/\u30c8\u30e9\u30de\u30ebdot.png.ggntex", "start": 27558755, "end": 28083059}, {"filename": "/assets/graphic/chara/dot/\u30ca\u30ba\u30fc\u30ea\u30f3dot.png.ggntex", "start": 28083059, "end": 28607363}, {"filename": "/assets/graphic/chara/dot/\u30cb\u30c8\u30eadot.png.ggntex", "start": 28607363, "end": 28869523}, {"filename": "/assets/graphic/chara/dot/\u30cc\u30a8dot.png.ggntex", "start": 28869523, "end": 29393827}, {"filename": "/assets/graphic/chara/dot/\u30ce\u30e9\u30b1\u30c0\u30dedot.png.ggntex", "start": 29393827, "end": 29655987}, {"filename": "/assets/graphic/chara/dot/\u30ce\u30e9\u30b1\u30c0\u30de\u30a2\u30b7\u30e5dot.png.ggntex", "start": 29655987, "end": 29918147}, {"filename": "/assets/graphic/chara/dot/\u30cf\u30bf\u30c6dot.png.ggntex", "start": 29918147, "end": 30442451}, {"filename": "/assets/graphic/chara/dot/\u30d1\u30c1\u30e5\u30ea\u30fcdot.png.ggntex", "start": 30442451, "end": 30704611}, {"filename": "/assets/graphic/chara/dot/\u30d1\u30eb\u30b9\u30a3dot.png.ggntex", "start": 30704611, "end": 30966771}, {"filename": "/assets/graphic/chara/dot/\u30d2\u30b8\u30eadot.png.ggntex", "start": 30966771, "end": 31228931}, {"filename": "/assets/graphic/chara/dot/\u30d2\u30cadot.png.ggntex", "start": 31228931, "end": 31491091}, {"filename": "/assets/graphic/chara/dot/\u30d5\u30e9\u30f3\u30c9\u30fc\u30ebdot.png.ggntex", "start": 31491091, "end": 32015395}, {"filename": "/assets/graphic/chara/dot/\u30de\u30ea\u30b5dot.png.ggntex", "start": 32015395, "end": 32277555}, {"filename": "/assets/graphic/chara/dot/\u30df\u30ac\u30ef\u30ea\u30cb\u30f3\u30ae\u30e7\u30a6dot.png.ggntex", "start": 32277555, "end": 32539715}, {"filename": "/assets/graphic/chara/dot/\u30df\u30b3dot.png.ggntex", "start": 32539715, "end": 32801875}, {"filename": "/assets/graphic/chara/dot/\u30df\u30b9\u30c6\u30a3\u30a2dot.png.ggntex", "start": 32801875, "end": 33326179}, {"filename": "/assets/graphic/chara/dot/\u30df\u30ce\u30ea\u30b3dot.png.ggntex", "start": 33326179, "end": 33588339}, {"filename": "/assets/graphic/chara/dot/\u30e0\u30e9\u30b5dot.png.ggntex", "start": 33588339, "end": 33850499}, {"filename": "/assets/graphic/chara/dot/\u30e1\u30a4\u30c9\u30e8\u30a6\u30bb\u30a4dot.png.ggntex", "start": 33850499, "end": 34374803}, {"filename": "/assets/graphic/chara/dot/\u30e1\u30a4\u30ea\u30f3dot.png.ggntex", "start": 34374803, "end": 34636963}, {"filename": "/assets/graphic/chara/dot/\u30e1\u30c7\u30a3\u30b9\u30f3dot.png.ggntex", "start": 34636963, "end": 34899123}, {"filename": "/assets/graphic/chara/dot/\u30e1\u30eb\u30e9\u30f3dot.png.ggntex", "start": 34899123, "end": 35161283}, {"filename": "/assets/graphic/chara/dot/\u30e2\u30b3\u30a6dot.png.ggntex", "start": 35161283, "end": 35423443}, {"filename": "/assets/graphic/chara/dot/\u30e2\u30df\u30b8dot.png.ggntex", "start": 35423443, "end": 35685603}, {"filename": "/assets/graphic/chara/dot/\u30e4\u30de\u30e1dot.png.ggntex", "start": 35685603, "end": 35947763}, {"filename": "/assets/graphic/chara/dot/\u30e4\u30ea\u30b9\u30b4\u30b7dot.png.ggntex", "start": 35947763, "end": 36472067}, {"filename": "/assets/graphic/chara/dot/\u30e6\u30a6\u30abdot.png.ggntex", "start": 36472067, "end": 36996371}, {"filename": "/assets/graphic/chara/dot/\u30e6\u30a6\u30aedot.png.ggntex", "start": 36996371, "end": 37258531}, {"filename": "/assets/graphic/chara/dot/\u30e6\u30ab\u30eadot.png.ggntex", "start": 37258531, "end": 37782835}, {"filename": "/assets/graphic/chara/dot/\u30e6\u30e6\u30b3dot.png.ggntex", "start": 37782835, "end": 38044995}, {"filename": "/assets/graphic/chara/dot/\u30e8\u30a6\u30bb\u30a4\u30e1\u30a4\u30c9dot.png.ggntex", "start": 38044995, "end": 38307155}, {"filename": "/assets/graphic/chara/dot/\u30e8\u30a6\u30e0dot.png.ggntex", "start": 38307155, "end": 38831459}, {"filename": "/assets/graphic/chara/dot/\u30e8\u30a6\u30e0\u30cf\u30f3\u30ec\u30a4dot.png.ggntex", "start": 38831459, "end": 39093619}, {"filename": "/assets/graphic/chara/dot/\u30e8\u30a6\u30e0\u30cf\u30f3\u30ec\u30a4\u30ca\u30b7dot.png.ggntex", "start": 39093619, "end": 39617923}, {"filename": "/assets/graphic/chara/dot/\u30e8\u30a6\u30e0\u30cf\u30f3\u30ec\u30a4\u30d2\u30c8\u30ac\u30bfdot.png.ggntex", "start": 39617923, "end": 40142227}, {"filename": "/assets/graphic/chara/dot/\u30e9\u30a4\u30b8\u30e5\u30a6dot.png.ggntex", "start": 40142227, "end": 40666531}, {"filename": "/assets/graphic/chara/dot/\u30e9\u30f3dot.png.ggntex", "start": 40666531, "end": 40928691}, {"filename": "/assets/graphic/chara/dot/\u30ea\u30b0\u30ebdot.png.ggntex", "start": 40928691, "end": 41190851}, {"filename": "/assets/graphic/chara/dot/\u30ea\u30ea\u30abdot.png.ggntex", "start": 41190851, "end": 41453011}, {"filename": "/assets/graphic/chara/dot/\u30ea\u30ea\u30fcdot.png.ggntex", "start": 41453011, "end": 41977315}, {"filename": "/assets/graphic/chara/dot/\u30ea\u30f3\u30ce\u30b9\u30b1dot.png.ggntex", "start": 41977315, "end": 42239475}, {"filename": "/assets/graphic/chara/dot/\u30eb\u30ca\u30b5dot.png.ggntex", "start": 42239475, "end": 42501635}, {"filename": "/assets/graphic/chara/dot/\u30eb\u30ca\u30c1\u30e3\u30a4\u30eb\u30c9dot.png.ggntex", "start": 42501635, "end": 43025939}, {"filename": "/assets/graphic/chara/dot/\u30eb\u30fc\u30df\u30a2dot.png.ggntex", "start": 43025939, "end": 43288099}, {"filename": "/assets/graphic/chara/dot/\u30ec\u30a4\u30e0dot.png.ggntex", "start": 43288099, "end": 43550259}, {"filename": "/assets/graphic/chara/dot/\u30ec\u30c6\u30a3dot.png.ggntex", "start": 43550259, "end": 43812419}, {"filename": "/assets/graphic/chara/dot/\u30ec\u30df\u30ea\u30a2dot.png.ggntex", "start": 43812419, "end": 44336723}, {"filename": "/assets/graphic/chara/op/graphicpass.id", "start": 44336723, "end": 44337597}, {"filename": "/assets/graphic/chara/op/\u30a2\u30ad\u30e5\u30a6op.png.ggntex", "start": 44337597, "end": 44353997}, {"filename": "/assets/graphic/chara/op/\u30a2\u30e4op.png.ggntex", "start": 44353997, "end": 44370397}, {"filename": "/assets/graphic/chara/op/\u30a2\u30ea\u30b9op.png.ggntex", "start": 44370397, "end": 44386797}, {"filename": "/assets/graphic/chara/op/\u30a4\u30afop.png.ggntex", "start": 44386797, "end": 44403197}, {"filename": "/assets/graphic/chara/op/\u30a4\u30c1\u30ea\u30f3op.png.ggntex", "start": 44403197, "end": 44419597}, {"filename": "/assets/graphic/chara/op/\u30a4\u30c1\u30ea\u30f3\u30ce\u30dfop.png.ggntex", "start": 44419597, "end": 44435997}, {"filename": "/assets/graphic/chara/op/\u30a6\u30c4\u30dbop.png.ggntex", "start": 44435997, "end": 44452397}, {"filename": "/assets/graphic/chara/op/\u30a6\u30c9\u30f3\u30b2op.png.ggntex", "start": 44452397, "end": 44468797}, {"filename": "/assets/graphic/chara/op/\u30a6\u30f3\u30b6\u30f3op.png.ggntex", "start": 44468797, "end": 44485197}, {"filename": "/assets/graphic/chara/op/\u30a8\u30a4\u30adop.png.ggntex", "start": 44485197, "end": 44501597}, {"filename": "/assets/graphic/chara/op/\u30a8\u30a4\u30ea\u30f3op.png.ggntex", "start": 44501597, "end": 44517997}, {"filename": "/assets/graphic/chara/op/\u30aa\u30aa\u30ef\u30b7op.png.ggntex", "start": 44517997, "end": 44534397}, {"filename": "/assets/graphic/chara/op/\u30aa\u30ea\u30f3op.png.ggntex", "start": 44534397, "end": 44550797}, {"filename": "/assets/graphic/chara/op/\u30aa\u30f3\u30ea\u30e7\u30a6op.png.ggntex", "start": 44550797, "end": 44567197}, {"filename": "/assets/graphic/chara/op/\u30ab\u30b0\u30e4op.png.ggntex", "start": 44567197, "end": 44583597}, {"filename": "/assets/graphic/chara/op/\u30ab\u30bb\u30f3op.png.ggntex", "start": 44583597, "end": 44599997}, {"filename": "/assets/graphic/chara/op/\u30ab\u30ca\u30b3op.png.ggntex", "start": 44599997, "end": 44616397}, {"filename": "/assets/graphic/chara/op/\u30ad\u30b9\u30e1op.png.ggntex", "start": 44616397, "end": 44632797}, {"filename": "/assets/graphic/chara/op/\u30ad\u30e2\u30b1\u30a4\u30cdop.png.ggntex", "start": 44632797, "end": 44649197}, {"filename": "/assets/graphic/chara/op/\u30ad\u30e5\u30a6\u30a2\u30ea\u30b9op.png.ggntex", "start": 44649197, "end": 44665597}, {"filename": "/assets/graphic/chara/op/\u30b1\u30a4\u30cdop.png.ggntex", "start": 44665597, "end": 44681997}, {"filename": "/assets/graphic/chara/op/\u30b3\u30a2\u30af\u30deop.png.ggntex", "start": 44681997, "end": 44698397}, {"filename": "/assets/graphic/chara/op/\u30b3\u30a4\u30b7op.png.ggntex", "start": 44698397, "end": 44714797}, {"filename": "/assets/graphic/chara/op/\u30b3\u30ac\u30b5op.png.ggntex", "start": 44714797, "end": 44731197}, {"filename": "/assets/graphic/chara/op/\u30b3\u30de\u30c1op.png.ggntex", "start": 44731197, "end": 44747597}, {"filename": "/assets/graphic/chara/op/\u30b5\u30af\u30e4op.png.ggntex", "start": 44747597, "end": 44763997}, {"filename": "/assets/graphic/chara/op/\u30b5\u30c8\u30eaop.png.ggntex", "start": 44763997, "end": 44780397}, {"filename": "/assets/graphic/chara/op/\u30b5\u30ca\u30a8op.png.ggntex", "start": 44780397, "end": 44796797}, {"filename": "/assets/graphic/chara/op/\u30b5\u30cb\u30fc\u30df\u30eb\u30afop.png.ggntex", "start": 44796797, "end": 44813197}, {"filename": "/assets/graphic/chara/op/\u30b6\u30b3\u30a6\u30b5\u30aeop.png.ggntex", "start": 44813197, "end": 44829597}, {"filename": "/assets/graphic/chara/op/\u30b6\u30b3\u30e8\u30a6\u30bb\u30a4op.png.ggntex", "start": 44829597, "end": 44845997}, {"filename": "/assets/graphic/chara/op/\u30b7\u30ba\u30cfop.png.ggntex", "start": 44845997, "end": 44862397}, {"filename": "/assets/graphic/chara/op/\u30b8\u30d0\u30af\u30cb\u30f3\u30ae\u30e7\u30a6op.png.ggntex", "start": 44862397, "end": 44878797}, {"filename": "/assets/graphic/chara/op/\u30b9\u30a4\u30abop.png.ggntex", "start": 44878797, "end": 44895197}, {"filename": "/assets/graphic/chara/op/\u30b9\u30bf\u30fc\u30b5\u30d5\u30a1\u30a4\u30a2op.png.ggntex", "start": 44895197, "end": 44911597}, {"filename": "/assets/graphic/chara/op/\u30b9\u30ef\u30b3op.png.ggntex", "start": 44911597, "end": 44927997}, {"filename": "/assets/graphic/chara/op/\u30be\u30f3\u30d3\u30d5\u30a7\u30a2\u30ea\u30fcop.png.ggntex", "start": 44927997, "end": 44944397}, {"filename": "/assets/graphic/chara/op/\u30c0\u30a4\u30e8\u30a6\u30bb\u30a4op.png.ggntex", "start": 44944397, "end": 44960797}, {"filename": "/assets/graphic/chara/op/\u30c1\u30a7\u30f3op.png.ggntex", "start": 44960797, "end": 44977197}, {"filename": "/assets/graphic/chara/op/\u30c1\u30eb\u30ceop.png.ggntex", "start": 44977197, "end": 44993597}, {"filename": "/assets/graphic/chara/op/\u30c6\u30a4op.png.ggntex", "start": 44993597, "end": 45009997}, {"filename": "/assets/graphic/chara/op/\u30c6\u30f3\u30b7op.png.ggntex", "start": 45009997, "end": 45026397}, {"filename": "/assets/graphic/chara/op/\u30c8\u30ad\u30b3op.png.ggntex", "start": 45026397, "end": 45042797}, {"filename": "/assets/graphic/chara/op/\u30c8\u30e9\u30de\u30ebop.png.ggntex", "start": 45042797, "end": 45059197}, {"filename": "/assets/graphic/chara/op/\u30ca\u30ba\u30fc\u30ea\u30f3op.png.ggntex", "start": 45059197, "end": 45075597}, {"filename": "/assets/graphic/chara/op/\u30cb\u30c8\u30eaop.png.ggntex", "start": 45075597, "end": 45091997}, {"filename": "/assets/graphic/chara/op/\u30cc\u30a8op.png.ggntex", "start": 45091997, "end": 45108397}, {"filename": "/assets/graphic/chara/op/\u30ce\u30e9\u30b1\u30c0\u30deop.png.ggntex", "start": 45108397, "end": 45124797}, {"filename": "/assets/graphic/chara/op/\u30ce\u30e9\u30b1\u30c0\u30de\u30a2\u30b7\u30e5op.png.ggntex", "start": 45124797, "end": 45141197}, {"filename": "/assets/graphic/chara/op/\u30cf\u30bf\u30c6op.png.ggntex", "start": 45141197, "end": 45157597}, {"filename": "/assets/graphic/chara/op/\u30d1\u30c1\u30e5\u30ea\u30fcop.png.ggntex", "start": 45157597, "end": 45173997}, {"filename": "/assets/graphic/chara/op/\u30d1\u30eb\u30b9\u30a3op.png.ggntex", "start": 45173997, "end": 45190397}, {"filename": "/assets/graphic/chara/op/\u30d2\u30b8\u30eaop.png.ggntex", "start": 45190397, "end": 45206797}, {"filename": "/assets/graphic/chara/op/\u30d2\u30caop.png.ggntex", "start": 45206797, "end": 45223197}, {"filename": "/assets/graphic/chara/op/\u30d5\u30e9\u30f3op.png.ggntex", "start": 45223197, "end": 45239597}, {"filename": "/assets/graphic/chara/op/\u30d5\u30e9\u30f3\u30c9\u30fc\u30ebop.png.ggntex", "start": 45239597, "end": 45255997}, {"filename": "/assets/graphic/chara/op/\u30de\u30ea\u30b5op.png.ggntex", "start": 45255997, "end": 45272397}, {"filename": "/assets/graphic/chara/op/\u30df\u30b9\u30c6\u30a3\u30a2op.png.ggntex", "start": 45272397, "end": 45288797}, {"filename": "/assets/graphic/chara/op/\u30df\u30ce\u30ea\u30b3op.png.ggntex", "start": 45288797, "end": 45305197}, {"filename": "/assets/graphic/chara/op/\u30e0\u30e9\u30b5op.png.ggntex", "start": 45305197, "end": 45321597}, {"filename": "/assets/graphic/chara/op/\u30e1\u30a4\u30c9\u30e8\u30a6\u30bb\u30a4op.png.ggntex", "start": 45321597, "end": 45337997}, {"filename": "/assets/graphic/chara/op/\u30e1\u30a4\u30ea\u30f3op.png.ggntex", "start": 45337997, "end": 45354397}, {"filename": "/assets/graphic/chara/op/\u30e1\u30c7\u30a3\u30b9\u30f3op.png.ggntex", "start": 45354397, "end": 45370797}, {"filename": "/assets/graphic/chara/op/\u30e1\u30eb\u30e9\u30f3op.png.ggntex", "start": 45370797, "end": 45387197}, {"filename": "/assets/graphic/chara/op/\u30e2\u30b3\u30a6op.png.ggntex", "start": 45387197, "end": 45403597}, {"filename": "/assets/graphic/chara/op/\u30e2\u30df\u30b8op.png.ggntex", "start": 45403597, "end": 45419997}, {"filename": "/assets/graphic/chara/op/\u30e4\u30de\u30e1op.png.ggntex", "start": 45419997, "end": 45436397}, {"filename": "/assets/graphic/chara/op/\u30e6\u30a6\u30abop.png.ggntex", "start": 45436397, "end": 45452797}, {"filename": "/assets/graphic/chara/op/\u30e6\u30a6\u30aeop.png.ggntex", "start": 45452797, "end": 45469197}, {"filename": "/assets/graphic/chara/op/\u30e6\u30ab\u30eaop.png.ggntex", "start": 45469197, "end": 45485597}, {"filename": "/assets/graphic/chara/op/\u30e6\u30e6\u30b3op.png.ggntex", "start": 45485597, "end": 45501997}, {"filename": "/assets/graphic/chara/op/\u30e8\u30a6\u30bb\u30a4\u30e1\u30a4\u30c9op.png.ggntex", "start": 45501997, "end": 45506109}, {"filename": "/assets/graphic/chara/op/\u30e8\u30a6\u30e0op.png.ggntex", "start": 45506109, "end": 45522509}, {"filename": "/assets/graphic/chara/op/\u30e9\u30a4\u30b8\u30e5\u30a6op.png.ggntex", "start": 45522509, "end": 45538909}, {"filename": "/assets/graphic/chara/op/\u30e9\u30f3op.png.ggntex", "start": 45538909, "end": 45555309}, {"filename": "/assets/graphic/chara/op/\u30ea\u30b0\u30ebop.png.ggntex", "start": 45555309, "end": 45571709}, {"filename": "/assets/graphic/chara/op/\u30ea\u30ea\u30abop.png.ggntex", "start": 45571709, "end": 45588109}, {"filename": "/assets/graphic/chara/op/\u30ea\u30ea\u30fcop.png.ggntex", "start": 45588109, "end": 45604509}, {"filename": "/assets/graphic/chara/op/\u30ea\u30f3\u30ce\u30b9\u30b1op.png.ggntex", "start": 45604509, "end": 45620909}, {"filename": "/assets/graphic/chara/op/\u30eb\u30ca\u30b5op.png.ggntex", "start": 45620909, "end": 45637309}, {"filename": "/assets/graphic/chara/op/\u30eb\u30ca\u30c1\u30e3\u30a4\u30eb\u30c9op.png.ggntex", "start": 45637309, "end": 45653709}, {"filename": "/assets/graphic/chara/op/\u30eb\u30fc\u30df\u30a2op.png.ggntex", "start": 45653709, "end": 45670109}, {"filename": "/assets/graphic/chara/op/\u30ec\u30a4\u30e0op.png.ggntex", "start": 45670109, "end": 45686509}, {"filename": "/assets/graphic/chara/op/\u30ec\u30c6\u30a3op.png.ggntex", "start": 45686509, "end": 45702909}, {"filename": "/assets/graphic/chara/op/\u30ec\u30df\u30ea\u30a2op.png.ggntex", "start": 45702909, "end": 45719309}, {"filename": "/assets/graphic/effect/attack_hit.png.ggntex", "start": 45719309, "end": 45981469}, {"filename": "/assets/graphic/effect/attack_swing.png.ggntex", "start": 45981469, "end": 46243629}, {"filename": "/assets/graphic/effect/basic/ball_antismoke.png.ggntex", "start": 46243629, "end": 46505789}, {"filename": "/assets/graphic/effect/basic/ball_radio.png.ggntex", "start": 46505789, "end": 46767949}, {"filename": "/assets/graphic/effect/basic/ball_simple.png.ggntex", "start": 46767949, "end": 47030109}, {"filename": "/assets/graphic/effect/basic/ball_smoke.png.ggntex", "start": 47030109, "end": 47292269}, {"filename": "/assets/graphic/effect/basic/filelist.csv", "start": 47292269, "end": 47292598}, {"filename": "/assets/graphic/effect/basic/graphicpass.id", "start": 47292598, "end": 47293158}, {"filename": "/assets/graphic/effect/basic/ring_cut.png.ggntex", "start": 47293158, "end": 47555318}, {"filename": "/assets/graphic/effect/basic/ring_in.png.ggntex", "start": 47555318, "end": 47817478}, {"filename": "/assets/graphic/effect/basic/ring_nagi.png.ggntex", "start": 47817478, "end": 48079638}, {"filename": "/assets/graphic/effect/basic/ring_nagi2.png.ggntex", "start": 48079638, "end": 48341798}, {"filename": "/assets/graphic/effect/basic/ring_out.png.ggntex", "start": 48341798, "end": 48603958}, {"filename": "/assets/graphic/effect/basic/ring_radio.png.ggntex", "start": 48603958, "end": 48866118}, {"filename": "/assets/graphic/effect/basic/ring_smoke.png.ggntex", "start": 48866118, "end": 49128278}, {"filename": "/assets/graphic/effect/basic/ring_smooth.png.ggntex", "start": 49128278, "end": 49390438}, {"filename": "/assets/graphic/effect/basic/ring_wind.png.ggntex", "start": 49390438, "end": 49652598}, {"filename": "/assets/graphic/effect/basic/splash.png.ggntex", "start": 49652598, "end": 49914758}, {"filename": "/assets/graphic/effect/basic/square_gradation.png.ggntex", "start": 49914758, "end": 50176918}, {"filename": "/assets/graphic/effect/basic/star.png.ggntex", "start": 50176918, "end": 50439078}, {"filename": "/assets/graphic/effect/belts.png.ggntex", "start": 50439078, "end": 51487670}, {"filename": "/assets/graphic/effect/defense_swing.png.ggntex", "start": 51487670, "end": 51749830}, {"filename": "/assets/graphic/effect/elec.png.ggntex", "start": 51749830, "end": 52798422}, {"filename": "/assets/graphic/effect/equip/butterfly.png.ggntex", "start": 52798422, "end": 53060582}, {"filename": "/assets/graphic/effect/equip/filelist.csv", "start": 53060582, "end": 53060981}, {"filename": "/assets/graphic/effect/equip/graphicpass.id", "start": 53060981, "end": 53061669}, {"filename": "/assets/graphic/effect/equip/haegrid.png.ggntex", "start": 53061669, "end": 53323829}, {"filename": "/assets/graphic/effect/equip/hina.png.ggntex", "start": 53323829, "end": 53585989}, {"filename": "/assets/graphic/effect/equip/ikari.png.ggntex", "start": 53585989, "end": 53651541}, {"filename": "/assets/graphic/effect/equip/kaname.png.ggntex", "start": 53651541, "end": 53667941}, {"filename": "/assets/graphic/effect/equip/kinsi.png.ggntex", "start": 53667941, "end": 53684341}, {"filename": "/assets/graphic/effect/equip/mikatuki.png.ggntex", "start": 53684341, "end": 53946501}, {"filename": "/assets/graphic/effect/equip/nagekama.png.ggntex", "start": 53946501, "end": 54995093}, {"filename": "/assets/graphic/effect/equip/onmyoball.png.ggntex", "start": 54995093, "end": 55257253}, {"filename": "/assets/graphic/effect/equip/particle_dero.png.ggntex", "start": 55257253, "end": 55519413}, {"filename": "/assets/graphic/effect/equip/particle_mizu.png.ggntex", "start": 55519413, "end": 55781573}, {"filename": "/assets/graphic/effect/equip/particle_momizi.png.ggntex", "start": 55781573, "end": 56043733}, {"filename": "/assets/graphic/effect/equip/particle_paper.png.ggntex", "start": 56043733, "end": 56305893}, {"filename": "/assets/graphic/effect/equip/particle_sakura.png.ggntex", "start": 56305893, "end": 56568053}, {"filename": "/assets/graphic/effect/equip/sword_beam01.png.ggntex", "start": 56568053, "end": 56699141}, {"filename": "/assets/graphic/effect/equip/sword_swing.png.ggntex", "start": 56699141, "end": 56764693}, {"filename": "/assets/graphic/effect/equip/sword_swing02.png.ggntex", "start": 56764693, "end": 56781093}, {"filename": "/assets/graphic/effect/equip/sword_swing2.png.ggntex", "start": 56781093, "end": 56846645}, {"filename": "/assets/graphic/effect/equip/sword_swing_blue2.png.ggntex", "start": 56846645, "end": 56912197}, {"filename": "/assets/graphic/effect/equip/wind_green.png.ggntex", "start": 56912197, "end": 56977749}, {"filename": "/assets/graphic/effect/fire/firetex1mini.png.ggntex", "start": 56977749, "end": 57239909}, {"filename": "/assets/graphic/effect/fire/firetex2mini.png.ggntex", "start": 57239909, "end": 57502069}, {"filename": "/assets/graphic/effect/fire/firetex3mini.png.ggntex", "start": 57502069, "end": 57764229}, {"filename": "/assets/graphic/effect/fire/firetex4mini.png.ggntex", "start": 57764229, "end": 58026389}, {"filename": "/assets/graphic/effect/fire/graphicpass.id", "start": 58026389, "end": 58026531}, {"filename": "/assets/graphic/effect/fire_chip.png.ggntex", "start": 58026531, "end": 58288691}, {"filename": "/assets/graphic/effect/graphicpass.id", "start": 58288691, "end": 58289229}, {"filename": "/assets/graphic/effect/ice.png.ggntex", "start": 58289229, "end": 59337821}, {"filename": "/assets/graphic/effect/magiccircle.png.ggntex", "start": 59337821, "end": 60386413}, {"filename": "/assets/graphic/effect/meisouzan.png.ggntex", "start": 60386413, "end": 61435005}, {"filename": "/assets/graphic/effect/mob/kobusi.png.ggntex", "start": 61435005, "end": 61719693}, {"filename": "/assets/graphic/effect/mob/shutter.png.ggntex", "start": 61719693, "end": 61981853}, {"filename": "/assets/graphic/effect/mob/yarihead.png.ggntex", "start": 61981853, "end": 62244013}, {"filename": "/assets/graphic/effect/spell/muzai.png.ggntex", "start": 62244013, "end": 62506173}, {"filename": "/assets/graphic/effect/spell/yuuzai.png.ggntex", "start": 62506173, "end": 62768333}, {"filename": "/assets/graphic/effect/spellcard.png.ggntex", "start": 62768333, "end": 62784733}, {"filename": "/assets/graphic/effect/sukima.png.ggntex", "start": 62784733, "end": 62850285}, {"filename": "/assets/graphic/effect/trap/bongo_a.png.ggntex", "start": 62850285, "end": 63112445}, {"filename": "/assets/graphic/effect/trap/ninoasi.png.ggntex", "start": 63112445, "end": 63374605}, {"filename": "/assets/graphic/effect/trap/shield_icon.png.ggntex", "start": 63374605, "end": 63636765}, {"filename": "/assets/graphic/effect/trap/sword_icon.png.ggntex", "start": 63636765, "end": 63898925}, {"filename": "/assets/graphic/effect/trap/trap_model.png.ggntex", "start": 63898925, "end": 64161085}, {"filename": "/assets/graphic/effect/trap/turu1.png.ggntex", "start": 64161085, "end": 64423245}, {"filename": "/assets/graphic/effect/trap/turu2.png.ggntex", "start": 64423245, "end": 64685405}, {"filename": "/assets/graphic/effect/trap/yaku.png.ggntex", "start": 64685405, "end": 64947565}, {"filename": "/assets/graphic/effect/trap/yen.png.ggntex", "start": 64947565, "end": 65209725}, {"filename": "/assets/graphic/effect/water.png.ggntex", "start": 65209725, "end": 66258317}, {"filename": "/assets/graphic/effect/\u3064\u308b\u3079\u304a\u3068\u3057\u30fb\u304d\u3059\u3081.png.ggntex", "start": 66258317, "end": 66277789}, {"filename": "/assets/graphic/effect/\u3064\u308b\u3079\u304a\u3068\u3057\u30fb\u305d\u306e\u305f.png.ggntex", "start": 66277789, "end": 66297261}, {"filename": "/assets/graphic/event/black_back.png.ggntex", "start": 66297261, "end": 67345853}, {"filename": "/assets/graphic/event/hakurei_back.png.ggntex", "start": 67345853, "end": 68394445}, {"filename": "/assets/graphic/event/staff/balanceTyousei.png.ggntex", "start": 68394445, "end": 69443037}, {"filename": "/assets/graphic/event/staff/bossBattleDesign.png.ggntex", "start": 69443037, "end": 70491629}, {"filename": "/assets/graphic/event/staff/dungeonDesign.png.ggntex", "start": 70491629, "end": 71540221}, {"filename": "/assets/graphic/event/staff/effectDesign.png.ggntex", "start": 71540221, "end": 72588813}, {"filename": "/assets/graphic/event/staff/ensyutu.png.ggntex", "start": 72588813, "end": 73637405}, {"filename": "/assets/graphic/event/staff/gameDesign.png.ggntex", "start": 73637405, "end": 74685997}, {"filename": "/assets/graphic/event/staff/gensaku.png.ggntex", "start": 74685997, "end": 75734589}, {"filename": "/assets/graphic/event/staff/graphic.png.ggntex", "start": 75734589, "end": 76783181}, {"filename": "/assets/graphic/event/staff/graphicGaibuStaff.png.ggntex", "start": 76783181, "end": 77831773}, {"filename": "/assets/graphic/event/staff/kikaku.png.ggntex", "start": 77831773, "end": 78880365}, {"filename": "/assets/graphic/event/staff/koukaon.png.ggntex", "start": 78880365, "end": 79928957}, {"filename": "/assets/graphic/event/staff/ongaku.png.ggntex", "start": 79928957, "end": 80977549}, {"filename": "/assets/graphic/event/staff/owari.png.ggntex", "start": 80977549, "end": 82026141}, {"filename": "/assets/graphic/event/staff/packageDesign.png.ggntex", "start": 82026141, "end": 83074733}, {"filename": "/assets/graphic/event/staff/program.png.ggntex", "start": 83074733, "end": 84123325}, {"filename": "/assets/graphic/event/staff/programKyouryoku.png.ggntex", "start": 84123325, "end": 85171917}, {"filename": "/assets/graphic/event/staff/saisaku.png.ggntex", "start": 85171917, "end": 86220509}, {"filename": "/assets/graphic/event/staff/scenario.png.ggntex", "start": 86220509, "end": 87269101}, {"filename": "/assets/graphic/event/staff/scenarioGenan.png.ggntex", "start": 87269101, "end": 88317693}, {"filename": "/assets/graphic/graphicpass.id", "start": 88317693, "end": 88318143}, {"filename": "/assets/graphic/graphicpassRaw4.id", "start": 88318143, "end": 88327689}, {"filename": "/assets/graphic/interface/SpellStar.png.ggntex", "start": 88327689, "end": 88328729}, {"filename": "/assets/graphic/interface/aspectArrows.png.ggntex", "start": 88328729, "end": 88394281}, {"filename": "/assets/graphic/interface/bartex.png.ggntex", "start": 88394281, "end": 88459833}, {"filename": "/assets/graphic/interface/frame1.png.ggntex", "start": 88459833, "end": 90557001}, {"filename": "/assets/graphic/interface/frame1_.png.ggntex", "start": 90557001, "end": 92477017}, {"filename": "/assets/graphic/interface/frame_default.png.ggntex", "start": 92477017, "end": 94397033}, {"filename": "/assets/graphic/interface/graphicpass.id", "start": 94397033, "end": 94397481}, {"filename": "/assets/graphic/interface/grid.png.ggntex", "start": 94397481, "end": 94413881}, {"filename": "/assets/graphic/interface/grid2.png.ggntex", "start": 94413881, "end": 94430281}, {"filename": "/assets/graphic/interface/grid3.png.ggntex", "start": 94430281, "end": 94446681}, {"filename": "/assets/graphic/interface/grid4.png.ggntex", "start": 94446681, "end": 94463081}, {"filename": "/assets/graphic/interface/loading.png.ggntex", "start": 94463081, "end": 94528633}, {"filename": "/assets/graphic/interface/multiselect.png.ggntex", "start": 94528633, "end": 94545033}, {"filename": "/assets/graphic/interface/nowloadingnow.png.ggntex", "start": 94545033, "end": 94610585}, {"filename": "/assets/graphic/interface/num.png.ggntex", "start": 94610585, "end": 95134889}, {"filename": "/assets/graphic/interface/title.png.ggntex", "start": 95134889, "end": 97054905}, {"filename": "/assets/graphic/interface/titleExplanation.png.ggntex", "start": 97054905, "end": 97185993}, {"filename": "/assets/graphic/interface/window2.png.ggntex", "start": 97185993, "end": 97251545}, {"filename": "/assets/graphic/interface/window4.png.ggntex", "start": 97251545, "end": 97317097}, {"filename": "/assets/graphic/interface/\u30bf\u30a4\u30c8\u30eb\u7d20\u6750.png.ggntex", "start": 97317097, "end": 98365689}, {"filename": "/assets/graphic/item/EquipIcon01.png.ggntex", "start": 98365689, "end": 98431241}, {"filename": "/assets/graphic/item/EquipIcon02.png.ggntex", "start": 98431241, "end": 98496793}, {"filename": "/assets/graphic/item/EquipIcon03.png.ggntex", "start": 98496793, "end": 98562345}, {"filename": "/assets/graphic/item/EquipIcon04.png.ggntex", "start": 98562345, "end": 98627897}, {"filename": "/assets/graphic/item/EquipIcon05.png.ggntex", "start": 98627897, "end": 98693449}, {"filename": "/assets/graphic/item/effectdrop.png.ggntex", "start": 98693449, "end": 98955609}, {"filename": "/assets/graphic/item/graphicpass.id", "start": 98955609, "end": 98955877}, {"filename": "/assets/graphic/item/itemstateicon.png.ggntex", "start": 98955877, "end": 98972277}, {"filename": "/assets/graphic/item/potato.png.ggntex", "start": 98972277, "end": 99037829}, {"filename": "/assets/graphic/item/\u304a\u91d1.png.ggntex", "start": 99037829, "end": 99103381}, {"filename": "/assets/graphic/item/\u3072\u3087\u3046\u305f\u3093.png.ggntex", "start": 99103381, "end": 99168933}, {"filename": "/assets/graphic/item/\u30a2\u30a4\u30c6\u30e0\u30c6\u30f3\u30d7\u30ec\u30fc\u30c8\uff11.png.ggntex", "start": 99168933, "end": 99234485}, {"filename": "/assets/graphic/item/\u30b7\u30b9\u30c6\u30e0\u30a2\u30a4\u30b3\u30f3.png.ggntex", "start": 99234485, "end": 99300037}, {"filename": "/assets/graphic/item/\u51fa\u5165\u53e3.png.ggntex", "start": 99300037, "end": 99365589}, {"filename": "/assets/graphic/item/\u56de\u5fa9\u30a2\u30a4\u30c6\u30e0.png.ggntex", "start": 99365589, "end": 99431141}, {"filename": "/assets/graphic/item/\u672c.png.ggntex", "start": 99431141, "end": 99496693}, {"filename": "/assets/graphic/item/\u6b66\u5668.png.ggntex", "start": 99496693, "end": 99562245}, {"filename": "/assets/graphic/item/\u7bb1.png.ggntex", "start": 99562245, "end": 99627797}, {"filename": "/assets/graphic/item/\u7f60\uff11.png.ggntex", "start": 99627797, "end": 99693349}, {"filename": "/assets/graphic/item/\u7f60\uff12.png.ggntex", "start": 99693349, "end": 99758901}, {"filename": "/assets/graphic/item/\u7f60\uff13.png.ggntex", "start": 99758901, "end": 99824453}, {"filename": "/assets/graphic/item/\u968e\u6bb5\u30bb\u30c3\u30c8.png.ggntex", "start": 99824453, "end": 99890005}, {"filename": "/assets/graphic/map/dark.png.ggntex", "start": 99890005, "end": 100414309}, {"filename": "/assets/graphic/map/dark2.png.ggntex", "start": 100414309, "end": 100938613}, {"filename": "/assets/graphic/map/file0296.png.ggntex", "start": 100938613, "end": 101462917}, {"filename": "/assets/graphic/map/graphicpass.id", "start": 101462917, "end": 101463459}, {"filename": "/assets/graphic/map/mapchip1.png.ggntex", "start": 101463459, "end": 101987763}, {"filename": "/assets/graphic/map/miniMapLazer.png.ggntex", "start": 101987763, "end": 101991875}, {"filename": "/assets/graphic/map/minimapchip.png.ggntex", "start": 101991875, "end": 102000083}, {"filename": "/assets/graphic/map/minimapchip2.png.ggntex", "start": 102000083, "end": 102008291}, {"filename": "/assets/graphic/map/\u51a5\u754c\u30bb\u30c3\u30c8.png.ggntex", "start": 102008291, "end": 103056883}, {"filename": "/assets/graphic/map/\u51a5\u754c\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 103056883, "end": 104105475}, {"filename": "/assets/graphic/map/\u56f3\u66f8\u9928_mapchip1.png.ggntex", "start": 104105475, "end": 104629779}, {"filename": "/assets/graphic/map/\u56f3\u66f8\u9928\u30bb\u30c3\u30c8.png.ggntex", "start": 104629779, "end": 105678371}, {"filename": "/assets/graphic/map/\u56f3\u66f8\u9928\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 105678371, "end": 106726963}, {"filename": "/assets/graphic/map/\u5730\u970a\u6bbf\u30bb\u30c3\u30c8.png.ggntex", "start": 106726963, "end": 107775555}, {"filename": "/assets/graphic/map/\u5730\u970a\u6bbf\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 107775555, "end": 108824147}, {"filename": "/assets/graphic/map/\u5996\u602a\u306e\u5c71\u5730\u4e0b\u30bb\u30c3\u30c8.png.ggntex", "start": 108824147, "end": 109872739}, {"filename": "/assets/graphic/map/\u5996\u602a\u306e\u5c71\u5730\u4e0b\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 109872739, "end": 110921331}, {"filename": "/assets/graphic/map/\u5b9d\u8239\u30bb\u30c3\u30c8.png.ggntex", "start": 110921331, "end": 111969923}, {"filename": "/assets/graphic/map/\u5b9d\u8239\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 111969923, "end": 113018515}, {"filename": "/assets/graphic/map/\u6c38\u9060\u4ead\u30bb\u30c3\u30c8.png.ggntex", "start": 113018515, "end": 114067107}, {"filename": "/assets/graphic/map/\u6c38\u9060\u4ead\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 114067107, "end": 115115699}, {"filename": "/assets/graphic/map/\u707c\u71b1\u5730\u7344\u30bb\u30c3\u30c8.png.ggntex", "start": 115115699, "end": 116164291}, {"filename": "/assets/graphic/map/\u707c\u71b1\u5730\u7344\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 116164291, "end": 117212883}, {"filename": "/assets/graphic/map/\u767d\u7389\u697c\u30bb\u30c3\u30c8.png.ggntex", "start": 117212883, "end": 118261475}, {"filename": "/assets/graphic/map/\u767d\u7389\u697c\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 118261475, "end": 119310067}, {"filename": "/assets/graphic/map/\u7af9\u6797\u30bb\u30c3\u30c8.png.ggntex", "start": 119310067, "end": 120358659}, {"filename": "/assets/graphic/map/\u7af9\u6797\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 120358659, "end": 121407251}, {"filename": "/assets/graphic/map/\u7d05\u9b54\u9928\u30bb\u30c3\u30c8 - \u30b3\u30d4\u30fc.png.ggntex", "start": 121407251, "end": 121931555}, {"filename": "/assets/graphic/map/\u7d05\u9b54\u9928\u30bb\u30c3\u30c8.png.ggntex", "start": 121931555, "end": 122980147}, {"filename": "/assets/graphic/map/\u7d05\u9b54\u9928\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 122980147, "end": 124028739}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u306e\u4fee\u884c\u5834_mapchip1.png.ggntex", "start": 124028739, "end": 124553043}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u306e\u4fee\u884c\u5834\u30bb\u30c3\u30c8.png.ggntex", "start": 124553043, "end": 125601635}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u306e\u4fee\u884c\u5834\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 125601635, "end": 126650227}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u8a70\u3081\u6240.png.ggntex", "start": 126650227, "end": 127174531}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u8a70\u3081\u6240_mapchip1.png.ggntex", "start": 127174531, "end": 127698835}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u8a70\u3081\u6240\u30bb\u30c3\u30c8.png.ggntex", "start": 127698835, "end": 128747427}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u8a70\u3081\u6240\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 128747427, "end": 129796019}, {"filename": "/assets/graphic/map/\u7f8e\u9234\u8a70\u3081\u6240\u30bb\u30c3\u30c8DecoCustom.png.ggntex", "start": 129796019, "end": 130844611}, {"filename": "/assets/graphic/map/\u9b54\u754c\u30bb\u30c3\u30c8.png.ggntex", "start": 130844611, "end": 131893203}, {"filename": "/assets/graphic/map/\u9b54\u754c\u30bb\u30c3\u30c8Deco.png.ggntex", "start": 131893203, "end": 132941795}, {"filename": "/assets/graphic/skin/cursor.png.ggntex", "start": 132941795, "end": 132945907}, {"filename": "/assets/graphic/skin/page.png.ggntex", "start": 132945907, "end": 132950019}, {"filename": "/assets/graphic/skin/wait.png.ggntex", "start": 132950019, "end": 132954131}, {"filename": "/assets/graphic/skin/window.png.ggntex", "start": 132954131, "end": 133019683}, {"filename": "/assets/graphic/skin\u306b\u3064\u3044\u3066.txt", "start": 133019683, "end": 133021053}, {"filename": "/data/Boss/boss_header.id", "start": 133021053, "end": 133021487}, {"filename": "/data/Boss/kanako/Kanako.csv", "start": 133021487, "end": 133022041}, {"filename": "/data/Boss/kanako/Kanako.id", "start": 133022041, "end": 133023155}, {"filename": "/data/Boss/kanako/KanakoFloor.id", "start": 133023155, "end": 133027517}, {"filename": "/data/Boss/kanako/KanakoSpell.lua", "start": 133027517, "end": 133027929}, {"filename": "/data/Boss/kanako/Kanakomap.csv", "start": 133027929, "end": 133038025}, {"filename": "/data/Boss/kanako/Remilia - \u30b3\u30d4\u30fc.id", "start": 133038025, "end": 133039559}, {"filename": "/data/Boss/kanako/TewiSpell.lua", "start": 133039559, "end": 133039971}, {"filename": "/data/Boss/kanako/spell/AncientDuper.lua", "start": 133039971, "end": 133054939}, {"filename": "/data/Boss/kanako/spell/AttemptEscape.lua", "start": 133054939, "end": 133060386}, {"filename": "/data/Boss/kanako/spell/MedicineOfOnamuti.lua", "start": 133060386, "end": 133069593}, {"filename": "/data/Boss/kanako/spell/MountainOfFaith.lua", "start": 133069593, "end": 133075833}, {"filename": "/data/Boss/kanako/spell/OkuninushiComplex.lua", "start": 133075833, "end": 133086153}, {"filename": "/data/Boss/kanako/spell/expandedOnbashira.lua", "start": 133086153, "end": 133093598}, {"filename": "/data/Boss/kanakoEx/Kanako.csv", "start": 133093598, "end": 133094270}, {"filename": "/data/Boss/kanakoEx/Kanako.id", "start": 133094270, "end": 133095384}, {"filename": "/data/Boss/kanakoEx/KanakoFloor.id", "start": 133095384, "end": 133099746}, {"filename": "/data/Boss/kanakoEx/KanakoSpell.lua", "start": 133099746, "end": 133100158}, {"filename": "/data/Boss/kanakoEx/Kanakomap.csv", "start": 133100158, "end": 133110254}, {"filename": "/data/Boss/kanakoEx/spell/MountainOfFaith.lua", "start": 133110254, "end": 133116658}, {"filename": "/data/Boss/kanakoEx/spell/OkuninushiComplex.lua", "start": 133116658, "end": 133127314}, {"filename": "/data/Boss/kanakoEx/spell/expandedOnbashira.lua", "start": 133127314, "end": 133135027}, {"filename": "/data/Boss/kyuualice/Kyuualice.csv", "start": 133135027, "end": 133135769}, {"filename": "/data/Boss/kyuualice/Kyuualice.id", "start": 133135769, "end": 133136949}, {"filename": "/data/Boss/kyuualice/KyuualiceFloor.id", "start": 133136949, "end": 133141369}, {"filename": "/data/Boss/kyuualice/KyuualiceSpell.lua", "start": 133141369, "end": 133141872}, {"filename": "/data/Boss/kyuualice/Kyuualicemap.csv", "start": 133141872, "end": 133151968}, {"filename": "/data/Boss/kyuualice/Remilia - \u30b3\u30d4\u30fc.id", "start": 133151968, "end": 133153502}, {"filename": "/data/Boss/kyuualice/spell/AttemptEscape.lua", "start": 133153502, "end": 133158949}, {"filename": "/data/Boss/kyuualice/spell/MedicineOfOnamuti.lua", "start": 133158949, "end": 133168156}, {"filename": "/data/Boss/kyuualice/spell/NightmareAllOfWorld.lua", "start": 133168156, "end": 133175717}, {"filename": "/data/Boss/kyuualice/spell/SpellA.lua", "start": 133175717, "end": 133183237}, {"filename": "/data/Boss/kyuualice/spell/spellC.lua", "start": 133183237, "end": 133195045}, {"filename": "/data/Boss/kyuualice/spell/spellC2.lua", "start": 133195045, "end": 133213062}, {"filename": "/data/Boss/kyuualice/spell/spellD.lua", "start": 133213062, "end": 133225939}, {"filename": "/data/Boss/kyuualice/spell/spellE.lua", "start": 133225939, "end": 133249178}, {"filename": "/data/Boss/kyuualiceEx/Kyuualice.csv", "start": 133249178, "end": 133250082}, {"filename": "/data/Boss/kyuualiceEx/Kyuualice.id", "start": 133250082, "end": 133251262}, {"filename": "/data/Boss/kyuualiceEx/KyuualiceFloor.id", "start": 133251262, "end": 133255682}, {"filename": "/data/Boss/kyuualiceEx/KyuualiceSpell.lua", "start": 133255682, "end": 133256185}, {"filename": "/data/Boss/kyuualiceEx/Kyuualicemap.csv", "start": 133256185, "end": 133266281}, {"filename": "/data/Boss/kyuualiceEx/spell/AttemptEscape.lua", "start": 133266281, "end": 133271728}, {"filename": "/data/Boss/kyuualiceEx/spell/MedicineOfOnamuti.lua", "start": 133271728, "end": 133280935}, {"filename": "/data/Boss/kyuualiceEx/spell/NightmareAllOfWorld.lua", "start": 133280935, "end": 133288496}, {"filename": "/data/Boss/kyuualiceEx/spell/SpellA.lua", "start": 133288496, "end": 133296344}, {"filename": "/data/Boss/kyuualiceEx/spell/spellC.lua", "start": 133296344, "end": 133308625}, {"filename": "/data/Boss/kyuualiceEx/spell/spellC2.lua", "start": 133308625, "end": 133326821}, {"filename": "/data/Boss/kyuualiceEx/spell/spellD.lua", "start": 133326821, "end": 133340078}, {"filename": "/data/Boss/kyuualiceEx/spell/spellE.lua", "start": 133340078, "end": 133363317}, {"filename": "/data/Boss/remilia/Remilia.csv", "start": 133363317, "end": 133363949}, {"filename": "/data/Boss/remilia/Remilia.id", "start": 133363949, "end": 133365679}, {"filename": "/data/Boss/remilia/RemiliaFloor.id", "start": 133365679, "end": 133369971}, {"filename": "/data/Boss/remilia/RemiliaSpell.lua", "start": 133369971, "end": 133374832}, {"filename": "/data/Boss/remilia/Remiliamap.csv", "start": 133374832, "end": 133384928}, {"filename": "/data/Boss/remilia/spell/CurseOfVlad - \u30b3\u30d4\u30fc.lua", "start": 133384928, "end": 133391355}, {"filename": "/data/Boss/remilia/spell/CurseOfVlad.lua", "start": 133391355, "end": 133397957}, {"filename": "/data/Boss/remilia/spell/FiveOfAKind.lua", "start": 133397957, "end": 133404315}, {"filename": "/data/Boss/remilia/spell/NightmareAllOfWorld.lua", "start": 133404315, "end": 133411876}, {"filename": "/data/Boss/remiliaEx/Remilia.csv", "start": 133411876, "end": 133412634}, {"filename": "/data/Boss/remiliaEx/Remilia.id", "start": 133412634, "end": 133414372}, {"filename": "/data/Boss/remiliaEx/RemiliaFloor.id", "start": 133414372, "end": 133418664}, {"filename": "/data/Boss/remiliaEx/RemiliaSpell.lua", "start": 133418664, "end": 133423525}, {"filename": "/data/Boss/remiliaEx/Remiliamap.csv", "start": 133423525, "end": 133433621}, {"filename": "/data/Boss/remiliaEx/spell/CurseOfVlad - \u30b3\u30d4\u30fc.lua", "start": 133433621, "end": 133440048}, {"filename": "/data/Boss/remiliaEx/spell/CurseOfVlad.lua", "start": 133440048, "end": 133446823}, {"filename": "/data/Boss/remiliaEx/spell/FiveOfAKind.lua", "start": 133446823, "end": 133453362}, {"filename": "/data/Boss/remiliaEx/spell/NightmareAllOfWorld.lua", "start": 133453362, "end": 133461274}, {"filename": "/data/Boss/tewi/Remilia - \u30b3\u30d4\u30fc.id", "start": 133461274, "end": 133462992}, {"filename": "/data/Boss/tewi/Tewi.csv", "start": 133462992, "end": 133463538}, {"filename": "/data/Boss/tewi/Tewi.id", "start": 133463538, "end": 133464620}, {"filename": "/data/Boss/tewi/TewiFloor.id", "start": 133464620, "end": 133469074}, {"filename": "/data/Boss/tewi/TewiSpell.lua", "start": 133469074, "end": 133469486}, {"filename": "/data/Boss/tewi/Tewimap.csv", "start": 133469486, "end": 133479582}, {"filename": "/data/Boss/tewi/spell/AncientDuper - \u30b3\u30d4\u30fc.lua", "start": 133479582, "end": 133494550}, {"filename": "/data/Boss/tewi/spell/AncientDuper.lua", "start": 133494550, "end": 133510267}, {"filename": "/data/Boss/tewi/spell/AttemptEscape.lua", "start": 133510267, "end": 133515643}, {"filename": "/data/Boss/tewi/spell/MedicineOfOnamuti.lua", "start": 133515643, "end": 133525144}, {"filename": "/data/Boss/tewiEx/Remilia - \u30b3\u30d4\u30fc.id", "start": 133525144, "end": 133526862}, {"filename": "/data/Boss/tewiEx/Tewi.csv", "start": 133526862, "end": 133527510}, {"filename": "/data/Boss/tewiEx/Tewi.id", "start": 133527510, "end": 133528592}, {"filename": "/data/Boss/tewiEx/TewiFloor.id", "start": 133528592, "end": 133533046}, {"filename": "/data/Boss/tewiEx/TewiSpell.lua", "start": 133533046, "end": 133533458}, {"filename": "/data/Boss/tewiEx/Tewimap.csv", "start": 133533458, "end": 133543554}, {"filename": "/data/Boss/tewiEx/spell/AncientDuper - \u30b3\u30d4\u30fc.lua", "start": 133543554, "end": 133558522}, {"filename": "/data/Boss/tewiEx/spell/AncientDuper.lua", "start": 133558522, "end": 133574391}, {"filename": "/data/Boss/tewiEx/spell/AttemptEscape.lua", "start": 133574391, "end": 133579926}, {"filename": "/data/Boss/tewiEx/spell/MedicineOfOnamuti.lua", "start": 133579926, "end": 133589586}, {"filename": "/data/Boss/youmu/Remilia - \u30b3\u30d4\u30fc.id", "start": 133589586, "end": 133591120}, {"filename": "/data/Boss/youmu/YoumuFloor.id", "start": 133591120, "end": 133595534}, {"filename": "/data/Boss/youmu/YoumuSpell.lua", "start": 133595534, "end": 133595946}, {"filename": "/data/Boss/youmu/Youmumap.csv", "start": 133595946, "end": 133606042}, {"filename": "/data/Boss/youmu/spell/Gensezan.lua", "start": 133606042, "end": 133611035}, {"filename": "/data/Boss/youmu/spell/Meisouzan.lua", "start": 133611035, "end": 133621599}, {"filename": "/data/Boss/youmu/spell/YumeinoKurin.lua", "start": 133621599, "end": 133626771}, {"filename": "/data/Boss/youmu/spell/subKurin.lua", "start": 133626771, "end": 133630192}, {"filename": "/data/Boss/youmu/youmu.csv", "start": 133630192, "end": 133630894}, {"filename": "/data/Boss/youmu/youmu.id", "start": 133630894, "end": 133632932}, {"filename": "/data/Boss/youmuEx/YoumuFloor.id", "start": 133632932, "end": 133637346}, {"filename": "/data/Boss/youmuEx/YoumuSpell.lua", "start": 133637346, "end": 133637758}, {"filename": "/data/Boss/youmuEx/Youmumap.csv", "start": 133637758, "end": 133647854}, {"filename": "/data/Boss/youmuEx/spell/Gensezan.lua", "start": 133647854, "end": 133653096}, {"filename": "/data/Boss/youmuEx/spell/Meisouzan.lua", "start": 133653096, "end": 133664014}, {"filename": "/data/Boss/youmuEx/spell/YumeinoKurin.lua", "start": 133664014, "end": 133669639}, {"filename": "/data/Boss/youmuEx/spell/subKurin.lua", "start": 133669639, "end": 133673075}, {"filename": "/data/Boss/youmuEx/youmu.csv", "start": 133673075, "end": 133673925}, {"filename": "/data/Boss/youmuEx/youmu.id", "start": 133673925, "end": 133675963}, {"filename": "/data/Chara.id", "start": 133675963, "end": 133676623}, {"filename": "/data/DropScripts/2000/LuaScriptList.id", "start": 133676623, "end": 133676661}, {"filename": "/data/DropScripts/2000/equipFunction.lua", "start": 133676661, "end": 133676661}, {"filename": "/data/Dungeon.id", "start": 133676661, "end": 133677037}, {"filename": "/data/Dungeon/99FDungeon/99FDungeon.lua", "start": 133677037, "end": 133679575}, {"filename": "/data/Dungeon/99FDungeon/99FDungeonData.id", "start": 133679575, "end": 133690669}, {"filename": "/data/Dungeon/99FDungeon/99FDungeonEnemyMap.csv", "start": 133690669, "end": 133721415}, {"filename": "/data/Dungeon/99FDungeon/99FDungeonRegulation.lua", "start": 133721415, "end": 133722073}, {"filename": "/data/Dungeon/99FDungeon/lastFloor.id", "start": 133722073, "end": 133726793}, {"filename": "/data/Dungeon/99FDungeon/lastFloorMap.csv", "start": 133726793, "end": 133737849}, {"filename": "/data/Dungeon/99FUraDungeon/99FUraDungeon.lua", "start": 133737849, "end": 133740387}, {"filename": "/data/Dungeon/99FUraDungeon/99FUraDungeonData.id", "start": 133740387, "end": 133751537}, {"filename": "/data/Dungeon/99FUraDungeon/99FUraDungeonEnemyMap.csv", "start": 133751537, "end": 133782283}, {"filename": "/data/Dungeon/99FUraDungeon/99FUraDungeonEnemyMap\uff3f.csv", "start": 133782283, "end": 133814583}, {"filename": "/data/Dungeon/99FUraDungeon/99FUraDungeonRegulation.lua", "start": 133814583, "end": 133815241}, {"filename": "/data/Dungeon/99FUraDungeon/lastFloor.id", "start": 133815241, "end": 133819961}, {"filename": "/data/Dungeon/99FUraDungeon/lastFloorMap.csv", "start": 133819961, "end": 133831017}, {"filename": "/data/Dungeon/DungeonList.csv", "start": 133831017, "end": 133832367}, {"filename": "/data/Dungeon/DungeonList_header.id", "start": 133832367, "end": 133836435}, {"filename": "/data/Dungeon/DungeonMacros.id", "start": 133836435, "end": 133862241}, {"filename": "/data/Dungeon/DungeonMapMacros.id", "start": 133862241, "end": 133872825}, {"filename": "/data/Dungeon/Map\u30de\u30af\u30ed\u306e\u4f7f\u3044\u65b9.txt", "start": 133872825, "end": 133875411}, {"filename": "/data/Dungeon/R10_99FDungeon/R10_99FDungeonData.id", "start": 133875411, "end": 133885635}, {"filename": "/data/Dungeon/R10_99FDungeon/R10_99FDungeonDataEnemyMap.csv", "start": 133885635, "end": 133912743}, {"filename": "/data/Dungeon/R10_99FDungeon/R10_99FDungeonDataMapChips.id", "start": 133912743, "end": 133960161}, {"filename": "/data/Dungeon/R10_99FDungeon/R10_99FDungeonDataRegulation.lua", "start": 133960161, "end": 133960554}, {"filename": "/data/Dungeon/R10_99FDungeon/story - OP\u30a4\u30d9\u30f3\u30c8(\u30c6\u30b9\u30c8).csv", "start": 133960554, "end": 133964982}, {"filename": "/data/Dungeon/_kgm_testDungeon/bossFloor.id", "start": 133964982, "end": 133969212}, {"filename": "/data/Dungeon/_kgm_testDungeon/bossMap.csv", "start": 133969212, "end": 133979308}, {"filename": "/data/Dungeon/_kgm_testDungeon/testDungeon.lua", "start": 133979308, "end": 133980331}, {"filename": "/data/Dungeon/_kgm_testDungeon/testDungeonData.id", "start": 133980331, "end": 133981217}, {"filename": "/data/Dungeon/_kgm_testDungeon/testDungeonRegulation.lua", "start": 133981217, "end": 133981776}, {"filename": "/data/Dungeon/_old_testDungeon/testDungeon - \u30b3\u30d4\u30fc.lua", "start": 133981776, "end": 133992609}, {"filename": "/data/Dungeon/_old_testDungeon/testDungeon - \u4f8b\u6587.lua", "start": 133992609, "end": 133998881}, {"filename": "/data/Dungeon/_old_testDungeon/testDungeon.lua", "start": 133998881, "end": 134003670}, {"filename": "/data/Dungeon/_old_testDungeon/testDungeonData.id", "start": 134003670, "end": 134006168}, {"filename": "/data/Dungeon/_old_testDungeon/testDungeonExtraSetting.lua", "start": 134006168, "end": 134006851}, {"filename": "/data/Dungeon/_old_testDungeon/testDungeonMap.csv", "start": 134006851, "end": 134017577}, {"filename": "/data/Dungeon/_old_testDungeon/testDungeonMap.ods", "start": 134017577, "end": 134030263}, {"filename": "/data/Dungeon/blueDungeon/blueDungeon.lua", "start": 134030263, "end": 134052814}, {"filename": "/data/Dungeon/blueDungeon/blueDungeonData.id", "start": 134052814, "end": 134070824}, {"filename": "/data/Dungeon/blueDungeon/blueDungeonRegulation.lua", "start": 134070824, "end": 134071383}, {"filename": "/data/Dungeon/blueExtraDungeon/BlueExtraDungeonEnemyMap.csv", "start": 134071383, "end": 134078265}, {"filename": "/data/Dungeon/blueExtraDungeon/blueExtraDungeonData.id", "start": 134078265, "end": 134103051}, {"filename": "/data/Dungeon/blueExtraDungeon/blueExtraDungeonRegulation.lua", "start": 134103051, "end": 134103444}, {"filename": "/data/Dungeon/eventDreamDungeon/eventDreamDungeon.lua", "start": 134103444, "end": 134115369}, {"filename": "/data/Dungeon/eventDreamDungeon/eventDreamDungeonData.id", "start": 134115369, "end": 134120001}, {"filename": "/data/Dungeon/eventDreamDungeon/eventDreamDungeonMap.id", "start": 134120001, "end": 134130611}, {"filename": "/data/Dungeon/eventEndingDungeon/eventEndingDungeon.lua", "start": 134130611, "end": 134130877}, {"filename": "/data/Dungeon/eventEndingDungeon/eventEndingDungeonData.id", "start": 134130877, "end": 134135513}, {"filename": "/data/Dungeon/eventEndingDungeon/eventEndingDungeonMap.id", "start": 134135513, "end": 134146123}, {"filename": "/data/Dungeon/firstHome/firstHome.lua", "start": 134146123, "end": 134149797}, {"filename": "/data/Dungeon/firstHome/firstHomeData.id", "start": 134149797, "end": 134151329}, {"filename": "/data/Dungeon/firstHome/homemap.id", "start": 134151329, "end": 134165847}, {"filename": "/data/Dungeon/globalScript/debugScript.lua", "start": 134165847, "end": 134167846}, {"filename": "/data/Dungeon/greenDungeon/GreenDungeonEnemyMap.csv", "start": 134167846, "end": 134170232}, {"filename": "/data/Dungeon/greenDungeon/greenDungeon.lua", "start": 134170232, "end": 134175746}, {"filename": "/data/Dungeon/greenDungeon/greenDungeonData.id", "start": 134175746, "end": 134184058}, {"filename": "/data/Dungeon/greenDungeon/greenDungeonRegulation.lua", "start": 134184058, "end": 134184617}, {"filename": "/data/Dungeon/greenExtraDungeon/GreenExtraDungeonEnemyMap.csv", "start": 134184617, "end": 134191305}, {"filename": "/data/Dungeon/greenExtraDungeon/greenExtraDungeonData.id", "start": 134191305, "end": 134199949}, {"filename": "/data/Dungeon/greenExtraDungeon/greenExtraDungeonRegulation.lua", "start": 134199949, "end": 134200342}, {"filename": "/data/Dungeon/hellDungeon/HellDungeonEnemyMap.csv", "start": 134200342, "end": 134202728}, {"filename": "/data/Dungeon/hellDungeon/hellDungeonData.id", "start": 134202728, "end": 134210690}, {"filename": "/data/Dungeon/introDungeon/introDungeon - \u30b3\u30d4\u30fc.lua", "start": 134210690, "end": 134217867}, {"filename": "/data/Dungeon/introDungeon/introDungeon.lua", "start": 134217867, "end": 134229784}, {"filename": "/data/Dungeon/introDungeon/introDungeonData.id", "start": 134229784, "end": 134234396}, {"filename": "/data/Dungeon/introDungeon/introDungeonMap.id", "start": 134234396, "end": 134245006}, {"filename": "/data/Dungeon/introDungeon/intro\u8a08\u753b\u66f8.txt", "start": 134245006, "end": 134245084}, {"filename": "/data/Dungeon/lastDungeon/lastDungeon.lua", "start": 134245084, "end": 134282592}, {"filename": "/data/Dungeon/lastDungeon/lastDungeonData.id", "start": 134282592, "end": 134290966}, {"filename": "/data/Dungeon/lastDungeon/lastDungeonEnemyMap.csv", "start": 134290966, "end": 134297744}, {"filename": "/data/Dungeon/lastDungeon/lastDungeonRegulation.lua", "start": 134297744, "end": 134298303}, {"filename": "/data/Dungeon/noEquipDungeon/lastFloor.id", "start": 134298303, "end": 134303023}, {"filename": "/data/Dungeon/noEquipDungeon/lastFloorMap.csv", "start": 134303023, "end": 134314079}, {"filename": "/data/Dungeon/noEquipDungeon/noEquipDungeon.lua", "start": 134314079, "end": 134316617}, {"filename": "/data/Dungeon/noEquipDungeon/noEquipDungeonData.id", "start": 134316617, "end": 134327771}, {"filename": "/data/Dungeon/noEquipDungeon/noEquipDungeonEnemyMap.csv", "start": 134327771, "end": 134333925}, {"filename": "/data/Dungeon/noEquipDungeon/noEquipDungeonRegulation.lua", "start": 134333925, "end": 134334583}, {"filename": "/data/Dungeon/noFoodDungeon/lastFloor.id", "start": 134334583, "end": 134339303}, {"filename": "/data/Dungeon/noFoodDungeon/lastFloorMap.csv", "start": 134339303, "end": 134350359}, {"filename": "/data/Dungeon/noFoodDungeon/noFoodDungeon.lua", "start": 134350359, "end": 134352897}, {"filename": "/data/Dungeon/noFoodDungeon/noFoodDungeonData.id", "start": 134352897, "end": 134364045}, {"filename": "/data/Dungeon/noFoodDungeon/noFoodDungeonEnemyMap.csv", "start": 134364045, "end": 134373149}, {"filename": "/data/Dungeon/noFoodDungeon/noFoodDungeonEnemyMap\uff3f.csv", "start": 134373149, "end": 134405449}, {"filename": "/data/Dungeon/noFoodDungeon/noFoodDungeonRegulation.lua", "start": 134405449, "end": 134406107}, {"filename": "/data/Dungeon/purpleDungeon/PurpleDungeonEnemyMap.csv", "start": 134406107, "end": 134408215}, {"filename": "/data/Dungeon/purpleDungeon/purpleDungeon.lua", "start": 134408215, "end": 134419763}, {"filename": "/data/Dungeon/purpleDungeon/purpleDungeonData.id", "start": 134419763, "end": 134428261}, {"filename": "/data/Dungeon/purpleDungeon/purpleDungeonRegulation.lua", "start": 134428261, "end": 134428820}, {"filename": "/data/Dungeon/purpleExtraDungeon/PurpleExtraDungeonEnemyMap.csv", "start": 134428820, "end": 134435294}, {"filename": "/data/Dungeon/purpleExtraDungeon/purpleExtraDungeonData.id", "start": 134435294, "end": 134443826}, {"filename": "/data/Dungeon/purpleExtraDungeon/purpleExtraDungeonRegulation.lua", "start": 134443826, "end": 134444219}, {"filename": "/data/Dungeon/redDungeon/redDungeon - \u30b3\u30d4\u30fc.lua", "start": 134444219, "end": 134455557}, {"filename": "/data/Dungeon/redDungeon/redDungeon.lua", "start": 134455557, "end": 134469341}, {"filename": "/data/Dungeon/redDungeon/redDungeonData - \u30b3\u30d4\u30fc.id", "start": 134469341, "end": 134485707}, {"filename": "/data/Dungeon/redDungeon/redDungeonData.id", "start": 134485707, "end": 134496233}, {"filename": "/data/Dungeon/redDungeon/redDungeonRegulation.lua", "start": 134496233, "end": 134496792}, {"filename": "/data/Dungeon/redExtraDungeon/RedExtraDungeonEnemyMap.csv", "start": 134496792, "end": 134501550}, {"filename": "/data/Dungeon/redExtraDungeon/redExtraDungeonData.id", "start": 134501550, "end": 134525864}, {"filename": "/data/Dungeon/redExtraDungeon/redExtraDungeonRegulation.lua", "start": 134525864, "end": 134526257}, {"filename": "/data/Dungeon/tenMatchDungeon/tenMatchDungeon.lua", "start": 134526257, "end": 134528381}, {"filename": "/data/Dungeon/tenMatchDungeon/tenMatchDungeonData.id", "start": 134528381, "end": 134540325}, {"filename": "/data/Dungeon/tenMatchDungeon/tenMatchDungeonEnemyMap.csv", "start": 134540325, "end": 134542705}, {"filename": "/data/Dungeon/tenMatchDungeon/tenMatchDungeonEnemyMap\u5909\u63db.csv", "start": 134542705, "end": 134548317}, {"filename": "/data/Dungeon/tenMatchDungeon/tenMatchDungeonMap.csv", "start": 134548317, "end": 134558413}, {"filename": "/data/Dungeon/tenMatchDungeon/tenMatchDungeonMap.ods", "start": 134558413, "end": 134571099}, {"filename": "/data/Dungeon/tenMatchDungeon/tenMatchDungeonRegulation.lua", "start": 134571099, "end": 134571906}, {"filename": "/data/Dungeon/testDungeon/bossFloor.id", "start": 134571906, "end": 134579520}, {"filename": "/data/Dungeon/testDungeon/bossMap.csv", "start": 134579520, "end": 134590704}, {"filename": "/data/Dungeon/testDungeon/testDungeon - \u30b3\u30d4\u30fc.lua", "start": 134590704, "end": 134601537}, {"filename": "/data/Dungeon/testDungeon/testDungeon - \u4f8b\u6587.lua", "start": 134601537, "end": 134607809}, {"filename": "/data/Dungeon/testDungeon/testDungeon.lua", "start": 134607809, "end": 134608884}, {"filename": "/data/Dungeon/testDungeon/testDungeonData.id", "start": 134608884, "end": 134609770}, {"filename": "/data/Dungeon/testDungeon/testDungeonExtraSetting.lua", "start": 134609770, "end": 134610453}, {"filename": "/data/Dungeon/testDungeon/testDungeonMap.csv", "start": 134610453, "end": 134621179}, {"filename": "/data/Dungeon/testDungeon/testDungeonMap.ods", "start": 134621179, "end": 134633865}, {"filename": "/data/Dungeon/testDungeon/testDungeonRegulation.lua", "start": 134633865, "end": 134634424}, {"filename": "/data/Dungeon/tutorialDungeon/tutorialDungeon - \u4f8b\u6587.lua", "start": 134634424, "end": 134640696}, {"filename": "/data/Dungeon/tutorialDungeon/tutorialDungeon.lua", "start": 134640696, "end": 134646553}, {"filename": "/data/Dungeon/tutorialDungeon/tutorialDungeonData.id", "start": 134646553, "end": 134651175}, {"filename": "/data/Dungeon/tutorialDungeon/tutorialDungeonExtraSetting.lua", "start": 134651175, "end": 134651858}, {"filename": "/data/Dungeon/tutorialDungeon/tutorialDungeonMap.csv", "start": 134651858, "end": 134662584}, {"filename": "/data/Dungeon/tutorialDungeon/tutorialDungeonMap.ods", "start": 134662584, "end": 134675270}, {"filename": "/data/Dungeon/tutorialDungeon/tutorial\u8a08\u753b\u66f8.txt", "start": 134675270, "end": 134675718}, {"filename": "/data/Dungeon/\u6575ID\u8868\u4f5c\u6210\u30de\u30af\u30ed.txt", "start": 134675718, "end": 134679270}, {"filename": "/data/Effect.id", "start": 134679270, "end": 134679326}, {"filename": "/data/Effect/Chara/Attack/Attack_header.id", "start": 134679326, "end": 134679382}, {"filename": "/data/Effect/Chara/Attack/EffectCharaAttack1000.lua", "start": 134679382, "end": 134681373}, {"filename": "/data/Effect/Chara/Chara_header.id", "start": 134681373, "end": 134681549}, {"filename": "/data/Effect/Chara/Special/EffectCharaSpecial2019.lua", "start": 134681549, "end": 134686773}, {"filename": "/data/Effect/Chara/Special/EffectCharaSpecial2042.lua", "start": 134686773, "end": 134689902}, {"filename": "/data/Effect/Chara/Special/EffectCharaSpecial2060.lua", "start": 134689902, "end": 134691765}, {"filename": "/data/Effect/Chara/Special/EffectCharaSpecial2064.lua", "start": 134691765, "end": 134694202}, {"filename": "/data/Effect/Chara/Special/EffectCharaSpecial2068.lua", "start": 134694202, "end": 134697798}, {"filename": "/data/Effect/Chara/Special/EffectCharaSpecial2070.lua", "start": 134697798, "end": 134699789}, {"filename": "/data/Effect/Chara/Special/Special_header.id", "start": 134699789, "end": 134700147}, {"filename": "/data/Effect/Chara/Trap_header.id", "start": 134700147, "end": 134700323}, {"filename": "/data/Effect/Effect_header.id", "start": 134700323, "end": 134700729}, {"filename": "/data/Effect/Equipment/Attack/Attack_header.id", "start": 134700729, "end": 134704323}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1000 - \u30b3\u30d4\u30fc.lua", "start": 134704323, "end": 134705321}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1000.lua", "start": 134705321, "end": 134708874}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1001.lua", "start": 134708874, "end": 134713821}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1002.lua", "start": 134713821, "end": 134717134}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1003.lua", "start": 134717134, "end": 134720641}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1004.lua", "start": 134720641, "end": 134723228}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1005.lua", "start": 134723228, "end": 134726300}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1006.lua", "start": 134726300, "end": 134729504}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1007.lua", "start": 134729504, "end": 134732717}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1008.lua", "start": 134732717, "end": 134735139}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1009.lua", "start": 134735139, "end": 134739160}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1010.lua", "start": 134739160, "end": 134742244}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1011.lua", "start": 134742244, "end": 134745328}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1012.lua", "start": 134745328, "end": 134748412}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1013.lua", "start": 134748412, "end": 134751496}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1014.lua", "start": 134751496, "end": 134754191}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1015.lua", "start": 134754191, "end": 134757275}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1016.lua", "start": 134757275, "end": 134760359}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1017.lua", "start": 134760359, "end": 134763443}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1018.lua", "start": 134763443, "end": 134767269}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1019.lua", "start": 134767269, "end": 134772904}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1020.lua", "start": 134772904, "end": 134777010}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1021.lua", "start": 134777010, "end": 134780299}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1022.lua", "start": 134780299, "end": 134784188}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1023.lua", "start": 134784188, "end": 134789836}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1024.lua", "start": 134789836, "end": 134793221}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1025.lua", "start": 134793221, "end": 134798077}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1026.lua", "start": 134798077, "end": 134801588}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1027.lua", "start": 134801588, "end": 134805404}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1028.lua", "start": 134805404, "end": 134808488}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1029.lua", "start": 134808488, "end": 134813084}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1030.lua", "start": 134813084, "end": 134816168}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1031.lua", "start": 134816168, "end": 134819978}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1032.lua", "start": 134819978, "end": 134822567}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1033.lua", "start": 134822567, "end": 134826802}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1034.lua", "start": 134826802, "end": 134833165}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1035.lua", "start": 134833165, "end": 134836249}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1036.lua", "start": 134836249, "end": 134839333}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1037.lua", "start": 134839333, "end": 134843960}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1038.lua", "start": 134843960, "end": 134848431}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1039.lua", "start": 134848431, "end": 134852426}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1040.lua", "start": 134852426, "end": 134856886}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1041.lua", "start": 134856886, "end": 134860920}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1042.lua", "start": 134860920, "end": 134865002}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1043.lua", "start": 134865002, "end": 134868602}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1044.lua", "start": 134868602, "end": 134871686}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1045.lua", "start": 134871686, "end": 134874273}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1046.lua", "start": 134874273, "end": 134878342}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1047.lua", "start": 134878342, "end": 134880929}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1048.lua", "start": 134880929, "end": 134884011}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1049.lua", "start": 134884011, "end": 134889148}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1050.lua", "start": 134889148, "end": 134891388}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1051.lua", "start": 134891388, "end": 134895338}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1052.lua", "start": 134895338, "end": 134899967}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1053.lua", "start": 134899967, "end": 134903051}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1054.lua", "start": 134903051, "end": 134906135}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1055.lua", "start": 134906135, "end": 134909219}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1056.lua", "start": 134909219, "end": 134912296}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1057.lua", "start": 134912296, "end": 134914889}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1058.lua", "start": 134914889, "end": 134917958}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1059.lua", "start": 134917958, "end": 134920563}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1060.lua", "start": 134920563, "end": 134925454}, {"filename": "/data/Effect/Equipment/Attack/EffectEquipAttack1061.lua", "start": 134925454, "end": 134928538}, {"filename": "/data/Effect/Equipment/Attack/\u30ea\u30d5\u30a1\u30ec\u30f3\u30b9.txt", "start": 134928538, "end": 134929230}, {"filename": "/data/Effect/Equipment/Attack/\u5263\u306e\u30a8\u30d5\u30a7\u30af\u30c8\u30c6\u30f3\u30d7\u30ec\u30fc\u30c8.lua", "start": 134929230, "end": 134932312}, {"filename": "/data/Effect/Equipment/Damage/Damage_header.id", "start": 134932312, "end": 134932368}, {"filename": "/data/Effect/Equipment/Equipment_header.id", "start": 134932368, "end": 134932538}, {"filename": "/data/Effect/Other/EffectBossHassan.lua", "start": 134932538, "end": 134934505}, {"filename": "/data/Effect/Other/EffectBossTame.lua", "start": 134934505, "end": 134936398}, {"filename": "/data/Effect/Other/EffectDanmakuAttack.lua", "start": 134936398, "end": 134937991}, {"filename": "/data/Effect/Other/EffectGeneralFog.lua", "start": 134937991, "end": 134940159}, {"filename": "/data/Effect/Other/EffectGeneralStamp.lua", "start": 134940159, "end": 134942740}, {"filename": "/data/Effect/Other/Other_header.id", "start": 134942740, "end": 134942984}, {"filename": "/data/Effect/Spell/EffectSpell3022.lua", "start": 134942984, "end": 134943808}, {"filename": "/data/Effect/Spell/EffectSpell3049.lua", "start": 134943808, "end": 134946506}, {"filename": "/data/Effect/Spell/EffectSpellEquip - \u30b3\u30d4\u30fc.lua", "start": 134946506, "end": 134950820}, {"filename": "/data/Effect/Spell/EffectSpellEquip.lua", "start": 134950820, "end": 134956341}, {"filename": "/data/Effect/Spell/Spell_header.id", "start": 134956341, "end": 134956479}, {"filename": "/data/Effect/Trap/EffectTrap9000 - \u30b3\u30d4\u30fc.lua", "start": 134956479, "end": 134956703}, {"filename": "/data/Effect/Trap/EffectTrap9000.lua", "start": 134956703, "end": 134957821}, {"filename": "/data/Effect/Trap/EffectTrap9001.lua", "start": 134957821, "end": 134958939}, {"filename": "/data/Effect/Trap/EffectTrap9004.lua", "start": 134958939, "end": 134960244}, {"filename": "/data/Effect/Trap/EffectTrap9010.lua", "start": 134960244, "end": 134961533}, {"filename": "/data/Effect/Trap/EffectTrap9011.lua", "start": 134961533, "end": 134962384}, {"filename": "/data/Effect/Trap/EffectTrap9012.lua", "start": 134962384, "end": 134963817}, {"filename": "/data/Effect/Trap/EffectTrap9013.lua", "start": 134963817, "end": 134965759}, {"filename": "/data/Effect/Trap/EffectTrap9014.lua", "start": 134965759, "end": 134966735}, {"filename": "/data/Effect/Trap/EffectTrap9016.lua", "start": 134966735, "end": 134967572}, {"filename": "/data/Effect/Trap/EffectTrap9017.lua", "start": 134967572, "end": 134968787}, {"filename": "/data/Effect/Trap/EffectTrap9018.lua", "start": 134968787, "end": 134970184}, {"filename": "/data/Effect/Trap/EffectTrap9020.lua", "start": 134970184, "end": 134971001}, {"filename": "/data/Effect/Trap/EffectTrap9021.lua", "start": 134971001, "end": 134972279}, {"filename": "/data/Effect/Trap/EffectTrap9024.lua", "start": 134972279, "end": 134973580}, {"filename": "/data/Effect/Trap/EffectTrap9025.lua", "start": 134973580, "end": 134974556}, {"filename": "/data/Effect/Trap/EffectTrap9026.lua", "start": 134974556, "end": 134975402}, {"filename": "/data/Effect/Trap/EffectTrap9027.lua", "start": 134975402, "end": 134976092}, {"filename": "/data/Effect/Trap/EffectTrap9028.lua", "start": 134976092, "end": 134976743}, {"filename": "/data/Effect/Trap/EffectTrap9029.lua", "start": 134976743, "end": 134977583}, {"filename": "/data/Effect/Trap/EffectTrap9030.lua", "start": 134977583, "end": 134978419}, {"filename": "/data/Effect/Trap/EffectTrapBase.lua", "start": 134978419, "end": 134979425}, {"filename": "/data/Effect/Trap/TrapEffect\u30ea\u30d5\u30a1\u30ec\u30f3\u30b9.txt", "start": 134979425, "end": 134979731}, {"filename": "/data/Effect/Trap/Trap_header.id", "start": 134979731, "end": 134981533}, {"filename": "/data/Enemy/EnemyTable.csv", "start": 134981533, "end": 135056263}, {"filename": "/data/Enemy/EnemyTableEx.csv", "start": 135056263, "end": 135061425}, {"filename": "/data/Enemy/objectchara3000Table.csv", "start": 135061425, "end": 135062789}, {"filename": "/data/Enemy/otherchara4000Table.csv", "start": 135062789, "end": 135079733}, {"filename": "/data/Enemy/pal_enemy2000.id", "start": 135079733, "end": 135082435}, {"filename": "/data/Enemy/pal_enemy_header.id", "start": 135082435, "end": 135082759}, {"filename": "/data/Enemy/pal_obj.id", "start": 135082759, "end": 135083789}, {"filename": "/data/Enemy/pal_objchara3000.id", "start": 135083789, "end": 135085099}, {"filename": "/data/Enemy/pal_otherchara4000.id", "start": 135085099, "end": 135091273}, {"filename": "/data/HomeDataBase.id", "start": 135091273, "end": 135091391}, {"filename": "/data/HomeDataBase/HomeEvents.id", "start": 135091391, "end": 135093493}, {"filename": "/data/HomeDataBase/HomeEvents/blueDungeonAppeared.lua", "start": 135093493, "end": 135094369}, {"filename": "/data/HomeDataBase/HomeEvents/blueDungeonCleared.lua", "start": 135094369, "end": 135095846}, {"filename": "/data/HomeDataBase/HomeEvents/blueExtraDungeonAppeared.lua", "start": 135095846, "end": 135096399}, {"filename": "/data/HomeDataBase/HomeEvents/dreamEventCleared.lua", "start": 135096399, "end": 135097708}, {"filename": "/data/HomeDataBase/HomeEvents/firstRun.lua", "start": 135097708, "end": 135097835}, {"filename": "/data/HomeDataBase/HomeEvents/greenAndPurpleDungeonAppeared.lua", "start": 135097835, "end": 135106835}, {"filename": "/data/HomeDataBase/HomeEvents/greenDungeonCleared.lua", "start": 135106835, "end": 135109824}, {"filename": "/data/HomeDataBase/HomeEvents/greenExtraDungeonAppeared.lua", "start": 135109824, "end": 135110381}, {"filename": "/data/HomeDataBase/HomeEvents/homeAfterCredit.lua", "start": 135110381, "end": 135115696}, {"filename": "/data/HomeDataBase/HomeEvents/homeAkyuAppearing.lua", "start": 135115696, "end": 135117068}, {"filename": "/data/HomeDataBase/HomeEvents/homeHinaAppearing.lua", "start": 135117068, "end": 135118373}, {"filename": "/data/HomeDataBase/HomeEvents/homeMarisaAppearing.lua", "start": 135118373, "end": 135119865}, {"filename": "/data/HomeDataBase/HomeEvents/homeNitoriAppearing.lua", "start": 135119865, "end": 135121816}, {"filename": "/data/HomeDataBase/HomeEvents/homeReset.lua", "start": 135121816, "end": 135122027}, {"filename": "/data/HomeDataBase/HomeEvents/homeYoumuAppearing.lua", "start": 135122027, "end": 135124068}, {"filename": "/data/HomeDataBase/HomeEvents/lastDungeonAppeared.lua", "start": 135124068, "end": 135132296}, {"filename": "/data/HomeDataBase/HomeEvents/mottoDungeonAppeared.lua", "start": 135132296, "end": 135136580}, {"filename": "/data/HomeDataBase/HomeEvents/noEquipDungeonAppeared.lua", "start": 135136580, "end": 135137127}, {"filename": "/data/HomeDataBase/HomeEvents/noFoodDungeonAppeared.lua", "start": 135137127, "end": 135137673}, {"filename": "/data/HomeDataBase/HomeEvents/playerLevelExplanation.lua", "start": 135137673, "end": 135138395}, {"filename": "/data/HomeDataBase/HomeEvents/purpleDungeonCleared.lua", "start": 135138395, "end": 135140022}, {"filename": "/data/HomeDataBase/HomeEvents/purpleExtraDungeonAppeared.lua", "start": 135140022, "end": 135140583}, {"filename": "/data/HomeDataBase/HomeEvents/redDungeonAppeared.lua", "start": 135140583, "end": 135145473}, {"filename": "/data/HomeDataBase/HomeEvents/redDungeonCleared.lua", "start": 135145473, "end": 135146550}, {"filename": "/data/HomeDataBase/HomeEvents/redExtraDungeonAppeared.lua", "start": 135146550, "end": 135147123}, {"filename": "/data/HomeDataBase/HomeEvents/tenMatchDungeonAppeared.lua", "start": 135147123, "end": 135147673}, {"filename": "/data/HomeDataBase/HomeEvents/tutorialDungeonAppeared.lua", "start": 135147673, "end": 135148811}, {"filename": "/data/HomeDataBase/MarisaShop.id", "start": 135148811, "end": 135154445}, {"filename": "/data/Interface/interfaceBase.id", "start": 135154445, "end": 135155711}, {"filename": "/data/Item.id", "start": 135155711, "end": 135156063}, {"filename": "/data/Item/Book.id", "start": 135156063, "end": 135166121}, {"filename": "/data/Item/BookTable.csv", "start": 135166121, "end": 135168889}, {"filename": "/data/Item/Box.id", "start": 135168889, "end": 135182443}, {"filename": "/data/Item/BoxTable.csv", "start": 135182443, "end": 135186347}, {"filename": "/data/Item/Bullet.id", "start": 135186347, "end": 135201419}, {"filename": "/data/Item/BulletTable.csv", "start": 135201419, "end": 135206143}, {"filename": "/data/Item/Drink.id", "start": 135206143, "end": 135209223}, {"filename": "/data/Item/DrinkTable.csv", "start": 135209223, "end": 135216077}, {"filename": "/data/Item/Equipment.id", "start": 135216077, "end": 135218607}, {"filename": "/data/Item/EquipmentTable.csv", "start": 135218607, "end": 135236747}, {"filename": "/data/Item/EquipmentTableTexture.csv", "start": 135236747, "end": 135241839}, {"filename": "/data/Item/Food.id", "start": 135241839, "end": 135243113}, {"filename": "/data/Item/FoodTable.csv", "start": 135243113, "end": 135245703}, {"filename": "/data/Item/ItemCommon.id", "start": 135245703, "end": 135246209}, {"filename": "/data/Item/ItemPals_header.id", "start": 135246209, "end": 135246921}, {"filename": "/data/Item/Money.id", "start": 135246921, "end": 135247633}, {"filename": "/data/Item/MoneyTable.csv", "start": 135247633, "end": 135249309}, {"filename": "/data/Item/Other.id", "start": 135249309, "end": 135250019}, {"filename": "/data/Item/Spell.id", "start": 135250019, "end": 135253313}, {"filename": "/data/Item/SpellTable.csv", "start": 135253313, "end": 135267675}, {"filename": "/data/Item/Trap.id", "start": 135267675, "end": 135268659}, {"filename": "/data/Item/TrapTable.csv", "start": 135268659, "end": 135271663}, {"filename": "/data/MKEditor\u7528\u30d7\u30ed\u30b8\u30a7\u30af\u30c8", "start": 135271663, "end": 135277388}, {"filename": "/data/Mapchip.id", "start": 135277388, "end": 135277742}, {"filename": "/data/Mapchip/CommonMapDecoMacro.id", "start": 135277742, "end": 135280054}, {"filename": "/data/Mapchip/MapChipFormData.csv", "start": 135280054, "end": 135281084}, {"filename": "/data/Mapchip/MapchipList.csv", "start": 135281084, "end": 135281812}, {"filename": "/data/Mapchip/MapchipList_header.id", "start": 135281812, "end": 135283814}, {"filename": "/data/Mapchip/\u51a5\u754c\u30bb\u30c3\u30c8Data.id", "start": 135283814, "end": 135326642}, {"filename": "/data/Mapchip/\u56f3\u66f8\u9928\u30bb\u30c3\u30c8Data.id", "start": 135326642, "end": 135351056}, {"filename": "/data/Mapchip/\u5730\u970a\u6bbf\u30bb\u30c3\u30c8Data.id", "start": 135351056, "end": 135359862}, {"filename": "/data/Mapchip/\u5996\u602a\u306e\u5c71\u5730\u4e0b\u30bb\u30c3\u30c8Data.id", "start": 135359862, "end": 135413138}, {"filename": "/data/Mapchip/\u5b9d\u8239\u30bb\u30c3\u30c8Data.id", "start": 135413138, "end": 135474146}, {"filename": "/data/Mapchip/\u5ddd\u8fba\u30bb\u30c3\u30c8Data.id", "start": 135474146, "end": 135476486}, {"filename": "/data/Mapchip/\u6c38\u9060\u4ead\u30bb\u30c3\u30c8Data.id", "start": 135476486, "end": 135543168}, {"filename": "/data/Mapchip/\u707c\u71b1\u5730\u7344\u30bb\u30c3\u30c8Data.id", "start": 135543168, "end": 135577252}, {"filename": "/data/Mapchip/\u767d\u7389\u697c\u30bb\u30c3\u30c8Data.id", "start": 135577252, "end": 135591174}, {"filename": "/data/Mapchip/\u7af9\u6797\u30bb\u30c3\u30c8Data.id", "start": 135591174, "end": 135680270}, {"filename": "/data/Mapchip/\u7d05\u9b54\u9928\u30bb\u30c3\u30c8Data.id", "start": 135680270, "end": 135699828}, {"filename": "/data/Mapchip/\u7d05\u9b54\u9928\u30bb\u30c3\u30c8Data1.id", "start": 135699828, "end": 135756258}, {"filename": "/data/Mapchip/\u7d05\u9b54\u9928\u30bb\u30c3\u30c8Data2.id", "start": 135756258, "end": 135771818}, {"filename": "/data/Mapchip/\u7f8e\u9234\u306e\u4fee\u884c\u5834\u30bb\u30c3\u30c8Data.id", "start": 135771818, "end": 135771968}, {"filename": "/data/Mapchip/\u7f8e\u9234\u306e\u4fee\u884c\u5834\u30bb\u30c3\u30c8Data1.id", "start": 135771968, "end": 135775748}, {"filename": "/data/Mapchip/\u7f8e\u9234\u306e\u4fee\u884c\u5834\u30bb\u30c3\u30c8Data2.id", "start": 135775748, "end": 135810386}, {"filename": "/data/Mapchip/\u7f8e\u9234\u8a70\u3081\u6240\u30bb\u30c3\u30c8Data.id", "start": 135810386, "end": 135828512}, {"filename": "/data/Mapchip/\u9b54\u754c\u30bb\u30c3\u30c8Data.id", "start": 135828512, "end": 135872850}, {"filename": "/data/OtherChara/OtherChara_header.id", "start": 135872850, "end": 135872946}, {"filename": "/data/OtherChara/akyuEvent.lua", "start": 135872946, "end": 135880527}, {"filename": "/data/PlayerChara/PChara.csv", "start": 135880527, "end": 135886561}, {"filename": "/data/PlayerChara/PChara2.csv", "start": 135886561, "end": 135892425}, {"filename": "/data/PlayerChara/PChara3.csv", "start": 135892425, "end": 135898625}, {"filename": "/data/PlayerChara/PChara_youmu.csv", "start": 135898625, "end": 135904539}, {"filename": "/data/PlayerChara/meirinEvent.lua", "start": 135904539, "end": 135905461}, {"filename": "/data/PlayerChara/pal_pc1000.id", "start": 135905461, "end": 135909981}, {"filename": "/data/PlayerChara/pal_pc_header.id", "start": 135909981, "end": 135910065}, {"filename": "/data/PlayerChara/tewiEvent.lua", "start": 135910065, "end": 135910983}, {"filename": "/data/PlayerChara/wanashi.lua", "start": 135910983, "end": 135913055}, {"filename": "/data/PlayerChara/youmuEvent.lua", "start": 135913055, "end": 135925615}, {"filename": "/data/dataconfig.txt", "start": 135925615, "end": 135925683}, {"filename": "/data/systems.id", "start": 135925683, "end": 135926655}], "remote_package_size": 135926655});

  })();

// end include: C:\Users\youfo\AppData\Local\Temp\tmpxijnp56n.js
// include: C:\Users\youfo\AppData\Local\Temp\tmp25ji1u6y.js

    // All the pre-js content up to here must remain later on, we need to run
    // it.
    if ((typeof ENVIRONMENT_IS_WASM_WORKER != 'undefined' && ENVIRONMENT_IS_WASM_WORKER) || (typeof ENVIRONMENT_IS_PTHREAD != 'undefined' && ENVIRONMENT_IS_PTHREAD) || (typeof ENVIRONMENT_IS_AUDIO_WORKLET != 'undefined' && ENVIRONMENT_IS_AUDIO_WORKLET)) Module['preRun'] = [];
    var necessaryPreJSTasks = Module['preRun'].slice();
  // end include: C:\Users\youfo\AppData\Local\Temp\tmp25ji1u6y.js
// include: C:\Users\youfo\AppData\Local\Temp\tmpsb53tlas.js

    if (!Module['preRun']) throw 'Module.preRun should exist because file support used it; did a pre-js delete it?';
    necessaryPreJSTasks.forEach((task) => {
      if (Module['preRun'].indexOf(task) < 0) throw 'All preRun tasks that exist before user pre-js code should remain after; did you replace Module or modify Module.preRun?';
    });
  // end include: C:\Users\youfo\AppData\Local\Temp\tmpsb53tlas.js


var programArgs = [];
var thisProgram = './this.program';
var quit_ = (status, toThrow) => {
  throw toThrow;
};

// In MODULARIZE mode _scriptName needs to be captured already at the very top of the page immediately when the page is parsed, so it is generated there
// before the page load. In non-MODULARIZE modes generate it here.
var _scriptName = globalThis.document?.currentScript?.src;

if (typeof __filename != 'undefined') { // Node
  _scriptName = __filename;
} else
if (ENVIRONMENT_IS_WORKER) {
  _scriptName = self.location.href;
}

// `/` should be present at the end if `scriptDirectory` is not empty
var scriptDirectory = '';
function locateFile(path) {
  if (Module['locateFile']) {
    return Module['locateFile'](path, scriptDirectory);
  }
  return scriptDirectory + path;
}

// Hooks that are implemented differently in different runtime environments.
var readAsync, readBinary;

if (ENVIRONMENT_IS_NODE) {
  const isNode = globalThis.process?.versions?.node && globalThis.process?.type != 'renderer';
  if (!isNode) throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

  // These modules will usually be used on Node.js. Load them eagerly to avoid
  // the complexity of lazy-loading.
  var fs = require('node:fs');

  scriptDirectory = __dirname + '/';

// include: node_shell_read.js
readBinary = (filename) => {
  // We need to re-wrap `file://` strings to URLs.
  filename = isFileURI(filename) ? new URL(filename) : filename;
  var ret = fs.readFileSync(filename);
  assert(Buffer.isBuffer(ret));
  return ret;
};

readAsync = async (filename, binary = true) => {
  // See the comment in the `readBinary` function.
  filename = isFileURI(filename) ? new URL(filename) : filename;
  var ret = fs.readFileSync(filename, binary ? undefined : 'utf8');
  assert(binary ? Buffer.isBuffer(ret) : typeof ret == 'string');
  return ret;
};
// end include: node_shell_read.js
  if (process.argv.length > 1) {
    thisProgram = process.argv[1].replace(/\\/g, '/');
  }

  programArgs = process.argv.slice(2);

  // MODULARIZE will export the module in the proper place outside, we don't need to export here
  if (typeof module != 'undefined') {
    module['exports'] = Module;
  }

  quit_ = (status, toThrow) => {
    process.exitCode = status;
    throw toThrow;
  };

} else
if (ENVIRONMENT_IS_SHELL) {

} else

// Note that this includes Node.js workers when relevant (pthreads is enabled).
// Node.js workers are detected as a combination of ENVIRONMENT_IS_WORKER and
// ENVIRONMENT_IS_NODE.
if (ENVIRONMENT_IS_WEB || ENVIRONMENT_IS_WORKER) {
  try {
    scriptDirectory = new URL('.', _scriptName).href; // includes trailing slash
  } catch {
    // Must be a `blob:` or `data:` URL (e.g. `blob:http://site.com/etc/etc`), we cannot
    // infer anything from them.
  }

  if (!(globalThis.window || globalThis.WorkerGlobalScope)) throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

  {
// include: web_or_worker_shell_read.js
if (ENVIRONMENT_IS_WORKER) {
    readBinary = (url) => {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', url, false);
      xhr.responseType = 'arraybuffer';
      xhr.send(null);
      return new Uint8Array(/** @type{!ArrayBuffer} */(xhr.response));
    };
  }

  readAsync = async (url) => {
    // Fetch has some additional restrictions over XHR, like it can't be used on a file:// url.
    // See https://github.com/github/fetch/pull/92#issuecomment-140665932
    // Cordova or Electron apps are typically loaded from a file:// url.
    // So use XHR on webview if URL is a file URL.
    if (isFileURI(url)) {
      return new Promise((resolve, reject) => {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', url, true);
        xhr.responseType = 'arraybuffer';
        xhr.onload = () => {
          if (xhr.status == 200 || (xhr.status == 0 && xhr.response)) { // file URLs can return 0
            resolve(xhr.response);
            return;
          }
          reject(xhr.status);
        };
        xhr.onerror = reject;
        xhr.send(null);
      });
    }
    var response = await fetch(url, { credentials: 'same-origin' });
    if (response.ok) {
      return response.arrayBuffer();
    }
    throw new Error(response.status + ' : ' + response.url);
  };
// end include: web_or_worker_shell_read.js
  }
} else
{
  throw new Error('environment detection error');
}

var out = console.log.bind(console);
var err = console.error.bind(console);


var PROXYFS = 'PROXYFS is no longer included by default; build with -lproxyfs.js';
var WORKERFS = 'WORKERFS is no longer included by default; build with -lworkerfs.js';
var FETCHFS = 'FETCHFS is no longer included by default; build with -lfetchfs.js';
var ICASEFS = 'ICASEFS is no longer included by default; build with -licasefs.js';
var JSFILEFS = 'JSFILEFS is no longer included by default; build with -ljsfilefs.js';
var OPFS = 'OPFS is no longer included by default; build with -lopfs.js';

var NODEFS = 'NODEFS is no longer included by default; build with -lnodefs.js';

// perform assertions in shell.js after we set up out() and err(), as otherwise
// if an assertion fails it cannot print the message

assert(!ENVIRONMENT_IS_SHELL, 'shell environment detected but not enabled at build time (add `shell` to `-sENVIRONMENT` to enable)');

// end include: shell.js

// include: preamble.js
// === Preamble library stuff ===

// Documentation for the public APIs defined in this file must be updated in:
//    site/source/docs/api_reference/preamble.js.rst
// A prebuilt local version of the documentation is available at:
//    site/build/text/docs/api_reference/preamble.js.txt
// You can also build docs locally as HTML or other formats in site/
// An online HTML version (which may be of a different version of Emscripten)
//    is up at http://kripken.github.io/emscripten-site/docs/api_reference/preamble.js.html

var wasmBinary;

if (!globalThis.WebAssembly) {
  err('no native wasm support detected');
}

// Wasm globals

//========================================
// Runtime essentials
//========================================

// whether we are quitting the application. no code should run after this.
// set in exit() and abort()
var ABORT = false;

// set by exit() and abort().  Passed to 'onExit' handler.
// NOTE: This is also used as the process return code in shell environments
// but only when noExitRuntime is false.
var EXITSTATUS;

// In STRICT mode, we only define assert() when ASSERTIONS is set.  i.e. we
// don't define it at all in release modes.  This matches the behaviour of
// MINIMAL_RUNTIME.
// TODO(sbc): Make this the default even without STRICT enabled.
/** @type {function(*, string=)} */
function assert(condition, text) {
  if (!condition) {
    abort('Assertion failed' + (text ? ': ' + text : ''));
  }
}

// We used to include malloc/free by default in the past. Show a helpful error in
// builds with assertions.
function _malloc() {
  abort('malloc() called but not included in the build - add `_malloc` to EXPORTED_FUNCTIONS');
}

/**
 * Indicates whether filename is delivered via file protocol (as opposed to http/https)
 * @noinline
 */
var isFileURI = (filename) => filename.startsWith('file://');

// include: runtime_common.js
// include: runtime_stack_check.js
// Initializes the stack cookie. Called at the startup of main and at the startup of each thread in pthreads mode.
function writeStackCookie() {
  var max = _emscripten_stack_get_end();
  assert((max & 3) == 0);
  // If the stack ends at address zero we write our cookies 4 bytes into the
  // stack.  This prevents interference with SAFE_HEAP and ASAN which also
  // monitor writes to address zero.
  if (max == 0) {
    max += 4;
  }
  // The stack grow downwards towards _emscripten_stack_get_end.
  // We write cookies to the final two words in the stack and detect if they are
  // ever overwritten.
  HEAPU32[((max)>>2)] = 0x02135467;
  HEAPU32[(((max)+(4))>>2)] = 0x89BACDFE;
  // Also test the global address 0 for integrity.
  HEAPU32[((0)>>2)] = 1668509029;
}

function checkStackCookie() {
  if (ABORT) return;
  var max = _emscripten_stack_get_end();
  // See writeStackCookie().
  if (max == 0) {
    max += 4;
  }
  var cookie1 = HEAPU32[((max)>>2)];
  var cookie2 = HEAPU32[(((max)+(4))>>2)];
  if (cookie1 != 0x02135467 || cookie2 != 0x89BACDFE) {
    abort(`Stack overflow! Stack cookie has been overwritten at ${ptrToString(max)}, expected hex dwords 0x89BACDFE and 0x2135467, but received ${ptrToString(cookie2)} ${ptrToString(cookie1)}`);
  }
  // Also test the global address 0 for integrity.
  if (HEAPU32[((0)>>2)] != 0x63736d65 /* 'emsc' */) {
    abort('Runtime error: The application has corrupted its heap memory area (address zero)!');
  }
}
// end include: runtime_stack_check.js
// include: runtime_exceptions.js
// Base Emscripten EH error class
class EmscriptenEH extends Error {}

class EmscriptenSjLj extends EmscriptenEH {}

class CppException extends EmscriptenEH {
  constructor(excPtr) {
    super(excPtr);
    this.excPtr = excPtr;
    const excInfo = getExceptionMessage(this);
    this.name = excInfo[0];
    this.message = excInfo[1];
  }
}

// end include: runtime_exceptions.js
// include: runtime_debug.js
var runtimeDebug = true; // Switch to false at runtime to disable logging at the right times

// Used by XXXXX_DEBUG settings to output debug messages.
function dbg(...args) {
  if (!runtimeDebug && typeof runtimeDebug != 'undefined') return;
  // TODO(sbc): Make this configurable somehow.  Its not always convenient for
  // logging to show up as warnings.
  console.warn(...args);
}

// Endianness check
(() => {
  var h16 = new Int16Array(1);
  var h8 = new Int8Array(h16.buffer);
  h16[0] = 0x6373;
  if (h8[0] !== 0x73 || h8[1] !== 0x63) abort('Runtime error: expected the system to be little-endian! (Run with -sSUPPORT_BIG_ENDIAN to bypass)');
})();

function consumedModuleProp(prop) {
  if (!Object.getOwnPropertyDescriptor(Module, prop)) {
    Object.defineProperty(Module, prop, {
      configurable: true,
      set() {
        abort(`Attempt to set \`Module.${prop}\` after it has already been processed.  This can happen, for example, when code is injected via '--post-js' rather than '--pre-js'`);

      }
    });
  }
}

function makeInvalidEarlyAccess(name) {
  return () => assert(false, `call to '${name}' via reference taken before Wasm module initialization`);

}

function ignoredModuleProp(prop) {
  if (Object.getOwnPropertyDescriptor(Module, prop)) {
    abort(`\`Module.${prop}\` was supplied but \`${prop}\` not included in INCOMING_MODULE_JS_API`);
  }
}

// forcing the filesystem exports a few things by default
function isExportedByForceFilesystem(name) {
  return name === 'FS_createPath' ||
         name === 'FS_createDataFile' ||
         name === 'FS_createPreloadedFile' ||
         name === 'FS_preloadFile' ||
         name === 'FS_unlink' ||
         name === 'addRunDependency' ||
         // The old FS has some functionality that WasmFS lacks.
         name === 'FS_createLazyFile' ||
         name === 'FS_createDevice' ||
         name === 'removeRunDependency';
}

/**
 * Intercept access to a symbols in the global symbol.  This enables us to give
 * informative warnings/errors when folks attempt to use symbols they did not
 * include in their build, or no symbols that no longer exist.
 *
 * We don't define this in MODULARIZE mode since in that mode emscripten symbols
 * are never placed in the global scope.
 */
function hookGlobalSymbolAccess(sym, func) {
  if (!Object.getOwnPropertyDescriptor(globalThis, sym)) {
    Object.defineProperty(globalThis, sym, {
      configurable: true,
      get() {
        func();
        return undefined;
      }
    });
  }
}

function missingGlobal(sym, msg) {
  hookGlobalSymbolAccess(sym, () => {
    warnOnce(`\`${sym}\` is no longer defined by emscripten. ${msg}`);
  });
}

missingGlobal('buffer', 'Please use HEAP8.buffer or wasmMemory.buffer');
missingGlobal('asm', 'Please use wasmExports instead');

function missingLibrarySymbol(sym) {
  hookGlobalSymbolAccess(sym, () => {
    // Can't `abort()` here because it would break code that does runtime
    // checks.  e.g. `if (typeof SDL === 'undefined')`.
    var msg = `\`${sym}\` is a library symbol and not included by default; add it to your library.js __deps or to DEFAULT_LIBRARY_FUNCS_TO_INCLUDE on the command line`;
    // DEFAULT_LIBRARY_FUNCS_TO_INCLUDE requires the name as it appears in
    // library.js, which means $name for a JS name with no prefix, or name
    // for a JS name like _name.
    var librarySymbol = sym;
    if (!librarySymbol.startsWith('_')) {
      librarySymbol = '$' + sym;
    }
    msg += ` (e.g. -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='${librarySymbol}')`;
    if (isExportedByForceFilesystem(sym)) {
      msg += '. Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you';
    }
    warnOnce(msg);
  });

  // Any symbol that is not included from the JS library is also (by definition)
  // not exported on the Module object.
  unexportedRuntimeSymbol(sym);
}

function unexportedRuntimeSymbol(sym) {
  if (!Object.getOwnPropertyDescriptor(Module, sym)) {
    Object.defineProperty(Module, sym, {
      configurable: true,
      get() {
        var msg = `'${sym}' was not exported. add it to EXPORTED_RUNTIME_METHODS (see the Emscripten FAQ)`;
        if (isExportedByForceFilesystem(sym)) {
          msg += '. Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you';
        }
        abort(msg);
      },
    });
  }
}

// end include: runtime_debug.js
// Memory management

var runtimeInitialized = false;



function updateMemoryViews() {
  var b = wasmMemory.buffer;
  HEAP8 = new Int8Array(b);
  HEAP16 = new Int16Array(b);
  HEAPU8 = new Uint8Array(b);
  HEAPU16 = new Uint16Array(b);
  HEAP32 = new Int32Array(b);
  HEAPU32 = new Uint32Array(b);
  HEAPF32 = new Float32Array(b);
  HEAPF64 = new Float64Array(b);
  HEAP64 = new BigInt64Array(b);
  HEAPU64 = new BigUint64Array(b);
}

// include: memoryprofiler.js
// end include: memoryprofiler.js
// end include: runtime_common.js
assert(globalThis.Int32Array && globalThis.Float64Array && Int32Array.prototype.subarray && Int32Array.prototype.set,
       'JS engine does not provide full typed array support');

function preRun() {
  if (Module['preRun']) {
    if (typeof Module['preRun'] == 'function') Module['preRun'] = [Module['preRun']];
    while (Module['preRun'].length) {
      addOnPreRun(Module['preRun'].shift());
    }
  }
  consumedModuleProp('preRun');
  // Begin ATPRERUNS hooks
  callRuntimeCallbacks(onPreRuns);
  // End ATPRERUNS hooks
}

function initRuntime() {
  assert(!runtimeInitialized);
  runtimeInitialized = true;

  checkStackCookie();

  // Begin ATINITS hooks
  if (!Module['noFSInit'] && !FS.initialized) FS.init();
TTY.init();
  // End ATINITS hooks

  wasmExports['__wasm_call_ctors']();

  // Begin ATPOSTCTORS hooks
  FS.ignorePermissions = false;
  // End ATPOSTCTORS hooks
}

function preMain() {
  checkStackCookie();
  // No ATMAINS hooks
}

function postRun() {
  checkStackCookie();
   // PThreads reuse the runtime from the main thread.

  if (Module['postRun']) {
    if (typeof Module['postRun'] == 'function') Module['postRun'] = [Module['postRun']];
    while (Module['postRun'].length) {
      addOnPostRun(Module['postRun'].shift());
    }
  }
  consumedModuleProp('postRun');

  // Begin ATPOSTRUNS hooks
  callRuntimeCallbacks(onPostRuns);
  // End ATPOSTRUNS hooks
}

/**
 * @param {string|number=} what
 */
function abort(what) {
  Module['onAbort']?.(what);

  what = `Aborted(${what})`;
  // TODO(sbc): Should we remove printing and leave it up to whoever
  // catches the exception?
  err(what);

  ABORT = true;

  // Use a wasm runtime error, because a JS error might be seen as a foreign
  // exception, which means we'd run destructors on it. We need the error to
  // simply make the program stop.
  // FIXME This approach does not work in Wasm EH because it currently does not assume
  // all RuntimeErrors are from traps; it decides whether a RuntimeError is from
  // a trap or not based on a hidden field within the object. So at the moment
  // we don't have a way of throwing a wasm trap from JS. TODO Make a JS API that
  // allows this in the wasm spec.

  // Suppress closure compiler warning here. Closure compiler's builtin extern
  // definition for WebAssembly.RuntimeError claims it takes no arguments even
  // though it can.
  // TODO(https://github.com/google/closure-compiler/pull/3913): Remove if/when upstream closure gets fixed.
  /** @suppress {checkTypes} */
  var e = new WebAssembly.RuntimeError(what);

  // Throw the error whether or not MODULARIZE is set because abort is used
  // in code paths apart from instantiation where an exception is expected
  // to be thrown when abort is called.
  throw e;
}

function createExportWrapper(name, nargs) {
  return (...args) => {
    assert(runtimeInitialized, `native function \`${name}\` called before runtime initialization`);
    var f = wasmExports[name];
    assert(f, `exported native function \`${name}\` not found`);
    // Only assert for too many arguments. Too few can be valid since the missing arguments will be zero filled.
    assert(args.length <= nargs, `native function \`${name}\` called with ${args.length} args but expects ${nargs}`);
    return f(...args);
  };
}

var wasmBinaryFile;

function findWasmBinary() {
  return locateFile('ggn.wasm');
}

function getBinarySync(file) {
  if (file == wasmBinaryFile && wasmBinary) {
    return new Uint8Array(wasmBinary);
  }
  if (readBinary) {
    return readBinary(file);
  }
  // Throwing a plain string here, even though it not normally advisable since
  // this gets turning into an `abort` in instantiateArrayBuffer.
  throw 'both async and sync fetching of the wasm failed';
}

async function getWasmBinary(binaryFile) {
  // If we don't have the binary yet, load it asynchronously using readAsync.
  if (!wasmBinary) {
    // Fetch the binary using readAsync
    try {
      var response = await readAsync(binaryFile);
      return new Uint8Array(response);
    } catch {
      // Fall back to getBinarySync below;
    }
  }

  // Otherwise, getBinarySync should be able to get it synchronously
  return getBinarySync(binaryFile);
}

async function instantiateArrayBuffer(binaryFile, imports) {
  try {
    var binary = await getWasmBinary(binaryFile);
    var instance = await WebAssembly.instantiate(binary, imports);
    return instance;
  } catch (reason) {
    err(`failed to asynchronously prepare wasm: ${reason}`);

    // Warn on some common problems.
    if (isFileURI(binaryFile)) {
      err(`warning: Loading from a file URI (${binaryFile}) is not supported in most browsers. See https://emscripten.org/docs/getting_started/FAQ.html#how-do-i-run-a-local-webserver-for-testing-why-does-my-program-stall-in-downloading-or-preparing`);
    }
    abort(reason);
  }
}

async function instantiateAsync(binary, binaryFile, imports) {
  if (!binary
      // Don't use streaming for file:// delivered objects in a webview, fetch them synchronously.
      && !isFileURI(binaryFile)
      // Avoid instantiateStreaming() on Node.js environment for now, as while
      // Node.js v18.1.0 implements it, it does not have a full fetch()
      // implementation yet.
      //
      // Reference:
      //   https://github.com/emscripten-core/emscripten/pull/16917
      && !ENVIRONMENT_IS_NODE
     ) {
    try {
      var response = fetch(binaryFile, { credentials: 'same-origin' });
      var instantiationResult = await WebAssembly.instantiateStreaming(response, imports);
      return instantiationResult;
    } catch (reason) {
      // We expect the most common failure cause to be a bad MIME type for the binary,
      // in which case falling back to ArrayBuffer instantiation should work.
      err(`wasm streaming compile failed: ${reason}`);
      err('falling back to ArrayBuffer instantiation');
      // fall back of instantiateArrayBuffer below
    };
  }
  return instantiateArrayBuffer(binaryFile, imports);
}

function getWasmImports() {
  // prepare imports
  var imports = {
    'env': wasmImports,
    'wasi_snapshot_preview1': wasmImports,
  };
  return imports;
}

// Create the wasm instance.
// Receives the wasm imports, returns the exports.
async function createWasm() {
  // Load the wasm module and create an instance of using native support in the JS engine.
  // handle a generated wasm instance, receiving its exports and
  // performing other necessary setup
  /** @param {WebAssembly.Module=} module*/
  function receiveInstance(instance, module) {
    wasmExports = instance.exports;

    assignWasmExports(wasmExports);

    updateMemoryViews();

    removeRunDependency('wasm-instantiate');
    return wasmExports;
  }
  addRunDependency('wasm-instantiate');

  // Prefer streaming instantiation if available.
  // Async compilation can be confusing when an error on the page overwrites Module
  // (for example, if the order of elements is wrong, and the one defining Module is
  // later), so we save Module and check it later.
  var trueModule = Module;
  function receiveInstantiationResult(result) {
    // 'result' is a ResultObject object which has both the module and instance.
    // receiveInstance() will swap in the exports (to Module.asm) so they can be called
    assert(Module === trueModule, 'the Module object should not be replaced during async compilation - perhaps the order of HTML elements is wrong?');
    trueModule = null;
    // TODO: Due to Closure regression https://github.com/google/closure-compiler/issues/3193, the above line no longer optimizes out down to the following line.
    // When the regression is fixed, can restore the above PTHREADS-enabled path.
    return receiveInstance(result['instance']);
  }

  var info = getWasmImports();

  // User shell pages can write their own Module.instantiateWasm = function(imports, successCallback) callback
  // to manually instantiate the Wasm module themselves. This allows pages to
  // run the instantiation parallel to any other async startup actions they are
  // performing.
  // Also pthreads and wasm workers initialize the wasm instance through this
  // path.
  if (Module['instantiateWasm']) {
    return new Promise((resolve, reject) => {
      try {
        Module['instantiateWasm'](info, (inst, mod) => {
          resolve(receiveInstance(inst, mod));
        });
      } catch(e) {
        err(`Module.instantiateWasm callback failed with error: ${e}`);
        reject(e);
      }
    });
  }

  wasmBinaryFile ??= findWasmBinary();
  var result = await instantiateAsync(wasmBinary, wasmBinaryFile, info);
  var exports = receiveInstantiationResult(result);
  return exports;
}

// end include: preamble.js

// Begin JS library code


  class ExitStatus {
      name = 'ExitStatus';
      constructor(status) {
        this.message = `Program terminated with exit(${status})`;
        this.status = status;
      }
    }

  /** @type {!Int16Array} */
  var HEAP16;

  /** @type {!Int32Array} */
  var HEAP32;

  /** not-@type {!BigInt64Array} */
  var HEAP64;

  /** @type {!Int8Array} */
  var HEAP8;

  /** @type {!Float32Array} */
  var HEAPF32;

  /** @type {!Float64Array} */
  var HEAPF64;

  /** @type {!Uint16Array} */
  var HEAPU16;

  /** @type {!Uint32Array} */
  var HEAPU32;

  /** not-@type {!BigUint64Array} */
  var HEAPU64;

  /** @type {!Uint8Array} */
  var HEAPU8;

  var callRuntimeCallbacks = (callbacks) => {
      while (callbacks.length > 0) {
        // Pass the module as the first argument.
        callbacks.shift()(Module);
      }
    };
  var onPostRuns = [];
  var addOnPostRun = (cb) => onPostRuns.push(cb);

  var onPreRuns = [];
  var addOnPreRun = (cb) => onPreRuns.push(cb);

  var runDependencies = 0;
  
  
  var dependenciesFulfilled = null;
  
  var runDependencyTracking = {
  };
  
  var runDependencyWatcher = null;
  var removeRunDependency = (id) => {
      runDependencies--;
  
      Module['monitorRunDependencies']?.(runDependencies);
  
      assert(id, 'removeRunDependency requires an ID');
      assert(runDependencyTracking[id]);
      delete runDependencyTracking[id];
      if (runDependencies == 0) {
        if (runDependencyWatcher !== null) {
          clearInterval(runDependencyWatcher);
          runDependencyWatcher = null;
        }
        if (dependenciesFulfilled) {
          var callback = dependenciesFulfilled;
          dependenciesFulfilled = null;
          callback(); // can add another dependenciesFulfilled
        }
      }
    };
  
  
  var addRunDependency = (id) => {
      runDependencies++;
  
      Module['monitorRunDependencies']?.(runDependencies);
  
      assert(id, 'addRunDependency requires an ID')
      assert(!runDependencyTracking[id]);
      runDependencyTracking[id] = 1;
      if (runDependencyWatcher === null && globalThis.setInterval) {
        // Check for missing dependencies every few seconds
        runDependencyWatcher = setInterval(() => {
          if (ABORT) {
            clearInterval(runDependencyWatcher);
            runDependencyWatcher = null;
            return;
          }
          var shown = false;
          for (var dep in runDependencyTracking) {
            if (!shown) {
              shown = true;
              err('still waiting on run dependencies:');
            }
            err(`dependency: ${dep}`);
          }
          if (shown) {
            err('(end of list)');
          }
        }, 10000);
        // Prevent this timer from keeping the runtime alive if nothing
        // else is.
        runDependencyWatcher.unref?.()
      }
    };


  
    /**
   * @param {number} ptr
   * @param {string} type
   */
  function getValue(ptr, type = 'i8') {
    if (type.endsWith('*')) type = '*';
    switch (type) {
      case 'i1': return HEAP8[ptr];
      case 'i8': return HEAP8[ptr];
      case 'i16': return HEAP16[((ptr)>>1)];
      case 'i32': return HEAP32[((ptr)>>2)];
      case 'i64': return HEAP64[((ptr)>>3)];
      case 'float': return HEAPF32[((ptr)>>2)];
      case 'double': return HEAPF64[((ptr)>>3)];
      case '*': return HEAPU32[((ptr)>>2)];
      default: abort(`invalid type for getValue: ${type}`);
    }
  }

  var noExitRuntime = true;

  function ptrToString(ptr) {
      assert(typeof ptr === 'number', `ptrToString expects a number, got ${typeof ptr}`);
      // Convert to 32-bit unsigned value
      ptr >>>= 0;
      return '0x' + ptr.toString(16).padStart(8, '0');
    }


  
    /**
   * @param {number} ptr
   * @param {number} value
   * @param {string} type
   */
  function setValue(ptr, value, type = 'i8') {
    if (type.endsWith('*')) type = '*';
    switch (type) {
      case 'i1': HEAP8[ptr] = value; break;
      case 'i8': HEAP8[ptr] = value; break;
      case 'i16': HEAP16[((ptr)>>1)] = value; break;
      case 'i32': HEAP32[((ptr)>>2)] = value; break;
      case 'i64': HEAP64[((ptr)>>3)] = BigInt(value); break;
      case 'float': HEAPF32[((ptr)>>2)] = value; break;
      case 'double': HEAPF64[((ptr)>>3)] = value; break;
      case '*': HEAPU32[((ptr)>>2)] = value; break;
      default: abort(`invalid type for setValue: ${type}`);
    }
  }

  var stackRestore = (val) => __emscripten_stack_restore(val);

  var stackSave = () => _emscripten_stack_get_current();

  var warnOnce = (text) => {
      warnOnce.shown ||= {};
      if (!warnOnce.shown[text]) {
        warnOnce.shown[text] = 1;
        if (ENVIRONMENT_IS_NODE) text = 'warning: ' + text;
        err(text);
      }
    };

  

  var UTF8Decoder = globalThis.TextDecoder && new TextDecoder();
  
  var findStringEnd = (heapOrArray, idx, maxBytesToRead, ignoreNul) => {
      var maxIdx = idx + maxBytesToRead;
      if (ignoreNul) return maxIdx;
      // TextDecoder needs to know the byte length in advance, it doesn't stop on
      // null terminator by itself.
      // As a tiny code save trick, compare idx against maxIdx using a negation,
      // so that maxBytesToRead=undefined/NaN means Infinity.
      while (heapOrArray[idx] && !(idx >= maxIdx)) ++idx;
      return idx;
    };
  
  
    /**
   * Given a pointer 'idx' to a null-terminated UTF8-encoded string in the given
   * array that contains uint8 values, returns a copy of that string as a
   * Javascript String object.
   * heapOrArray is either a regular array, or a JavaScript typed array view.
   * @param {number=} idx
   * @param {number=} maxBytesToRead
   * @param {boolean=} ignoreNul - If true, the function will not stop on a NUL character.
   * @return {string}
   */
  var UTF8ArrayToString = (heapOrArray, idx = 0, maxBytesToRead, ignoreNul) => {
  
      var endPtr = findStringEnd(heapOrArray, idx, maxBytesToRead, ignoreNul);
  
      // When using conditional TextDecoder, skip it for short strings as the overhead of the native call is not worth it.
      if (endPtr - idx > 16 && heapOrArray.buffer && UTF8Decoder) {
        return UTF8Decoder.decode(heapOrArray.subarray(idx, endPtr));
      }
      var str = '';
      while (idx < endPtr) {
        // For UTF8 byte structure, see:
        // http://en.wikipedia.org/wiki/UTF-8#Description
        // https://www.ietf.org/rfc/rfc2279.txt
        // https://tools.ietf.org/html/rfc3629
        var u0 = heapOrArray[idx++];
        if (!(u0 & 0x80)) { str += String.fromCharCode(u0); continue; }
        var u1 = heapOrArray[idx++] & 63;
        if ((u0 & 0xE0) == 0xC0) { str += String.fromCharCode(((u0 & 31) << 6) | u1); continue; }
        var u2 = heapOrArray[idx++] & 63;
        if ((u0 & 0xF0) == 0xE0) {
          u0 = ((u0 & 15) << 12) | (u1 << 6) | u2;
        } else {
          if ((u0 & 0xF8) != 0xF0) warnOnce(`Invalid UTF-8 leading byte ${ptrToString(u0)} encountered when deserializing a UTF-8 string in wasm memory to a JS string!`);
          u0 = ((u0 & 7) << 18) | (u1 << 12) | (u2 << 6) | (heapOrArray[idx++] & 63);
        }
  
        if (u0 < 0x10000) {
          str += String.fromCharCode(u0);
        } else {
          var ch = u0 - 0x10000;
          str += String.fromCharCode(0xD800 | (ch >> 10), 0xDC00 | (ch & 0x3FF));
        }
      }
      return str;
    };
  
    /**
   * Given a pointer 'ptr' to a null-terminated UTF8-encoded string in the
   * emscripten HEAP, returns a copy of that string as a Javascript String object.
   *
   * @param {number} ptr
   * @param {number=} maxBytesToRead - An optional length that specifies the
   *   maximum number of bytes to read. You can omit this parameter to scan the
   *   string until the first 0 byte. If maxBytesToRead is passed, and the string
   *   at [ptr, ptr+maxBytesToReadr[ contains a null byte in the middle, then the
   *   string will cut short at that byte index.
   * @param {boolean=} ignoreNul - If true, the function will not stop on a NUL character.
   * @return {string}
   */
  var UTF8ToString = (ptr, maxBytesToRead, ignoreNul) => {
      assert(typeof ptr == 'number', `UTF8ToString expects a number (got ${typeof ptr})`);
      return ptr ? UTF8ArrayToString(HEAPU8, ptr, maxBytesToRead, ignoreNul) : '';
    };
  var ___assert_fail = (condition, filename, line, func) =>
      abort(`Assertion failed: ${UTF8ToString(condition)}, at: ` + [filename ? UTF8ToString(filename) : 'unknown filename', line, func ? UTF8ToString(func) : 'unknown function']);

  var exceptionCaught =  [];
  
  
  var uncaughtExceptionCount = 0;
  var ___cxa_begin_catch = (ptr) => {
      var info = new ExceptionInfo(ptr);
      if (!info.get_caught()) {
        info.set_caught(true);
        uncaughtExceptionCount--;
      }
      info.set_rethrown(false);
      exceptionCaught.push(info);
      return ___cxa_get_exception_ptr(ptr);
    };

  
  
  
  var exceptionLast = null;
  var ___cxa_end_catch = () => {
      // Clear state flag.
      _setThrew(0, 0);
      assert(exceptionCaught.length > 0);
      // Call destructor if one is registered then clear it.
      var info = exceptionCaught.pop();
  
      ___cxa_decrement_exception_refcount(info.excPtr);
      exceptionLast = null; // XXX in decRef?
    };

  
  class ExceptionInfo {
      // excPtr - Thrown object pointer to wrap. Metadata pointer is calculated from it.
      constructor(excPtr) {
        this.excPtr = excPtr;
        this.ptr = excPtr - 24;
      }
  
      set_type(type) {
        HEAPU32[(((this.ptr)+(4))>>2)] = type;
      }
  
      get_type() {
        return HEAPU32[(((this.ptr)+(4))>>2)];
      }
  
      set_destructor(destructor) {
        HEAPU32[(((this.ptr)+(8))>>2)] = destructor;
      }
  
      get_destructor() {
        return HEAPU32[(((this.ptr)+(8))>>2)];
      }
  
      set_caught(caught) {
        caught = caught ? 1 : 0;
        HEAP8[(this.ptr)+(12)] = caught;
      }
  
      get_caught() {
        return HEAP8[(this.ptr)+(12)] != 0;
      }
  
      set_rethrown(rethrown) {
        rethrown = rethrown ? 1 : 0;
        HEAP8[(this.ptr)+(13)] = rethrown;
      }
  
      get_rethrown() {
        return HEAP8[(this.ptr)+(13)] != 0;
      }
  
      // Initialize native structure fields. Should be called once after allocated.
      init(type, destructor) {
        this.set_adjusted_ptr(0);
        this.set_type(type);
        this.set_destructor(destructor);
      }
  
      set_adjusted_ptr(adjustedPtr) {
        HEAPU32[(((this.ptr)+(16))>>2)] = adjustedPtr;
      }
  
      get_adjusted_ptr() {
        return HEAPU32[(((this.ptr)+(16))>>2)];
      }
    }
  
  
  var setTempRet0 = (val) => __emscripten_tempret_set(val);
  var findMatchingCatch = (args) => {
      var thrown = exceptionLast?.excPtr;
      if (!thrown) {
        // just pass through the null ptr
        setTempRet0(0);
        return 0;
      }
      var info = new ExceptionInfo(thrown);
      info.set_adjusted_ptr(thrown);
      var thrownType = info.get_type();
      if (!thrownType) {
        // just pass through the thrown ptr
        setTempRet0(0);
        return thrown;
      }
  
      // can_catch receives a **, add indirection
      // The different catch blocks are denoted by different types.
      // Due to inheritance, those types may not precisely match the
      // type of the thrown object. Find one which matches, and
      // return the type of the catch block which should be called.
      for (var caughtType of args) {
        if (caughtType === 0 || caughtType === thrownType) {
          // Catch all clause matched or exactly the same type is caught
          break;
        }
        var adjusted_ptr_addr = info.ptr + 16;
        if (___cxa_can_catch(caughtType, thrownType, adjusted_ptr_addr)) {
          setTempRet0(caughtType);
          return thrown;
        }
      }
      setTempRet0(thrownType);
      return thrown;
    };
  var ___cxa_find_matching_catch_2 = () => findMatchingCatch([]);

  var ___cxa_find_matching_catch_3 = (arg0) => findMatchingCatch([arg0]);

  var ___cxa_find_matching_catch_8 = (arg0,arg1,arg2,arg3,arg4,arg5) => findMatchingCatch([arg0,arg1,arg2,arg3,arg4,arg5]);

  
  
  
  var ___cxa_rethrow = () => {
      if (!exceptionCaught.length) {
        abort('no exception to throw');
      }
      var info = exceptionCaught.at(-1);
      var ptr = info.excPtr;
      info.set_rethrown(true);
      info.set_caught(false);
      uncaughtExceptionCount++;
      ___cxa_increment_exception_refcount(ptr);
      exceptionLast = new CppException(ptr);
      throw exceptionLast;
    };

  
  
  
  
  
  
  
  
  var stackAlloc = (sz) => __emscripten_stack_alloc(sz);
  
  var getExceptionMessageCommon = (ptr) => {
      var sp = stackSave();
      var type_addr_addr = stackAlloc(4);
      var message_addr_addr = stackAlloc(4);
      ___get_exception_message(ptr, type_addr_addr, message_addr_addr);
      var type_addr = HEAPU32[((type_addr_addr)>>2)];
      var message_addr = HEAPU32[((message_addr_addr)>>2)];
      var type = UTF8ToString(type_addr);
      _free(type_addr);
      var message;
      if (message_addr) {
        message = UTF8ToString(message_addr);
        _free(message_addr);
      }
      stackRestore(sp);
      return [type, message];
    };
  var getExceptionMessage = (exn) => getExceptionMessageCommon(exn.excPtr);
  
  var decrementExceptionRefcount = (exn) => ___cxa_decrement_exception_refcount(exn.excPtr);
  
  var incrementExceptionRefcount = (exn) => ___cxa_increment_exception_refcount(exn.excPtr);
  var ___cxa_throw = (ptr, type, destructor) => {
      var info = new ExceptionInfo(ptr);
      // Initialize ExceptionInfo content after it was allocated in __cxa_allocate_exception.
      info.init(type, destructor);
      ___cxa_increment_exception_refcount(ptr);
      exceptionLast = new CppException(ptr);
      uncaughtExceptionCount++;
      throw exceptionLast;
    };

  var ___cxa_uncaught_exceptions = () => uncaughtExceptionCount;

  var ___resumeException = (ptr) => {
      if (!exceptionLast) {
        exceptionLast = new CppException(ptr);
      }
      throw exceptionLast;
    };

  var PATH = {
  isAbs:(path) => path.charAt(0) === '/',
  splitPath:(filename) => {
        var splitPathRe = /^(\/?|)([\s\S]*?)((?:\.{1,2}|[^\/]+?|)(\.[^.\/]*|))(?:[\/]*)$/;
        return splitPathRe.exec(filename).slice(1);
      },
  normalizeArray:(parts, allowAboveRoot) => {
        // if the path tries to go above the root, `up` ends up > 0
        var up = 0;
        for (var i = parts.length - 1; i >= 0; i--) {
          var last = parts[i];
          if (last === '.') {
            parts.splice(i, 1);
          } else if (last === '..') {
            parts.splice(i, 1);
            up++;
          } else if (up) {
            parts.splice(i, 1);
            up--;
          }
        }
        // if the path is allowed to go above the root, restore leading ..s
        if (allowAboveRoot) {
          for (; up; up--) {
            parts.unshift('..');
          }
        }
        return parts;
      },
  normalize:(path) => {
        var isAbsolute = PATH.isAbs(path),
            trailingSlash = path.slice(-1) === '/';
        // Normalize the path
        path = PATH.normalizeArray(path.split('/').filter((p) => !!p), !isAbsolute).join('/');
        if (!path && !isAbsolute) {
          path = '.';
        }
        if (path && trailingSlash) {
          path += '/';
        }
        return (isAbsolute ? '/' : '') + path;
      },
  dirname:(path) => {
        var result = PATH.splitPath(path),
            root = result[0],
            dir = result[1];
        if (!root && !dir) {
          // No dirname whatsoever
          return '.';
        }
        if (dir) {
          // It has a dirname, strip trailing slash
          dir = dir.slice(0, -1);
        }
        return root + dir;
      },
  basename:(path) => path && path.match(/([^\/]+|\/)\/*$/)[1],
join:(...paths) => PATH.normalize(paths.join('/')),
join2:(l, r) => PATH.normalize(l + '/' + r),
};

var initRandomFill = () => {
    // This block is not needed on v19+ since crypto.getRandomValues is builtin
    if (ENVIRONMENT_IS_NODE) {
      var nodeCrypto = require('node:crypto');
      return (view) => nodeCrypto.randomFillSync(view);
    }

    return (view) => (crypto.getRandomValues(view), 0);
  };
var randomFill = (view) => (randomFill = initRandomFill())(view);



var PATH_FS = {
resolve:(...args) => {
      var resolvedPath = '',
        resolvedAbsolute = false;
      for (var i = args.length - 1; i >= -1 && !resolvedAbsolute; i--) {
        var path = (i >= 0) ? args[i] : FS.cwd();
        // Skip empty and invalid entries
        if (typeof path != 'string') {
          throw new TypeError('Arguments to path.resolve must be strings');
        } else if (!path) {
          return ''; // an invalid portion invalidates the whole thing
        }
        resolvedPath = path + '/' + resolvedPath;
        resolvedAbsolute = PATH.isAbs(path);
      }
      // At this point the path should be resolved to a full absolute path, but
      // handle relative paths to be safe (might happen when process.cwd() fails)
      resolvedPath = PATH.normalizeArray(resolvedPath.split('/').filter((p) => !!p), !resolvedAbsolute).join('/');
      return ((resolvedAbsolute ? '/' : '') + resolvedPath) || '.';
    },
relative:(from, to) => {
      from = PATH_FS.resolve(from).slice(1);
      to = PATH_FS.resolve(to).slice(1);
      function trim(arr) {
        var start = 0;
        for (; start < arr.length; start++) {
          if (arr[start] !== '') break;
        }
        var end = arr.length - 1;
        for (; end >= 0; end--) {
          if (arr[end] !== '') break;
        }
        if (start > end) return [];
        return arr.slice(start, end - start + 1);
      }
      var fromParts = trim(from.split('/'));
      var toParts = trim(to.split('/'));
      var length = Math.min(fromParts.length, toParts.length);
      var samePartsLength = length;
      for (var i = 0; i < length; i++) {
        if (fromParts[i] !== toParts[i]) {
          samePartsLength = i;
          break;
        }
      }
      var outputParts = [];
      for (var i = samePartsLength; i < fromParts.length; i++) {
        outputParts.push('..');
      }
      outputParts = outputParts.concat(toParts.slice(samePartsLength));
      return outputParts.join('/');
    },
};



var FS_stdin_getChar_buffer = [];

var lengthBytesUTF8 = (str) => {
    var len = 0;
    for (var i = 0; i < str.length; ++i) {
      // Gotcha: charCodeAt returns a 16-bit word that is a UTF-16 encoded code
      // unit, not a Unicode code point of the character! So decode
      // UTF16->UTF32->UTF8.
      // See http://unicode.org/faq/utf_bom.html#utf16-3
      var c = str.charCodeAt(i); // possibly a lead surrogate
      if (c <= 0x7F) {
        len++;
      } else if (c <= 0x7FF) {
        len += 2;
      } else if (c >= 0xD800 && c <= 0xDFFF) {
        len += 4; ++i;
      } else {
        len += 3;
      }
    }
    return len;
  };

var stringToUTF8Array = (str, heap, outIdx, maxBytesToWrite) => {
    assert(typeof str === 'string', `stringToUTF8Array expects a string (got ${typeof str})`);
    // Parameter maxBytesToWrite is not optional. Negative values, 0, null,
    // undefined and false each don't write out any bytes.
    if (!(maxBytesToWrite > 0))
      return 0;

    var startIdx = outIdx;
    var endIdx = outIdx + maxBytesToWrite - 1; // -1 for string null terminator.
    for (var i = 0; i < str.length; ++i) {
      // For UTF8 byte structure, see http://en.wikipedia.org/wiki/UTF-8#Description
      // and https://www.ietf.org/rfc/rfc2279.txt
      // and https://tools.ietf.org/html/rfc3629
      var u = str.codePointAt(i);
      if (u <= 0x7F) {
        if (outIdx >= endIdx) break;
        heap[outIdx++] = u;
      } else if (u <= 0x7FF) {
        if (outIdx + 1 >= endIdx) break;
        heap[outIdx++] = 0xC0 | (u >> 6);
        heap[outIdx++] = 0x80 | (u & 63);
      } else if (u <= 0xFFFF) {
        if (outIdx + 2 >= endIdx) break;
        heap[outIdx++] = 0xE0 | (u >> 12);
        heap[outIdx++] = 0x80 | ((u >> 6) & 63);
        heap[outIdx++] = 0x80 | (u & 63);
      } else {
        if (outIdx + 3 >= endIdx) break;
        if (u > 0x10FFFF) warnOnce(`Invalid Unicode code point ${ptrToString(u)} encountered when serializing a JS string to a UTF-8 string in wasm memory! (Valid unicode code points should be in range 0-0x10FFFF).`);
        heap[outIdx++] = 0xF0 | (u >> 18);
        heap[outIdx++] = 0x80 | ((u >> 12) & 63);
        heap[outIdx++] = 0x80 | ((u >> 6) & 63);
        heap[outIdx++] = 0x80 | (u & 63);
        // Gotcha: if codePoint is over 0xFFFF, it is represented as a surrogate pair in UTF-16.
        // We need to manually skip over the second code unit for correct iteration.
        i++;
      }
    }
    // Null-terminate the pointer to the buffer.
    heap[outIdx] = 0;
    return outIdx - startIdx;
  };
/** @type {function(string, boolean=, number=)} */
  var intArrayFromString = (stringy, dontAddNull, length) => {
      var len = length > 0 ? length : lengthBytesUTF8(stringy)+1;
      var u8array = new Array(len);
      var numBytesWritten = stringToUTF8Array(stringy, u8array, 0, u8array.length);
      if (dontAddNull) u8array.length = numBytesWritten;
      return u8array;
    };
  var FS_stdin_getChar = () => {
      if (!FS_stdin_getChar_buffer.length) {
        var result = null;
        if (ENVIRONMENT_IS_NODE) {
          // we will read data by chunks of BUFSIZE
          var BUFSIZE = 256;
          var buf = Buffer.alloc(BUFSIZE);
          var bytesRead = 0;
  
          // For some reason we must suppress a closure warning here, even though
          // fd definitely exists on process.stdin, and is even the proper way to
          // get the fd of stdin,
          // https://github.com/nodejs/help/issues/2136#issuecomment-523649904
          // This started to happen after moving this logic out of library_tty.js,
          // so it is related to the surrounding code in some unclear manner.
          /** @suppress {missingProperties} */
          var fd = process.stdin.fd;
  
          try {
            bytesRead = fs.readSync(fd, buf, 0, BUFSIZE);
          } catch(e) {
            // Cross-platform differences: on Windows, reading EOF throws an
            // exception, but on other OSes, reading EOF returns 0. Uniformize
            // behavior by treating the EOF exception to return 0.
            if (e.toString().includes('EOF')) bytesRead = 0;
            else throw e;
          }
  
          if (bytesRead > 0) {
            result = buf.slice(0, bytesRead).toString('utf-8');
          }
        } else
        if (globalThis.window?.prompt) {
          // Browser.
          result = window.prompt('Input: ');  // returns null on cancel
          if (result !== null) {
            result += '\n';
          }
        } else
        {}
        if (!result) {
          return null;
        }
        FS_stdin_getChar_buffer = intArrayFromString(result, true);
      }
      return FS_stdin_getChar_buffer.shift();
    };
  var TTY = {
  ttys:[],
  init() {
        // https://github.com/emscripten-core/emscripten/pull/1555
        // if (ENVIRONMENT_IS_NODE) {
        //   // currently, FS.init does not distinguish if process.stdin is a file or TTY
        //   // device, it always assumes it's a TTY device. because of this, we're forcing
        //   // process.stdin to UTF8 encoding to at least make stdin reading compatible
        //   // with text files until FS.init can be refactored.
        //   process.stdin.setEncoding('utf8');
        // }
      },
  shutdown() {
        // https://github.com/emscripten-core/emscripten/pull/1555
        // if (ENVIRONMENT_IS_NODE) {
        //   // inolen: any idea as to why node -e 'process.stdin.read()' wouldn't exit immediately (with process.stdin being a tty)?
        //   // isaacs: because now it's reading from the stream, you've expressed interest in it, so that read() kicks off a _read() which creates a ReadReq operation
        //   // inolen: I thought read() in that case was a synchronous operation that just grabbed some amount of buffered data if it exists?
        //   // isaacs: it is. but it also triggers a _read() call, which calls readStart() on the handle
        //   // isaacs: do process.stdin.pause() and i'd think it'd probably close the pending call
        //   process.stdin.pause();
        // }
      },
  register(dev, ops) {
        TTY.ttys[dev] = { input: [], output: [], ops: ops };
        FS.registerDevice(dev, TTY.stream_ops);
      },
  stream_ops:{
  open(stream) {
          var tty = TTY.ttys[stream.node.rdev];
          if (!tty) {
            throw new FS.ErrnoError(43);
          }
          stream.tty = tty;
          stream.seekable = false;
        },
  close(stream) {
          // flush any pending line data
          stream.tty.ops.fsync(stream.tty);
        },
  fsync(stream) {
          stream.tty.ops.fsync(stream.tty);
        },
  read(stream, buffer, offset, length, pos /* ignored */) {
          if (!stream.tty || !stream.tty.ops.get_char) {
            throw new FS.ErrnoError(60);
          }
          var bytesRead = 0;
          for (var i = 0; i < length; i++) {
            var result;
            try {
              result = stream.tty.ops.get_char(stream.tty);
            } catch (e) {
              throw new FS.ErrnoError(29);
            }
            if (result === undefined && bytesRead === 0) {
              throw new FS.ErrnoError(6);
            }
            if (result === null || result === undefined) break;
            bytesRead++;
            buffer[offset+i] = result;
          }
          if (bytesRead) {
            stream.node.atime = Date.now();
          }
          return bytesRead;
        },
  write(stream, buffer, offset, length, pos) {
          if (!stream.tty || !stream.tty.ops.put_char) {
            throw new FS.ErrnoError(60);
          }
          try {
            for (var i = 0; i < length; i++) {
              stream.tty.ops.put_char(stream.tty, buffer[offset+i]);
            }
          } catch (e) {
            throw new FS.ErrnoError(29);
          }
          if (length) {
            stream.node.mtime = stream.node.ctime = Date.now();
          }
          return i;
        },
  },
  default_tty_ops:{
  get_char(tty) {
          return FS_stdin_getChar();
        },
  put_char(tty, val) {
          if (val === null || val === 10) {
            out(UTF8ArrayToString(tty.output));
            tty.output = [];
          } else {
            if (val != 0) tty.output.push(val); // val == 0 would cut text output off in the middle.
          }
        },
  fsync(tty) {
          if (tty.output?.length > 0) {
            out(UTF8ArrayToString(tty.output));
            tty.output = [];
          }
        },
  ioctl_tcgets(tty) {
          // typical setting
          return {
            c_iflag: 25856,
            c_oflag: 5,
            c_cflag: 191,
            c_lflag: 35387,
            c_cc: [
              0x03, 0x1c, 0x7f, 0x15, 0x04, 0x00, 0x01, 0x00, 0x11, 0x13, 0x1a, 0x00,
              0x12, 0x0f, 0x17, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ]
          };
        },
  ioctl_tcsets(tty, optional_actions, data) {
          // currently just ignore
          return 0;
        },
  ioctl_tiocgwinsz(tty) {
          return [24, 80];
        },
  },
  default_tty1_ops:{
  put_char(tty, val) {
          if (val === null || val === 10) {
            err(UTF8ArrayToString(tty.output));
            tty.output = [];
          } else {
            if (val != 0) tty.output.push(val);
          }
        },
  fsync(tty) {
          if (tty.output?.length > 0) {
            err(UTF8ArrayToString(tty.output));
            tty.output = [];
          }
        },
  },
  };
  
  
  var mmapAlloc = (size) => {
      abort('internal error: mmapAlloc called but `emscripten_builtin_memalign` native symbol not exported');
    };
  var MEMFS = {
  ops_table:null,
  mount(mount) {
        return MEMFS.createNode(null, '/', 16895, 0);
      },
  createNode(parent, name, mode, dev) {
        if (FS.isBlkdev(mode) || FS.isFIFO(mode)) {
          // not supported
          throw new FS.ErrnoError(63);
        }
        MEMFS.ops_table ||= {
          dir: {
            node: {
              getattr: MEMFS.node_ops.getattr,
              setattr: MEMFS.node_ops.setattr,
              lookup: MEMFS.node_ops.lookup,
              mknod: MEMFS.node_ops.mknod,
              rename: MEMFS.node_ops.rename,
              unlink: MEMFS.node_ops.unlink,
              rmdir: MEMFS.node_ops.rmdir,
              readdir: MEMFS.node_ops.readdir,
              symlink: MEMFS.node_ops.symlink
            },
            stream: {
              llseek: MEMFS.stream_ops.llseek
            }
          },
          file: {
            node: {
              getattr: MEMFS.node_ops.getattr,
              setattr: MEMFS.node_ops.setattr
            },
            stream: {
              llseek: MEMFS.stream_ops.llseek,
              read: MEMFS.stream_ops.read,
              write: MEMFS.stream_ops.write,
              mmap: MEMFS.stream_ops.mmap,
              msync: MEMFS.stream_ops.msync
            }
          },
          link: {
            node: {
              getattr: MEMFS.node_ops.getattr,
              setattr: MEMFS.node_ops.setattr,
              readlink: MEMFS.node_ops.readlink
            },
            stream: {}
          },
          chrdev: {
            node: {
              getattr: MEMFS.node_ops.getattr,
              setattr: MEMFS.node_ops.setattr
            },
            stream: FS.chrdev_stream_ops
          }
        };
        var node = FS.createNode(parent, name, mode, dev);
        if (FS.isDir(node.mode)) {
          node.node_ops = MEMFS.ops_table.dir.node;
          node.stream_ops = MEMFS.ops_table.dir.stream;
          node.contents = {};
        } else if (FS.isFile(node.mode)) {
          node.node_ops = MEMFS.ops_table.file.node;
          node.stream_ops = MEMFS.ops_table.file.stream;
          // The actual number of bytes used in the typed array, as opposed to
          // contents.length which gives the whole capacity.
          node.usedBytes = 0;
          // The byte data of the file is stored in a typed array.
          // Note: typed arrays are not resizable like normal JS arrays are, so
          // there is a small penalty involved for appending file writes that
          // continuously grow a file similar to std::vector capacity vs used.
          node.contents = MEMFS.emptyFileContents ??= new Uint8Array(0);
        } else if (FS.isLink(node.mode)) {
          node.node_ops = MEMFS.ops_table.link.node;
          node.stream_ops = MEMFS.ops_table.link.stream;
        } else if (FS.isChrdev(node.mode)) {
          node.node_ops = MEMFS.ops_table.chrdev.node;
          node.stream_ops = MEMFS.ops_table.chrdev.stream;
        }
        node.atime = node.mtime = node.ctime = Date.now();
        // add the new node to the parent
        if (parent) {
          parent.contents[name] = node;
          parent.atime = parent.mtime = parent.ctime = node.atime;
        }
        return node;
      },
  getFileDataAsTypedArray(node) {
        assert(FS.isFile(node.mode), 'getFileDataAsTypedArray called on non-file');
        return node.contents.subarray(0, node.usedBytes); // Make sure to not return excess unused bytes.
      },
  expandFileStorage(node, newCapacity) {
        var prevCapacity = node.contents.length;
        if (prevCapacity >= newCapacity) return; // No need to expand, the storage was already large enough.
        // Don't expand strictly to the given requested limit if it's only a very
        // small increase, but instead geometrically grow capacity.
        // For small filesizes (<1MB), perform size*2 geometric increase, but for
        // large sizes, do a much more conservative size*1.125 increase to avoid
        // overshooting the allocation cap by a very large margin.
        var CAPACITY_DOUBLING_MAX = 1024 * 1024;
        newCapacity = Math.max(newCapacity, (prevCapacity * (prevCapacity < CAPACITY_DOUBLING_MAX ? 2.0 : 1.125)) >>> 0);
        if (prevCapacity) newCapacity = Math.max(newCapacity, 256); // At minimum allocate 256b for each file when expanding.
        var oldContents = MEMFS.getFileDataAsTypedArray(node);
        node.contents = new Uint8Array(newCapacity); // Allocate new storage.
        node.contents.set(oldContents);
      },
  resizeFileStorage(node, newSize) {
        if (node.usedBytes == newSize) return;
        var oldContents = node.contents;
        node.contents = new Uint8Array(newSize); // Allocate new storage.
        node.contents.set(oldContents.subarray(0, Math.min(newSize, node.usedBytes))); // Copy old data over to the new storage.
        node.usedBytes = newSize;
      },
  node_ops:{
  getattr(node) {
          var attr = {};
          // device numbers reuse inode numbers.
          attr.dev = FS.isChrdev(node.mode) ? node.id : 1;
          attr.ino = node.id;
          attr.mode = node.mode;
          attr.nlink = 1;
          attr.uid = 0;
          attr.gid = 0;
          attr.rdev = node.rdev;
          if (FS.isDir(node.mode)) {
            attr.size = 4096;
          } else if (FS.isFile(node.mode)) {
            attr.size = node.usedBytes;
          } else if (FS.isLink(node.mode)) {
            attr.size = node.link.length;
          } else {
            attr.size = 0;
          }
          attr.atime = new Date(node.atime);
          attr.mtime = new Date(node.mtime);
          attr.ctime = new Date(node.ctime);
          // NOTE: In our implementation, st_blocks = Math.ceil(st_size/st_blksize),
          //       but this is not required by the standard.
          attr.blksize = 4096;
          attr.blocks = Math.ceil(attr.size / attr.blksize);
          return attr;
        },
  setattr(node, attr) {
          for (const key of ["mode", "atime", "mtime", "ctime"]) {
            if (attr[key] != null) {
              node[key] = attr[key];
            }
          }
          if (attr.size !== undefined) {
            MEMFS.resizeFileStorage(node, attr.size);
          }
        },
  lookup(parent, name) {
          throw new FS.ErrnoError(44);
        },
  mknod(parent, name, mode, dev) {
          return MEMFS.createNode(parent, name, mode, dev);
        },
  rename(old_node, new_dir, new_name) {
          var new_node;
          try {
            new_node = FS.lookupNode(new_dir, new_name);
          } catch (e) {}
          if (new_node) {
            if (FS.isDir(old_node.mode)) {
              // if we're overwriting a directory at new_name, make sure it's empty.
              for (var i in new_node.contents) {
                throw new FS.ErrnoError(55);
              }
            }
            FS.hashRemoveNode(new_node);
          }
          // do the internal rewiring
          delete old_node.parent.contents[old_node.name];
          new_dir.contents[new_name] = old_node;
          old_node.name = new_name;
          new_dir.ctime = new_dir.mtime = old_node.parent.ctime = old_node.parent.mtime = Date.now();
        },
  unlink(parent, name) {
          delete parent.contents[name];
          parent.ctime = parent.mtime = Date.now();
        },
  rmdir(parent, name) {
          var node = FS.lookupNode(parent, name);
          for (var i in node.contents) {
            throw new FS.ErrnoError(55);
          }
          delete parent.contents[name];
          parent.ctime = parent.mtime = Date.now();
        },
  readdir(node) {
          return ['.', '..', ...Object.keys(node.contents)];
        },
  symlink(parent, newname, oldpath) {
          var node = MEMFS.createNode(parent, newname, 0o777 | 40960, 0);
          node.link = oldpath;
          return node;
        },
  readlink(node) {
          if (!FS.isLink(node.mode)) {
            throw new FS.ErrnoError(28);
          }
          return node.link;
        },
  },
  stream_ops:{
  read(stream, buffer, offset, length, position) {
          var contents = stream.node.contents;
          if (position >= stream.node.usedBytes) return 0;
          var size = Math.min(stream.node.usedBytes - position, length);
          assert(size >= 0);
          buffer.set(contents.subarray(position, position + size), offset);
          return size;
        },
  write(stream, buffer, offset, length, position, canOwn) {
          assert(buffer.subarray, 'FS.write expects a TypedArray');
          // If the buffer is located in main memory (HEAP), and if
          // memory can grow, we can't hold on to references of the
          // memory buffer, as they may get invalidated. That means we
          // need to copy its contents.
          if (buffer.buffer === HEAP8.buffer) {
            canOwn = false;
          }
  
          if (!length) return 0;
          var node = stream.node;
          node.mtime = node.ctime = Date.now();
  
          if (canOwn) {
            assert(position === 0, 'canOwn must imply no weird position inside the file');
            node.contents = buffer.subarray(offset, offset + length);
            node.usedBytes = length;
          } else if (node.usedBytes === 0 && position === 0) { // If this is a simple first write to an empty file, do a fast set since we don't need to care about old data.
            node.contents = buffer.slice(offset, offset + length);
            node.usedBytes = length;
          } else {
            MEMFS.expandFileStorage(node, position+length);
            // Use typed array write which is available.
            node.contents.set(buffer.subarray(offset, offset + length), position);
            node.usedBytes = Math.max(node.usedBytes, position + length);
          }
          return length;
        },
  llseek(stream, offset, whence) {
          var position = offset;
          if (whence === 1) {
            position += stream.position;
          } else if (whence === 2) {
            if (FS.isFile(stream.node.mode)) {
              position += stream.node.usedBytes;
            }
          }
          if (position < 0) {
            throw new FS.ErrnoError(28);
          }
          return position;
        },
  mmap(stream, length, position, prot, flags) {
          if (!FS.isFile(stream.node.mode)) {
            throw new FS.ErrnoError(43);
          }
          var ptr;
          var allocated;
          var contents = stream.node.contents;
          // Only make a new copy when MAP_PRIVATE is specified.
          if (!(flags & 2) && contents.buffer === HEAP8.buffer) {
            // We can't emulate MAP_SHARED when the file is not backed by the
            // buffer we're mapping to (e.g. the HEAP buffer).
            allocated = false;
            ptr = contents.byteOffset;
          } else {
            allocated = true;
            ptr = mmapAlloc(length);
            if (!ptr) {
              throw new FS.ErrnoError(48);
            }
            if (contents) {
              // Try to avoid unnecessary slices.
              if (position > 0 || position + length < contents.length) {
                if (contents.subarray) {
                  contents = contents.subarray(position, position + length);
                } else {
                  contents = Array.prototype.slice.call(contents, position, position + length);
                }
              }
              HEAP8.set(contents, ptr);
            }
          }
          return { ptr, allocated };
        },
  msync(stream, buffer, offset, length, mmapFlags) {
          MEMFS.stream_ops.write(stream, buffer, 0, length, offset, false);
          // should we check if bytesWritten and length are the same?
          return 0;
        },
  },
  };
  
  var FS_modeStringToFlags = (str) => {
      if (typeof str != 'string') return str;
      var flagModes = {
        'r': 0,
        'r+': 2,
        'w': 512 | 64 | 1,
        'w+': 512 | 64 | 2,
        'a': 1024 | 64 | 1,
        'a+': 1024 | 64 | 2,
      };
      var flags = flagModes[str];
      if (typeof flags == 'undefined') {
        throw new Error(`Unknown file open mode: ${str}`);
      }
      return flags;
    };
  
  var FS_fileDataToTypedArray = (data) => {
      if (typeof data == 'string') {
        data = intArrayFromString(data, true);
      }
      if (!data.subarray) {
        data = new Uint8Array(data);
      }
      return data;
    };
  
  var FS_getMode = (canRead, canWrite) => {
      var mode = 0;
      if (canRead) mode |= 292 | 73;
      if (canWrite) mode |= 146;
      return mode;
    };
  
  
  
  
  var IDBFS = {
  dbs:{
  },
  indexedDB:() => {
        assert(typeof indexedDB != 'undefined', 'IDBFS used, but indexedDB not supported');
        return indexedDB;
      },
  DB_VERSION:21,
  DB_STORE_NAME:"FILE_DATA",
  queuePersist:(mount) => {
        function onPersistComplete() {
          if (mount.idbPersistState === 'again') startPersist(); // If a new sync request has appeared in between, kick off a new sync
          else {
            mount.idbPersistState = 0; // Otherwise reset sync state back to idle to wait for a new sync later
            IDBFS.onAutoPersistStateChanged?.(false);
          }
        }
        function startPersist() {
          mount.idbPersistState = 'idb'; // Mark that we are currently running a sync operation
          IDBFS.onAutoPersistStateChanged?.(true);
          IDBFS.syncfs(mount, /*populate:*/false, onPersistComplete);
        }
  
        if (!mount.idbPersistState) {
          // Programs typically write/copy/move multiple files in the in-memory
          // filesystem within a single app frame, so when a filesystem sync
          // command is triggered, do not start it immediately, but only after
          // the current frame is finished. This way all the modified files
          // inside the main loop tick will be batched up to the same sync.
          mount.idbPersistState = setTimeout(startPersist, 0);
        } else if (mount.idbPersistState === 'idb') {
          // There is an active IndexedDB sync operation in-flight, but we now
          // have accumulated more files to sync. We should therefore queue up
          // a new sync after the current one finishes so that all writes
          // will be properly persisted.
          mount.idbPersistState = 'again';
        }
      },
  mount:(mount) => {
        // reuse core MEMFS functionality
        var mnt = MEMFS.mount(mount);
        // If the automatic IDBFS persistence option has been selected, then automatically persist
        // all modifications to the filesystem as they occur.
        if (mount?.opts?.autoPersist) {
          mount.idbPersistState = 0; // IndexedDB sync starts in idle state
          var memfs_node_ops = mnt.node_ops;
          mnt.node_ops = {...mnt.node_ops}; // Clone node_ops to inject write tracking
          mnt.node_ops.mknod = (parent, name, mode, dev) => {
            var node = memfs_node_ops.mknod(parent, name, mode, dev);
            // Propagate injected node_ops to the newly created child node
            node.node_ops = mnt.node_ops;
            // Remember for each IDBFS node which IDBFS mount point they came from so we know which mount to persist on modification.
            node.idbfs_mount = mnt.mount;
            // Remember original MEMFS stream_ops for this node
            node.memfs_stream_ops = node.stream_ops;
            // Clone stream_ops to inject write tracking
            node.stream_ops = {...node.stream_ops};
  
            // Track all file writes
            node.stream_ops.write = (stream, buffer, offset, length, position, canOwn) => {
              // This file has been modified, we must persist IndexedDB when this file closes
              stream.node.isModified = true;
              return node.memfs_stream_ops.write(stream, buffer, offset, length, position, canOwn);
            };
  
            // Persist IndexedDB on file close
            node.stream_ops.close = (stream) => {
              var n = stream.node;
              if (n.isModified) {
                IDBFS.queuePersist(n.idbfs_mount);
                n.isModified = false;
              }
              if (n.memfs_stream_ops.close) return n.memfs_stream_ops.close(stream);
            };
  
            // Persist the node we just created to IndexedDB
            IDBFS.queuePersist(mnt.mount);
  
            return node;
          };
          // Also kick off persisting the filesystem on other operations that modify the filesystem.
          mnt.node_ops.rmdir   = (...args) => (IDBFS.queuePersist(mnt.mount), memfs_node_ops.rmdir(...args));
          mnt.node_ops.symlink = (...args) => (IDBFS.queuePersist(mnt.mount), memfs_node_ops.symlink(...args));
          mnt.node_ops.unlink  = (...args) => (IDBFS.queuePersist(mnt.mount), memfs_node_ops.unlink(...args));
          mnt.node_ops.rename  = (...args) => (IDBFS.queuePersist(mnt.mount), memfs_node_ops.rename(...args));
        }
        return mnt;
      },
  syncfs:(mount, populate, callback) => {
        IDBFS.getLocalSet(mount, (err, local) => {
          if (err) return callback(err);
  
          IDBFS.getRemoteSet(mount, (err, remote) => {
            if (err) return callback(err);
  
            var src = populate ? remote : local;
            var dst = populate ? local : remote;
  
            IDBFS.reconcile(src, dst, callback);
          });
        });
      },
  quit:() => {
        for (var value of Object.values(IDBFS.dbs)) {
          value.close()
        }
        IDBFS.dbs = {};
      },
  getDB:(name, callback) => {
        // check the cache first
        var db = IDBFS.dbs[name];
        if (db) {
          return callback(null, db);
        }
  
        var req;
        try {
          req = IDBFS.indexedDB().open(name, IDBFS.DB_VERSION);
        } catch (e) {
          return callback(e);
        }
        if (!req) {
          return callback("Unable to connect to IndexedDB");
        }
        req.onupgradeneeded = (e) => {
          var db = /** @type {IDBDatabase} */ (e.target.result);
          var transaction = e.target.transaction;
  
          var fileStore;
  
          if (db.objectStoreNames.contains(IDBFS.DB_STORE_NAME)) {
            fileStore = transaction.objectStore(IDBFS.DB_STORE_NAME);
          } else {
            fileStore = db.createObjectStore(IDBFS.DB_STORE_NAME);
          }
  
          if (!fileStore.indexNames.contains('timestamp')) {
            fileStore.createIndex('timestamp', 'timestamp', { unique: false });
          }
        };
        req.onsuccess = () => {
          db = /** @type {IDBDatabase} */ (req.result);
  
          // add to the cache
          IDBFS.dbs[name] = db;
          callback(null, db);
        };
        req.onerror = (e) => {
          callback(e.target.error);
          e.preventDefault();
        };
      },
  getLocalSet:(mount, callback) => {
        var entries = {};
  
        function isRealDir(p) {
          return p !== '.' && p !== '..';
        };
        function toAbsolute(root) {
          return (p) => PATH.join2(root, p);
        };
  
        var check = FS.readdir(mount.mountpoint).filter(isRealDir).map(toAbsolute(mount.mountpoint));
  
        while (check.length) {
          var path = check.pop();
          var stat;
  
          try {
            stat = FS.lstat(path);
          } catch (e) {
            return callback(e);
          }
  
          if (FS.isDir(stat.mode)) {
            check.push(...FS.readdir(path).filter(isRealDir).map(toAbsolute(path)));
          }
  
          entries[path] = { 'timestamp': stat.mtime };
        }
  
        return callback(null, { type: 'local', entries: entries });
      },
  getRemoteSet:(mount, callback) => {
        var entries = {};
  
        IDBFS.getDB(mount.mountpoint, (err, db) => {
          if (err) return callback(err);
  
          try {
            var transaction = db.transaction([IDBFS.DB_STORE_NAME], 'readonly');
            transaction.onerror = (e) => {
              callback(e.target.error);
              e.preventDefault();
            };
  
            var store = transaction.objectStore(IDBFS.DB_STORE_NAME);
            var index = store.index('timestamp');
  
            index.openKeyCursor().onsuccess = (event) => {
              var cursor = event.target.result;
  
              if (!cursor) {
                return callback(null, { type: 'remote', db, entries });
              }
  
              entries[cursor.primaryKey] = { 'timestamp': cursor.key };
  
              cursor.continue();
            };
          } catch (e) {
            return callback(e);
          }
        });
      },
  loadLocalEntry:(path, callback) => {
        var stat, node;
  
        try {
          var lookup = FS.lookupPath(path);
          node = lookup.node;
          stat = FS.lstat(path);
        } catch (e) {
          return callback(e);
        }
  
        if (FS.isDir(stat.mode)) {
          return callback(null, { 'timestamp': stat.mtime, 'mode': stat.mode });
        } else if (FS.isLink(stat.mode)) {
          return callback(null, { 'timestamp': stat.mtime, 'mode': stat.mode, 'link': node.link, });
        } else if (FS.isFile(stat.mode)) {
          // Performance consideration: storing a normal JavaScript array to a IndexedDB is much slower than storing a typed array.
          // Therefore always convert the file contents to a typed array first before writing the data to IndexedDB.
          node.contents = MEMFS.getFileDataAsTypedArray(node);
          return callback(null, { 'timestamp': stat.mtime, 'mode': stat.mode, 'contents': node.contents });
        } else {
          return callback(new Error('node type not supported'));
        }
      },
  storeLocalEntry:(path, entry, callback) => {
        try {
          if (FS.isDir(entry['mode'])) {
            FS.mkdirTree(path, entry['mode']);
          } else if (FS.isLink(entry['mode'])) {
            FS.symlink(entry['link'], path);
          } else if (FS.isFile(entry['mode'])) {
            FS.writeFile(path, entry['contents'], { canOwn: true });
          } else {
            return callback(new Error('node type not supported'));
          }
  
          FS.chmod(path, entry['mode']);
          FS.utime(path, entry['timestamp'], entry['timestamp']);
        } catch (e) {
          return callback(e);
        }
  
        callback(null);
      },
  removeLocalEntry:(path, callback) => {
        try {
          var stat = FS.lstat(path);
  
          if (FS.isDir(stat.mode)) {
            FS.rmdir(path);
          } else {
            FS.unlink(path);
          }
        } catch (e) {
          return callback(e);
        }
  
        callback(null);
      },
  loadRemoteEntry:(store, path, callback) => {
        var req = store.get(path);
        req.onsuccess = (event) => callback(null, event.target.result);
        req.onerror = (e) => {
          callback(e.target.error);
          e.preventDefault();
        };
      },
  storeRemoteEntry:(store, path, entry, callback) => {
        try {
          var req = store.put(entry, path);
        } catch (e) {
          callback(e);
          return;
        }
        req.onsuccess = (event) => callback();
        req.onerror = (e) => {
          callback(e.target.error);
          e.preventDefault();
        };
      },
  removeRemoteEntry:(store, path, callback) => {
        var req = store.delete(path);
        req.onsuccess = (event) => callback();
        req.onerror = (e) => {
          callback(e.target.error);
          e.preventDefault();
        };
      },
  reconcile:(src, dst, callback) => {
        var total = 0;
  
        var create = [];
        for (var [key, e] of Object.entries(src.entries)) {
          var e2 = dst.entries[key];
          if (!e2 || e['timestamp'].getTime() != e2['timestamp'].getTime()) {
            create.push(key);
            total++;
          }
        }
  
        var remove = [];
        for (var key of Object.keys(dst.entries)) {
          if (!src.entries[key]) {
            remove.push(key);
            total++;
          }
        }
  
        if (!total) {
          return callback(null);
        }
  
        var errored = false;
        var db = src.type === 'remote' ? src.db : dst.db;
        var transaction = db.transaction([IDBFS.DB_STORE_NAME], 'readwrite');
        var store = transaction.objectStore(IDBFS.DB_STORE_NAME);
  
        function done(err) {
          if (err && !errored) {
            errored = true;
            return callback(err);
          }
        };
  
        // transaction may abort if (for example) there is a QuotaExceededError
        transaction.onerror = transaction.onabort = (e) => {
          done(e.target.error);
          e.preventDefault();
        };
  
        transaction.oncomplete = (e) => {
          if (!errored) {
            callback(null);
          }
        };
  
        // sort paths in ascending order so directory entries are created
        // before the files inside them
        for (const path of create.sort()) {
          if (dst.type === 'local') {
            IDBFS.loadRemoteEntry(store, path, (err, entry) => {
              if (err) return done(err);
              IDBFS.storeLocalEntry(path, entry, done);
            });
          } else {
            IDBFS.loadLocalEntry(path, (err, entry) => {
              if (err) return done(err);
              IDBFS.storeRemoteEntry(store, path, entry, done);
            });
          }
        }
  
        // sort paths in descending order so files are deleted before their
        // parent directories
        for (var path of remove.sort().reverse()) {
          if (dst.type === 'local') {
            IDBFS.removeLocalEntry(path, done);
          } else {
            IDBFS.removeRemoteEntry(store, path, done);
          }
        }
      },
  };
  
  
  
  var strError = (errno) => UTF8ToString(_strerror(errno));
  
  var ERRNO_CODES = {
      'EPERM': 63,
      'ENOENT': 44,
      'ESRCH': 71,
      'EINTR': 27,
      'EIO': 29,
      'ENXIO': 60,
      'E2BIG': 1,
      'ENOEXEC': 45,
      'EBADF': 8,
      'ECHILD': 12,
      'EAGAIN': 6,
      'EWOULDBLOCK': 6,
      'ENOMEM': 48,
      'EACCES': 2,
      'EFAULT': 21,
      'ENOTBLK': 105,
      'EBUSY': 10,
      'EEXIST': 20,
      'EXDEV': 75,
      'ENODEV': 43,
      'ENOTDIR': 54,
      'EISDIR': 31,
      'EINVAL': 28,
      'ENFILE': 41,
      'EMFILE': 33,
      'ENOTTY': 59,
      'ETXTBSY': 74,
      'EFBIG': 22,
      'ENOSPC': 51,
      'ESPIPE': 70,
      'EROFS': 69,
      'EMLINK': 34,
      'EPIPE': 64,
      'EDOM': 18,
      'ERANGE': 68,
      'ENOMSG': 49,
      'EIDRM': 24,
      'ECHRNG': 106,
      'EL2NSYNC': 156,
      'EL3HLT': 107,
      'EL3RST': 108,
      'ELNRNG': 109,
      'EUNATCH': 110,
      'ENOCSI': 111,
      'EL2HLT': 112,
      'EDEADLK': 16,
      'ENOLCK': 46,
      'EBADE': 113,
      'EBADR': 114,
      'EXFULL': 115,
      'ENOANO': 104,
      'EBADRQC': 103,
      'EBADSLT': 102,
      'EDEADLOCK': 16,
      'EBFONT': 101,
      'ENOSTR': 100,
      'ENODATA': 116,
      'ETIME': 117,
      'ENOSR': 118,
      'ENONET': 119,
      'ENOPKG': 120,
      'EREMOTE': 121,
      'ENOLINK': 47,
      'EADV': 122,
      'ESRMNT': 123,
      'ECOMM': 124,
      'EPROTO': 65,
      'EMULTIHOP': 36,
      'EDOTDOT': 125,
      'EBADMSG': 9,
      'ENOTUNIQ': 126,
      'EBADFD': 127,
      'EREMCHG': 128,
      'ELIBACC': 129,
      'ELIBBAD': 130,
      'ELIBSCN': 131,
      'ELIBMAX': 132,
      'ELIBEXEC': 133,
      'ENOSYS': 52,
      'ENOTEMPTY': 55,
      'ENAMETOOLONG': 37,
      'ELOOP': 32,
      'EOPNOTSUPP': 138,
      'EPFNOSUPPORT': 139,
      'ECONNRESET': 15,
      'ENOBUFS': 42,
      'EAFNOSUPPORT': 5,
      'EPROTOTYPE': 67,
      'ENOTSOCK': 57,
      'ENOPROTOOPT': 50,
      'ESHUTDOWN': 140,
      'ECONNREFUSED': 14,
      'EADDRINUSE': 3,
      'ECONNABORTED': 13,
      'ENETUNREACH': 40,
      'ENETDOWN': 38,
      'ETIMEDOUT': 73,
      'EHOSTDOWN': 142,
      'EHOSTUNREACH': 23,
      'EINPROGRESS': 26,
      'EALREADY': 7,
      'EDESTADDRREQ': 17,
      'EMSGSIZE': 35,
      'EPROTONOSUPPORT': 66,
      'ESOCKTNOSUPPORT': 137,
      'EADDRNOTAVAIL': 4,
      'ENETRESET': 39,
      'EISCONN': 30,
      'ENOTCONN': 53,
      'ETOOMANYREFS': 141,
      'EUSERS': 136,
      'EDQUOT': 19,
      'ESTALE': 72,
      'ENOTSUP': 138,
      'ENOMEDIUM': 148,
      'EILSEQ': 25,
      'EOVERFLOW': 61,
      'ECANCELED': 11,
      'ENOTRECOVERABLE': 56,
      'EOWNERDEAD': 62,
      'ESTRPIPE': 135,
    };
  
  var asyncLoad = async (url) => {
      var arrayBuffer = await readAsync(url);
      assert(arrayBuffer, `Loading data file "${url}" failed (no arrayBuffer).`);
      return new Uint8Array(arrayBuffer);
    };
  
  
  var FS_createDataFile = (...args) => FS.createDataFile(...args);
  
  var getUniqueRunDependency = (id) => {
      var orig = id;
      while (1) {
        if (!runDependencyTracking[id]) return id;
        id = orig + Math.random();
      }
    };
  
  
  
  var preloadPlugins = [];
  var FS_handledByPreloadPlugin = async (byteArray, fullname) => {
      // Ensure plugins are ready.
      if (typeof Browser != 'undefined') Browser.init();
  
      for (var plugin of preloadPlugins) {
        if (plugin['canHandle'](fullname)) {
          assert(plugin['handle'].constructor.name === 'AsyncFunction', 'Filesystem plugin handlers must be async functions (See #24914)')
          return plugin['handle'](byteArray, fullname);
        }
      }
      // If no plugin handled this file then return the original/unmodified
      // byteArray.
      return byteArray;
    };
  var FS_preloadFile = async (parent, name, url, canRead, canWrite, dontCreateFile, canOwn, preFinish) => {
      // TODO we should allow people to just pass in a complete filename instead
      // of parent and name being that we just join them anyways
      var fullname = name ? PATH_FS.resolve(PATH.join2(parent, name)) : parent;
      var dep = getUniqueRunDependency(`cp ${fullname}`); // might have several active requests for the same fullname
      addRunDependency(dep);
  
      try {
        var byteArray = url;
        if (typeof url == 'string') {
          byteArray = await asyncLoad(url);
        }
  
        byteArray = await FS_handledByPreloadPlugin(byteArray, fullname);
        preFinish?.();
        if (!dontCreateFile) {
          FS_createDataFile(parent, name, byteArray, canRead, canWrite, canOwn);
        }
      } finally {
        removeRunDependency(dep);
      }
    };
  var FS_createPreloadedFile = (parent, name, url, canRead, canWrite, onload, onerror, dontCreateFile, canOwn, preFinish) => {
      FS_preloadFile(parent, name, url, canRead, canWrite, dontCreateFile, canOwn, preFinish).then(onload).catch(onerror);
    };
  var FS = {
  root:null,
  mounts:[],
  devices:{
  },
  streams:[],
  nextInode:1,
  nameTable:null,
  currentPath:"/",
  initialized:false,
  ignorePermissions:true,
  filesystems:null,
  syncFSRequests:0,
  ErrnoError:class extends Error {
        name = 'ErrnoError';
        // We set the `name` property to be able to identify `FS.ErrnoError`
        // - the `name` is a standard ECMA-262 property of error objects. Kind of good to have it anyway.
        // - when using PROXYFS, an error can come from an underlying FS
        // as different FS objects have their own FS.ErrnoError each,
        // the test `err instanceof FS.ErrnoError` won't detect an error coming from another filesystem, causing bugs.
        // we'll use the reliable test `err.name == "ErrnoError"` instead
        constructor(errno) {
          super(runtimeInitialized ? strError(errno) : '');
          this.errno = errno;
          for (var key in ERRNO_CODES) {
            if (ERRNO_CODES[key] === errno) {
              this.code = key;
              break;
            }
          }
        }
      },
  FSStream:class {
        shared = {};
        get object() {
          return this.node;
        }
        set object(val) {
          this.node = val;
        }
        get isRead() {
          return (this.flags & 2097155) !== 1;
        }
        get isWrite() {
          return (this.flags & 2097155) !== 0;
        }
        get isAppend() {
          return (this.flags & 1024);
        }
        get flags() {
          return this.shared.flags;
        }
        set flags(val) {
          this.shared.flags = val;
        }
        get position() {
          return this.shared.position;
        }
        set position(val) {
          this.shared.position = val;
        }
      },
  FSNode:class {
        node_ops = {};
        stream_ops = {};
        readMode = 292 | 73;
        writeMode = 146;
        mounted = null;
        constructor(parent, name, mode, rdev) {
          if (!parent) {
            parent = this;  // root node sets parent to itself
          }
          this.parent = parent;
          this.mount = parent.mount;
          this.id = FS.nextInode++;
          this.name = name;
          this.mode = mode;
          this.rdev = rdev;
          this.atime = this.mtime = this.ctime = Date.now();
        }
        get read() {
          return (this.mode & this.readMode) === this.readMode;
        }
        set read(val) {
          val ? this.mode |= this.readMode : this.mode &= ~this.readMode;
        }
        get write() {
          return (this.mode & this.writeMode) === this.writeMode;
        }
        set write(val) {
          val ? this.mode |= this.writeMode : this.mode &= ~this.writeMode;
        }
        get isFolder() {
          return FS.isDir(this.mode);
        }
        get isDevice() {
          return FS.isChrdev(this.mode);
        }
      },
  lookupPath(path, opts = {}) {
        if (!path) {
          throw new FS.ErrnoError(44);
        }
        opts.follow_mount ??= true
  
        if (!PATH.isAbs(path)) {
          path = FS.cwd() + '/' + path;
        }
  
        // limit max consecutive symlinks to SYMLOOP_MAX.
        linkloop: for (var nlinks = 0; nlinks < 40; nlinks++) {
          // split the absolute path
          var parts = path.split('/').filter((p) => !!p);
  
          // start at the root
          var current = FS.root;
          var current_path = '/';
  
          for (var i = 0; i < parts.length; i++) {
            var islast = (i === parts.length-1);
            if (islast && opts.parent) {
              // stop resolving
              break;
            }
  
            if (parts[i] === '.') {
              continue;
            }
  
            if (parts[i] === '..') {
              current_path = PATH.dirname(current_path);
              if (FS.isRoot(current)) {
                path = current_path + '/' + parts.slice(i + 1).join('/');
                // We're making progress here, don't let many consecutive ..'s
                // lead to ELOOP
                nlinks--;
                continue linkloop;
              } else {
                current = current.parent;
              }
              continue;
            }
  
            current_path = PATH.join2(current_path, parts[i]);
            try {
              current = FS.lookupNode(current, parts[i]);
            } catch (e) {
              // if noent_okay is true, suppress a ENOENT in the last component
              // and return an object with an undefined node. This is needed for
              // resolving symlinks in the path when creating a file.
              if ((e?.errno === 44) && islast && opts.noent_okay) {
                return { path: current_path };
              }
              throw e;
            }
  
            // jump to the mount's root node if this is a mountpoint
            if (FS.isMountpoint(current) && (!islast || opts.follow_mount)) {
              current = current.mounted.root;
            }
  
            // by default, lookupPath will not follow a symlink if it is the final path component.
            // setting opts.follow = true will override this behavior.
            if (FS.isLink(current.mode) && (!islast || opts.follow)) {
              if (!current.node_ops.readlink) {
                throw new FS.ErrnoError(52);
              }
              var link = current.node_ops.readlink(current);
              if (!PATH.isAbs(link)) {
                link = PATH.dirname(current_path) + '/' + link;
              }
              path = link + '/' + parts.slice(i + 1).join('/');
              continue linkloop;
            }
          }
          return { path: current_path, node: current };
        }
        throw new FS.ErrnoError(32);
      },
  getPath(node) {
        var path;
        while (true) {
          if (FS.isRoot(node)) {
            var mount = node.mount.mountpoint;
            if (!path) return mount;
            return mount[mount.length-1] !== '/' ? `${mount}/${path}` : mount + path;
          }
          path = path ? `${node.name}/${path}` : node.name;
          node = node.parent;
        }
      },
  hashName(parentid, name) {
        var hash = 0;
  
        for (var i = 0; i < name.length; i++) {
          hash = ((hash << 5) - hash + name.charCodeAt(i)) | 0;
        }
        return ((parentid + hash) >>> 0) % FS.nameTable.length;
      },
  hashAddNode(node) {
        var hash = FS.hashName(node.parent.id, node.name);
        node.name_next = FS.nameTable[hash];
        FS.nameTable[hash] = node;
      },
  hashRemoveNode(node) {
        var hash = FS.hashName(node.parent.id, node.name);
        if (FS.nameTable[hash] === node) {
          FS.nameTable[hash] = node.name_next;
        } else {
          var current = FS.nameTable[hash];
          while (current) {
            if (current.name_next === node) {
              current.name_next = node.name_next;
              break;
            }
            current = current.name_next;
          }
        }
      },
  lookupNode(parent, name) {
        var errCode = FS.mayLookup(parent);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        var hash = FS.hashName(parent.id, name);
        for (var node = FS.nameTable[hash]; node; node = node.name_next) {
          var nodeName = node.name;
          if (node.parent.id === parent.id && nodeName === name) {
            return node;
          }
        }
        // if we failed to find it in the cache, call into the VFS
        return FS.lookup(parent, name);
      },
  createNode(parent, name, mode, rdev) {
        assert(typeof parent == 'object')
        var node = new FS.FSNode(parent, name, mode, rdev);
  
        FS.hashAddNode(node);
  
        return node;
      },
  destroyNode(node) {
        FS.hashRemoveNode(node);
      },
  isRoot(node) {
        return node === node.parent;
      },
  isMountpoint(node) {
        return !!node.mounted;
      },
  isFile(mode) {
        return (mode & 61440) === 32768;
      },
  isDir(mode) {
        return (mode & 61440) === 16384;
      },
  isLink(mode) {
        return (mode & 61440) === 40960;
      },
  isChrdev(mode) {
        return (mode & 61440) === 8192;
      },
  isBlkdev(mode) {
        return (mode & 61440) === 24576;
      },
  isFIFO(mode) {
        return (mode & 61440) === 4096;
      },
  isSocket(mode) {
        return (mode & 49152) === 49152;
      },
  flagsToPermissionString(flag) {
        var perms = ['r', 'w', 'rw'][flag & 3];
        if ((flag & 512)) {
          perms += 'w';
        }
        return perms;
      },
  nodePermissions(node, perms) {
        if (FS.ignorePermissions) {
          return 0;
        }
        // return 0 if any user, group or owner bits are set.
        if (perms.includes('r') && !(node.mode & 292)) {
          return 2;
        }
        if (perms.includes('w') && !(node.mode & 146)) {
          return 2;
        }
        if (perms.includes('x') && !(node.mode & 73)) {
          return 2;
        }
        return 0;
      },
  mayLookup(dir) {
        if (!FS.isDir(dir.mode)) return 54;
        var errCode = FS.nodePermissions(dir, 'x');
        if (errCode) return errCode;
        if (!dir.node_ops.lookup) return 2;
        return 0;
      },
  mayCreate(dir, name) {
        if (!FS.isDir(dir.mode)) {
          return 54;
        }
        try {
          var node = FS.lookupNode(dir, name);
          return 20;
        } catch (e) {
        }
        return FS.nodePermissions(dir, 'wx');
      },
  mayDelete(dir, name, isdir) {
        var node;
        try {
          node = FS.lookupNode(dir, name);
        } catch (e) {
          return e.errno;
        }
        var errCode = FS.nodePermissions(dir, 'wx');
        if (errCode) {
          return errCode;
        }
        if (isdir) {
          if (!FS.isDir(node.mode)) {
            return 54;
          }
          if (FS.isRoot(node) || FS.getPath(node) === FS.cwd()) {
            return 10;
          }
        } else if (FS.isDir(node.mode)) {
          return 31;
        }
        return 0;
      },
  mayOpen(node, flags) {
        if (!node) {
          return 44;
        }
        if (FS.isLink(node.mode)) {
          return 32;
        }
        var mode = FS.flagsToPermissionString(flags);
        if (FS.isDir(node.mode)) {
          // opening for write
          // TODO: check for O_SEARCH? (== search for dir only)
          if (mode !== 'r' || (flags & (512 | 64))) {
            return 31;
          }
        }
        return FS.nodePermissions(node, mode);
      },
  checkOpExists(op, err) {
        if (!op) {
          throw new FS.ErrnoError(err);
        }
        return op;
      },
  MAX_OPEN_FDS:4096,
  nextfd() {
        for (var fd = 0; fd <= FS.MAX_OPEN_FDS; fd++) {
          if (!FS.streams[fd]) {
            return fd;
          }
        }
        throw new FS.ErrnoError(33);
      },
  getStreamChecked(fd) {
        var stream = FS.getStream(fd);
        if (!stream) {
          throw new FS.ErrnoError(8);
        }
        return stream;
      },
  getStream:(fd) => FS.streams[fd],
  createStream(stream, fd = -1) {
        assert(fd >= -1);
  
        // clone it, so we can return an instance of FSStream
        stream = Object.assign(new FS.FSStream(), stream);
        if (fd == -1) {
          fd = FS.nextfd();
        }
        stream.fd = fd;
        FS.streams[fd] = stream;
        return stream;
      },
  closeStream(fd) {
        FS.streams[fd] = null;
      },
  dupStream(origStream, fd = -1) {
        var stream = FS.createStream(origStream, fd);
        stream.stream_ops?.dup?.(stream);
        return stream;
      },
  doSetAttr(stream, node, attr) {
        var setattr = stream?.stream_ops.setattr;
        var arg = setattr ? stream : node;
        setattr ??= node.node_ops.setattr;
        FS.checkOpExists(setattr, 63)
        try {
          setattr(arg, attr);
        } catch (e) {
          if (e instanceof RangeError) {
            throw new FS.ErrnoError(22);
          }
          throw e;
        }
      },
  chrdev_stream_ops:{
  open(stream) {
          var device = FS.getDevice(stream.node.rdev);
          // override node's stream ops with the device's
          stream.stream_ops = device.stream_ops;
          // forward the open call
          stream.stream_ops.open?.(stream);
        },
  llseek() {
          throw new FS.ErrnoError(70);
        },
  },
  major:(dev) => ((dev) >> 8),
  minor:(dev) => ((dev) & 0xff),
  makedev:(ma, mi) => ((ma) << 8 | (mi)),
  registerDevice(dev, ops) {
        FS.devices[dev] = { stream_ops: ops };
      },
  getDevice:(dev) => FS.devices[dev],
  getMounts(mount) {
        var mounts = [];
        var check = [mount];
  
        while (check.length) {
          var m = check.pop();
  
          mounts.push(m);
  
          check.push(...m.mounts);
        }
  
        return mounts;
      },
  syncfs(populate, callback) {
        if (typeof populate == 'function') {
          callback = populate;
          populate = false;
        }
  
        FS.syncFSRequests++;
  
        if (FS.syncFSRequests > 1) {
          err(`warning: ${FS.syncFSRequests} FS.syncfs operations in flight at once, probably just doing extra work`);
        }
  
        var mounts = FS.getMounts(FS.root.mount);
        var completed = 0;
  
        function doCallback(errCode) {
          assert(FS.syncFSRequests > 0);
          FS.syncFSRequests--;
          return callback(errCode);
        }
  
        function done(errCode) {
          if (errCode) {
            if (!done.errored) {
              done.errored = true;
              return doCallback(errCode);
            }
            return;
          }
          if (++completed >= mounts.length) {
            doCallback(null);
          }
        };
  
        // sync all mounts
        for (var mount of mounts) {
          if (mount.type.syncfs) {
            mount.type.syncfs(mount, populate, done);
          } else {
            done(null);
          }
        }
      },
  mount(type, opts, mountpoint) {
        if (typeof type == 'string') {
          // The filesystem was not included, and instead we have an error
          // message stored in the variable.
          throw type;
        }
        var root = mountpoint === '/';
        var pseudo = !mountpoint;
        var node;
  
        if (root && FS.root) {
          throw new FS.ErrnoError(10);
        } else if (!root && !pseudo) {
          var lookup = FS.lookupPath(mountpoint, { follow_mount: false });
  
          mountpoint = lookup.path;  // use the absolute path
          node = lookup.node;
  
          if (FS.isMountpoint(node)) {
            throw new FS.ErrnoError(10);
          }
  
          if (!FS.isDir(node.mode)) {
            throw new FS.ErrnoError(54);
          }
        }
  
        var mount = {
          type,
          opts,
          mountpoint,
          mounts: []
        };
  
        // create a root node for the fs
        var mountRoot = type.mount(mount);
        mountRoot.mount = mount;
        mount.root = mountRoot;
  
        if (root) {
          FS.root = mountRoot;
        } else if (node) {
          // set as a mountpoint
          node.mounted = mount;
  
          // add the new mount to the current mount's children
          if (node.mount) {
            node.mount.mounts.push(mount);
          }
        }
  
        return mountRoot;
      },
  unmount(mountpoint) {
        var lookup = FS.lookupPath(mountpoint, { follow_mount: false });
  
        if (!FS.isMountpoint(lookup.node)) {
          throw new FS.ErrnoError(28);
        }
  
        // destroy the nodes for this mount, and all its child mounts
        var node = lookup.node;
        var mount = node.mounted;
        var mounts = FS.getMounts(mount);
  
        for (var [hash, current] of Object.entries(FS.nameTable)) {
          while (current) {
            var next = current.name_next;
  
            if (mounts.includes(current.mount)) {
              FS.destroyNode(current);
            }
  
            current = next;
          }
        }
  
        // no longer a mountpoint
        node.mounted = null;
  
        // remove this mount from the child mounts
        var idx = node.mount.mounts.indexOf(mount);
        assert(idx !== -1);
        node.mount.mounts.splice(idx, 1);
      },
  lookup(parent, name) {
        return parent.node_ops.lookup(parent, name);
      },
  mknod(path, mode, dev) {
        var lookup = FS.lookupPath(path, { parent: true });
        var parent = lookup.node;
        var name = PATH.basename(path);
        if (!name) {
          throw new FS.ErrnoError(28);
        }
        if (name === '.' || name === '..') {
          throw new FS.ErrnoError(20);
        }
        var errCode = FS.mayCreate(parent, name);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.mknod) {
          throw new FS.ErrnoError(63);
        }
        return parent.node_ops.mknod(parent, name, mode, dev);
      },
  statfs(path) {
        return FS.statfsNode(FS.lookupPath(path, {follow: true}).node);
      },
  statfsStream(stream) {
        // We keep a separate statfsStream function because noderawfs overrides
        // it. In noderawfs, stream.node is sometimes null. Instead, we need to
        // look at stream.path.
        return FS.statfsNode(stream.node);
      },
  statfsNode(node) {
        // NOTE: None of the defaults here are true. We're just returning safe and
        //       sane values. Currently nodefs and rawfs replace these defaults,
        //       other file systems leave them alone.
        var rtn = {
          bsize: 4096,
          frsize: 4096,
          blocks: 1e6,
          bfree: 5e5,
          bavail: 5e5,
          files: FS.nextInode,
          ffree: FS.nextInode - 1,
          fsid: 42,
          flags: 2,
          namelen: 255,
        };
  
        if (node.node_ops.statfs) {
          Object.assign(rtn, node.node_ops.statfs(node.mount.opts.root));
        }
        return rtn;
      },
  create(path, mode = 0o666) {
        mode &= 4095;
        mode |= 32768;
        return FS.mknod(path, mode, 0);
      },
  mkdir(path, mode = 0o777) {
        mode &= 511 | 512;
        mode |= 16384;
        return FS.mknod(path, mode, 0);
      },
  mkdirTree(path, mode) {
        var dirs = path.split('/');
        var d = '';
        for (var dir of dirs) {
          if (!dir) continue;
          if (d || PATH.isAbs(path)) d += '/';
          d += dir;
          try {
            FS.mkdir(d, mode);
          } catch(e) {
            if (e.errno != 20) throw e;
          }
        }
      },
  mkdev(path, mode, dev) {
        if (typeof dev == 'undefined') {
          dev = mode;
          mode = 0o666;
        }
        mode |= 8192;
        return FS.mknod(path, mode, dev);
      },
  symlink(oldpath, newpath) {
        if (!PATH_FS.resolve(oldpath)) {
          throw new FS.ErrnoError(44);
        }
        var lookup = FS.lookupPath(newpath, { parent: true });
        var parent = lookup.node;
        if (!parent) {
          throw new FS.ErrnoError(44);
        }
        var newname = PATH.basename(newpath);
        var errCode = FS.mayCreate(parent, newname);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.symlink) {
          throw new FS.ErrnoError(63);
        }
        return parent.node_ops.symlink(parent, newname, oldpath);
      },
  rename(old_path, new_path) {
        var old_dirname = PATH.dirname(old_path);
        var new_dirname = PATH.dirname(new_path);
        var old_name = PATH.basename(old_path);
        var new_name = PATH.basename(new_path);
        // parents must exist
        var lookup, old_dir, new_dir;
  
        // let the errors from non existent directories percolate up
        lookup = FS.lookupPath(old_path, { parent: true });
        old_dir = lookup.node;
        lookup = FS.lookupPath(new_path, { parent: true });
        new_dir = lookup.node;
  
        if (!old_dir || !new_dir) throw new FS.ErrnoError(44);
        // need to be part of the same mount
        if (old_dir.mount !== new_dir.mount) {
          throw new FS.ErrnoError(75);
        }
        // source must exist
        var old_node = FS.lookupNode(old_dir, old_name);
        // old path should not be an ancestor of the new path
        var relative = PATH_FS.relative(old_path, new_dirname);
        if (relative.charAt(0) !== '.') {
          throw new FS.ErrnoError(28);
        }
        // new path should not be an ancestor of the old path
        relative = PATH_FS.relative(new_path, old_dirname);
        if (relative.charAt(0) !== '.') {
          throw new FS.ErrnoError(55);
        }
        // see if the new path already exists
        var new_node;
        try {
          new_node = FS.lookupNode(new_dir, new_name);
        } catch (e) {
          // not fatal
        }
        // early out if nothing needs to change
        if (old_node === new_node) {
          return;
        }
        // we'll need to delete the old entry
        var isdir = FS.isDir(old_node.mode);
        var errCode = FS.mayDelete(old_dir, old_name, isdir);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        // need delete permissions if we'll be overwriting.
        // need create permissions if new doesn't already exist.
        errCode = new_node ?
          FS.mayDelete(new_dir, new_name, isdir) :
          FS.mayCreate(new_dir, new_name);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!old_dir.node_ops.rename) {
          throw new FS.ErrnoError(63);
        }
        if (FS.isMountpoint(old_node) || (new_node && FS.isMountpoint(new_node))) {
          throw new FS.ErrnoError(10);
        }
        // if we are going to change the parent, check write permissions
        if (new_dir !== old_dir) {
          errCode = FS.nodePermissions(old_dir, 'w');
          if (errCode) {
            throw new FS.ErrnoError(errCode);
          }
        }
        // remove the node from the lookup hash
        FS.hashRemoveNode(old_node);
        // do the underlying fs rename
        try {
          old_dir.node_ops.rename(old_node, new_dir, new_name);
          // update old node (we do this here to avoid each backend
          // needing to)
          old_node.parent = new_dir;
        } catch (e) {
          throw e;
        } finally {
          // add the node back to the hash (in case node_ops.rename
          // changed its name)
          FS.hashAddNode(old_node);
        }
      },
  rmdir(path) {
        var lookup = FS.lookupPath(path, { parent: true });
        var parent = lookup.node;
        var name = PATH.basename(path);
        var node = FS.lookupNode(parent, name);
        var errCode = FS.mayDelete(parent, name, true);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.rmdir) {
          throw new FS.ErrnoError(63);
        }
        if (FS.isMountpoint(node)) {
          throw new FS.ErrnoError(10);
        }
        parent.node_ops.rmdir(parent, name);
        FS.destroyNode(node);
      },
  readdir(path) {
        var lookup = FS.lookupPath(path, { follow: true });
        var node = lookup.node;
        var readdir = FS.checkOpExists(node.node_ops.readdir, 54);
        return readdir(node);
      },
  unlink(path) {
        var lookup = FS.lookupPath(path, { parent: true });
        var parent = lookup.node;
        if (!parent) {
          throw new FS.ErrnoError(44);
        }
        var name = PATH.basename(path);
        var node = FS.lookupNode(parent, name);
        var errCode = FS.mayDelete(parent, name, false);
        if (errCode) {
          // According to POSIX, we should map EISDIR to EPERM, but
          // we instead do what Linux does (and we must, as we use
          // the musl linux libc).
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.unlink) {
          throw new FS.ErrnoError(63);
        }
        if (FS.isMountpoint(node)) {
          throw new FS.ErrnoError(10);
        }
        parent.node_ops.unlink(parent, name);
        FS.destroyNode(node);
      },
  readlink(path) {
        var lookup = FS.lookupPath(path);
        var link = lookup.node;
        if (!link) {
          throw new FS.ErrnoError(44);
        }
        if (!link.node_ops.readlink) {
          throw new FS.ErrnoError(28);
        }
        return link.node_ops.readlink(link);
      },
  stat(path, dontFollow) {
        var lookup = FS.lookupPath(path, { follow: !dontFollow });
        var node = lookup.node;
        var getattr = FS.checkOpExists(node.node_ops.getattr, 63);
        return getattr(node);
      },
  fstat(fd) {
        var stream = FS.getStreamChecked(fd);
        var node = stream.node;
        var getattr = stream.stream_ops.getattr;
        var arg = getattr ? stream : node;
        getattr ??= node.node_ops.getattr;
        FS.checkOpExists(getattr, 63)
        return getattr(arg);
      },
  lstat(path) {
        return FS.stat(path, true);
      },
  doChmod(stream, node, mode, dontFollow) {
        FS.doSetAttr(stream, node, {
          mode: (mode & 4095) | (node.mode & ~4095),
          ctime: Date.now(),
          dontFollow
        });
      },
  chmod(path, mode, dontFollow) {
        var node;
        if (typeof path == 'string') {
          var lookup = FS.lookupPath(path, { follow: !dontFollow });
          node = lookup.node;
        } else {
          node = path;
        }
        FS.doChmod(null, node, mode, dontFollow);
      },
  lchmod(path, mode) {
        FS.chmod(path, mode, true);
      },
  fchmod(fd, mode) {
        var stream = FS.getStreamChecked(fd);
        FS.doChmod(stream, stream.node, mode, false);
      },
  doChown(stream, node, dontFollow) {
        FS.doSetAttr(stream, node, {
          timestamp: Date.now(),
          dontFollow
          // we ignore the uid / gid for now
        });
      },
  chown(path, uid, gid, dontFollow) {
        var node;
        if (typeof path == 'string') {
          var lookup = FS.lookupPath(path, { follow: !dontFollow });
          node = lookup.node;
        } else {
          node = path;
        }
        FS.doChown(null, node, dontFollow);
      },
  lchown(path, uid, gid) {
        FS.chown(path, uid, gid, true);
      },
  fchown(fd, uid, gid) {
        var stream = FS.getStreamChecked(fd);
        FS.doChown(stream, stream.node, false);
      },
  doTruncate(stream, node, len) {
        if (FS.isDir(node.mode)) {
          throw new FS.ErrnoError(31);
        }
        if (!FS.isFile(node.mode)) {
          throw new FS.ErrnoError(28);
        }
        var errCode = FS.nodePermissions(node, 'w');
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        FS.doSetAttr(stream, node, {
          size: len,
          timestamp: Date.now()
        });
      },
  truncate(path, len) {
        if (len < 0) {
          throw new FS.ErrnoError(28);
        }
        var node;
        if (typeof path == 'string') {
          var lookup = FS.lookupPath(path, { follow: true });
          node = lookup.node;
        } else {
          node = path;
        }
        FS.doTruncate(null, node, len);
      },
  ftruncate(fd, len) {
        var stream = FS.getStreamChecked(fd);
        if (len < 0 || (stream.flags & 2097155) === 0) {
          throw new FS.ErrnoError(28);
        }
        FS.doTruncate(stream, stream.node, len);
      },
  utime(path, atime, mtime) {
        var lookup = FS.lookupPath(path, { follow: true });
        var node = lookup.node;
        var setattr = FS.checkOpExists(node.node_ops.setattr, 63);
        setattr(node, {
          atime: atime,
          mtime: mtime
        });
      },
  open(path, flags, mode = 0o666) {
        if (path === "") {
          throw new FS.ErrnoError(44);
        }
        flags = FS_modeStringToFlags(flags);
        if ((flags & 64)) {
          mode = (mode & 4095) | 32768;
        } else {
          mode = 0;
        }
        var node;
        var isDirPath;
        if (typeof path == 'object') {
          node = path;
        } else {
          isDirPath = path.endsWith("/");
          // noent_okay makes it so that if the final component of the path
          // doesn't exist, lookupPath returns `node: undefined`. `path` will be
          // updated to point to the target of all symlinks.
          var lookup = FS.lookupPath(path, {
            follow: !(flags & 131072),
            noent_okay: true
          });
          node = lookup.node;
          path = lookup.path;
        }
        // perhaps we need to create the node
        var created = false;
        if ((flags & 64)) {
          if (node) {
            // if O_CREAT and O_EXCL are set, error out if the node already exists
            if ((flags & 128)) {
              throw new FS.ErrnoError(20);
            }
          } else if (isDirPath) {
            throw new FS.ErrnoError(31);
          } else {
            // node doesn't exist, try to create it
            // Ignore the permission bits here to ensure we can `open` this new
            // file below. We use chmod below to apply the permissions once the
            // file is open.
            node = FS.mknod(path, mode | 0o777, 0);
            created = true;
          }
        }
        if (!node) {
          throw new FS.ErrnoError(44);
        }
        // can't truncate a device
        if (FS.isChrdev(node.mode)) {
          flags &= ~512;
        }
        // if asked only for a directory, then this must be one
        if ((flags & 65536) && !FS.isDir(node.mode)) {
          throw new FS.ErrnoError(54);
        }
        // check permissions, if this is not a file we just created now (it is ok to
        // create and write to a file with read-only permissions; it is read-only
        // for later use)
        if (!created) {
          var errCode = FS.mayOpen(node, flags);
          if (errCode) {
            throw new FS.ErrnoError(errCode);
          }
        }
        // do truncation if necessary
        if ((flags & 512) && !created) {
          FS.truncate(node, 0);
        }
        // we've already handled these, don't pass down to the underlying vfs
        flags &= ~(128 | 512 | 131072);
  
        // register the stream with the filesystem
        var stream = FS.createStream({
          node,
          path: FS.getPath(node),  // we want the absolute path to the node
          flags,
          seekable: true,
          position: 0,
          stream_ops: node.stream_ops,
          // used by the file family libc calls (fopen, fwrite, ferror, etc.)
          ungotten: [],
          error: false
        });
        // call the new stream's open function
        if (stream.stream_ops.open) {
          stream.stream_ops.open(stream);
        }
        if (created) {
          FS.chmod(node, mode & 0o777);
        }
        return stream;
      },
  close(stream) {
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if (stream.getdents) stream.getdents = null; // free readdir state
        try {
          if (stream.stream_ops.close) {
            stream.stream_ops.close(stream);
          }
        } catch (e) {
          throw e;
        } finally {
          FS.closeStream(stream.fd);
        }
        stream.fd = null;
      },
  isClosed(stream) {
        return stream.fd === null;
      },
  llseek(stream, offset, whence) {
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if (!stream.seekable || !stream.stream_ops.llseek) {
          throw new FS.ErrnoError(70);
        }
        if (whence != 0 && whence != 1 && whence != 2) {
          throw new FS.ErrnoError(28);
        }
        stream.position = stream.stream_ops.llseek(stream, offset, whence);
        stream.ungotten = [];
        return stream.position;
      },
  read(stream, buffer, offset, length, position) {
        assert(offset >= 0);
        if (length < 0 || position < 0) {
          throw new FS.ErrnoError(28);
        }
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if ((stream.flags & 2097155) === 1) {
          throw new FS.ErrnoError(8);
        }
        if (FS.isDir(stream.node.mode)) {
          throw new FS.ErrnoError(31);
        }
        if (!stream.stream_ops.read) {
          throw new FS.ErrnoError(28);
        }
        var seeking = typeof position != 'undefined';
        if (!seeking) {
          position = stream.position;
        } else if (!stream.seekable) {
          throw new FS.ErrnoError(70);
        }
        var bytesRead = stream.stream_ops.read(stream, buffer, offset, length, position);
        if (!seeking) stream.position += bytesRead;
        return bytesRead;
      },
  write(stream, buffer, offset, length, position, canOwn) {
        assert(offset >= 0);
        assert(buffer.subarray, 'FS.write expects a TypedArray');
        if (length < 0 || position < 0) {
          throw new FS.ErrnoError(28);
        }
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if ((stream.flags & 2097155) === 0) {
          throw new FS.ErrnoError(8);
        }
        if (FS.isDir(stream.node.mode)) {
          throw new FS.ErrnoError(31);
        }
        if (!stream.stream_ops.write) {
          throw new FS.ErrnoError(28);
        }
        if (stream.seekable && stream.flags & 1024) {
          // seek to the end before writing in append mode
          FS.llseek(stream, 0, 2);
        }
        var seeking = typeof position != 'undefined';
        if (!seeking) {
          position = stream.position;
        } else if (!stream.seekable) {
          throw new FS.ErrnoError(70);
        }
        var bytesWritten = stream.stream_ops.write(stream, buffer, offset, length, position, canOwn);
        if (!seeking) stream.position += bytesWritten;
        return bytesWritten;
      },
  mmap(stream, length, position, prot, flags) {
        // User requests writing to file (prot & PROT_WRITE != 0).
        // Checking if we have permissions to write to the file unless
        // MAP_PRIVATE flag is set. According to POSIX spec it is possible
        // to write to file opened in read-only mode with MAP_PRIVATE flag,
        // as all modifications will be visible only in the memory of
        // the current process.
        if ((prot & 2) !== 0
            && (flags & 2) === 0
            && (stream.flags & 2097155) !== 2) {
          throw new FS.ErrnoError(2);
        }
        if ((stream.flags & 2097155) === 1) {
          throw new FS.ErrnoError(2);
        }
        if (!stream.stream_ops.mmap) {
          throw new FS.ErrnoError(43);
        }
        if (!length) {
          throw new FS.ErrnoError(28);
        }
        return stream.stream_ops.mmap(stream, length, position, prot, flags);
      },
  msync(stream, buffer, offset, length, mmapFlags) {
        assert(offset >= 0);
        if (!stream.stream_ops.msync) {
          return 0;
        }
        return stream.stream_ops.msync(stream, buffer, offset, length, mmapFlags);
      },
  ioctl(stream, cmd, arg) {
        if (!stream.stream_ops.ioctl) {
          throw new FS.ErrnoError(59);
        }
        return stream.stream_ops.ioctl(stream, cmd, arg);
      },
  readFile(path, opts = {}) {
        opts.flags = opts.flags ?? 0;
        opts.encoding = opts.encoding ?? 'binary';
        if (opts.encoding !== 'utf8' && opts.encoding !== 'binary') {
          abort(`Invalid encoding type "${opts.encoding}"`);
        }
        var stream = FS.open(path, opts.flags);
        var stat = FS.stat(path);
        var length = stat.size;
        var buf = new Uint8Array(length);
        FS.read(stream, buf, 0, length, 0);
        if (opts.encoding === 'utf8') {
          buf = UTF8ArrayToString(buf);
        }
        FS.close(stream);
        return buf;
      },
  writeFile(path, data, opts = {}) {
        opts.flags = opts.flags ?? 577;
        var stream = FS.open(path, opts.flags, opts.mode);
        data = FS_fileDataToTypedArray(data);
        FS.write(stream, data, 0, data.byteLength, undefined, opts.canOwn);
        FS.close(stream);
      },
  cwd:() => FS.currentPath,
  chdir(path) {
        var lookup = FS.lookupPath(path, { follow: true });
        if (lookup.node === null) {
          throw new FS.ErrnoError(44);
        }
        if (!FS.isDir(lookup.node.mode)) {
          throw new FS.ErrnoError(54);
        }
        var errCode = FS.nodePermissions(lookup.node, 'x');
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        FS.currentPath = lookup.path;
      },
  createDefaultDirectories() {
        FS.mkdir('/tmp');
        FS.mkdir('/home');
        FS.mkdir('/home/web_user');
      },
  createDefaultDevices() {
        // create /dev
        FS.mkdir('/dev');
        // setup /dev/null
        FS.registerDevice(FS.makedev(1, 3), {
          read: () => 0,
          write: (stream, buffer, offset, length, pos) => length,
          llseek: () => 0,
        });
        FS.mkdev('/dev/null', FS.makedev(1, 3));
        // setup /dev/tty and /dev/tty1
        // stderr needs to print output using err() rather than out()
        // so we register a second tty just for it.
        TTY.register(FS.makedev(5, 0), TTY.default_tty_ops);
        TTY.register(FS.makedev(6, 0), TTY.default_tty1_ops);
        FS.mkdev('/dev/tty', FS.makedev(5, 0));
        FS.mkdev('/dev/tty1', FS.makedev(6, 0));
        // setup /dev/[u]random
        // use a buffer to avoid overhead of individual crypto calls per byte
        var randomBuffer = new Uint8Array(1024), randomLeft = 0;
        var randomByte = () => {
          if (randomLeft === 0) {
            randomFill(randomBuffer);
            randomLeft = randomBuffer.byteLength;
          }
          return randomBuffer[--randomLeft];
        };
        FS.createDevice('/dev', 'random', randomByte);
        FS.createDevice('/dev', 'urandom', randomByte);
        // we're not going to emulate the actual shm device,
        // just create the tmp dirs that reside in it commonly
        FS.mkdir('/dev/shm');
        FS.mkdir('/dev/shm/tmp');
      },
  createSpecialDirectories() {
        // create /proc/self/fd which allows /proc/self/fd/6 => readlink gives the
        // name of the stream for fd 6 (see test_unistd_ttyname)
        FS.mkdir('/proc');
        var proc_self = FS.mkdir('/proc/self');
        FS.mkdir('/proc/self/fd');
        FS.mount({
          mount() {
            var node = FS.createNode(proc_self, 'fd', 16895, 73);
            node.stream_ops = {
              llseek: MEMFS.stream_ops.llseek,
            };
            node.node_ops = {
              lookup(parent, name) {
                var fd = +name;
                var stream = FS.getStreamChecked(fd);
                var ret = {
                  parent: null,
                  mount: { mountpoint: 'fake' },
                  node_ops: { readlink: () => stream.path },
                  id: fd + 1,
                };
                ret.parent = ret; // make it look like a simple root node
                return ret;
              },
              readdir() {
                return Array.from(FS.streams.entries())
                  .filter(([k, v]) => v)
                  .map(([k, v]) => k.toString());
              }
            };
            return node;
          }
        }, {}, '/proc/self/fd');
      },
  createStandardStreams(input, output, error) {
        // TODO deprecate the old functionality of a single
        // input / output callback and that utilizes FS.createDevice
        // and instead require a unique set of stream ops
  
        // by default, we symlink the standard streams to the
        // default tty devices. however, if the standard streams
        // have been overwritten we create a unique device for
        // them instead.
        if (input) {
          FS.createDevice('/dev', 'stdin', input);
        } else {
          FS.symlink('/dev/tty', '/dev/stdin');
        }
        if (output) {
          FS.createDevice('/dev', 'stdout', null, output);
        } else {
          FS.symlink('/dev/tty', '/dev/stdout');
        }
        if (error) {
          FS.createDevice('/dev', 'stderr', null, error);
        } else {
          FS.symlink('/dev/tty1', '/dev/stderr');
        }
  
        // open default streams for the stdin, stdout and stderr devices
        var stdin = FS.open('/dev/stdin', 0);
        var stdout = FS.open('/dev/stdout', 1);
        var stderr = FS.open('/dev/stderr', 1);
        assert(stdin.fd === 0, `invalid handle for stdin (${stdin.fd})`);
        assert(stdout.fd === 1, `invalid handle for stdout (${stdout.fd})`);
        assert(stderr.fd === 2, `invalid handle for stderr (${stderr.fd})`);
      },
  staticInit() {
        FS.nameTable = new Array(4096);
  
        FS.mount(MEMFS, {}, '/');
  
        FS.createDefaultDirectories();
        FS.createDefaultDevices();
        FS.createSpecialDirectories();
  
        FS.filesystems = {
          'MEMFS': MEMFS,
          'IDBFS': IDBFS,
        };
      },
  init(input, output, error) {
        assert(!FS.initialized, 'FS.init was previously called. If you want to initialize later with custom parameters, remove any earlier calls (note that one is automatically added to the generated code)');
        FS.initialized = true;
  
        // Allow Module.stdin etc. to provide defaults, if none explicitly passed to us here
        input ??= Module['stdin'];
        output ??= Module['stdout'];
        error ??= Module['stderr'];
  
        FS.createStandardStreams(input, output, error);
      },
  quit() {
        FS.initialized = false;
        // force-flush all streams, so we get musl std streams printed out
        _fflush(0);
        // close all of our streams
        for (var stream of FS.streams) {
          if (stream) {
            FS.close(stream);
          }
        }
      },
  findObject(path, dontResolveLastLink) {
        var ret = FS.analyzePath(path, dontResolveLastLink);
        if (!ret.exists) {
          return null;
        }
        return ret.object;
      },
  analyzePath(path, dontResolveLastLink) {
        // operate from within the context of the symlink's target
        try {
          var lookup = FS.lookupPath(path, { follow: !dontResolveLastLink });
          path = lookup.path;
        } catch (e) {
        }
        var ret = {
          isRoot: false, exists: false, error: 0, name: null, path: null, object: null,
          parentExists: false, parentPath: null, parentObject: null
        };
        try {
          var lookup = FS.lookupPath(path, { parent: true });
          ret.parentExists = true;
          ret.parentPath = lookup.path;
          ret.parentObject = lookup.node;
          ret.name = PATH.basename(path);
          lookup = FS.lookupPath(path, { follow: !dontResolveLastLink });
          ret.exists = true;
          ret.path = lookup.path;
          ret.object = lookup.node;
          ret.name = lookup.node.name;
          ret.isRoot = lookup.path === '/';
        } catch (e) {
          ret.error = e.errno;
        };
        return ret;
      },
  createPath(parent, path, canRead, canWrite) {
        parent = typeof parent == 'string' ? parent : FS.getPath(parent);
        var parts = path.split('/').reverse();
        while (parts.length) {
          var part = parts.pop();
          if (!part) continue;
          var current = PATH.join2(parent, part);
          try {
            FS.mkdir(current);
          } catch (e) {
            if (e.errno != 20) throw e;
          }
          parent = current;
        }
        return current;
      },
  createFile(parent, name, properties, canRead, canWrite) {
        var path = PATH.join2(typeof parent == 'string' ? parent : FS.getPath(parent), name);
        var mode = FS_getMode(canRead, canWrite);
        return FS.create(path, mode);
      },
  createDataFile(parent, name, data, canRead, canWrite, canOwn) {
        var path = name;
        if (parent) {
          parent = typeof parent == 'string' ? parent : FS.getPath(parent);
          path = name ? PATH.join2(parent, name) : parent;
        }
        var mode = FS_getMode(canRead, canWrite);
        var node = FS.create(path, mode);
        if (data) {
          data = FS_fileDataToTypedArray(data);
          // make sure we can write to the file
          FS.chmod(node, mode | 146);
          var stream = FS.open(node, 577);
          FS.write(stream, data, 0, data.length, 0, canOwn);
          FS.close(stream);
          FS.chmod(node, mode);
        }
      },
  createDevice(parent, name, input, output) {
        var path = PATH.join2(typeof parent == 'string' ? parent : FS.getPath(parent), name);
        var mode = FS_getMode(!!input, !!output);
        FS.createDevice.major ??= 64;
        var dev = FS.makedev(FS.createDevice.major++, 0);
        // Create a fake device that a set of stream ops to emulate
        // the old behavior.
        FS.registerDevice(dev, {
          open(stream) {
            stream.seekable = false;
          },
          close(stream) {
            // flush any pending line data
            if (output?.buffer?.length) {
              output(10);
            }
          },
          read(stream, buffer, offset, length, pos /* ignored */) {
            var bytesRead = 0;
            for (var i = 0; i < length; i++) {
              var result;
              try {
                result = input();
              } catch (e) {
                throw new FS.ErrnoError(29);
              }
              if (result === undefined && bytesRead === 0) {
                throw new FS.ErrnoError(6);
              }
              if (result === null || result === undefined) break;
              bytesRead++;
              buffer[offset+i] = result;
            }
            if (bytesRead) {
              stream.node.atime = Date.now();
            }
            return bytesRead;
          },
          write(stream, buffer, offset, length, pos) {
            for (var i = 0; i < length; i++) {
              try {
                output(buffer[offset+i]);
              } catch (e) {
                throw new FS.ErrnoError(29);
              }
            }
            if (length) {
              stream.node.mtime = stream.node.ctime = Date.now();
            }
            return i;
          }
        });
        return FS.mkdev(path, mode, dev);
      },
  forceLoadFile(obj) {
        if (obj.isDevice || obj.isFolder || obj.link || obj.contents) return true;
        if (globalThis.XMLHttpRequest) {
          abort("Lazy loading should have been performed (contents set) in createLazyFile, but it was not. Lazy loading only works in web workers. Use --embed-file or --preload-file in emcc on the main thread.");
        } else { // Command-line.
          try {
            obj.contents = readBinary(obj.url);
          } catch (e) {
            throw new FS.ErrnoError(29);
          }
        }
      },
  createLazyFile(parent, name, url, canRead, canWrite) {
        // Lazy chunked Uint8Array (implements get and length from Uint8Array).
        // Actual getting is abstracted away for eventual reuse.
        class LazyUint8Array {
          lengthKnown = false;
          chunks = []; // Loaded chunks. Index is the chunk number
          get(idx) {
            if (idx > this.length-1 || idx < 0) {
              return undefined;
            }
            var chunkOffset = idx % this.chunkSize;
            var chunkNum = (idx / this.chunkSize)|0;
            return this.getter(chunkNum)[chunkOffset];
          }
          setDataGetter(getter) {
            this.getter = getter;
          }
          cacheLength() {
            // Find length
            var xhr = new XMLHttpRequest();
            xhr.open('HEAD', url, false);
            xhr.send(null);
            if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) abort("Couldn't load " + url + ". Status: " + xhr.status);
            var datalength = Number(xhr.getResponseHeader("Content-length"));
            var header;
            var hasByteServing = (header = xhr.getResponseHeader("Accept-Ranges")) && header === "bytes";
            var usesGzip = (header = xhr.getResponseHeader("Content-Encoding")) && header === "gzip";
  
            var chunkSize = 1024*1024; // Chunk size in bytes
  
            if (!hasByteServing) chunkSize = datalength;
  
            // Function to get a range from the remote URL.
            var doXHR = (from, to) => {
              if (from > to) abort(`invalid range (${from}, ${to}) or no bytes requested!`);
              if (to > datalength-1) abort(`only ${datalength} bytes available! programmer error!`);
  
              // TODO: Use mozResponseArrayBuffer, responseStream, etc. if available.
              var xhr = new XMLHttpRequest();
              xhr.open('GET', url, false);
              if (datalength !== chunkSize) xhr.setRequestHeader("Range", "bytes=" + from + "-" + to);
  
              // Some hints to the browser that we want binary data.
              xhr.responseType = 'arraybuffer';
              if (xhr.overrideMimeType) {
                xhr.overrideMimeType('text/plain; charset=x-user-defined');
              }
  
              xhr.send(null);
              if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) abort("Couldn't load " + url + ". Status: " + xhr.status);
              if (xhr.response !== undefined) {
                return new Uint8Array(/** @type{Array<number>} */(xhr.response || []));
              }
              return intArrayFromString(xhr.responseText ?? '', true);
            };
            var lazyArray = this;
            lazyArray.setDataGetter((chunkNum) => {
              var start = chunkNum * chunkSize;
              var end = (chunkNum+1) * chunkSize - 1; // including this byte
              end = Math.min(end, datalength-1); // if datalength-1 is selected, this is the last block
              if (typeof lazyArray.chunks[chunkNum] == 'undefined') {
                lazyArray.chunks[chunkNum] = doXHR(start, end);
              }
              if (typeof lazyArray.chunks[chunkNum] == 'undefined') abort('doXHR failed!');
              return lazyArray.chunks[chunkNum];
            });
  
            if (usesGzip || !datalength) {
              // if the server uses gzip or doesn't supply the length, we have to download the whole file to get the (uncompressed) length
              chunkSize = datalength = 1; // this will force getter(0)/doXHR do download the whole file
              datalength = this.getter(0).length;
              chunkSize = datalength;
              out("LazyFiles on gzip forces download of the whole file when length is accessed");
            }
  
            this._length = datalength;
            this._chunkSize = chunkSize;
            this.lengthKnown = true;
          }
          get length() {
            if (!this.lengthKnown) {
              this.cacheLength();
            }
            return this._length;
          }
          get chunkSize() {
            if (!this.lengthKnown) {
              this.cacheLength();
            }
            return this._chunkSize;
          }
        }
  
        if (globalThis.XMLHttpRequest) {
          if (!ENVIRONMENT_IS_WORKER) abort('Cannot do synchronous binary XHRs outside webworkers in modern browsers. Use --embed-file or --preload-file in emcc');
          var lazyArray = new LazyUint8Array();
          var properties = { isDevice: false, contents: lazyArray };
        } else {
          var properties = { isDevice: false, url: url };
        }
  
        var node = FS.createFile(parent, name, properties, canRead, canWrite);
        // This is a total hack, but I want to get this lazy file code out of the
        // core of MEMFS. If we want to keep this lazy file concept I feel it should
        // be its own thin LAZYFS proxying calls to MEMFS.
        if (properties.contents) {
          node.contents = properties.contents;
        } else if (properties.url) {
          node.contents = null;
          node.url = properties.url;
        }
        // Add a function that defers querying the file size until it is asked the first time.
        Object.defineProperties(node, {
          usedBytes: {
            get: function() { return this.contents.length; }
          }
        });
        // override each stream op with one that tries to force load the lazy file first
        var stream_ops = {};
        for (const [key, fn] of Object.entries(node.stream_ops)) {
          stream_ops[key] = (...args) => {
            FS.forceLoadFile(node);
            return fn(...args);
          };
        }
        function writeChunks(stream, buffer, offset, length, position) {
          var contents = stream.node.contents;
          if (position >= contents.length)
            return 0;
          var size = Math.min(contents.length - position, length);
          assert(size >= 0);
          if (contents.slice) { // normal array
            for (var i = 0; i < size; i++) {
              buffer[offset + i] = contents[position + i];
            }
          } else {
            for (var i = 0; i < size; i++) { // LazyUint8Array from sync binary XHR
              buffer[offset + i] = contents.get(position + i);
            }
          }
          return size;
        }
        // use a custom read function
        stream_ops.read = (stream, buffer, offset, length, position) => {
          FS.forceLoadFile(node);
          return writeChunks(stream, buffer, offset, length, position)
        };
        // use a custom mmap function
        stream_ops.mmap = (stream, length, position, prot, flags) => {
          FS.forceLoadFile(node);
          var ptr = mmapAlloc(length);
          if (!ptr) {
            throw new FS.ErrnoError(48);
          }
          writeChunks(stream, HEAP8, ptr, length, position);
          return { ptr, allocated: true };
        };
        node.stream_ops = stream_ops;
        return node;
      },
  };
  
  var SYSCALLS = {
  currentUmask:18,
  calculateAt(dirfd, path, allowEmpty) {
        if (PATH.isAbs(path)) {
          return path;
        }
        // relative path
        var dir;
        if (dirfd === -100) {
          dir = FS.cwd();
        } else {
          var dirstream = SYSCALLS.getStreamFromFD(dirfd);
          dir = dirstream.path;
        }
        if (path.length == 0) {
          if (!allowEmpty) {
            throw new FS.ErrnoError(44);;
          }
          return dir;
        }
        return dir + '/' + path;
      },
  writeStat(buf, stat) {
        HEAPU32[((buf)>>2)] = stat.dev;
        HEAPU32[(((buf)+(4))>>2)] = stat.mode;
        HEAPU32[(((buf)+(8))>>2)] = stat.nlink;
        HEAPU32[(((buf)+(12))>>2)] = stat.uid;
        HEAPU32[(((buf)+(16))>>2)] = stat.gid;
        HEAPU32[(((buf)+(20))>>2)] = stat.rdev;
        HEAP64[(((buf)+(24))>>3)] = BigInt(stat.size);
        HEAP32[(((buf)+(32))>>2)] = 4096;
        HEAP32[(((buf)+(36))>>2)] = stat.blocks;
        var atime = stat.atime.getTime();
        var mtime = stat.mtime.getTime();
        var ctime = stat.ctime.getTime();
        HEAP64[(((buf)+(40))>>3)] = BigInt(Math.floor(atime / 1000));
        HEAPU32[(((buf)+(48))>>2)] = (atime % 1000) * 1000 * 1000;
        HEAP64[(((buf)+(56))>>3)] = BigInt(Math.floor(mtime / 1000));
        HEAPU32[(((buf)+(64))>>2)] = (mtime % 1000) * 1000 * 1000;
        HEAP64[(((buf)+(72))>>3)] = BigInt(Math.floor(ctime / 1000));
        HEAPU32[(((buf)+(80))>>2)] = (ctime % 1000) * 1000 * 1000;
        HEAP64[(((buf)+(88))>>3)] = BigInt(stat.ino);
        return 0;
      },
  writeStatFs(buf, stats) {
        HEAPU32[(((buf)+(4))>>2)] = stats.bsize;
        HEAPU32[(((buf)+(60))>>2)] = stats.bsize;
        HEAP64[(((buf)+(8))>>3)] = BigInt(stats.blocks);
        HEAP64[(((buf)+(16))>>3)] = BigInt(stats.bfree);
        HEAP64[(((buf)+(24))>>3)] = BigInt(stats.bavail);
        HEAP64[(((buf)+(32))>>3)] = BigInt(stats.files);
        HEAP64[(((buf)+(40))>>3)] = BigInt(stats.ffree);
        HEAPU32[(((buf)+(48))>>2)] = stats.fsid;
        HEAPU32[(((buf)+(64))>>2)] = stats.flags;  // ST_NOSUID
        HEAPU32[(((buf)+(56))>>2)] = stats.namelen;
      },
  doMsync(addr, stream, len, flags, offset) {
        if (!FS.isFile(stream.node.mode)) {
          throw new FS.ErrnoError(43);
        }
        if (flags & 2) {
          // MAP_PRIVATE calls need not to be synced back to underlying fs
          return 0;
        }
        var buffer = HEAPU8.slice(addr, addr + len);
        FS.msync(stream, buffer, offset, len, flags);
      },
  getStreamFromFD(fd) {
        var stream = FS.getStreamChecked(fd);
        return stream;
      },
  varargs:undefined,
  getStr(ptr) {
        var ret = UTF8ToString(ptr);
        return ret;
      },
  };
  function ___syscall_dup3(fd, newfd, flags) {
  try {
  
      if (fd === newfd) return -28;
      if (flags & ~524288) return -28;
      var old = SYSCALLS.getStreamFromFD(fd);
      // Check newfd is within range of valid open file descriptors.
      if (newfd < 0 || newfd >= FS.MAX_OPEN_FDS) return -8;
      var existing = FS.getStream(newfd);
      if (existing) FS.close(existing);
      var stream = FS.dupStream(old, newfd);
      if (flags & 524288) {
        stream.flags |= 524288;
      }
      return stream.fd;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  var syscallGetVarargI = () => {
      assert(SYSCALLS.varargs != undefined);
      // the `+` prepended here is necessary to convince the JSCompiler that varargs is indeed a number.
      var ret = HEAP32[((+SYSCALLS.varargs)>>2)];
      SYSCALLS.varargs += 4;
      return ret;
    };
  var syscallGetVarargP = syscallGetVarargI;
  
  
  function ___syscall_fcntl64(fd, cmd, varargs) {
  SYSCALLS.varargs = varargs;
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      switch (cmd) {
        case 0: {
          var arg = syscallGetVarargI();
          if (arg < 0) {
            return -28;
          }
          while (FS.streams[arg]) {
            arg++;
          }
          var newStream;
          newStream = FS.dupStream(stream, arg);
          return newStream.fd;
        }
        case 1:
        case 2:
          return 0;  // FD_CLOEXEC makes no sense for a single process.
        case 3:
          return stream.flags;
        case 4: {
          var arg = syscallGetVarargI();
          var mask = 289792;
          stream.flags = (stream.flags & ~mask) | (arg & mask);
          return 0;
        }
        case 12: {
          var arg = syscallGetVarargP();
          var offset = 0;
          // We're always unlocked.
          HEAP16[(((arg)+(offset))>>1)] = 2;
          return 0;
        }
        case 13:
        case 14:
          // Pretend that the locking is successful. These are process-level locks,
          // and Emscripten programs are a single process. If we supported linking a
          // filesystem between programs, we'd need to do more here.
          // See https://github.com/emscripten-core/emscripten/issues/23697
          return 0;
      }
      return -28;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_fstat64(fd, buf) {
  try {
  
      return SYSCALLS.writeStat(buf, FS.fstat(fd));
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  
  function ___syscall_ioctl(fd, op, varargs) {
  SYSCALLS.varargs = varargs;
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      switch (op) {
        case 21509: {
          if (!stream.tty) return -59;
          return 0;
        }
        case 21505: {
          if (!stream.tty) return -59;
          if (stream.tty.ops.ioctl_tcgets) {
            var termios = stream.tty.ops.ioctl_tcgets(stream);
            var argp = syscallGetVarargP();
            HEAP32[((argp)>>2)] = termios.c_iflag || 0;
            HEAP32[(((argp)+(4))>>2)] = termios.c_oflag || 0;
            HEAP32[(((argp)+(8))>>2)] = termios.c_cflag || 0;
            HEAP32[(((argp)+(12))>>2)] = termios.c_lflag || 0;
            for (var i = 0; i < 32; i++) {
              HEAP8[(argp + i)+(17)] = termios.c_cc[i] || 0;
            }
            return 0;
          }
          return 0;
        }
        case 21510:
        case 21511:
        case 21512: {
          if (!stream.tty) return -59;
          return 0; // no-op, not actually adjusting terminal settings
        }
        case 21506:
        case 21507:
        case 21508: {
          if (!stream.tty) return -59;
          if (stream.tty.ops.ioctl_tcsets) {
            var argp = syscallGetVarargP();
            var c_iflag = HEAP32[((argp)>>2)];
            var c_oflag = HEAP32[(((argp)+(4))>>2)];
            var c_cflag = HEAP32[(((argp)+(8))>>2)];
            var c_lflag = HEAP32[(((argp)+(12))>>2)];
            var c_cc = []
            for (var i = 0; i < 32; i++) {
              c_cc.push(HEAP8[(argp + i)+(17)]);
            }
            return stream.tty.ops.ioctl_tcsets(stream.tty, op, { c_iflag, c_oflag, c_cflag, c_lflag, c_cc });
          }
          return 0; // no-op, not actually adjusting terminal settings
        }
        case 21519: {
          if (!stream.tty) return -59;
          var argp = syscallGetVarargP();
          HEAP32[((argp)>>2)] = 0;
          return 0;
        }
        case 21520: {
          if (!stream.tty) return -59;
          return -28; // not supported
        }
        case 21537:
        case 21531: {
          var argp = syscallGetVarargP();
          return FS.ioctl(stream, op, argp);
        }
        case 21523: {
          // TODO: in theory we should write to the winsize struct that gets
          // passed in, but for now musl doesn't read anything on it
          if (!stream.tty) return -59;
          if (stream.tty.ops.ioctl_tiocgwinsz) {
            var winsize = stream.tty.ops.ioctl_tiocgwinsz(stream.tty);
            var argp = syscallGetVarargP();
            HEAP16[((argp)>>1)] = winsize[0];
            HEAP16[(((argp)+(2))>>1)] = winsize[1];
          }
          return 0;
        }
        case 21524: {
          // TODO: technically, this ioctl call should change the window size.
          // but, since emscripten doesn't have any concept of a terminal window
          // yet, we'll just silently throw it away as we do TIOCGWINSZ
          if (!stream.tty) return -59;
          return 0;
        }
        case 21515: {
          if (!stream.tty) return -59;
          return 0;
        }
        default: return -28; // not supported
      }
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_lstat64(path, buf) {
  try {
  
      path = SYSCALLS.getStr(path);
      return SYSCALLS.writeStat(buf, FS.lstat(path));
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_mkdirat(dirfd, path, mode) {
  try {
  
      path = SYSCALLS.getStr(path);
      path = SYSCALLS.calculateAt(dirfd, path);
      mode &= ~SYSCALLS.currentUmask;
      FS.mkdir(path, mode, 0);
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_newfstatat(dirfd, path, buf, flags) {
  try {
  
      path = SYSCALLS.getStr(path);
      var nofollow = flags & 256;
      var allowEmpty = flags & 4096;
      flags = flags & (~6400);
      assert(!flags, `unknown flags in __syscall_newfstatat: ${flags}`);
      path = SYSCALLS.calculateAt(dirfd, path, allowEmpty);
      return SYSCALLS.writeStat(buf, nofollow ? FS.lstat(path) : FS.stat(path));
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  
  function ___syscall_openat(dirfd, path, flags, varargs) {
  SYSCALLS.varargs = varargs;
  try {
  
      path = SYSCALLS.getStr(path);
      path = SYSCALLS.calculateAt(dirfd, path);
      var mode = varargs ? syscallGetVarargI() : 0;
      if (flags & 64) {
        mode &= ~SYSCALLS.currentUmask;
      }
      return FS.open(path, flags, mode).fd;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  
  var stringToUTF8 = (str, outPtr, maxBytesToWrite) => {
      assert(typeof maxBytesToWrite == 'number', 'stringToUTF8 requires a third parameter that specifies the length of the output buffer');
      return stringToUTF8Array(str, HEAPU8, outPtr, maxBytesToWrite);
    };
  
  function ___syscall_readlinkat(dirfd, path, buf, bufsize) {
  try {
  
      path = SYSCALLS.getStr(path);
      path = SYSCALLS.calculateAt(dirfd, path);
      if (bufsize <= 0) return -28;
      var ret = FS.readlink(path);
  
      var len = Math.min(bufsize, lengthBytesUTF8(ret));
      var endChar = HEAP8[buf+len];
      stringToUTF8(ret, buf, bufsize+1);
      // readlink is one of the rare functions that write out a C string, but does never append a null to the output buffer(!)
      // stringToUTF8() always appends a null byte, so restore the character under the null byte after the write.
      HEAP8[buf+len] = endChar;
      return len;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_renameat(olddirfd, oldpath, newdirfd, newpath) {
  try {
  
      oldpath = SYSCALLS.getStr(oldpath);
      newpath = SYSCALLS.getStr(newpath);
      oldpath = SYSCALLS.calculateAt(olddirfd, oldpath);
      newpath = SYSCALLS.calculateAt(newdirfd, newpath);
      FS.rename(oldpath, newpath);
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_rmdir(path) {
  try {
  
      path = SYSCALLS.getStr(path);
      FS.rmdir(path);
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_stat64(path, buf) {
  try {
  
      path = SYSCALLS.getStr(path);
      return SYSCALLS.writeStat(buf, FS.stat(path));
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  function ___syscall_unlinkat(dirfd, path, flags) {
  try {
  
      path = SYSCALLS.getStr(path);
      path = SYSCALLS.calculateAt(dirfd, path);
      if (!flags) {
        FS.unlink(path);
      } else if (flags === 512) {
        FS.rmdir(path);
      } else {
        return -28;
      }
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }
  

  var __abort_js = () =>
      abort('native code called abort()');

  var __emscripten_system = (command) => {
      if (ENVIRONMENT_IS_NODE) {
        if (!command) return 1; // shell is available
  
        var cmdstr = UTF8ToString(command);
        if (!cmdstr.length) return 0; // this is what glibc seems to do (shell works test?)
  
        var cp = require('node:child_process');
        var ret = cp.spawnSync(cmdstr, [], {shell:true, stdio:'inherit'});
  
        var _W_EXITCODE = (ret, sig) => ((ret) << 8 | (sig));
  
        // this really only can happen if process is killed by signal
        if (ret.status === null) {
          // sadly node doesn't expose such function
          var signalToNumber = (sig) => {
            // implement only the most common ones, and fallback to SIGINT
            switch (sig) {
              case 'SIGHUP': return 1;
              case 'SIGQUIT': return 3;
              case 'SIGFPE': return 8;
              case 'SIGKILL': return 9;
              case 'SIGALRM': return 14;
              case 'SIGTERM': return 15;
              default: return 2;
            }
          }
          return _W_EXITCODE(0, signalToNumber(ret.signal));
        }
  
        return _W_EXITCODE(ret.status, 0);
      }
      // int system(const char *command);
      // http://pubs.opengroup.org/onlinepubs/000095399/functions/system.html
      // Can't call external programs.
      if (!command) return 0; // no shell available
      return -52;
    };

  var __emscripten_throw_longjmp = () => {
      throw new EmscriptenSjLj;
    };

  var INT53_MAX = 9007199254740992;
  
  var INT53_MIN = -9007199254740992;
  var bigintToI53Checked = (num) => (num < INT53_MIN || num > INT53_MAX) ? NaN : Number(num);
  function __gmtime_js(time, tmPtr) {
    time = bigintToI53Checked(time);
  
  
      var date = new Date(time * 1000);
      HEAP32[((tmPtr)>>2)] = date.getUTCSeconds();
      HEAP32[(((tmPtr)+(4))>>2)] = date.getUTCMinutes();
      HEAP32[(((tmPtr)+(8))>>2)] = date.getUTCHours();
      HEAP32[(((tmPtr)+(12))>>2)] = date.getUTCDate();
      HEAP32[(((tmPtr)+(16))>>2)] = date.getUTCMonth();
      HEAP32[(((tmPtr)+(20))>>2)] = date.getUTCFullYear()-1900;
      HEAP32[(((tmPtr)+(24))>>2)] = date.getUTCDay();
      var start = Date.UTC(date.getUTCFullYear(), 0, 1, 0, 0, 0, 0);
      var yday = ((date.getTime() - start) / (1000 * 60 * 60 * 24))|0;
      HEAP32[(((tmPtr)+(28))>>2)] = yday;
    ;
  }

  var isLeapYear = (year) => year%4 === 0 && (year%100 !== 0 || year%400 === 0);
  
  var MONTH_DAYS_LEAP_CUMULATIVE = [0,31,60,91,121,152,182,213,244,274,305,335];
  
  var MONTH_DAYS_REGULAR_CUMULATIVE = [0,31,59,90,120,151,181,212,243,273,304,334];
  var ydayFromDate = (date) => {
      var leap = isLeapYear(date.getFullYear());
      var monthDaysCumulative = (leap ? MONTH_DAYS_LEAP_CUMULATIVE : MONTH_DAYS_REGULAR_CUMULATIVE);
      var yday = monthDaysCumulative[date.getMonth()] + date.getDate() - 1; // -1 since it's days since Jan 1
  
      return yday;
    };
  
  function __localtime_js(time, tmPtr) {
    time = bigintToI53Checked(time);
  
  
      var date = new Date(time*1000);
      HEAP32[((tmPtr)>>2)] = date.getSeconds();
      HEAP32[(((tmPtr)+(4))>>2)] = date.getMinutes();
      HEAP32[(((tmPtr)+(8))>>2)] = date.getHours();
      HEAP32[(((tmPtr)+(12))>>2)] = date.getDate();
      HEAP32[(((tmPtr)+(16))>>2)] = date.getMonth();
      HEAP32[(((tmPtr)+(20))>>2)] = date.getFullYear()-1900;
      HEAP32[(((tmPtr)+(24))>>2)] = date.getDay();
  
      var yday = ydayFromDate(date)|0;
      HEAP32[(((tmPtr)+(28))>>2)] = yday;
      HEAP32[(((tmPtr)+(36))>>2)] = -(date.getTimezoneOffset() * 60);
  
      // Attention: DST is in December in South, and some regions don't have DST at all.
      var start = new Date(date.getFullYear(), 0, 1);
      var summerOffset = new Date(date.getFullYear(), 6, 1).getTimezoneOffset();
      var winterOffset = start.getTimezoneOffset();
      var dst = (summerOffset != winterOffset && date.getTimezoneOffset() == Math.min(winterOffset, summerOffset))|0;
      HEAP32[(((tmPtr)+(32))>>2)] = dst;
    ;
  }

  
  var __mktime_js = function(tmPtr) {
  
  var ret = (() => { 
      var date = new Date(HEAP32[(((tmPtr)+(20))>>2)] + 1900,
                          HEAP32[(((tmPtr)+(16))>>2)],
                          HEAP32[(((tmPtr)+(12))>>2)],
                          HEAP32[(((tmPtr)+(8))>>2)],
                          HEAP32[(((tmPtr)+(4))>>2)],
                          HEAP32[((tmPtr)>>2)],
                          0);
      if (isNaN(date.getTime())) {
        return -1;
      }
  
      // There's an ambiguous hour when the time goes back; the tm_isdst field is
      // used to disambiguate it.  Date() basically guesses, so we fix it up if it
      // guessed wrong, or fill in tm_isdst with the guess if it's -1.
      var dst = HEAP32[(((tmPtr)+(32))>>2)];
      var guessedOffset = date.getTimezoneOffset();
      var start = new Date(date.getFullYear(), 0, 1);
      var summerOffset = new Date(date.getFullYear(), 6, 1).getTimezoneOffset();
      var winterOffset = start.getTimezoneOffset();
      var dstOffset = Math.min(winterOffset, summerOffset); // DST is in December in South
      if (dst < 0) {
        // Attention: some regions don't have DST at all.
        HEAP32[(((tmPtr)+(32))>>2)] = Number(summerOffset != winterOffset && dstOffset == guessedOffset);
      } else if ((dst > 0) != (dstOffset == guessedOffset)) {
        var nonDstOffset = Math.max(winterOffset, summerOffset);
        var trueOffset = dst > 0 ? dstOffset : nonDstOffset;
        // Don't try setMinutes(date.getMinutes() + ...) -- it's messed up.
        date.setTime(date.getTime() + (trueOffset - guessedOffset)*60000);
      }
  
      HEAP32[(((tmPtr)+(24))>>2)] = date.getDay();
      var yday = ydayFromDate(date)|0;
      HEAP32[(((tmPtr)+(28))>>2)] = yday;
      // To match expected behavior, update fields from date
      HEAP32[((tmPtr)>>2)] = date.getSeconds();
      HEAP32[(((tmPtr)+(4))>>2)] = date.getMinutes();
      HEAP32[(((tmPtr)+(8))>>2)] = date.getHours();
      HEAP32[(((tmPtr)+(12))>>2)] = date.getDate();
      HEAP32[(((tmPtr)+(16))>>2)] = date.getMonth();
      HEAP32[(((tmPtr)+(20))>>2)] = date.getYear();
  
      // Return time in seconds
      return date.getTime() / 1000;
     })();
  return BigInt(ret);
  };

  
  var __tzset_js = (timezone, daylight, std_name, dst_name) => {
      // TODO: Use (malleable) environment variables instead of system settings.
      var currentYear = new Date().getFullYear();
      var winter = new Date(currentYear, 0, 1);
      var summer = new Date(currentYear, 6, 1);
      var winterOffset = winter.getTimezoneOffset();
      var summerOffset = summer.getTimezoneOffset();
  
      // Local standard timezone offset. Local standard time is not adjusted for
      // daylight savings.  This code uses the fact that getTimezoneOffset returns
      // a greater value during Standard Time versus Daylight Saving Time (DST).
      // Thus it determines the expected output during Standard Time, and it
      // compares whether the output of the given date the same (Standard) or less
      // (DST).
      var stdTimezoneOffset = Math.max(winterOffset, summerOffset);
  
      // timezone is specified as seconds west of UTC ("The external variable
      // `timezone` shall be set to the difference, in seconds, between
      // Coordinated Universal Time (UTC) and local standard time."), the same
      // as returned by stdTimezoneOffset.
      // See http://pubs.opengroup.org/onlinepubs/009695399/functions/tzset.html
      HEAPU32[((timezone)>>2)] = stdTimezoneOffset * 60;
  
      HEAP32[((daylight)>>2)] = Number(winterOffset != summerOffset);
  
      var extractZone = (timezoneOffset) => {
        // Why inverse sign?
        // Read here https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getTimezoneOffset
        var sign = timezoneOffset >= 0 ? "-" : "+";
  
        var absOffset = Math.abs(timezoneOffset)
        var hours = String(Math.floor(absOffset / 60)).padStart(2, "0");
        var minutes = String(absOffset % 60).padStart(2, "0");
  
        return `UTC${sign}${hours}${minutes}`;
      }
  
      var winterName = extractZone(winterOffset);
      var summerName = extractZone(summerOffset);
      assert(winterName);
      assert(summerName);
      assert(lengthBytesUTF8(winterName) <= 16, `timezone name truncated to fit in TZNAME_MAX (${winterName})`);
      assert(lengthBytesUTF8(summerName) <= 16, `timezone name truncated to fit in TZNAME_MAX (${summerName})`);
      if (summerOffset < winterOffset) {
        // Northern hemisphere
        stringToUTF8(winterName, std_name, 17);
        stringToUTF8(summerName, dst_name, 17);
      } else {
        stringToUTF8(winterName, dst_name, 17);
        stringToUTF8(summerName, std_name, 17);
      }
    };

  var _emscripten_get_now = () => performance.now();
  
  var _emscripten_date_now = () => Date.now();
  
  var nowIsMonotonic = 1;
  
  var checkWasiClock = (clock_id) => clock_id >= 0 && clock_id <= 3;
  
  function _clock_time_get(clk_id, ignored_precision, ptime) {
    ignored_precision = bigintToI53Checked(ignored_precision);
  
  
      if (!checkWasiClock(clk_id)) {
        return 28;
      }
      var now;
      // all wasi clocks but realtime are monotonic
      if (clk_id === 0) {
        now = _emscripten_date_now();
      } else if (nowIsMonotonic) {
        now = _emscripten_get_now();
      } else {
        return 52;
      }
      // "now" is in ms, and wasi times are in ns.
      var nsec = Math.round(now * 1000 * 1000);
      HEAP64[((ptime)>>3)] = BigInt(nsec);
      return 0;
    ;
  }

  var readEmAsmArgsArray = [];
  var readEmAsmArgs = (sigPtr, buf) => {
      // Nobody should have mutated _readEmAsmArgsArray underneath us to be something else than an array.
      assert(Array.isArray(readEmAsmArgsArray));
      // The input buffer is allocated on the stack, so it must be stack-aligned.
      assert(buf % 16 == 0);
      readEmAsmArgsArray.length = 0;
      var ch;
      // Most arguments are i32s, so shift the buffer pointer so it is a plain
      // index into HEAP32.
      while (ch = HEAPU8[sigPtr++]) {
        var chr = String.fromCharCode(ch);
        var validChars = ['d', 'f', 'i', 'p'];
        // In WASM_BIGINT mode we support passing i64 values as bigint.
        validChars.push('j');
        assert(validChars.includes(chr), `Invalid character ${ch}("${chr}") in readEmAsmArgs! Use only [${validChars}], and do not specify "v" for void return argument.`);
        // Floats are always passed as doubles, so all types except for 'i'
        // are 8 bytes and require alignment.
        var wide = (ch != 105);
        wide &= (ch != 112);
        buf += wide && (buf % 8) ? 4 : 0;
        readEmAsmArgsArray.push(
          // Special case for pointers under wasm64 or CAN_ADDRESS_2GB mode.
          ch == 112 ? HEAPU32[((buf)>>2)] :
          ch == 106 ? HEAP64[((buf)>>3)] :
          ch == 105 ?
            HEAP32[((buf)>>2)] :
            HEAPF64[((buf)>>3)]
        );
        buf += wide ? 8 : 4;
      }
      return readEmAsmArgsArray;
    };
  var runEmAsmFunction = (code, sigPtr, argbuf) => {
      var args = readEmAsmArgs(sigPtr, argbuf);
      assert(ASM_CONSTS.hasOwnProperty(code), `No EM_ASM constant found at address ${code}.  The loaded WebAssembly file is likely out of sync with the generated JavaScript.`);
      return ASM_CONSTS[code](...args);
    };
  var _emscripten_asm_const_double = (code, sigPtr, argbuf) => {
      return runEmAsmFunction(code, sigPtr, argbuf);
    };

  var _emscripten_asm_const_int = (code, sigPtr, argbuf) => {
      return runEmAsmFunction(code, sigPtr, argbuf);
    };

  
  var _emscripten_set_main_loop_timing = (mode, value) => {
      MainLoop.timingMode = mode;
      MainLoop.timingValue = value;
  
      if (!MainLoop.func) {
        err('emscripten_set_main_loop_timing: Cannot set timing mode for main loop since a main loop does not exist! Call emscripten_set_main_loop first to set one up.');
        return 1; // Return non-zero on failure, can't set timing mode when there is no main loop.
      }
  
      if (!MainLoop.running) {
        
        MainLoop.running = true;
      }
      if (mode == 0) {
        MainLoop.scheduler = function MainLoop_scheduler_setTimeout() {
          var timeUntilNextTick = Math.max(0, MainLoop.tickStartTime + value - _emscripten_get_now())|0;
          setTimeout(MainLoop.runner, timeUntilNextTick); // doing this each time means that on exception, we stop
        };
      } else if (mode == 1) {
        MainLoop.scheduler = function MainLoop_scheduler_rAF() {
          MainLoop.requestAnimationFrame(MainLoop.runner);
        };
      } else {
        assert(mode == 2);
        if (!MainLoop.setImmediate) {
          if (globalThis.scheduler) {
            // Some modern browsers implement scheduler.postTask, but not all.
            MainLoop.setImmediate = scheduler.postTask.bind(scheduler);
          } else if (globalThis.setImmediate) {
            MainLoop.setImmediate = setImmediate;
          } else {
            // Emulate setImmediate. (note: not a complete polyfill, we don't emulate clearImmediate() to keep code size to minimum, since not needed)
            var setImmediates = [];
            var emscriptenMainLoopMessageId = 'setimmediate';
            /** @param {Event} event */
            var MainLoop_setImmediate_messageHandler = (event) => {
              if (event.data === emscriptenMainLoopMessageId) {
                event.stopPropagation();
                setImmediates.shift()();
              }
            };
            addEventListener("message", MainLoop_setImmediate_messageHandler, true);
            MainLoop.setImmediate = /** @type{function(function(): ?, ...?): number} */((func) => {
              setImmediates.push(func);
              if (ENVIRONMENT_IS_WORKER) {
                // The postMessge API in a Worker, sends message to the main
                // thread and does not support the `targetOrigin` (*) argument.
                postMessage(emscriptenMainLoopMessageId);
              } else {
                postMessage(emscriptenMainLoopMessageId, '*');
              }
            });
          }
        }
        MainLoop.scheduler = function MainLoop_scheduler_setImmediate() {
          MainLoop.setImmediate(MainLoop.runner);
        };
      }
      return 0;
    };
  
  
  
  var runtimeKeepaliveCounter = 0;
  var keepRuntimeAlive = () => noExitRuntime || runtimeKeepaliveCounter > 0;
  var _proc_exit = (code) => {
      EXITSTATUS = code;
      if (!keepRuntimeAlive()) {
        Module['onExit']?.(code);
        ABORT = true;
      }
      quit_(code, new ExitStatus(code));
    };
  
  
  /** @param {boolean|number=} implicit */
  var exitJS = (status, implicit) => {
      EXITSTATUS = status;
  
      checkUnflushedContent();
  
      // if exit() was called explicitly, warn the user if the runtime isn't actually being shut down
      if (keepRuntimeAlive() && !implicit) {
        var msg = `program exited (with status: ${status}), but keepRuntimeAlive() is set (counter=${runtimeKeepaliveCounter}) due to an async operation, so halting execution but not exiting the runtime or preventing further async execution (you can use emscripten_force_exit, if you want to force a true shutdown)`;
        err(msg);
      }
  
      _proc_exit(status);
    };
  var _exit = exitJS;
  
  var handleException = (e) => {
      // Certain exception types we do not treat as errors since they are used for
      // internal control flow.
      // 1. ExitStatus, which is thrown by exit()
      // 2. "unwind", which is thrown by emscripten_unwind_to_js_event_loop() and others
      //    that wish to return to JS event loop.
      if (e instanceof ExitStatus || e == 'unwind') {
        return EXITSTATUS;
      }
      checkStackCookie();
      if (e instanceof WebAssembly.RuntimeError) {
        if (_emscripten_stack_get_current() <= 0) {
          err('Stack overflow detected.  You can try increasing -sSTACK_SIZE (currently set to 65536)');
        }
      }
      quit_(1, e);
    };
  
  var maybeExit = () => {
      if (!keepRuntimeAlive()) {
        try {
          _exit(EXITSTATUS);
        } catch (e) {
          handleException(e);
        }
      }
    };
  
    /**
   * @param {number=} arg
   * @param {boolean=} noSetTiming
   */
  var setMainLoop = (iterFunc, fps, simulateInfiniteLoop, arg, noSetTiming) => {
      assert(!MainLoop.func, 'emscripten_set_main_loop: there can only be one main loop function at once')
      MainLoop.func = iterFunc;
      MainLoop.arg = arg;
  
      var thisMainLoopId = MainLoop.currentlyRunningMainloop;
      function checkIsRunning() {
        if (thisMainLoopId < MainLoop.currentlyRunningMainloop) {
          
          maybeExit();
          return false;
        }
        return true;
      }
  
      // We create the loop runner here but it is not actually running until
      // _emscripten_set_main_loop_timing is called (which might happen at a
      // later time).  This member signifies that the current runner has not
      // yet been started so that we can call runtimeKeepalivePush when it
      // gets its timing set for the first time.
      MainLoop.running = false;
      MainLoop.runner = function MainLoop_runner() {
        if (ABORT) return;
        if (MainLoop.queue.length > 0) {
          var start = Date.now();
          var blocker = MainLoop.queue.shift();
          blocker.func(blocker.arg);
          if (MainLoop.remainingBlockers) {
            var remaining = MainLoop.remainingBlockers;
            var next = remaining%1 == 0 ? remaining-1 : Math.floor(remaining);
            if (blocker.counted) {
              MainLoop.remainingBlockers = next;
            } else {
              // not counted, but move the progress along a tiny bit
              next = next + 0.5; // do not steal all the next one's progress
              MainLoop.remainingBlockers = (8*remaining + next)/9;
            }
          }
          MainLoop.updateStatus();
  
          // catches pause/resume main loop from blocker execution
          if (!checkIsRunning()) return;
  
          setTimeout(MainLoop.runner, 0);
          return;
        }
  
        // catch pauses from non-main loop sources
        if (!checkIsRunning()) return;
  
        // Implement very basic swap interval control
        MainLoop.currentFrameNumber = MainLoop.currentFrameNumber + 1 | 0;
        if (MainLoop.timingMode == 1 && MainLoop.timingValue > 1 && MainLoop.currentFrameNumber % MainLoop.timingValue != 0) {
          // Not the scheduled time to render this frame - skip.
          MainLoop.scheduler();
          return;
        } else if (MainLoop.timingMode == 0) {
          MainLoop.tickStartTime = _emscripten_get_now();
          if (Module['ctx']) {
            warnOnce('Looks like you are rendering without using requestAnimationFrame for the main loop. You should use 0 for the frame rate in emscripten_set_main_loop in order to use requestAnimationFrame, as that can greatly improve your frame rates!');
          }
        }
  
        MainLoop.runIter(iterFunc);
  
        // catch pauses from the main loop itself
        if (!checkIsRunning()) return;
  
        MainLoop.scheduler();
      }
  
      if (!noSetTiming) {
        if (fps > 0) {
          _emscripten_set_main_loop_timing(0, 1000.0 / fps);
        } else {
          // Do rAF by rendering each frame (no decimating)
          _emscripten_set_main_loop_timing(1, 1);
        }
  
        MainLoop.scheduler();
      }
  
      if (simulateInfiniteLoop) {
        throw 'unwind';
      }
    };
  
  
  var callUserCallback = (func) => {
      if (ABORT) {
        err('user callback triggered after runtime exited or application aborted.  Ignoring.');
        return;
      }
      try {
        return func();
      } catch (e) {
        handleException(e);
      } finally {
        maybeExit();
      }
    };
  
  var MainLoop = {
  running:false,
  scheduler:null,
  currentlyRunningMainloop:0,
  func:null,
  arg:0,
  timingMode:0,
  timingValue:0,
  currentFrameNumber:0,
  queue:[],
  preMainLoop:[],
  postMainLoop:[],
  pause() {
        MainLoop.scheduler = null;
        // Incrementing this signals the previous main loop that it's now become old, and it must return.
        MainLoop.currentlyRunningMainloop++;
      },
  resume() {
        MainLoop.currentlyRunningMainloop++;
        var timingMode = MainLoop.timingMode;
        var timingValue = MainLoop.timingValue;
        var func = MainLoop.func;
        MainLoop.func = null;
        // do not set timing and call scheduler, we will do it on the next lines
        setMainLoop(func, 0, false, MainLoop.arg, true);
        _emscripten_set_main_loop_timing(timingMode, timingValue);
        MainLoop.scheduler();
      },
  updateStatus() {
        if (Module['setStatus']) {
          var message = Module['statusMessage'] || 'Please wait...';
          var remaining = MainLoop.remainingBlockers ?? 0;
          var expected = MainLoop.expectedBlockers ?? 0;
          if (remaining) {
            if (remaining < expected) {
              Module['setStatus'](`{message} ({expected - remaining}/{expected})`);
            } else {
              Module['setStatus'](message);
            }
          } else {
            Module['setStatus']('');
          }
        }
      },
  init() {
        Module['preMainLoop'] && MainLoop.preMainLoop.push(Module['preMainLoop']);
        Module['postMainLoop'] && MainLoop.postMainLoop.push(Module['postMainLoop']);
      },
  runIter(func) {
        if (ABORT) return;
        for (var pre of MainLoop.preMainLoop) {
          if (pre() === false) {
            return; // |return false| skips a frame
          }
        }
        callUserCallback(func);
        for (var post of MainLoop.postMainLoop) {
          post();
        }
        checkStackCookie();
      },
  nextRAF:0,
  fakeRequestAnimationFrame(func) {
        // try to keep 60fps between calls to here
        var now = Date.now();
        if (MainLoop.nextRAF === 0) {
          MainLoop.nextRAF = now + 1000/60;
        } else {
          while (now + 2 >= MainLoop.nextRAF) { // fudge a little, to avoid timer jitter causing us to do lots of delay:0
            MainLoop.nextRAF += 1000/60;
          }
        }
        var delay = Math.max(MainLoop.nextRAF - now, 0);
        setTimeout(func, delay);
      },
  requestAnimationFrame(func) {
        if (globalThis.requestAnimationFrame) {
          requestAnimationFrame(func);
        } else {
          MainLoop.fakeRequestAnimationFrame(func);
        }
      },
  };
  var _emscripten_cancel_main_loop = () => {
      MainLoop.pause();
      MainLoop.func = null;
    };



  var getHeapMax = () =>
      // Stay one Wasm page short of 4GB: while e.g. Chrome is able to allocate
      // full 4GB Wasm memories, the size will wrap back to 0 bytes in Wasm side
      // for any code that deals with heap sizes, which would require special
      // casing all heap size related code to treat 0 specially.
      2147483648;
  
  var alignMemory = (size, alignment) => {
      assert(alignment, 'alignment argument is required');
      return Math.ceil(size / alignment) * alignment;
    };
  
  var growMemory = (size) => {
      var oldHeapSize = wasmMemory.buffer.byteLength;
      var pages = ((size - oldHeapSize + 65535) / 65536) | 0;
      try {
        // round size grow request up to wasm page size (fixed 64KB per spec)
        wasmMemory.grow(pages); // .grow() takes a delta compared to the previous size
        updateMemoryViews();
        return 1 /*success*/;
      } catch(e) {
        err(`growMemory: Attempted to grow heap from ${oldHeapSize} bytes to ${size} bytes, but got error: ${e}`);
      }
      // implicit 0 return to save code size (caller will cast "undefined" into 0
      // anyhow)
    };
  var _emscripten_resize_heap = (requestedSize) => {
      var oldSize = HEAPU8.length;
      // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
      requestedSize >>>= 0;
      // With multithreaded builds, races can happen (another thread might increase the size
      // in between), so return a failure, and let the caller retry.
      assert(requestedSize > oldSize);
  
      // Memory resize rules:
      // 1.  Always increase heap size to at least the requested size, rounded up
      //     to next page multiple.
      // 2a. If MEMORY_GROWTH_LINEAR_STEP == -1, excessively resize the heap
      //     geometrically: increase the heap size according to
      //     MEMORY_GROWTH_GEOMETRIC_STEP factor (default +20%), At most
      //     overreserve by MEMORY_GROWTH_GEOMETRIC_CAP bytes (default 96MB).
      // 2b. If MEMORY_GROWTH_LINEAR_STEP != -1, excessively resize the heap
      //     linearly: increase the heap size by at least
      //     MEMORY_GROWTH_LINEAR_STEP bytes.
      // 3.  Max size for the heap is capped at 2048MB-WASM_PAGE_SIZE, or by
      //     MAXIMUM_MEMORY, or by ASAN limit, depending on which is smallest
      // 4.  If we were unable to allocate as much memory, it may be due to
      //     over-eager decision to excessively reserve due to (3) above.
      //     Hence if an allocation fails, cut down on the amount of excess
      //     growth, in an attempt to succeed to perform a smaller allocation.
  
      // A limit is set for how much we can grow. We should not exceed that
      // (the wasm binary specifies it, so if we tried, we'd fail anyhow).
      var maxHeapSize = getHeapMax();
      if (requestedSize > maxHeapSize) {
        err(`Cannot enlarge memory, requested ${requestedSize} bytes, but the limit is ${maxHeapSize} bytes!`);
        return false;
      }
  
      // Loop through potential heap size increases. If we attempt a too eager
      // reservation that fails, cut down on the attempted size and reserve a
      // smaller bump instead. (max 3 times, chosen somewhat arbitrarily)
      for (var cutDown = 1; cutDown <= 4; cutDown *= 2) {
        var overGrownHeapSize = oldSize * (1 + 0.2 / cutDown); // ensure geometric growth
        // but limit overreserving (default to capping at +96MB overgrowth at most)
        overGrownHeapSize = Math.min(overGrownHeapSize, requestedSize + 100663296 );
  
        var newSize = Math.min(maxHeapSize, alignMemory(Math.max(requestedSize, overGrownHeapSize), 65536));
  
        var replacement = growMemory(newSize);
        if (replacement) {
  
          return true;
        }
      }
      err(`Failed to grow the heap from ${oldSize} bytes to ${newSize} bytes, not enough memory!`);
      return false;
    };

  
  var wasmTableMirror = [];
  
  
  var getWasmTableEntry = (funcPtr) => {
      var func = wasmTableMirror[funcPtr];
      if (!func) {
        /** @suppress {checkTypes} */
        wasmTableMirror[funcPtr] = func = wasmTable.get(funcPtr);
      }
      /** @suppress {checkTypes} */
      assert(wasmTable.get(funcPtr) == func, 'table mirror is out of date');
      return func;
    };
  var _emscripten_set_main_loop = (func, fps, simulateInfiniteLoop) => {
      var iterFunc = getWasmTableEntry(func);
      setMainLoop(iterFunc, fps, simulateInfiniteLoop);
    };

  var ENV = {
  };
  
  var getExecutableName = () => thisProgram;
  var getEnvStrings = () => {
      if (!getEnvStrings.strings) {
        // Default values.
        var lang = (globalThis.navigator?.language ?? 'C').replace('-', '_') + '.UTF-8';
        var env = {
          'USER': 'web_user',
          'LOGNAME': 'web_user',
          'PATH': '/',
          'PWD': '/',
          'HOME': '/home/web_user',
          'LANG': lang,
          '_': getExecutableName()
        };
        // Apply the user-provided values, if any.
        for (var x in ENV) {
          // x is a key in ENV; if ENV[x] is undefined, that means it was
          // explicitly set to be so. We allow user code to do that to
          // force variables with default values to remain unset.
          if (ENV[x] === undefined) delete env[x];
          else env[x] = ENV[x];
        }
        var strings = [];
        for (var x in env) {
          strings.push(`${x}=${env[x]}`);
        }
        getEnvStrings.strings = strings;
      }
      return getEnvStrings.strings;
    };
  
  var _environ_get = (__environ, environ_buf) => {
      var bufSize = 0;
      var envp = 0;
      for (var string of getEnvStrings()) {
        var ptr = environ_buf + bufSize;
        HEAPU32[(((__environ)+(envp))>>2)] = ptr;
        bufSize += stringToUTF8(string, ptr, Infinity) + 1;
        envp += 4;
      }
      return 0;
    };

  
  var _environ_sizes_get = (penviron_count, penviron_buf_size) => {
      var strings = getEnvStrings();
      HEAPU32[((penviron_count)>>2)] = strings.length;
      var bufSize = 0;
      for (var string of strings) {
        bufSize += lengthBytesUTF8(string) + 1;
      }
      HEAPU32[((penviron_buf_size)>>2)] = bufSize;
      return 0;
    };


  function _fd_close(fd) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      FS.close(stream);
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  }
  

  /** @param {number=} offset */
  var doReadv = (stream, iov, iovcnt, offset) => {
      var ret = 0;
      for (var i = 0; i < iovcnt; i++) {
        var ptr = HEAPU32[((iov)>>2)];
        var len = HEAPU32[(((iov)+(4))>>2)];
        iov += 8;
        var curr = FS.read(stream, HEAP8, ptr, len, offset);
        if (curr < 0) return -1;
        ret += curr;
        if (curr < len) break; // nothing more to read
        if (typeof offset != 'undefined') {
          offset += curr;
        }
      }
      return ret;
    };
  
  function _fd_read(fd, iov, iovcnt, pnum) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      var num = doReadv(stream, iov, iovcnt);
      HEAPU32[((pnum)>>2)] = num;
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  }
  

  
  function _fd_seek(fd, offset, whence, newOffset) {
    offset = bigintToI53Checked(offset);
  
  
  try {
  
      if (isNaN(offset)) return 22;
      var stream = SYSCALLS.getStreamFromFD(fd);
      FS.llseek(stream, offset, whence);
      HEAP64[((newOffset)>>3)] = BigInt(stream.position);
      if (stream.getdents && offset === 0 && whence === 0) stream.getdents = null; // reset readdir state
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  ;
  }

  /** @param {number=} offset */
  var doWritev = (stream, iov, iovcnt, offset) => {
      var ret = 0;
      for (var i = 0; i < iovcnt; i++) {
        var ptr = HEAPU32[((iov)>>2)];
        var len = HEAPU32[(((iov)+(4))>>2)];
        iov += 8;
        var curr = FS.write(stream, HEAP8, ptr, len, offset);
        if (curr < 0) return -1;
        ret += curr;
        if (curr < len) {
          // No more space to write.
          break;
        }
        if (typeof offset != 'undefined') {
          offset += curr;
        }
      }
      return ret;
    };
  
  function _fd_write(fd, iov, iovcnt, pnum) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      var num = doWritev(stream, iov, iovcnt);
      HEAPU32[((pnum)>>2)] = num;
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  }
  

  var _llvm_eh_typeid_for = (type) => type;



  
  
  var stringToUTF8OnStack = (str) => {
      var size = lengthBytesUTF8(str) + 1;
      var ret = stackAlloc(size);
      stringToUTF8(str, ret, size);
      return ret;
    };



  
  function getFullscreenElement() {
      return document.fullscreenElement || document.mozFullScreenElement ||
             document.webkitFullscreenElement || document.webkitCurrentFullScreenElement ||
             document.msFullscreenElement;
    }
  
  /** @param {number=} timeout */
  var safeSetTimeout = (func, timeout) => {
      
      return setTimeout(() => {
        
        callUserCallback(func);
      }, timeout);
    };
  
  
  
  var Browser = {
  useWebGL:false,
  isFullscreen:false,
  pointerLock:false,
  moduleContextCreatedCallbacks:[],
  workers:[],
  preloadedImages:{
  },
  preloadedAudios:{
  },
  getCanvas:() => Module['canvas'],
  init() {
        if (Browser.initted) return;
        Browser.initted = true;
  
        // Support for plugins that can process preloaded files. You can add more of these to
        // your app by creating and appending to preloadPlugins.
        //
        // Each plugin is asked if it can handle a file based on the file's name. If it can,
        // it is given the file's raw data. When it is done, it calls a callback with the file's
        // (possibly modified) data. For example, a plugin might decompress a file, or it
        // might create some side data structure for use later (like an Image element, etc.).
  
        var imagePlugin = {};
        imagePlugin['canHandle'] = (name) => {
          return !Module['noImageDecoding'] && /\.(jpg|jpeg|png|bmp|webp)$/i.test(name);
        };
        imagePlugin['handle'] = async (byteArray, name) => {
          var b = new Blob([byteArray], { type: Browser.getMimetype(name) });
          if (b.size !== byteArray.length) { // Safari bug #118630
            // Safari's Blob can only take an ArrayBuffer
            b = new Blob([(new Uint8Array(byteArray)).buffer], { type: Browser.getMimetype(name) });
          }
          var url = URL.createObjectURL(b);
          return new Promise((resolve, reject) => {
            var img = new Image();
            img.onload = () => {
              assert(img.complete, `Image ${name} could not be decoded`);
              var canvas = /** @type {!HTMLCanvasElement} */ (document.createElement('canvas'));
              canvas.width = img.width;
              canvas.height = img.height;
              var ctx = canvas.getContext('2d');
              ctx.drawImage(img, 0, 0);
              Browser.preloadedImages[name] = canvas;
              URL.revokeObjectURL(url);
              resolve(byteArray);
            };
            img.onerror = (event) => {
              err(`Image ${url} could not be decoded`);
              reject();
            };
            img.src = url;
          });
        };
        preloadPlugins.push(imagePlugin);
  
        var audioPlugin = {};
        audioPlugin['canHandle'] = (name) => {
          return !Module['noAudioDecoding'] && name.slice(-4) in { '.ogg': 1, '.wav': 1, '.mp3': 1 };
        };
        audioPlugin['handle'] = async (byteArray, name) => {
          return new Promise((resolve, reject) => {
            var done = false;
            function finish(audio) {
              if (done) return;
              done = true;
              Browser.preloadedAudios[name] = audio;
              resolve(byteArray);
            }
            var b = new Blob([byteArray], { type: Browser.getMimetype(name) });
            var url = URL.createObjectURL(b); // XXX we never revoke this!
            var audio = new Audio();
            audio.addEventListener('canplaythrough', () => finish(audio), false); // use addEventListener due to chromium bug 124926
            audio.onerror = (event) => {
              if (done) return;
              err(`warning: browser could not fully decode audio ${name}, trying slower base64 approach`);
              function encode64(data) {
                var BASE = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
                var PAD = '=';
                var ret = '';
                var leftchar = 0;
                var leftbits = 0;
                for (var i = 0; i < data.length; i++) {
                  leftchar = (leftchar << 8) | data[i];
                  leftbits += 8;
                  while (leftbits >= 6) {
                    var curr = (leftchar >> (leftbits-6)) & 0x3f;
                    leftbits -= 6;
                    ret += BASE[curr];
                  }
                }
                if (leftbits == 2) {
                  ret += BASE[(leftchar&3) << 4];
                  ret += PAD + PAD;
                } else if (leftbits == 4) {
                  ret += BASE[(leftchar&0xf) << 2];
                  ret += PAD;
                }
                return ret;
              }
              audio.src = 'data:audio/x-' + name.slice(-3) + ';base64,' + encode64(byteArray);
              finish(audio); // we don't wait for confirmation this worked - but it's worth trying
            };
            audio.src = url;
            // workaround for chrome bug 124926 - we do not always get oncanplaythrough or onerror
            safeSetTimeout(() => {
              finish(audio); // try to use it even though it is not necessarily ready to play
            }, 10000);
          });
        };
        preloadPlugins.push(audioPlugin);
  
        // Canvas event setup
  
        function pointerLockChange() {
          var canvas = Browser.getCanvas();
          Browser.pointerLock = document.pointerLockElement === canvas;
        }
        var canvas = Browser.getCanvas();
        if (canvas) {
          // forced aspect ratio can be enabled by defining 'forcedAspectRatio' on Module
          // Module['forcedAspectRatio'] = 4 / 3;
  
          document.addEventListener('pointerlockchange', pointerLockChange, false);
  
          if (Module['elementPointerLock']) {
            canvas.addEventListener("click", (ev) => {
              if (!Browser.pointerLock && Browser.getCanvas().requestPointerLock) {
                Browser.getCanvas().requestPointerLock();
                ev.preventDefault();
              }
            }, false);
          }
        }
      },
  createContext(/** @type {HTMLCanvasElement} */ canvas, useWebGL, setInModule, webGLContextAttributes) {
        if (useWebGL && Module['ctx'] && canvas == Browser.getCanvas()) return Module['ctx']; // no need to recreate GL context if it's already been created for this canvas.
  
        var ctx;
        var contextHandle;
        if (useWebGL) {
          // For GLES2/desktop GL compatibility, adjust a few defaults to be different to WebGL defaults, so that they align better with the desktop defaults.
          var contextAttributes = {
            antialias: false,
            alpha: false,
            majorVersion: 1,
          };
  
          if (webGLContextAttributes) {
            for (var attribute in webGLContextAttributes) {
              contextAttributes[attribute] = webGLContextAttributes[attribute];
            }
          }
  
          // This check of existence of GL is here to satisfy Closure compiler, which yells if variable GL is referenced below but GL object is not
          // actually compiled in because application is not doing any GL operations. TODO: Ideally if GL is not being used, this function
          // Browser.createContext() should not even be emitted.
          if (typeof GL != 'undefined') {
            contextHandle = GL.createContext(canvas, contextAttributes);
            if (contextHandle) {
              ctx = GL.getContext(contextHandle).GLctx;
            }
          }
        } else {
          ctx = canvas.getContext('2d');
        }
  
        if (!ctx) return null;
  
        if (setInModule) {
          if (!useWebGL) assert(typeof GLctx == 'undefined', 'cannot set in module if GLctx is used, but we are a non-GL context that would replace it');
          Module['ctx'] = ctx;
          if (useWebGL) GL.makeContextCurrent(contextHandle);
          Browser.useWebGL = useWebGL;
          Browser.moduleContextCreatedCallbacks.forEach((callback) => callback());
          Browser.init();
        }
        return ctx;
      },
  fullscreenHandlersInstalled:false,
  lockPointer:undefined,
  resizeCanvas:undefined,
  requestFullscreen(lockPointer, resizeCanvas) {
        Browser.lockPointer = lockPointer;
        Browser.resizeCanvas = resizeCanvas;
        if (typeof Browser.lockPointer == 'undefined') Browser.lockPointer = true;
        if (typeof Browser.resizeCanvas == 'undefined') Browser.resizeCanvas = false;
  
        var canvas = Browser.getCanvas();
        function fullscreenChange() {
          Browser.isFullscreen = false;
          var canvasContainer = canvas.parentNode;
          if (getFullscreenElement() === canvasContainer) {
            canvas.exitFullscreen = Browser.exitFullscreen;
            if (Browser.lockPointer) canvas.requestPointerLock();
            Browser.isFullscreen = true;
            if (Browser.resizeCanvas) {
              Browser.setFullscreenCanvasSize();
            } else {
              Browser.updateCanvasDimensions(canvas);
            }
          } else {
            // remove the full screen specific parent of the canvas again to restore the HTML structure from before going full screen
            canvasContainer.parentNode.insertBefore(canvas, canvasContainer);
            canvasContainer.parentNode.removeChild(canvasContainer);
  
            if (Browser.resizeCanvas) {
              Browser.setWindowedCanvasSize();
            } else {
              Browser.updateCanvasDimensions(canvas);
            }
          }
          Module['onFullScreen']?.(Browser.isFullscreen);
          Module['onFullscreen']?.(Browser.isFullscreen);
        }
  
        if (!Browser.fullscreenHandlersInstalled) {
          Browser.fullscreenHandlersInstalled = true;
          document.addEventListener('fullscreenchange', fullscreenChange, false);
          document.addEventListener('mozfullscreenchange', fullscreenChange, false);
          document.addEventListener('webkitfullscreenchange', fullscreenChange, false);
          document.addEventListener('MSFullscreenChange', fullscreenChange, false);
        }
  
        // create a new parent to ensure the canvas has no siblings. this allows browsers to optimize full screen performance when its parent is the full screen root
        var canvasContainer = document.createElement("div");
        canvas.parentNode.insertBefore(canvasContainer, canvas);
        canvasContainer.appendChild(canvas);
  
        // use parent of canvas as full screen root to allow aspect ratio correction (Firefox stretches the root to screen size)
        canvasContainer.requestFullscreen = canvasContainer['requestFullscreen'] ||
                                            canvasContainer['mozRequestFullScreen'] ||
                                            canvasContainer['msRequestFullscreen'] ||
                                           (canvasContainer['webkitRequestFullscreen'] ? () => canvasContainer['webkitRequestFullscreen'](Element['ALLOW_KEYBOARD_INPUT']) : null) ||
                                           (canvasContainer['webkitRequestFullScreen'] ? () => canvasContainer['webkitRequestFullScreen'](Element['ALLOW_KEYBOARD_INPUT']) : null);
  
        canvasContainer.requestFullscreen();
      },
  requestFullScreen() {
        abort('Module.requestFullScreen has been replaced by Module.requestFullscreen (without a capital S)');
      },
  exitFullscreen() {
        // This is workaround for chrome. Trying to exit from fullscreen
        // not in fullscreen state will cause "TypeError: Document not active"
        // in chrome. See https://github.com/emscripten-core/emscripten/pull/8236
        if (!Browser.isFullscreen) {
          return false;
        }
  
        var CFS = document['exitFullscreen'] ||
                  document['cancelFullScreen'] ||
                  document['mozCancelFullScreen'] ||
                  document['msExitFullscreen'] ||
                  document['webkitCancelFullScreen'] ||
            (() => {});
        CFS.apply(document, []);
        return true;
      },
  safeSetTimeout(func, timeout) {
        // Legacy function, this is used by the SDL2 port so we need to keep it
        // around at least until that is updated.
        // See https://github.com/libsdl-org/SDL/pull/6304
        return safeSetTimeout(func, timeout);
      },
  getMimetype(name) {
        return {
          'jpg': 'image/jpeg',
          'jpeg': 'image/jpeg',
          'png': 'image/png',
          'bmp': 'image/bmp',
          'ogg': 'audio/ogg',
          'wav': 'audio/wav',
          'mp3': 'audio/mpeg'
        }[name.slice(name.lastIndexOf('.')+1)];
      },
  getUserMedia(func) {
        window.getUserMedia ||= navigator['getUserMedia'] ||
                                navigator['mozGetUserMedia'];
        window.getUserMedia(func);
      },
  getMovementX(event) {
        return event['movementX'] ||
               event['mozMovementX'] ||
               event['webkitMovementX'] ||
               0;
      },
  getMovementY(event) {
        return event['movementY'] ||
               event['mozMovementY'] ||
               event['webkitMovementY'] ||
               0;
      },
  getMouseWheelDelta(event) {
        var delta = 0;
        switch (event.type) {
          case 'DOMMouseScroll':
            // 3 lines make up a step
            delta = event.detail / 3;
            break;
          case 'mousewheel':
            // 120 units make up a step
            delta = event.wheelDelta / 120;
            break;
          case 'wheel':
            delta = event.deltaY
            switch (event.deltaMode) {
              case 0:
                // DOM_DELTA_PIXEL: 100 pixels make up a step
                delta /= 100;
                break;
              case 1:
                // DOM_DELTA_LINE: 3 lines make up a step
                delta /= 3;
                break;
              case 2:
                // DOM_DELTA_PAGE: A page makes up 80 steps
                delta *= 80;
                break;
              default:
                abort('unrecognized mouse wheel delta mode: ' + event.deltaMode);
            }
            break;
          default:
            abort('unrecognized mouse wheel event: ' + event.type);
        }
        return delta;
      },
  mouseX:0,
  mouseY:0,
  mouseMovementX:0,
  mouseMovementY:0,
  touches:{
  },
  lastTouches:{
  },
  calculateMouseCoords(pageX, pageY) {
        // Calculate the movement based on the changes
        // in the coordinates.
        var canvas = Browser.getCanvas();
        var rect = canvas.getBoundingClientRect();
  
        var adjustedX = pageX - (window.scrollX + rect.left);
        var adjustedY = pageY - (window.scrollY + rect.top);
  
        // the canvas might be CSS-scaled compared to its backbuffer;
        // SDL-using content will want mouse coordinates in terms
        // of backbuffer units.
        adjustedX = adjustedX * (canvas.width / rect.width);
        adjustedY = adjustedY * (canvas.height / rect.height);
  
        return { x: adjustedX, y: adjustedY };
      },
  setMouseCoords(pageX, pageY) {
        const {x, y} = Browser.calculateMouseCoords(pageX, pageY);
        Browser.mouseMovementX = x - Browser.mouseX;
        Browser.mouseMovementY = y - Browser.mouseY;
        Browser.mouseX = x;
        Browser.mouseY = y;
      },
  calculateMouseEvent(event) { // event should be mousemove, mousedown or mouseup
        if (Browser.pointerLock) {
          // When the pointer is locked, calculate the coordinates
          // based on the movement of the mouse.
          // Workaround for Firefox bug 764498
          if (event.type != 'mousemove' &&
              ('mozMovementX' in event)) {
            Browser.mouseMovementX = Browser.mouseMovementY = 0;
          } else {
            Browser.mouseMovementX = Browser.getMovementX(event);
            Browser.mouseMovementY = Browser.getMovementY(event);
          }
  
          // add the mouse delta to the current absolute mouse position
          Browser.mouseX += Browser.mouseMovementX;
          Browser.mouseY += Browser.mouseMovementY;
        } else {
          if (event.type === 'touchstart' || event.type === 'touchend' || event.type === 'touchmove') {
            var touch = event.touch;
            if (touch === undefined) {
              return; // the "touch" property is only defined in SDL
  
            }
            var coords = Browser.calculateMouseCoords(touch.pageX, touch.pageY);
  
            if (event.type === 'touchstart') {
              Browser.lastTouches[touch.identifier] = coords;
              Browser.touches[touch.identifier] = coords;
            } else if (event.type === 'touchend' || event.type === 'touchmove') {
              var last = Browser.touches[touch.identifier];
              last ||= coords;
              Browser.lastTouches[touch.identifier] = last;
              Browser.touches[touch.identifier] = coords;
            }
            return;
          }
  
          Browser.setMouseCoords(event.pageX, event.pageY);
        }
      },
  resizeListeners:[],
  updateResizeListeners() {
        var canvas = Browser.getCanvas();
        Browser.resizeListeners.forEach((listener) => listener(canvas.width, canvas.height));
      },
  setCanvasSize(width, height, noUpdates) {
        var canvas = Browser.getCanvas();
        Browser.updateCanvasDimensions(canvas, width, height);
        if (!noUpdates) Browser.updateResizeListeners();
      },
  windowedWidth:0,
  windowedHeight:0,
  setFullscreenCanvasSize() {
        // check if SDL is available
        if (typeof SDL != "undefined") {
          var flags = HEAPU32[((SDL.screen)>>2)];
          flags = flags | 0x00800000; // set SDL_FULLSCREEN flag
          HEAP32[((SDL.screen)>>2)] = flags;
        }
        Browser.updateCanvasDimensions(Browser.getCanvas());
        Browser.updateResizeListeners();
      },
  setWindowedCanvasSize() {
        // check if SDL is available
        if (typeof SDL != "undefined") {
          var flags = HEAPU32[((SDL.screen)>>2)];
          flags = flags & ~0x00800000; // clear SDL_FULLSCREEN flag
          HEAP32[((SDL.screen)>>2)] = flags;
        }
        Browser.updateCanvasDimensions(Browser.getCanvas());
        Browser.updateResizeListeners();
      },
  updateCanvasDimensions(canvas, wNative, hNative) {
        if (wNative && hNative) {
          canvas.widthNative = wNative;
          canvas.heightNative = hNative;
        } else {
          wNative = canvas.widthNative;
          hNative = canvas.heightNative;
        }
        var w = wNative;
        var h = hNative;
        if (Module['forcedAspectRatio'] > 0) {
          if (w/h < Module['forcedAspectRatio']) {
            w = Math.round(h * Module['forcedAspectRatio']);
          } else {
            h = Math.round(w / Module['forcedAspectRatio']);
          }
        }
        if ((getFullscreenElement() === canvas.parentNode) && (typeof screen != 'undefined')) {
           var factor = Math.min(screen.width / w, screen.height / h);
           w = Math.round(w * factor);
           h = Math.round(h * factor);
        }
        if (Browser.resizeCanvas) {
          if (canvas.width  != w) canvas.width  = w;
          if (canvas.height != h) canvas.height = h;
          if (typeof canvas.style != 'undefined') {
            canvas.style.removeProperty( "width");
            canvas.style.removeProperty("height");
          }
        } else {
          if (canvas.width  != wNative) canvas.width  = wNative;
          if (canvas.height != hNative) canvas.height = hNative;
          if (typeof canvas.style != 'undefined') {
            if (w != wNative || h != hNative) {
              canvas.style.setProperty( "width", w + "px", "important");
              canvas.style.setProperty("height", h + "px", "important");
            } else {
              canvas.style.removeProperty( "width");
              canvas.style.removeProperty("height");
            }
          }
        }
      },
  };
  var requestFullscreen = Browser.requestFullscreen;

  var FS_createPath = (...args) => FS.createPath(...args);



  var FS_unlink = (...args) => FS.unlink(...args);

  var FS_createLazyFile = (...args) => FS.createLazyFile(...args);

  var FS_createDevice = (...args) => FS.createDevice(...args);



  FS.createPreloadedFile = FS_createPreloadedFile;
  FS.preloadFile = FS_preloadFile;
  FS.staticInit();;

      Module['requestAnimationFrame'] = MainLoop.requestAnimationFrame;
      Module['pauseMainLoop'] = MainLoop.pause;
      Module['resumeMainLoop'] = MainLoop.resume;
      MainLoop.init();;
// End JS library code

// include: postlibrary.js
// This file is included after the automatically-generated JS library code
// but before the wasm module is created.

{

  // Begin ATMODULES hooks
  if (Module['noExitRuntime']) noExitRuntime = Module['noExitRuntime'];
if (Module['preloadPlugins']) preloadPlugins = Module['preloadPlugins'];
if (Module['print']) out = Module['print'];
if (Module['printErr']) err = Module['printErr'];
if (Module['wasmBinary']) wasmBinary = Module['wasmBinary'];
  // End ATMODULES hooks

  checkIncomingModuleAPI();

  if (Module['arguments']) programArgs = Module['arguments'];
  if (Module['thisProgram']) thisProgram = Module['thisProgram'];

  // Assertions on removed incoming Module JS APIs.
  assert(typeof Module['memoryInitializerPrefixURL'] == 'undefined', 'Module.memoryInitializerPrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['pthreadMainPrefixURL'] == 'undefined', 'Module.pthreadMainPrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['cdInitializerPrefixURL'] == 'undefined', 'Module.cdInitializerPrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['filePackagePrefixURL'] == 'undefined', 'Module.filePackagePrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['read'] == 'undefined', 'Module.read option was removed');
  assert(typeof Module['readAsync'] == 'undefined', 'Module.readAsync option was removed (modify readAsync in JS)');
  assert(typeof Module['readBinary'] == 'undefined', 'Module.readBinary option was removed (modify readBinary in JS)');
  assert(typeof Module['setWindowTitle'] == 'undefined', 'Module.setWindowTitle option was removed (modify emscripten_set_window_title in JS)');
  assert(typeof Module['TOTAL_MEMORY'] == 'undefined', 'Module.TOTAL_MEMORY has been renamed Module.INITIAL_MEMORY');
  assert(typeof Module['ENVIRONMENT'] == 'undefined', 'Module.ENVIRONMENT has been deprecated. To force the environment, use the ENVIRONMENT compile-time option (for example, -sENVIRONMENT=web or -sENVIRONMENT=node)');
  assert(typeof Module['STACK_SIZE'] == 'undefined', 'STACK_SIZE can no longer be set at runtime.  Use -sSTACK_SIZE at link time')
  // If memory is defined in wasm, the user can't provide it, or set INITIAL_MEMORY
  assert(typeof Module['wasmMemory'] == 'undefined', 'Use of `wasmMemory` detected.  Use -sIMPORTED_MEMORY to define wasmMemory externally');
  assert(typeof Module['INITIAL_MEMORY'] == 'undefined', 'Detected runtime INITIAL_MEMORY setting.  Use -sIMPORTED_MEMORY to define wasmMemory dynamically');

  if (Module['preInit']) {
    if (typeof Module['preInit'] == 'function') Module['preInit'] = [Module['preInit']];
    while (Module['preInit'].length > 0) {
      Module['preInit'].shift()();
    }
  }
  consumedModuleProp('preInit');
}

// Begin runtime exports
  Module['addRunDependency'] = addRunDependency;
  Module['removeRunDependency'] = removeRunDependency;
  Module['requestFullscreen'] = requestFullscreen;
  Module['FS_preloadFile'] = FS_preloadFile;
  Module['FS_unlink'] = FS_unlink;
  Module['FS_createPath'] = FS_createPath;
  Module['FS_createDevice'] = FS_createDevice;
  Module['FS_createDataFile'] = FS_createDataFile;
  Module['FS_createLazyFile'] = FS_createLazyFile;
  var missingLibrarySymbols = [
  'writeI53ToI64',
  'writeI53ToI64Clamped',
  'writeI53ToI64Signaling',
  'writeI53ToU64Clamped',
  'writeI53ToU64Signaling',
  'readI53FromI64',
  'readI53FromU64',
  'convertI32PairToI53',
  'convertI32PairToI53Checked',
  'convertU32PairToI53',
  'getTempRet0',
  'createNamedFunction',
  'zeroMemory',
  'withStackSave',
  'inetPton4',
  'inetNtop4',
  'inetPton6',
  'inetNtop6',
  'readSockaddr',
  'writeSockaddr',
  'runMainThreadEmAsm',
  'jstoi_q',
  'autoResumeAudioContext',
  'getDynCaller',
  'dynCall',
  'runtimeKeepalivePush',
  'runtimeKeepalivePop',
  'asmjsMangle',
  'HandleAllocator',
  'addOnInit',
  'addOnPostCtor',
  'addOnPreMain',
  'addOnExit',
  'STACK_SIZE',
  'STACK_ALIGN',
  'POINTER_SIZE',
  'ASSERTIONS',
  'ccall',
  'cwrap',
  'convertJsFunctionToWasm',
  'getEmptyTableSlot',
  'updateTableMap',
  'getFunctionAddress',
  'addFunction',
  'removeFunction',
  'intArrayToString',
  'AsciiToString',
  'stringToAscii',
  'UTF16ToString',
  'stringToUTF16',
  'lengthBytesUTF16',
  'UTF32ToString',
  'stringToUTF32',
  'lengthBytesUTF32',
  'stringToNewUTF8',
  'writeArrayToMemory',
  'registerKeyEventCallback',
  'maybeCStringToJsString',
  'findEventTarget',
  'getBoundingClientRect',
  'fillMouseEventData',
  'registerMouseEventCallback',
  'registerWheelEventCallback',
  'registerUiEventCallback',
  'registerFocusEventCallback',
  'fillDeviceOrientationEventData',
  'registerDeviceOrientationEventCallback',
  'fillDeviceMotionEventData',
  'registerDeviceMotionEventCallback',
  'screenOrientation',
  'fillOrientationChangeEventData',
  'registerOrientationChangeEventCallback',
  'fillFullscreenChangeEventData',
  'registerFullscreenChangeEventCallback',
  'JSEvents_requestFullscreen',
  'JSEvents_resizeCanvasForFullscreen',
  'registerRestoreOldStyle',
  'hideEverythingExceptGivenElement',
  'restoreHiddenElements',
  'setLetterbox',
  'softFullscreenResizeWebGLRenderTarget',
  'doRequestFullscreen',
  'fillPointerlockChangeEventData',
  'registerPointerlockChangeEventCallback',
  'registerPointerlockErrorEventCallback',
  'requestPointerLock',
  'fillVisibilityChangeEventData',
  'registerVisibilityChangeEventCallback',
  'registerTouchEventCallback',
  'fillGamepadEventData',
  'registerGamepadEventCallback',
  'registerBeforeUnloadEventCallback',
  'fillBatteryEventData',
  'registerBatteryEventCallback',
  'setCanvasElementSize',
  'getCanvasElementSize',
  'jsStackTrace',
  'getCallstack',
  'convertPCtoSourceLocation',
  'wasiRightsToMuslOFlags',
  'wasiOFlagsToMuslOFlags',
  'setImmediateWrapped',
  'safeRequestAnimationFrame',
  'clearImmediateWrapped',
  'registerPostMainLoop',
  'registerPreMainLoop',
  'getPromise',
  'makePromise',
  'addPromise',
  'idsToPromises',
  'makePromiseCallback',
  'incrementUncaughtExceptionCount',
  'decrementUncaughtExceptionCount',
  'Browser_asyncPrepareDataCounter',
  'arraySum',
  'addDays',
  'getSocketFromFD',
  'getSocketAddress',
  'FS_mkdirTree',
  '_setNetworkCallback',
  'heapObjectForWebGLType',
  'toTypedArrayIndex',
  'webgl_enable_ANGLE_instanced_arrays',
  'webgl_enable_OES_vertex_array_object',
  'webgl_enable_WEBGL_draw_buffers',
  'webgl_enable_WEBGL_multi_draw',
  'webgl_enable_EXT_polygon_offset_clamp',
  'webgl_enable_EXT_clip_control',
  'webgl_enable_WEBGL_polygon_mode',
  'emscriptenWebGLGet',
  'computeUnpackAlignedImageSize',
  'colorChannelsInGlTextureFormat',
  'emscriptenWebGLGetTexPixelData',
  'emscriptenWebGLGetUniform',
  'webglGetProgramUniformLocation',
  'webglGetUniformLocation',
  'webglPrepareUniformLocationsBeforeFirstUse',
  'webglGetLeftBracePos',
  'emscriptenWebGLGetVertexAttrib',
  '__glGetActiveAttribOrUniform',
  'writeGLArray',
  'registerWebGlEventCallback',
  'runAndAbortIfError',
  'ALLOC_NORMAL',
  'ALLOC_STACK',
  'allocate',
  'writeStringToMemory',
  'writeAsciiToMemory',
  'allocateUTF8',
  'allocateUTF8OnStack',
  'demangle',
  'stackTrace',
  'getNativeTypeSize',
];
missingLibrarySymbols.forEach(missingLibrarySymbol)

  var unexportedSymbols = [
  'run',
  'out',
  'err',
  'callMain',
  'abort',
  'wasmExports',
  'writeStackCookie',
  'checkStackCookie',
  'INT53_MAX',
  'INT53_MIN',
  'bigintToI53Checked',
  'HEAP8',
  'HEAPU8',
  'HEAP16',
  'HEAPU16',
  'HEAP32',
  'HEAPU32',
  'HEAPF32',
  'HEAPF64',
  'HEAP64',
  'HEAPU64',
  'stackSave',
  'stackRestore',
  'stackAlloc',
  'setTempRet0',
  'ptrToString',
  'exitJS',
  'getHeapMax',
  'growMemory',
  'ENV',
  'ERRNO_CODES',
  'strError',
  'DNS',
  'Protocols',
  'Sockets',
  'timers',
  'warnOnce',
  'readEmAsmArgsArray',
  'readEmAsmArgs',
  'runEmAsmFunction',
  'getExecutableName',
  'handleException',
  'keepRuntimeAlive',
  'callUserCallback',
  'maybeExit',
  'asyncLoad',
  'alignMemory',
  'mmapAlloc',
  'wasmTable',
  'wasmMemory',
  'getUniqueRunDependency',
  'noExitRuntime',
  'addOnPreRun',
  'addOnPostRun',
  'freeTableIndexes',
  'functionsInTableMap',
  'setValue',
  'getValue',
  'PATH',
  'PATH_FS',
  'UTF8Decoder',
  'UTF8ArrayToString',
  'UTF8ToString',
  'stringToUTF8Array',
  'stringToUTF8',
  'lengthBytesUTF8',
  'intArrayFromString',
  'UTF16Decoder',
  'stringToUTF8OnStack',
  'JSEvents',
  'specialHTMLTargets',
  'findCanvasEventTarget',
  'currentFullscreenStrategy',
  'restoreOldWindowedStyle',
  'UNWIND_CACHE',
  'ExitStatus',
  'getEnvStrings',
  'checkWasiClock',
  'doReadv',
  'doWritev',
  'initRandomFill',
  'randomFill',
  'safeSetTimeout',
  'emSetImmediate',
  'emClearImmediate_deps',
  'emClearImmediate',
  'promiseMap',
  'uncaughtExceptionCount',
  'exceptionLast',
  'exceptionCaught',
  'ExceptionInfo',
  'findMatchingCatch',
  'getExceptionMessageCommon',
  'incrementExceptionRefcount',
  'decrementExceptionRefcount',
  'getExceptionMessage',
  'Browser',
  'requestFullScreen',
  'setCanvasSize',
  'getUserMedia',
  'createContext',
  'getPreloadedImageData__data',
  'wget',
  'MONTH_DAYS_REGULAR',
  'MONTH_DAYS_LEAP',
  'MONTH_DAYS_REGULAR_CUMULATIVE',
  'MONTH_DAYS_LEAP_CUMULATIVE',
  'isLeapYear',
  'ydayFromDate',
  'SYSCALLS',
  'preloadPlugins',
  'FS_createPreloadedFile',
  'FS_modeStringToFlags',
  'FS_getMode',
  'FS_fileDataToTypedArray',
  'FS_stdin_getChar_buffer',
  'FS_stdin_getChar',
  'FS_readFile',
  'FS',
  'FS_root',
  'FS_mounts',
  'FS_devices',
  'FS_streams',
  'FS_nextInode',
  'FS_nameTable',
  'FS_currentPath',
  'FS_initialized',
  'FS_ignorePermissions',
  'FS_filesystems',
  'FS_syncFSRequests',
  'FS_lookupPath',
  'FS_getPath',
  'FS_hashName',
  'FS_hashAddNode',
  'FS_hashRemoveNode',
  'FS_lookupNode',
  'FS_createNode',
  'FS_destroyNode',
  'FS_isRoot',
  'FS_isMountpoint',
  'FS_isFile',
  'FS_isDir',
  'FS_isLink',
  'FS_isChrdev',
  'FS_isBlkdev',
  'FS_isFIFO',
  'FS_isSocket',
  'FS_flagsToPermissionString',
  'FS_nodePermissions',
  'FS_mayLookup',
  'FS_mayCreate',
  'FS_mayDelete',
  'FS_mayOpen',
  'FS_checkOpExists',
  'FS_nextfd',
  'FS_getStreamChecked',
  'FS_getStream',
  'FS_createStream',
  'FS_closeStream',
  'FS_dupStream',
  'FS_doSetAttr',
  'FS_chrdev_stream_ops',
  'FS_major',
  'FS_minor',
  'FS_makedev',
  'FS_registerDevice',
  'FS_getDevice',
  'FS_getMounts',
  'FS_syncfs',
  'FS_mount',
  'FS_unmount',
  'FS_lookup',
  'FS_mknod',
  'FS_statfs',
  'FS_statfsStream',
  'FS_statfsNode',
  'FS_create',
  'FS_mkdir',
  'FS_mkdev',
  'FS_symlink',
  'FS_rename',
  'FS_rmdir',
  'FS_readdir',
  'FS_readlink',
  'FS_stat',
  'FS_fstat',
  'FS_lstat',
  'FS_doChmod',
  'FS_chmod',
  'FS_lchmod',
  'FS_fchmod',
  'FS_doChown',
  'FS_chown',
  'FS_lchown',
  'FS_fchown',
  'FS_doTruncate',
  'FS_truncate',
  'FS_ftruncate',
  'FS_utime',
  'FS_open',
  'FS_close',
  'FS_isClosed',
  'FS_llseek',
  'FS_read',
  'FS_write',
  'FS_mmap',
  'FS_msync',
  'FS_ioctl',
  'FS_writeFile',
  'FS_cwd',
  'FS_chdir',
  'FS_createDefaultDirectories',
  'FS_createDefaultDevices',
  'FS_createSpecialDirectories',
  'FS_createStandardStreams',
  'FS_staticInit',
  'FS_init',
  'FS_quit',
  'FS_findObject',
  'FS_analyzePath',
  'FS_createFile',
  'FS_forceLoadFile',
  'MEMFS',
  'TTY',
  'PIPEFS',
  'SOCKFS',
  'tempFixedLengthArray',
  'miniTempWebGLFloatBuffers',
  'miniTempWebGLIntBuffers',
  'GL',
  'AL',
  'GLUT',
  'EGL',
  'GLEW',
  'IDBStore',
  'SDL',
  'SDL_gfx',
  'print',
  'printErr',
  'jstoi_s',
  'IDBFS',
];
unexportedSymbols.forEach(unexportedRuntimeSymbol);

  // End runtime exports
  // Begin JS library exports
  // End JS library exports

// end include: postlibrary.js

function checkIncomingModuleAPI() {
  ignoredModuleProp('fetchSettings');
  ignoredModuleProp('logReadFiles');
  ignoredModuleProp('loadSplitModule');
  ignoredModuleProp('onMalloc');
  ignoredModuleProp('onRealloc');
  ignoredModuleProp('onFree');
  ignoredModuleProp('onSbrkGrow');
}
var ASM_CONSTS = {
  1052812: () => { (function() { if (typeof window === 'undefined') return; if (Module['ggnInputInstalled']) return; Module['ggnInputInstalled'] = true; Module['ggnKeys'] = Module['ggnKeys'] || {}; Module['ggnShouldBlockKey'] = function(keyCode) { switch (keyCode | 0) { case 13: case 16: case 32: case 37: case 38: case 39: case 40: case 65: case 67: case 68: case 73: case 74: case 75: case 76: case 83: case 86: case 88: case 90: return true; default: return false; } }; function setKey(keyCode, pressed) { Module['ggnKeys'][keyCode | 0] = pressed ? 1 : 0; } window.addEventListener('keydown', function(e) { setKey(e.keyCode | 0, true); if (Module['ggnShouldBlockKey'](e.keyCode | 0)) e.preventDefault(); }, false); window.addEventListener('keyup', function(e) { setKey(e.keyCode | 0, false); if (Module['ggnShouldBlockKey'](e.keyCode | 0)) e.preventDefault(); }, false); window.addEventListener('blur', function() { Module['ggnKeys'] = {}; }, false); window.addEventListener('contextmenu', function(e) { e.preventDefault(); }, false); function installMobileViewport() { if (!document.querySelector('meta[name="viewport"]')) { var viewport = document.createElement('meta'); viewport.name = 'viewport'; viewport.content = 'width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no, viewport-fit=cover'; document.head.appendChild(viewport); } } function installMobileStyles() { if (document.getElementById('ggn-mobile-style')) return; var style = document.createElement('style'); style.id = 'ggn-mobile-style'; style.textContent = [ 'html, body { margin: 0; background: #050505; overscroll-behavior: none; }', 'body.ggn-mobile-ready { overflow: hidden; touch-action: none; }', '#emscripten_logo, #controls, #output { display: none !important; }', 'div.emscripten_border { border: 0 !important; width: 100vw; height: 100vh; height: 100dvh; display: flex; align-items: center; justify-content: center; background: #000; }', 'canvas.emscripten { width: min(100vw, calc(100vh * 1.333333)); width: min(100vw, calc(100dvh * 1.333333)); height: min(100vh, calc(100vw * 0.75)); height: min(100dvh, calc(100vw * 0.75)); image-rendering: pixelated; image-rendering: crisp-edges; }', '#ggn-touch-controls { display: none; position: fixed; inset: auto 0 0 0; height: min(42vh, 260px); z-index: 20; pointer-events: none; user-select: none; -webkit-user-select: none; touch-action: none; }', '#ggn-touch-controls .ggn-pad { position: absolute; bottom: max(12px, env(safe-area-inset-bottom)); display: grid; grid-template-columns: repeat(3, 54px); grid-template-rows: repeat(3, 54px); gap: 8px; pointer-events: none; }', '#ggn-touch-controls .ggn-left { left: max(12px, env(safe-area-inset-left)); }', '#ggn-touch-controls .ggn-right { right: max(12px, env(safe-area-inset-right)); }', '#ggn-touch-controls button { pointer-events: auto; width: 54px; height: 54px; border: 1px solid rgba(255,255,255,.65); border-radius: 8px; background: rgba(10,10,10,.62); color: #fff; font: 700 13px/1 Arial, sans-serif; padding: 0; touch-action: none; -webkit-tap-highlight-color: transparent; }', '#ggn-touch-controls button.ggn-wide { font-size: 11px; }', '#ggn-touch-controls button.ggn-active { background: rgba(255,255,255,.86); color: #000; }', '#ggn-btn-up { grid-column: 2; grid-row: 1; } #ggn-btn-left { grid-column: 1; grid-row: 2; } #ggn-btn-right { grid-column: 3; grid-row: 2; } #ggn-btn-down { grid-column: 2; grid-row: 3; }', '#ggn-btn-diag { grid-column: 1; grid-row: 1; } #ggn-btn-turn { grid-column: 2; grid-row: 1; } #ggn-btn-map { grid-column: 3; grid-row: 1; }', '#ggn-btn-dash { grid-column: 1; grid-row: 2; } #ggn-btn-attack { grid-column: 2; grid-row: 2; } #ggn-btn-menu { grid-column: 3; grid-row: 2; }', '#ggn-btn-smartdash { grid-column: 1; grid-row: 3; } #ggn-btn-shot { grid-column: 2; grid-row: 3; }', '@media (pointer: coarse), (max-width: 900px) { #ggn-touch-controls { display: block; } }', '@media (max-width: 560px) { #ggn-touch-controls .ggn-pad { grid-template-columns: repeat(3, 48px); grid-template-rows: repeat(3, 48px); gap: 7px; } #ggn-touch-controls button { width: 48px; height: 48px; font-size: 12px; } }' ].join(String.fromCharCode(10)); document.head.appendChild(style); document.body.classList.add('ggn-mobile-ready'); } function installTouchControls() { if (document.getElementById('ggn-touch-controls')) return; var controls = document.createElement('div'); controls.id = 'ggn-touch-controls'; var left = document.createElement('div'); left.className = 'ggn-pad ggn-left'; var right = document.createElement('div'); right.className = 'ggn-pad ggn-right'; controls.appendChild(left); controls.appendChild(right); document.body.appendChild(controls); var specs = [ { parent: left, id: 'ggn-btn-up', key: 38, label: '^', name: 'Up' }, { parent: left, id: 'ggn-btn-left', key: 37, label: '<', name: 'Left' }, { parent: left, id: 'ggn-btn-right', key: 39, label: '>', name: 'Right' }, { parent: left, id: 'ggn-btn-down', key: 40, label: 'v', name: 'Down' }, { parent: right, id: 'ggn-btn-diag', key: 16, label: 'Diag', name: 'Diagonal', wide: true }, { parent: right, id: 'ggn-btn-turn', key: 67, label: 'Turn', name: 'Turn', wide: true }, { parent: right, id: 'ggn-btn-map', key: 32, label: 'Map', name: 'Map', wide: true }, { parent: right, id: 'ggn-btn-dash', key: 88, label: 'Dash', name: 'Dash', wide: true }, { parent: right, id: 'ggn-btn-attack', key: 90, label: 'Z', name: 'Attack' }, { parent: right, id: 'ggn-btn-menu', key: 86, label: 'Menu', name: 'Menu', wide: true }, { parent: right, id: 'ggn-btn-smartdash', key: 68, label: 'SD', name: 'Smart dash' }, { parent: right, id: 'ggn-btn-shot', key: 83, label: 'Shot', name: 'Shot', wide: true } ]; function releaseButton(button, key) { setKey(key, false); button.classList.remove('ggn-active'); } specs.forEach(function(spec) { var button = document.createElement('button'); button.type = 'button'; button.id = spec.id; button.textContent = spec.label; button.setAttribute('aria-label', spec.name); if (spec.wide) button.className = 'ggn-wide'; button.addEventListener('pointerdown', function(e) { e.preventDefault(); try { button.setPointerCapture(e.pointerId); } catch (ignore) {} setKey(spec.key, true); button.classList.add('ggn-active'); }, { passive: false }); button.addEventListener('pointerup', function(e) { e.preventDefault(); releaseButton(button, spec.key); }, { passive: false }); button.addEventListener('pointercancel', function(e) { e.preventDefault(); releaseButton(button, spec.key); }, { passive: false }); button.addEventListener('lostpointercapture', function() { releaseButton(button, spec.key); }, false); spec.parent.appendChild(button); }); window.addEventListener('visibilitychange', function() { if (document.hidden) Module['ggnKeys'] = {}; }, false); } installMobileViewport(); installMobileStyles(); installTouchControls(); })() },  
 1059644: ($0) => { var keys = Module['ggnKeys']; return keys && keys[$0 | 0] ? 1 : 0; },  
 1059715: () => { if (typeof FS === 'undefined' || !Module['ggnSaveMounted']) return; if (Module['ggnSaveSyncRunning']) { Module['ggnSaveSyncAgain'] = 1; return; } var runSync = function() { Module['ggnSaveSyncRunning'] = 1; FS.syncfs(false, function(err) { if (err && typeof console !== 'undefined') console.error('save sync failed', err); Module['ggnSaveSyncRunning'] = 0; if (Module['ggnSaveSyncAgain']) { Module['ggnSaveSyncAgain'] = 0; runSync(); } }); }; runSync(); },  
 1060173: () => { if (typeof FS === 'undefined' || typeof IDBFS === 'undefined') { Module['ggnSaveReady'] = 1; return; } try { FS.mkdir('/save'); } catch (e) { } try { if (!Module['ggnSaveMounted']) { FS.mount(IDBFS, {}, '/save'); Module['ggnSaveMounted'] = 1; } } catch (e) { if (typeof console !== 'undefined') console.error('save mount failed', e); Module['ggnSaveReady'] = 1; return; } Module['ggnSaveReady'] = 0; FS.syncfs(true, function(err) { if (err && typeof console !== 'undefined') console.error('save load failed', err); Module['ggnSaveReady'] = 1; }); },  
 1060724: () => { return Module['ggnSaveReady'] ? 1 : 0; },  
 1060767: () => { function clamp(value) { if (!isFinite(value) || value <= 0) return 1.0; return Math.max(0.25, Math.min(1.0, value)); } if (typeof Module['ggnRenderScale'] !== 'undefined') { var moduleScale = parseFloat(Module['ggnRenderScale']); if (isFinite(moduleScale) && moduleScale > 0) return clamp(moduleScale); } var paramScale = NaN; try { paramScale = parseFloat(new URLSearchParams(location.search).get('renderScale')); } catch (e) { } if (isFinite(paramScale) && paramScale > 0) return clamp(paramScale); var coarse = false; try { coarse = !!(window.matchMedia && window.matchMedia('(pointer: coarse)').matches); } catch (e) { } var layoutShortSide = Math.min(window.innerWidth || 800, window.innerHeight || 600); var screenShortSide = layoutShortSide; try { if (window.screen && screen.width > 0 && screen.height > 0) { screenShortSide = Math.min(screen.width, screen.height); } } catch (e) { } var shortSide = Math.min(layoutShortSide, screenShortSide); if (shortSide <= 520) return 0.375; if (coarse || shortSide <= 700) return 0.5; return 1.0; },  
 1061815: ($0, $1, $2, $3, $4, $5, $6) => { var ptr = $0 >>> 0; var width = $1 | 0; var height = $2 | 0; var canvas = Module['canvas'] || document.getElementById('canvas'); if (!canvas) { canvas = document.createElement('canvas'); canvas.id = 'canvas'; document.body.appendChild(canvas); Module['canvas'] = canvas; } if (canvas.width !== width) canvas.width = width; if (canvas.height !== height) canvas.height = height; var context = Module['ggnCanvasContext']; if (!context || Module['ggnCanvas'] !== canvas) { context = canvas.getContext('2d'); Module['ggnCanvasContext'] = context; Module['ggnCanvas'] = canvas; context.imageSmoothingEnabled = false; } var image = Module['ggnImageData']; if (!image || image.width !== width || image.height !== height) { image = context.createImageData(width, height); Module['ggnImageData'] = image; } var pixels = HEAPU32.subarray(ptr >>> 2, (ptr >>> 2) + width * height); var output = image.data; var output32 = Module['ggnImageData32']; if (!output32 || output32.buffer !== output.buffer) { output32 = new Uint32Array(output.buffer); Module['ggnImageData32'] = output32; } for (var i = 0; i < pixels.length; ++i) { var color = pixels[i] >>> 0; output32[i] = (color & 0xff000000) | ((color & 0x000000ff) << 16) | (color & 0x0000ff00) | ((color & 0x00ff0000) >>> 16); } context.putImageData(image, 0, 0); var now = performance.now(); Module['ggnPresentCount'] = (Module['ggnPresentCount'] || 0) + 1; if (!Module['ggnPresentFpsStart']) { Module['ggnPresentFpsStart'] = now; Module['ggnPresentFpsFrames'] = 0; } Module['ggnPresentFpsFrames'] = (Module['ggnPresentFpsFrames'] || 0) + 1; var elapsed = now - Module['ggnPresentFpsStart']; if (elapsed >= 1000) { Module['ggnPresentFps'] = Module['ggnPresentFpsFrames'] * 1000 / elapsed; Module['ggnPresentFpsStart'] = now; Module['ggnPresentFpsFrames'] = 0; } var renderInfo = Module['ggnRenderInfo']; if (!renderInfo) { renderInfo = new Object(); Module['ggnRenderInfo'] = renderInfo; } renderInfo['width'] = width; renderInfo['height'] = height; renderInfo['logicalWidth'] = $3 | 0; renderInfo['logicalHeight'] = $4 | 0; renderInfo['scaleX'] = +$5; renderInfo['scaleY'] = +$6; renderInfo['presentFps'] = Module['ggnPresentFps'] || 0; },  
 1063995: () => { (function() { if (Module.ggnAudio) return; var state = { context: null, masterGain: null, seGain: null, bgmGain: null, buffers: {}, loading: {}, resolvedPaths: {}, errors: [], loadCount: 0, playCount: 0, bgmToken: 0, bgmSources: [], bgmPath: '' }; function normalize(path) { path = (path || '').split(String.fromCharCode(92)).join('/'); while (path.indexOf('./') === 0) path = path.substring(2); return path; } function context() { if (state.context) return state.context; var Ctor = window.AudioContext || window.webkitAudioContext; if (!Ctor) return null; state.context = new Ctor(); state.masterGain = state.context.createGain(); state.seGain = state.context.createGain(); state.bgmGain = state.context.createGain(); state.seGain.gain.value = 1; state.bgmGain.gain.value = 1; state.seGain.connect(state.masterGain); state.bgmGain.connect(state.masterGain); state.masterGain.connect(state.context.destination); return state.context; } function resume() { var ctx = context(); if (ctx && ctx.state === 'suspended') ctx.resume(); } ['keydown', 'pointerdown', 'mousedown', 'touchstart'].forEach(function(name) { window.addEventListener(name, resume, { once: false, passive: true }); }); function candidatesForPath(path) { if (path.indexOf('/music/') >= 0 && path.slice(-4).toLowerCase() === '.wav') { return [path.slice(0, -4) + '.m4a', path]; } return [path]; } function decodeCandidate(requestedPath, candidates, index) { var ctx = context(); if (!ctx || index >= candidates.length) return Promise.resolve(null); var actualPath = candidates[index]; return fetch(actualPath) .then(function(response) { if (!response.ok) { if (index + 1 < candidates.length) return decodeCandidate(requestedPath, candidates, index + 1); throw new Error('audio fetch ' + response.status + ': ' + actualPath); } return response.arrayBuffer().then(function(data) { return ctx.decodeAudioData(data); }).then(function(buffer) { state.buffers[requestedPath] = buffer; state.resolvedPaths[requestedPath] = actualPath; state.loading[requestedPath] = null; return buffer; }); }) .catch(function(error) { if (index + 1 < candidates.length) return decodeCandidate(requestedPath, candidates, index + 1); console.warn('[ggn audio]', error); state.errors.push(String(error)); state.loading[requestedPath] = null; return null; }); } function load(path) { path = normalize(path); if (!path) return Promise.resolve(null); if (state.buffers[path]) return Promise.resolve(state.buffers[path]); if (state.loading[path]) return state.loading[path]; if (!context()) return Promise.resolve(null); state.loadCount++; state.loading[path] = decodeCandidate(path, candidatesForPath(path), 0); return state.loading[path]; } function stopBgm() { state.bgmToken++; state.bgmSources.forEach(function(source) { try { source.stop(0); } catch (e) {} }); state.bgmSources = []; } function startSource(buffer, gain, loop, when) { var ctx = context(); if (!ctx || !buffer) return null; var source = ctx.createBufferSource(); source.buffer = buffer; source.loop = !!loop; source.connect(gain); source.start(typeof when === 'number' ? when : 0); state.playCount++; return source; } Module.ggnAudio = state; Module.ggnAudioEnsure = function() { return !!context(); }; Module.ggnAudioLoad = function(path) { return load(path); }; Module.ggnAudioSetVolume = function(seVolume, bgmVolume) { context(); var se = Math.max(0, Math.min(1, seVolume / 100)); var bgm = Math.max(0, Math.min(1, bgmVolume / 100)); if (state.seGain) state.seGain.gain.value = se; if (state.bgmGain) state.bgmGain.gain.value = bgm; }; Module.ggnAudioPlaySe = function(path) { path = normalize(path); resume(); load(path).then(function(buffer) { if (buffer) startSource(buffer, state.seGain, false, null); }); }; Module.ggnAudioStopBgm = stopBgm; Module.ggnAudioPlayBgm = function(beginPath, repeatPath) { beginPath = normalize(beginPath); repeatPath = normalize(repeatPath); stopBgm(); state.bgmPath = beginPath + ';' + repeatPath; var token = state.bgmToken; resume(); if (beginPath && repeatPath) { Promise.all([load(beginPath), load(repeatPath)]).then(function(buffers) { if (state.bgmToken !== token || !buffers[0] || !buffers[1]) return; var ctx = context(); if (!ctx) return; var startAt = ctx.currentTime + 0.05; var introSource = startSource(buffers[0], state.bgmGain, false, startAt); var loopSource = startSource(buffers[1], state.bgmGain, true, startAt + buffers[0].duration); if (introSource) state.bgmSources.push(introSource); if (loopSource) state.bgmSources.push(loopSource); }); } else if (beginPath) { load(beginPath).then(function(buffer) { if (state.bgmToken !== token || !buffer) return; var source = startSource(buffer, state.bgmGain, true, 0); if (source) state.bgmSources.push(source); }); } else if (repeatPath) { load(repeatPath).then(function(buffer) { if (state.bgmToken !== token || !buffer) return; var source = startSource(buffer, state.bgmGain, true, 0); if (source) state.bgmSources.push(source); }); } }; })() },  
 1068951: ($0, $1) => { if (Module.ggnAudioSetVolume) Module.ggnAudioSetVolume($0, $1); },  
 1069019: ($0) => { if (Module.ggnAudioLoad) Module.ggnAudioLoad(UTF8ToString($0)); },  
 1069087: ($0) => { if (Module.ggnAudioPlaySe) Module.ggnAudioPlaySe(UTF8ToString($0)); },  
 1069159: ($0, $1) => { if (Module.ggnAudioPlayBgm) Module.ggnAudioPlayBgm(UTF8ToString($0), UTF8ToString($1)); },  
 1069251: () => { if (Module.ggnAudioStopBgm) Module.ggnAudioStopBgm(); },  
 1069309: ($0, $1, $2) => { var code = $0 >>> 0; var size = $1 | 0; var weight = $2 | 0; if (size <= 0) size = 16; var canvas = Module['ggnGlyphMeasureCanvas']; if (!canvas) { canvas = document.createElement('canvas'); Module['ggnGlyphMeasureCanvas'] = canvas; } var ctx = canvas.getContext('2d', { willReadFrequently: true }); var ch = '?'; try { ch = String.fromCodePoint(code); } catch (e) {} ctx.font = (weight >= 700 ? 'bold ' : '') + size + 'px Meiryo, "Yu Gothic", sans-serif'; var width = Math.ceil(ctx.measureText(ch).width); return width > 0 ? width : 0; },  
 1069850: ($0, $1, $2, $3, $4, $5) => { var code = $0 >>> 0; var width = $1 | 0; var height = $2 | 0; var out = $3 >>> 0; var pitch = $4 | 0; var weight = $5 | 0; var canvas = Module['ggnGlyphCanvas']; if (!canvas) { canvas = document.createElement('canvas'); Module['ggnGlyphCanvas'] = canvas; } canvas.width = width; canvas.height = height; var ctx = canvas.getContext('2d', { willReadFrequently: true }); ctx.clearRect(0, 0, width, height); ctx.fillStyle = '#fff'; ctx.textBaseline = 'top'; ctx.font = (weight >= 700 ? 'bold ' : '') + height + 'px Meiryo, "Yu Gothic", sans-serif'; var ch = '?'; try { ch = String.fromCodePoint(code); } catch (e) {} ctx.fillText(ch, 0, 0); var pixels = ctx.getImageData(0, 0, width, height).data; for (var y = 0; y < height; ++y) { for (var x = 0; x < width; ++x) { var alpha = pixels[(y * width + x) * 4 + 3]; HEAPU8[out + y * pitch + x] = Math.min(64, Math.round(alpha * 64 / 255)); } } },  
 1070740: ($0, $1, $2, $3, $4, $5) => { var code = $0 >>> 0; var width = $1 | 0; var height = $2 | 0; var out = $3 >>> 0; var pitch = $4 | 0; var weight = $5 | 0; var canvas = Module['ggnGlyphCanvas']; if (!canvas) { canvas = document.createElement('canvas'); Module['ggnGlyphCanvas'] = canvas; } canvas.width = width; canvas.height = height; var ctx = canvas.getContext('2d', { willReadFrequently: true }); ctx.clearRect(0, 0, width, height); ctx.fillStyle = '#fff'; ctx.textBaseline = 'top'; ctx.font = (weight >= 700 ? 'bold ' : '') + height + 'px Meiryo, "Yu Gothic", sans-serif'; var ch = '?'; try { ch = String.fromCodePoint(code); } catch (e) {} ctx.fillText(ch, 0, 0); var pixels = ctx.getImageData(0, 0, width, height).data; for (var y = 0; y < height; ++y) { for (var x = 0; x < width; ++x) { var alpha = pixels[(y * width + x) * 4 + 3]; if (alpha > 32) HEAPU8[out + y * pitch + (x >> 3)] |= 0x80 >> (x & 7); } } }
};

// Imports from the Wasm binary.
var _free = makeInvalidEarlyAccess('_free');
var _main = Module['_main'] = makeInvalidEarlyAccess('_main');
var _strerror = makeInvalidEarlyAccess('_strerror');
var _fflush = makeInvalidEarlyAccess('_fflush');
var _emscripten_stack_get_end = makeInvalidEarlyAccess('_emscripten_stack_get_end');
var _emscripten_stack_get_base = makeInvalidEarlyAccess('_emscripten_stack_get_base');
var _setThrew = makeInvalidEarlyAccess('_setThrew');
var __emscripten_tempret_set = makeInvalidEarlyAccess('__emscripten_tempret_set');
var _emscripten_stack_init = makeInvalidEarlyAccess('_emscripten_stack_init');
var _emscripten_stack_get_free = makeInvalidEarlyAccess('_emscripten_stack_get_free');
var __emscripten_stack_restore = makeInvalidEarlyAccess('__emscripten_stack_restore');
var __emscripten_stack_alloc = makeInvalidEarlyAccess('__emscripten_stack_alloc');
var _emscripten_stack_get_current = makeInvalidEarlyAccess('_emscripten_stack_get_current');
var ___cxa_decrement_exception_refcount = makeInvalidEarlyAccess('___cxa_decrement_exception_refcount');
var ___cxa_increment_exception_refcount = makeInvalidEarlyAccess('___cxa_increment_exception_refcount');
var ___get_exception_message = makeInvalidEarlyAccess('___get_exception_message');
var ___cxa_can_catch = makeInvalidEarlyAccess('___cxa_can_catch');
var ___cxa_get_exception_ptr = makeInvalidEarlyAccess('___cxa_get_exception_ptr');
var memory = makeInvalidEarlyAccess('memory');
var __indirect_function_table = makeInvalidEarlyAccess('__indirect_function_table');
var wasmMemory = makeInvalidEarlyAccess('wasmMemory');
var wasmTable = makeInvalidEarlyAccess('wasmTable');

function assignWasmExports(wasmExports) {
  assert(typeof wasmExports['free'] != 'undefined', 'missing Wasm export: free');
  assert(typeof wasmExports['__main_argc_argv'] != 'undefined', 'missing Wasm export: __main_argc_argv');
  assert(typeof wasmExports['strerror'] != 'undefined', 'missing Wasm export: strerror');
  assert(typeof wasmExports['fflush'] != 'undefined', 'missing Wasm export: fflush');
  assert(typeof wasmExports['emscripten_stack_get_end'] != 'undefined', 'missing Wasm export: emscripten_stack_get_end');
  assert(typeof wasmExports['emscripten_stack_get_base'] != 'undefined', 'missing Wasm export: emscripten_stack_get_base');
  assert(typeof wasmExports['setThrew'] != 'undefined', 'missing Wasm export: setThrew');
  assert(typeof wasmExports['_emscripten_tempret_set'] != 'undefined', 'missing Wasm export: _emscripten_tempret_set');
  assert(typeof wasmExports['emscripten_stack_init'] != 'undefined', 'missing Wasm export: emscripten_stack_init');
  assert(typeof wasmExports['emscripten_stack_get_free'] != 'undefined', 'missing Wasm export: emscripten_stack_get_free');
  assert(typeof wasmExports['_emscripten_stack_restore'] != 'undefined', 'missing Wasm export: _emscripten_stack_restore');
  assert(typeof wasmExports['_emscripten_stack_alloc'] != 'undefined', 'missing Wasm export: _emscripten_stack_alloc');
  assert(typeof wasmExports['emscripten_stack_get_current'] != 'undefined', 'missing Wasm export: emscripten_stack_get_current');
  assert(typeof wasmExports['__cxa_decrement_exception_refcount'] != 'undefined', 'missing Wasm export: __cxa_decrement_exception_refcount');
  assert(typeof wasmExports['__cxa_increment_exception_refcount'] != 'undefined', 'missing Wasm export: __cxa_increment_exception_refcount');
  assert(typeof wasmExports['__get_exception_message'] != 'undefined', 'missing Wasm export: __get_exception_message');
  assert(typeof wasmExports['__cxa_can_catch'] != 'undefined', 'missing Wasm export: __cxa_can_catch');
  assert(typeof wasmExports['__cxa_get_exception_ptr'] != 'undefined', 'missing Wasm export: __cxa_get_exception_ptr');
  assert(typeof wasmExports['memory'] != 'undefined', 'missing Wasm export: memory');
  assert(typeof wasmExports['__indirect_function_table'] != 'undefined', 'missing Wasm export: __indirect_function_table');
  _free = createExportWrapper('free', 1);
  _main = Module['_main'] = createExportWrapper('__main_argc_argv', 2);
  _strerror = createExportWrapper('strerror', 1);
  _fflush = createExportWrapper('fflush', 1);
  _emscripten_stack_get_end = wasmExports['emscripten_stack_get_end'];
  _emscripten_stack_get_base = wasmExports['emscripten_stack_get_base'];
  _setThrew = createExportWrapper('setThrew', 2);
  __emscripten_tempret_set = createExportWrapper('_emscripten_tempret_set', 1);
  _emscripten_stack_init = wasmExports['emscripten_stack_init'];
  _emscripten_stack_get_free = wasmExports['emscripten_stack_get_free'];
  __emscripten_stack_restore = wasmExports['_emscripten_stack_restore'];
  __emscripten_stack_alloc = wasmExports['_emscripten_stack_alloc'];
  _emscripten_stack_get_current = wasmExports['emscripten_stack_get_current'];
  ___cxa_decrement_exception_refcount = createExportWrapper('__cxa_decrement_exception_refcount', 1);
  ___cxa_increment_exception_refcount = createExportWrapper('__cxa_increment_exception_refcount', 1);
  ___get_exception_message = createExportWrapper('__get_exception_message', 3);
  ___cxa_can_catch = createExportWrapper('__cxa_can_catch', 3);
  ___cxa_get_exception_ptr = createExportWrapper('__cxa_get_exception_ptr', 1);
  memory = wasmMemory = wasmExports['memory'];
  __indirect_function_table = wasmTable = wasmExports['__indirect_function_table'];
}

var wasmImports = {
  /** @export */
  __assert_fail: ___assert_fail,
  /** @export */
  __cxa_begin_catch: ___cxa_begin_catch,
  /** @export */
  __cxa_end_catch: ___cxa_end_catch,
  /** @export */
  __cxa_find_matching_catch_2: ___cxa_find_matching_catch_2,
  /** @export */
  __cxa_find_matching_catch_3: ___cxa_find_matching_catch_3,
  /** @export */
  __cxa_find_matching_catch_8: ___cxa_find_matching_catch_8,
  /** @export */
  __cxa_rethrow: ___cxa_rethrow,
  /** @export */
  __cxa_throw: ___cxa_throw,
  /** @export */
  __cxa_uncaught_exceptions: ___cxa_uncaught_exceptions,
  /** @export */
  __resumeException: ___resumeException,
  /** @export */
  __syscall_dup3: ___syscall_dup3,
  /** @export */
  __syscall_fcntl64: ___syscall_fcntl64,
  /** @export */
  __syscall_fstat64: ___syscall_fstat64,
  /** @export */
  __syscall_ioctl: ___syscall_ioctl,
  /** @export */
  __syscall_lstat64: ___syscall_lstat64,
  /** @export */
  __syscall_mkdirat: ___syscall_mkdirat,
  /** @export */
  __syscall_newfstatat: ___syscall_newfstatat,
  /** @export */
  __syscall_openat: ___syscall_openat,
  /** @export */
  __syscall_readlinkat: ___syscall_readlinkat,
  /** @export */
  __syscall_renameat: ___syscall_renameat,
  /** @export */
  __syscall_rmdir: ___syscall_rmdir,
  /** @export */
  __syscall_stat64: ___syscall_stat64,
  /** @export */
  __syscall_unlinkat: ___syscall_unlinkat,
  /** @export */
  _abort_js: __abort_js,
  /** @export */
  _emscripten_system: __emscripten_system,
  /** @export */
  _emscripten_throw_longjmp: __emscripten_throw_longjmp,
  /** @export */
  _gmtime_js: __gmtime_js,
  /** @export */
  _localtime_js: __localtime_js,
  /** @export */
  _mktime_js: __mktime_js,
  /** @export */
  _tzset_js: __tzset_js,
  /** @export */
  clock_time_get: _clock_time_get,
  /** @export */
  emscripten_asm_const_double: _emscripten_asm_const_double,
  /** @export */
  emscripten_asm_const_int: _emscripten_asm_const_int,
  /** @export */
  emscripten_cancel_main_loop: _emscripten_cancel_main_loop,
  /** @export */
  emscripten_date_now: _emscripten_date_now,
  /** @export */
  emscripten_get_now: _emscripten_get_now,
  /** @export */
  emscripten_resize_heap: _emscripten_resize_heap,
  /** @export */
  emscripten_set_main_loop: _emscripten_set_main_loop,
  /** @export */
  environ_get: _environ_get,
  /** @export */
  environ_sizes_get: _environ_sizes_get,
  /** @export */
  exit: _exit,
  /** @export */
  fd_close: _fd_close,
  /** @export */
  fd_read: _fd_read,
  /** @export */
  fd_seek: _fd_seek,
  /** @export */
  fd_write: _fd_write,
  /** @export */
  invoke_d,
  /** @export */
  invoke_ddd,
  /** @export */
  invoke_di,
  /** @export */
  invoke_did,
  /** @export */
  invoke_dii,
  /** @export */
  invoke_diidi,
  /** @export */
  invoke_diii,
  /** @export */
  invoke_diiidi,
  /** @export */
  invoke_fiii,
  /** @export */
  invoke_i,
  /** @export */
  invoke_ii,
  /** @export */
  invoke_iid,
  /** @export */
  invoke_iidd,
  /** @export */
  invoke_iidddd,
  /** @export */
  invoke_iiddddd,
  /** @export */
  invoke_iiddddddddd,
  /** @export */
  invoke_iiddi,
  /** @export */
  invoke_iidi,
  /** @export */
  invoke_iif,
  /** @export */
  invoke_iii,
  /** @export */
  invoke_iiid,
  /** @export */
  invoke_iiidi,
  /** @export */
  invoke_iiididi,
  /** @export */
  invoke_iiidiii,
  /** @export */
  invoke_iiidiiiiii,
  /** @export */
  invoke_iiii,
  /** @export */
  invoke_iiiid,
  /** @export */
  invoke_iiiidi,
  /** @export */
  invoke_iiiididi,
  /** @export */
  invoke_iiiiffi,
  /** @export */
  invoke_iiiii,
  /** @export */
  invoke_iiiiid,
  /** @export */
  invoke_iiiiidi,
  /** @export */
  invoke_iiiiii,
  /** @export */
  invoke_iiiiiid,
  /** @export */
  invoke_iiiiiidi,
  /** @export */
  invoke_iiiiiii,
  /** @export */
  invoke_iiiiiiid,
  /** @export */
  invoke_iiiiiiidi,
  /** @export */
  invoke_iiiiiiii,
  /** @export */
  invoke_iiiiiiiii,
  /** @export */
  invoke_iiiiiiiiii,
  /** @export */
  invoke_iiiiiiiiiidd,
  /** @export */
  invoke_iiiiiiiiiiddi,
  /** @export */
  invoke_iiiiiiiiiii,
  /** @export */
  invoke_iiiiiiiiiiii,
  /** @export */
  invoke_iiiiiiiiiiiii,
  /** @export */
  invoke_iiji,
  /** @export */
  invoke_ji,
  /** @export */
  invoke_jiiii,
  /** @export */
  invoke_v,
  /** @export */
  invoke_vdd,
  /** @export */
  invoke_vddd,
  /** @export */
  invoke_vddi,
  /** @export */
  invoke_vddii,
  /** @export */
  invoke_vddiiidddii,
  /** @export */
  invoke_vddiiidii,
  /** @export */
  invoke_vi,
  /** @export */
  invoke_vid,
  /** @export */
  invoke_viddd,
  /** @export */
  invoke_vidddd,
  /** @export */
  invoke_vidi,
  /** @export */
  invoke_vidiffi,
  /** @export */
  invoke_vidii,
  /** @export */
  invoke_vidiiffi,
  /** @export */
  invoke_vif,
  /** @export */
  invoke_vii,
  /** @export */
  invoke_viid,
  /** @export */
  invoke_viidd,
  /** @export */
  invoke_viidiii,
  /** @export */
  invoke_viidiiii,
  /** @export */
  invoke_viiff,
  /** @export */
  invoke_viifff,
  /** @export */
  invoke_viiffff,
  /** @export */
  invoke_viii,
  /** @export */
  invoke_viiid,
  /** @export */
  invoke_viiidiii,
  /** @export */
  invoke_viiiffi,
  /** @export */
  invoke_viiii,
  /** @export */
  invoke_viiiid,
  /** @export */
  invoke_viiiiffi,
  /** @export */
  invoke_viiiii,
  /** @export */
  invoke_viiiiii,
  /** @export */
  invoke_viiiiiid,
  /** @export */
  invoke_viiiiiidd,
  /** @export */
  invoke_viiiiiii,
  /** @export */
  invoke_viiiiiiii,
  /** @export */
  invoke_viiiiiiiii,
  /** @export */
  invoke_viiiiiiiiii,
  /** @export */
  invoke_viiiiiiiiiii,
  /** @export */
  invoke_viiiiiiiiiiii,
  /** @export */
  invoke_viiiiiiiiiiiiiii,
  /** @export */
  invoke_viijii,
  /** @export */
  llvm_eh_typeid_for: _llvm_eh_typeid_for
};

function invoke_ii(index,a1) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiii(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiii(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iii(index,a1,a2) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vii(index,a1,a2) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viii(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiii(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiii(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vi(index,a1) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiii(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiii(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_v(index) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)();
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiii(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vidddd(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vid(index,a1,a2) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viid(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vif(index,a1,a2) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiffi(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiffi(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_did(index,a1,a2) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiid(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiid(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_i(index) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)();
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiid(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiid(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiid(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viddd(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_d(index) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)();
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iif(index,a1,a2) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vidii(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vidi(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_dii(index,a1,a2) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_di(index,a1) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiii(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiidi(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiid(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iidddd(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiidiii(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiji(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiffi(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iid(index,a1,a2) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiddi(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_diiidi(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiidi(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iidi(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vdd(index,a1,a2) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vddii(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_diii(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vddd(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiidiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiidiii(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vidiiffi(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiddddddddd(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_ddd(index,a1,a2) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vddi(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiii(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiidi(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiidi(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiididi(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiidi(index,a1,a2,a3,a4,a5,a6,a7,a8) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiidd(index,a1,a2,a3,a4,a5,a6,a7,a8) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_ji(index,a1) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
    return 0n;
  }
}

function invoke_iidd(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiid(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiiiidd(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiiiiddi(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viidd(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_diidi(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viidiiii(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viidiii(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vidiffi(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_fiii(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viifff(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiff(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiffff(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiid(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiididi(index,a1,a2,a3,a4,a5,a6,a7) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiddddd(index,a1,a2,a3,a4,a5,a6) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vddiiidii(index,a1,a2,a3,a4,a5,a6,a7,a8) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vddiiidddii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viijii(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_jiiii(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
    return 0n;
  }
}

function invoke_iiiiiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiiiiiiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15);
  } catch(e) {
    stackRestore(sp);
    if (!(e instanceof EmscriptenEH)) throw e;
    _setThrew(1, 0);
  }
}


// include: postamble.js
// === Auto-generated postamble setup entry stuff ===

var calledRun;

function callMain(args = []) {
  assert(runDependencies == 0, 'cannot call main when async dependencies remain! (listen on Module["onRuntimeInitialized"])');
  assert(typeof onPreRuns === 'undefined' || onPreRuns.length == 0, 'cannot call main when preRun functions remain to be called');

  var entryFunction = _main;

  args.unshift(thisProgram);

  var argc = args.length;
  var argv = stackAlloc((argc + 1) * 4);
  var argv_ptr = argv;
  for (var arg of args) {
    HEAPU32[((argv_ptr)>>2)] = stringToUTF8OnStack(arg);
    argv_ptr += 4;
  }
  HEAPU32[((argv_ptr)>>2)] = 0;

  try {

    var ret = entryFunction(argc, argv);

    // if we're not running an evented main loop, it's time to exit
    exitJS(ret, /* implicit = */ true);
    return ret;
  } catch (e) {
    return handleException(e);
  }
}

function stackCheckInit() {
  // This is normally called automatically during __wasm_call_ctors but need to
  // get these values before even running any of the ctors so we call it redundantly
  // here.
  _emscripten_stack_init();
  // TODO(sbc): Move writeStackCookie to native to to avoid this.
  writeStackCookie();
}

function run(args = programArgs) {

  if (runDependencies > 0) {
    dependenciesFulfilled = run;
    return;
  }

  stackCheckInit();

  preRun();

  // a preRun added a dependency, run will be called later
  if (runDependencies > 0) {
    dependenciesFulfilled = run;
    return;
  }

  function doRun() {
    // run may have just been called through dependencies being fulfilled just in this very frame,
    // or while the async setStatus time below was happening
    assert(!calledRun);
    calledRun = true;
    Module['calledRun'] = true;

    if (ABORT) return;

    initRuntime();

    preMain();

    Module['onRuntimeInitialized']?.();
    consumedModuleProp('onRuntimeInitialized');

    var noInitialRun = Module['noInitialRun'] || false;
    if (!noInitialRun) callMain(args);

    postRun();
  }

  if (Module['setStatus']) {
    Module['setStatus']('Running...');
    setTimeout(() => {
      setTimeout(() => Module['setStatus'](''), 1);
      doRun();
    }, 1);
  } else
  {
    doRun();
  }
  checkStackCookie();
}

function checkUnflushedContent() {
  // Compiler settings do not allow exiting the runtime, so flushing
  // the streams is not possible. but in ASSERTIONS mode we check
  // if there was something to flush, and if so tell the user they
  // should request that the runtime be exitable.
  // Normally we would not even include flush() at all, but in ASSERTIONS
  // builds we do so just for this check, and here we see if there is any
  // content to flush, that is, we check if there would have been
  // something a non-ASSERTIONS build would have not seen.
  // How we flush the streams depends on whether we are in SYSCALLS_REQUIRE_FILESYSTEM=0
  // mode (which has its own special function for this; otherwise, all
  // the code is inside libc)
  var oldOut = out;
  var oldErr = err;
  var has = false;
  out = err = (x) => {
    has = true;
  }
  try { // it doesn't matter if it fails
    _fflush(0);
    // also flush in the JS FS layer
    for (var name of ['stdout', 'stderr']) {
      var info = FS.analyzePath('/dev/' + name);
      if (!info) return;
      var stream = info.object;
      var rdev = stream.rdev;
      var tty = TTY.ttys[rdev];
      if (tty?.output?.length) {
        has = true;
      }
    }
  } catch(e) {}
  out = oldOut;
  err = oldErr;
  if (has) {
    warnOnce('stdio streams had content in them that was not flushed. you should set EXIT_RUNTIME to 1 (see the Emscripten FAQ), or make sure to emit a newline when you printf etc.');
  }
}

var wasmExports;

// With async instantation wasmExports is assigned asynchronously when the
// instance is received.
createWasm();

run();

// end include: postamble.js

