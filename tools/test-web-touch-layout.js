const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');

const root = path.resolve(__dirname, '..');
const source = fs.readFileSync(path.join(root, 'source', 'gameMainSystem', 'cInput.cpp'), 'utf8');
const saveSource = fs.readFileSync(path.join(root, 'source', 'cSaveStore.cpp'), 'latin1');
const buildScript = fs.readFileSync(path.join(root, 'tools', 'build-web.ps1'), 'utf8');

function assertSourceContains(fragment) {
  assert.ok(
    source.includes(fragment),
    `source\\gameMainSystem\\cInput.cpp should contain: ${fragment}`,
  );
}

assertSourceContains('--ggn-pad-width');
assertSourceContains('@media (pointer: coarse) and (orientation: landscape), (max-width: 900px) and (orientation: landscape)');
assertSourceContains('padding-left: calc(var(--ggn-pad-width) + max(var(--ggn-edge), env(safe-area-inset-left)) + 8px)');
assertSourceContains('padding-right: calc(var(--ggn-pad-width) + max(var(--ggn-edge), env(safe-area-inset-right)) + 8px)');
assertSourceContains('#ggn-touch-controls .ggn-pad { bottom: auto; top: 50%; transform: translateY(-50%); }');
assertSourceContains('aspect-ratio: 4 / 3');
assertSourceContains('body.ggn-mobile-ready #ggn-page-links { top: calc(50% + (var(--ggn-panel-height) / 2) - var(--ggn-cell)); right: max(var(--ggn-edge), env(safe-area-inset-right)); left: auto; width: var(--ggn-pad-width); padding: 0; text-align: center; transform: none; font-size: 10px; line-height: 1.1; }');
assertSourceContains('body.ggn-mobile-ready #ggn-page-links a { padding: 1px 6px; }');
assertSourceContains('#ggn-btn-step { grid-column: 1 / span 2; grid-row: 10 / span 1; }');
assertSourceContains("Module['ggnPadConfig'] = Module['ggnPadConfig'] || [0, 1, 2, 3, 4, 5, 6, 7]");
assertSourceContains("typeof navigator.getGamepads !== 'function'");
assertSourceContains("Module['ggnGamepadButtonDown'] = function(buttonIndex)");
assertSourceContains("Module['ggnGamepadDirectionDown'] = function(buttonIndex, axisIndex, axisSign)");
assertSourceContains("function configuredActionButton(actionIndex)");
assertSourceContains("button._ggnPressedButtons = specActions(spec).map(configuredActionButton)");
assertSourceContains("BrowserReadInputState(platformInputState_, patInputManager->padconfigI2B)");
assertSourceContains("return BrowserPadButtonDown(buttom)");
assertSourceContains('state.ue = BrowserKeyDown(38) | BrowserKeyDown(73) | BrowserGamepadDirectionDown(12, 1, -1)');
assertSourceContains('state.shita = BrowserKeyDown(40) | BrowserKeyDown(75) | BrowserGamepadDirectionDown(13, 1, 1)');
assertSourceContains('state.hidari = BrowserKeyDown(37) | BrowserKeyDown(74) | BrowserGamepadDirectionDown(14, 0, -1)');
assertSourceContains('state.migi = BrowserKeyDown(39) | BrowserKeyDown(76) | BrowserGamepadDirectionDown(15, 0, 1)');
assert.ok(
  saveSource.includes('ppadconfig_->assign(g_GameEnv.m_Input.patInputManager->padconfigI2B.begin(),'),
  'web pad configuration should be copied into the existing config save data',
);
assert.ok(
  saveSource.includes('padconfigI2B.assign(ppadconfig_->begin(), ppadconfig_->end())'),
  'saved pad configuration should be restored for the web input backend',
);

const gamepadHelperStart = source.indexOf("Module['ggnGamepadDeadzone'] = 0.45;");
const gamepadHelperEnd = source.indexOf("Module['ggnShouldBlockKey']", gamepadHelperStart);
assert.ok(gamepadHelperStart >= 0 && gamepadHelperEnd > gamepadHelperStart, 'gamepad helpers should be extractable');
const gamepadModule = {};
const gamepadButtons = Array.from({ length: 16 }, () => ({ pressed: false, value: 0 }));
const testGamepad = { connected: true, buttons: gamepadButtons, axes: [0, 0] };
gamepadModule.ggnGamepadProvider = () => [null, testGamepad];
new Function('Module', 'navigator', source.slice(gamepadHelperStart, gamepadHelperEnd))(gamepadModule, {});

gamepadButtons[2] = { pressed: true, value: 1 };
assert.equal(gamepadModule.ggnGamepadButtonDown(2), 1, 'physical button input should be detected');
assert.equal(gamepadModule.ggnGamepadButtonDown(3), 0, 'unpressed physical buttons should stay off');
gamepadButtons[2] = { pressed: false, value: 0 };
gamepadButtons[12] = { pressed: true, value: 1 };
assert.equal(gamepadModule.ggnGamepadDirectionDown(12, 1, -1), 1, 'standard D-pad input should be detected');
gamepadButtons[12] = { pressed: false, value: 0 };
testGamepad.axes = [-0.7, 0.8];
assert.equal(gamepadModule.ggnGamepadDirectionDown(14, 0, -1), 1, 'left-stick horizontal input should be detected');
assert.equal(gamepadModule.ggnGamepadDirectionDown(13, 1, 1), 1, 'left-stick vertical input should be detected');
testGamepad.axes = [0.2, -0.2];
assert.equal(gamepadModule.ggnGamepadDirectionDown(12, 1, -1), 0, 'stick drift inside the deadzone should be ignored');
assert.ok(
  buildScript.includes('$hashInput += (Get-FileHash -LiteralPath $dataChunk.FullName -Algorithm SHA256).Hash'),
  'web build id should include the compressed data package',
);
assert.ok(
  buildScript.includes('$chunkVersionedName = $dataChunk.Name + "?v=$buildId"'),
  'compressed data requests should use the same build cache key',
);
assert.ok(buildScript.includes('id="ggn-fullscreen-button"'), 'the web page should expose a fullscreen toggle');
assert.ok(
  buildScript.includes('root.requestFullscreen||root.webkitRequestFullscreen'),
  'the fullscreen toggle should support standard and WebKit entry APIs',
);
assert.ok(
  buildScript.includes('document.exitFullscreen||document.webkitExitFullscreen'),
  'the fullscreen toggle should support standard and WebKit exit APIs',
);
assert.ok(buildScript.includes('navigationUI:"hide"'), 'fullscreen should request hidden browser navigation');
assert.ok(buildScript.includes('fullscreenchange'), 'the toggle label should follow browser fullscreen state');
assert.ok(buildScript.includes('setTimeout(sync,1000)'), 'the toggle should resync after browser-initiated fullscreen changes');
assert.ok(buildScript.includes('setInterval(sync,1000)'), 'embedded browsers should periodically resync fullscreen state');
assert.ok(
  buildScript.includes('name="apple-mobile-web-app-capable" content="yes"'),
  'iOS home-screen launches should opt into app-style display',
);
assert.ok(
  buildScript.includes('「Webアプリとして開く」を有効にし'),
  'unsupported browsers should explain the iOS home-screen fallback',
);
assert.ok(buildScript.includes('id="ggn-controls-toggle"'), 'mobile users should have a touch-control visibility toggle');
assert.ok(
  buildScript.includes('html.ggn-controls-hidden #ggn-touch-controls{display:none!important}'),
  'hidden mode should remove the touch controls from layout',
);
assert.ok(
  buildScript.includes('html.ggn-controls-hidden body.ggn-mobile-ready div.emscripten_border'),
  'hidden mode should return the full viewport to the game canvas',
);
assert.ok(
  buildScript.includes('align-items:center;padding-left:0;padding-right:0'),
  'landscape hidden mode should remove the controller side gutters',
);
assert.ok(
  buildScript.includes('html.ggn-controls-hidden #ggn-fullscreen-button,html.ggn-controls-hidden #ggn-page-links a{display:none!important}'),
  'hidden mode should leave only the restore control visible beside an expanded canvas',
);
assert.ok(buildScript.includes('ggn-touch-controls-hidden'), 'hidden mode should persist across reloads');
assert.ok(
  buildScript.includes('window.dispatchEvent(new Event("blur"))'),
  'hiding touch controls should release any held input',
);

const generatedHtml = ['ggn.html', 'index.html'].map((name) => ({
  name,
  content: fs.readFileSync(path.join(root, 'docs', 'play', name), 'utf8'),
}));
generatedHtml.forEach(({ name, content }) => {
  assert.match(content, /id=(?:"ggn-fullscreen-button"|ggn-fullscreen-button)/, `${name} should contain the fullscreen toggle`);
  assert.ok(content.includes('requestFullscreen||root.webkitRequestFullscreen'), `${name} should contain fullscreen entry logic`);
  assert.ok(content.includes('navigationUI:"hide"'), `${name} should request hidden browser navigation`);
  assert.ok(content.includes('apple-mobile-web-app-capable'), `${name} should contain the iOS app-mode metadata`);
  assert.ok(content.includes('このブラウザでは全画面表示を利用できません'), `${name} should contain the unsupported-browser guidance`);
  assert.match(content, /id=(?:"ggn-controls-toggle"|ggn-controls-toggle)/, `${name} should contain the touch-control toggle`);
  assert.ok(content.includes('ggn-controls-hidden #ggn-touch-controls'), `${name} should contain hidden-mode styling`);
  assert.ok(content.includes('align-items:center;padding-left:0;padding-right:0'), `${name} should remove hidden landscape gutters`);
  assert.ok(content.includes('ggn-controls-hidden #ggn-fullscreen-button'), `${name} should hide utility controls except restore`);
  assert.ok(content.includes('ggn-touch-controls-hidden'), `${name} should persist hidden mode`);
  assert.ok(!content.includes('&#25147;&#12377;'), `${name} script labels should not contain undecoded HTML entities`);
});
assert.equal(generatedHtml[0].content, generatedHtml[1].content, 'ggn.html and index.html should stay identical');

const fullscreenScriptMatch = generatedHtml[0].content.match(
  /<script>(\(function\(\)\{var b=document\.getElementById\("ggn-fullscreen-button"\).*?\}\)\(\);)<\/script>/s,
);
assert.ok(fullscreenScriptMatch, 'the generated fullscreen controller should be executable in isolation');
const fullscreenEventHandlers = {};
const fullscreenButtonHandlers = {};
const controlsButtonHandlers = {};
const fullscreenButton = {
  textContent: '全画面',
  attributes: {},
  addEventListener(type, handler) { fullscreenButtonHandlers[type] = handler; },
  setAttribute(name, value) { this.attributes[name] = value; },
};
const controlsButton = {
  textContent: '操作非表示',
  attributes: {},
  addEventListener(type, handler) { controlsButtonHandlers[type] = handler; },
  setAttribute(name, value) { this.attributes[name] = value; },
};
const fullscreenRootClasses = new Set();
const fullscreenRoot = {
  classList: {
    contains(name) { return fullscreenRootClasses.has(name); },
    toggle(name, force) {
      if (force === true) fullscreenRootClasses.add(name);
      else if (force === false) fullscreenRootClasses.delete(name);
      else if (fullscreenRootClasses.has(name)) fullscreenRootClasses.delete(name);
      else fullscreenRootClasses.add(name);
      return fullscreenRootClasses.has(name);
    },
  },
};
const fullscreenDocument = {
  fullscreenElement: null,
  webkitFullscreenElement: null,
  documentElement: fullscreenRoot,
  getElementById(id) {
    if (id === 'ggn-fullscreen-button') return fullscreenButton;
    if (id === 'ggn-controls-toggle') return controlsButton;
    return null;
  },
  addEventListener(type, handler) {
    fullscreenEventHandlers[type] = fullscreenEventHandlers[type] || [];
    fullscreenEventHandlers[type].push(handler);
  },
};
const dispatchFullscreenEvent = (type) => (fullscreenEventHandlers[type] || []).forEach((handler) => handler({ type }));
let requestedFullscreenOptions = null;
let periodicFullscreenSync = null;
let fullscreenFallbackMessage = null;
let blurDispatchCount = 0;
const controlsStorage = new Map();
const fullscreenWindow = {
  dispatchEvent(event) { if (event.type === 'blur') blurDispatchCount += 1; },
};
function MockEvent(type) { this.type = type; }
const fullscreenLocalStorage = {
  getItem(key) { return controlsStorage.has(key) ? controlsStorage.get(key) : null; },
  setItem(key, value) { controlsStorage.set(key, String(value)); },
};
fullscreenDocument.documentElement.requestFullscreen = (options) => {
  requestedFullscreenOptions = options;
  fullscreenDocument.fullscreenElement = fullscreenDocument.documentElement;
  dispatchFullscreenEvent('fullscreenchange');
};
fullscreenDocument.exitFullscreen = () => {
  fullscreenDocument.fullscreenElement = null;
  dispatchFullscreenEvent('fullscreenchange');
};
new Function('document', 'alert', 'setTimeout', 'setInterval', 'window', 'Event', 'localStorage', fullscreenScriptMatch[1])(
  fullscreenDocument,
  (message) => { fullscreenFallbackMessage = message; },
  (callback) => { callback(); return 1; },
  (callback) => { periodicFullscreenSync = callback; return 1; },
  fullscreenWindow,
  MockEvent,
  fullscreenLocalStorage,
);
const fullscreenClickEvent = { preventDefault() {}, stopPropagation() {} };
fullscreenButtonHandlers.click(fullscreenClickEvent);
assert.deepEqual(requestedFullscreenOptions, { navigationUI: 'hide' }, 'fullscreen entry should hide browser navigation');
assert.equal(fullscreenButton.textContent, '戻す', 'the fullscreen button should become an exit control');
assert.equal(fullscreenButton.attributes['aria-pressed'], 'true', 'fullscreen entry should update pressed state');
fullscreenButtonHandlers.click(fullscreenClickEvent);
assert.equal(fullscreenButton.textContent, '全画面', 'the exit control should restore the entry label');
assert.equal(fullscreenButton.attributes['aria-pressed'], 'false', 'fullscreen exit should clear pressed state');
fullscreenDocument.fullscreenElement = fullscreenDocument.documentElement;
periodicFullscreenSync();
assert.equal(fullscreenButton.textContent, '戻す', 'periodic sync should detect external fullscreen entry');
fullscreenDocument.fullscreenElement = null;
periodicFullscreenSync();
assert.equal(fullscreenButton.textContent, '全画面', 'periodic sync should detect external fullscreen exit');
assert.equal(fullscreenFallbackMessage, null, 'supported fullscreen flows should not show fallback guidance');
controlsButtonHandlers.click(fullscreenClickEvent);
assert.equal(fullscreenRootClasses.has('ggn-controls-hidden'), true, 'hidden mode should add the root state class');
assert.equal(controlsButton.textContent, '操作表示', 'hidden mode should keep a visible restore control');
assert.equal(controlsButton.attributes['aria-pressed'], 'true', 'hidden mode should update pressed state');
assert.equal(controlsStorage.get('ggn-touch-controls-hidden'), '1', 'hidden mode should persist its state');
assert.equal(blurDispatchCount, 1, 'hidden mode should release active inputs');
controlsButtonHandlers.click(fullscreenClickEvent);
assert.equal(fullscreenRootClasses.has('ggn-controls-hidden'), false, 'visible mode should remove the root state class');
assert.equal(controlsButton.textContent, '操作非表示', 'visible mode should restore the hide label');
assert.equal(controlsButton.attributes['aria-pressed'], 'false', 'visible mode should clear pressed state');
assert.equal(controlsStorage.get('ggn-touch-controls-hidden'), '0', 'visible mode should persist its state');

const expectedActionIndices = {
  attack: 0,
  dash: 1,
  turn: 2,
  menu: 3,
  diagon: 4,
  shot: 5,
  miniMap: 6,
  smartdash: 7,
};
const configuredReads = Object.fromEntries(
  [...source.matchAll(/state\.(\w+)\s*=.*BrowserConfiguredButtonDown\(padConfig,\s*(\d+)\);/g)]
    .map((match) => [match[1], Number(match[2])]),
);
assert.deepEqual(configuredReads, expectedActionIndices, 'all configurable actions should use the saved pad mapping');

const remappedButtons = [1, 0, 5, 6, 7, 2, 3, 4];
Object.entries(expectedActionIndices).forEach(([name, action]) => {
  const emulatedRawButton = remappedButtons[action];
  const activeActions = Object.entries(configuredReads)
    .filter(([, configuredAction]) => remappedButtons[configuredAction] === emulatedRawButton)
    .map(([actionName]) => actionName);
  assert.deepEqual(activeActions, [name], `${name} touch input should survive a non-default key configuration`);
});

const GAP = 4;
const EDGE = 8;
const SIDE_GAP = 8;
const MIN_LANDSCAPE_CELL = 18;
const MAX_CELL = 28;
const MANUAL_LINK_WIDTH = 96;
const MANUAL_LINK_HEIGHT = 32;

function clamp(value, min, max) {
  return Math.max(min, Math.min(max, value));
}

function padWidth(cell) {
  return 6 * cell + 5 * GAP;
}

function panelHeight(cell) {
  return 10 * cell + 9 * GAP;
}

function portraitCell(width) {
  return Math.min(MAX_CELL, (width - 66) / 12);
}

function landscapeCell(width, height) {
  return Math.max(
    MIN_LANDSCAPE_CELL,
    Math.min(MAX_CELL, (height - 52) / 10, (width - 420) / 12),
  );
}

function rect(name, x, y, width, height) {
  return {
    name,
    x,
    y,
    width,
    height,
    get right() {
      return this.x + this.width;
    },
    get bottom() {
      return this.y + this.height;
    },
  };
}

function buttonGrid(pad, cell, name, col, row, colSpan, rowSpan) {
  return rect(
    name,
    pad.x + (col - 1) * (cell + GAP),
    pad.y + (row - 1) * (cell + GAP),
    colSpan * cell + (colSpan - 1) * GAP,
    rowSpan * cell + (rowSpan - 1) * GAP,
  );
}

function buttonRects(layout) {
  const { leftPad, rightPad, cell } = layout;
  return [
    buttonGrid(leftPad, cell, 'map', 1, 1, 6, 2),
    buttonGrid(leftPad, cell, 'up-left', 1, 3, 2, 2),
    buttonGrid(leftPad, cell, 'up', 3, 3, 2, 2),
    buttonGrid(leftPad, cell, 'up-right', 5, 3, 2, 2),
    buttonGrid(leftPad, cell, 'left', 1, 5, 2, 2),
    buttonGrid(leftPad, cell, 'right', 5, 5, 2, 2),
    buttonGrid(leftPad, cell, 'down-left', 1, 7, 2, 2),
    buttonGrid(leftPad, cell, 'down', 3, 7, 2, 2),
    buttonGrid(leftPad, cell, 'down-right', 5, 7, 2, 2),
    buttonGrid(leftPad, cell, 'step', 1, 10, 2, 1),
    buttonGrid(rightPad, cell, 'menu', 1, 1, 6, 2),
    buttonGrid(rightPad, cell, 'turn', 1, 3, 2, 2),
    buttonGrid(rightPad, cell, 'diag', 3, 3, 2, 2),
    buttonGrid(rightPad, cell, 'shot', 5, 3, 2, 2),
    buttonGrid(rightPad, cell, 'attack', 1, 5, 3, 3),
    buttonGrid(rightPad, cell, 'dash', 4, 5, 3, 3),
    buttonGrid(rightPad, cell, 'smartdash', 1, 8, 6, 2),
  ];
}

function overlaps(a, b) {
  return a.x < b.right && a.right > b.x && a.y < b.bottom && a.bottom > b.y;
}

function assertInside(container, child) {
  const epsilon = 0.001;
  assert.ok(child.x >= container.x - epsilon, `${child.name} left is outside ${container.name}`);
  assert.ok(child.y >= container.y - epsilon, `${child.name} top is outside ${container.name}`);
  assert.ok(child.right <= container.right + epsilon, `${child.name} right is outside ${container.name}`);
  assert.ok(child.bottom <= container.bottom + epsilon, `${child.name} bottom is outside ${container.name}`);
}

function assertNoOverlap(rects) {
  for (let i = 0; i < rects.length; i += 1) {
    for (let j = i + 1; j < rects.length; j += 1) {
      assert.ok(!overlaps(rects[i], rects[j]), `${rects[i].name} overlaps ${rects[j].name}`);
    }
  }
}

function portraitLayout({ width, height, safeBottom = 0 }) {
  const cell = portraitCell(width);
  const panel = panelHeight(cell);
  const controlsHeight = panel + Math.max(12, safeBottom);
  const gameHeight = height - controlsHeight - 12;
  const canvasWidth = Math.min(width, gameHeight * 4 / 3);
  const canvasHeight = Math.min(gameHeight, width * 0.75);
  const padW = padWidth(cell);
  return {
    cell,
    viewport: rect('viewport', 0, 0, width, height),
    canvas: rect('canvas', (width - canvasWidth) / 2, gameHeight - canvasHeight, canvasWidth, canvasHeight),
    manualLink: rect('manualLink', width - EDGE - MANUAL_LINK_WIDTH, 4, MANUAL_LINK_WIDTH, MANUAL_LINK_HEIGHT),
    leftPad: rect('leftPad', EDGE, height - Math.max(12, safeBottom) - panel, padW, panel),
    rightPad: rect('rightPad', width - EDGE - padW, height - Math.max(12, safeBottom) - panel, padW, panel),
    contentGap: 0,
  };
}

function landscapeLayout({ width, height, safeLeft = 0, safeRight = 0 }) {
  const cell = landscapeCell(width, height);
  const padW = padWidth(cell);
  const panel = panelHeight(cell);
  const leftX = Math.max(EDGE, safeLeft);
  const rightX = width - Math.max(EDGE, safeRight) - padW;
  const padY = (height - panel) / 2;
  const contentLeft = leftX + padW + SIDE_GAP;
  const contentRight = rightX - SIDE_GAP;
  const contentWidth = contentRight - contentLeft;
  const canvasWidth = Math.min(contentWidth, height * 4 / 3);
  const canvasHeight = canvasWidth * 0.75;
  const rightPad = rect('rightPad', rightX, padY, padW, panel);
  return {
    cell,
    viewport: rect('viewport', 0, 0, width, height),
    canvas: rect('canvas', contentLeft + (contentWidth - canvasWidth) / 2, (height - canvasHeight) / 2, canvasWidth, canvasHeight),
    manualLink: rect('manualLink', rightX + (padW - MANUAL_LINK_WIDTH) / 2, padY + panel - cell, MANUAL_LINK_WIDTH, Math.min(MANUAL_LINK_HEIGHT, cell)),
    leftPad: rect('leftPad', leftX, padY, padW, panel),
    rightPad,
    contentGap: SIDE_GAP,
  };
}

function assertLayout(layout, orientation) {
  assert.ok(layout.cell >= 0, 'cell size should be non-negative');
  assertInside(layout.viewport, layout.canvas);
  assertInside(layout.viewport, layout.manualLink);
  assertInside(layout.viewport, layout.leftPad);
  assertInside(layout.viewport, layout.rightPad);
  assertNoOverlap([layout.leftPad, layout.rightPad]);

  const buttons = buttonRects(layout);
  const leftButtons = buttons.filter((button) => button.x < layout.rightPad.x);
  const rightButtons = buttons.filter((button) => button.x >= layout.rightPad.x);
  leftButtons.forEach((button) => assertInside(layout.leftPad, button));
  rightButtons.forEach((button) => assertInside(layout.rightPad, button));
  assertNoOverlap(leftButtons);
  assertNoOverlap(rightButtons);

  buttons.forEach((button) => {
    assertInside(layout.viewport, button);
    assert.ok(button.width >= 40 || orientation === 'portrait', `${button.name} is too narrow for landscape touch`);
    assert.ok(button.height >= 40 || orientation === 'portrait' || button.name === 'step', `${button.name} is too short for landscape touch`);
  });

  if (orientation === 'landscape') {
    assert.ok(layout.leftPad.right + layout.contentGap <= layout.canvas.x + 0.001, 'left pad overlaps canvas');
    assert.ok(layout.canvas.right + layout.contentGap <= layout.rightPad.x + 0.001, 'right pad overlaps canvas');
    assertInside(layout.rightPad, layout.manualLink);
    assert.ok(!overlaps(layout.manualLink, layout.canvas), 'manual link overlaps canvas');
    buttons.forEach((button) => {
      assert.ok(!overlaps(layout.manualLink, button), `manual link overlaps ${button.name}`);
    });
    assert.ok(layout.canvas.width >= 180, 'landscape canvas became too narrow');
    assert.ok(layout.canvas.height >= 135, 'landscape canvas became too short');
  } else {
    assert.ok(layout.canvas.bottom <= layout.leftPad.y - 12 + 0.001, 'portrait controls overlap canvas');
  }
}

[
  { name: 'phone portrait', width: 390, height: 844 },
  { name: 'small phone portrait', width: 360, height: 640 },
  { name: 'large phone portrait', width: 412, height: 915, safeBottom: 24 },
  { name: 'ipad portrait', width: 768, height: 1024, safeBottom: 20 },
].forEach((viewport) => {
  assertLayout(portraitLayout(viewport), 'portrait');
});

[
  { name: 'phone landscape', width: 844, height: 390 },
  { name: 'small phone landscape', width: 667, height: 375 },
  { name: 'compact phone landscape', width: 568, height: 320 },
  { name: 'android landscape', width: 915, height: 412 },
  { name: 'iphone notch landscape', width: 844, height: 390, safeLeft: 47, safeRight: 47 },
  { name: 'ipad landscape', width: 1024, height: 768 },
  { name: 'ipad pro landscape', width: 1180, height: 820, safeLeft: 24, safeRight: 24 },
].forEach((viewport) => {
  assertLayout(landscapeLayout(viewport), 'landscape');
});

console.log('web touch layout tests passed');
