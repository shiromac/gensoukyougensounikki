const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');

const root = path.resolve(__dirname, '..');
const source = fs.readFileSync(path.join(root, 'source', 'gameMainSystem', 'cInput.cpp'), 'utf8');
const saveSource = fs.readFileSync(path.join(root, 'source', 'cSaveStore.cpp'), 'latin1');
const buildScript = fs.readFileSync(path.join(root, 'tools', 'build-web.ps1'), 'utf8');

// Historical contract: 0cfa159 (last authored two-pad layout before e7d3a4b).
const ORIGINAL_BUTTON_LAYOUT = Object.freeze({
  map: { parent: 'left', col: 1, row: 1, colSpan: 6, rowSpan: 2 },
  'up-left': { parent: 'left', col: 1, row: 3, colSpan: 2, rowSpan: 2 },
  up: { parent: 'left', col: 3, row: 3, colSpan: 2, rowSpan: 2 },
  'up-right': { parent: 'left', col: 5, row: 3, colSpan: 2, rowSpan: 2 },
  left: { parent: 'left', col: 1, row: 5, colSpan: 2, rowSpan: 2 },
  right: { parent: 'left', col: 5, row: 5, colSpan: 2, rowSpan: 2 },
  'down-left': { parent: 'left', col: 1, row: 7, colSpan: 2, rowSpan: 2 },
  down: { parent: 'left', col: 3, row: 7, colSpan: 2, rowSpan: 2 },
  'down-right': { parent: 'left', col: 5, row: 7, colSpan: 2, rowSpan: 2 },
  step: { parent: 'left', col: 1, row: 10, colSpan: 2, rowSpan: 1 },
  menu: { parent: 'right', col: 1, row: 1, colSpan: 6, rowSpan: 2 },
  turn: { parent: 'right', col: 1, row: 3, colSpan: 2, rowSpan: 2 },
  diag: { parent: 'right', col: 3, row: 3, colSpan: 2, rowSpan: 2 },
  shot: { parent: 'right', col: 5, row: 3, colSpan: 2, rowSpan: 2 },
  attack: { parent: 'right', col: 1, row: 5, colSpan: 3, rowSpan: 3 },
  dash: { parent: 'right', col: 4, row: 5, colSpan: 3, rowSpan: 3 },
  smartdash: { parent: 'right', col: 1, row: 8, colSpan: 6, rowSpan: 2 },
});

const productionGapMatch = source.match(/--ggn-gap:\s*(\d+(?:\.\d+)?)px/);
assert.ok(productionGapMatch, 'production touch styles should define --ggn-gap in pixels');
const PRODUCTION_TOUCH_GAP = Number(productionGapMatch[1]);
assert.equal(PRODUCTION_TOUCH_GAP, 4, 'portrait geometry contract should use the production 4px touch gap');

const productionSpecsBlock = source.match(/var specs = \[([\s\S]*?)\n\s*\];/);
assert.ok(productionSpecsBlock, 'production touch-control specs should be extractable');
const productionTouchSpecs = [...productionSpecsBlock[1].matchAll(/^\s*\{ parent: (left|right), id: 'ggn-btn-([^']+)', (.*) \},?\s*$/gm)]
  .map((match) => {
    const [, parent, id, fields] = match;
    const keys = fields.match(/keys:\s*\[([^\]]+)\]/);
    const key = fields.match(/(?:^|, )key:\s*(\d+)/);
    const actions = fields.match(/actions:\s*\[([^\]]+)\]/);
    const action = fields.match(/(?:^|, )action:\s*(\d+)/);
    return {
      id,
      parent,
      keys: keys ? keys[1].split(',').map(Number) : key ? [Number(key[1])] : [],
      actions: actions ? actions[1].split(',').map(Number) : action ? [Number(action[1])] : [],
      toggle: /(?:^|, )toggle:\s*true/.test(fields),
      wide: /(?:^|, )wide:\s*true/.test(fields),
      big: /(?:^|, )big:\s*true/.test(fields),
      caption: /(?:^|, )caption:\s*\[/.test(fields),
    };
  });
assert.equal(productionTouchSpecs.length, 17, 'production should define exactly 17 touch controls');
assert.equal(new Set(productionTouchSpecs.map((spec) => spec.id)).size, 17, 'production touch-control ids should be unique');
assert.deepEqual(
  new Set(productionTouchSpecs.map((spec) => spec.id)),
  new Set(Object.keys(ORIGINAL_BUTTON_LAYOUT)),
  'production specs and portrait placement contract should cover the same 17 ids',
);
const expectedProductionTouchSpecs = {
  map: { parent: 'left', actions: [6] },
  'up-left': { parent: 'left', keys: [38, 37] },
  up: { parent: 'left', keys: [38] },
  'up-right': { parent: 'left', keys: [38, 39] },
  left: { parent: 'left', keys: [37] },
  right: { parent: 'left', keys: [39] },
  'down-left': { parent: 'left', keys: [40, 37] },
  down: { parent: 'left', keys: [40] },
  'down-right': { parent: 'left', keys: [40, 39] },
  step: { parent: 'left', actions: [0, 1] },
  menu: { parent: 'right', actions: [3] },
  turn: { parent: 'right', actions: [2] },
  diag: { parent: 'right', actions: [4] },
  shot: { parent: 'right', actions: [5] },
  attack: { parent: 'right', actions: [0] },
  dash: { parent: 'right', actions: [1] },
  smartdash: { parent: 'right', actions: [7], toggle: true },
};
Object.entries(expectedProductionTouchSpecs).forEach(([id, expected]) => {
  const actual = productionTouchSpecs.find((spec) => spec.id === id);
  assert.ok(actual, `production touch specs should contain ${id}`);
  assert.equal(actual.parent, expected.parent, `${id} should remain in the ${expected.parent} production pad`);
  assert.deepEqual(actual.keys, expected.keys || [], `${id} should retain its production key semantics`);
  assert.deepEqual(actual.actions, expected.actions || [], `${id} should retain its production action semantics`);
  assert.equal(actual.toggle, expected.toggle || false, `${id} should retain its production toggle semantics`);
  assert.equal(actual.parent, ORIGINAL_BUTTON_LAYOUT[id].parent, `${id} should retain its authored pad`);
  assert.equal(actual.wide, ['map', 'menu', 'turn', 'diag', 'shot', 'smartdash'].includes(id), `${id} should retain its authored wide flag`);
  assert.equal(actual.big, ['attack', 'dash'].includes(id), `${id} should retain its authored big flag`);
  assert.equal(actual.caption, ['turn', 'diag', 'attack', 'dash'].includes(id), `${id} should retain its authored caption hierarchy`);
});

function assertSourceContains(fragment) {
  assert.ok(
    source.includes(fragment),
    `source\\gameMainSystem\\cInput.cpp should contain: ${fragment}`,
  );
}

const originalGridRules = [...source.matchAll(/#ggn-btn-([a-z-]+)\s*\{\s*grid-column:\s*(\d+)\s*\/\s*span\s*(\d+);\s*grid-row:\s*(\d+)\s*\/\s*span\s*(\d+);\s*\}/g)];
assert.equal(originalGridRules.length, 17, 'runtime CSS should define exactly the original 17 control placements');
assert.equal(new Set(originalGridRules.map((m) => m[1])).size, 17, 'original runtime placements should be unique');
originalGridRules.forEach(([, id, col, colSpan, row, rowSpan]) => {
  const expected = ORIGINAL_BUTTON_LAYOUT[id];
  assert.ok(expected, `unexpected runtime placement ${id}`);
  assert.deepEqual({ col: Number(col), colSpan: Number(colSpan), row: Number(row), rowSpan: Number(rowSpan) },
    { col: expected.col, colSpan: expected.colSpan, row: expected.row, rowSpan: expected.rowSpan },
    `${id} must retain the author's historical grid position and spans`);
});
assertSourceContains('--ggn-cell: min(28px, calc((100vw - 66px) / 12))');
assertSourceContains('grid-template-columns: repeat(6, var(--ggn-cell)); grid-template-rows: repeat(10, var(--ggn-cell)); gap: var(--ggn-gap)');
assertSourceContains('font: 700 clamp(12px, 3.4vw, 14px)/1.05 Arial, sans-serif');
assertSourceContains('#ggn-touch-controls button.ggn-wide { font-size: clamp(13px, 3.7vw, 15px); }');
assertSourceContains('#ggn-touch-controls button.ggn-big { font-size: clamp(19px, 5.6vw, 23px); }');
assertSourceContains('#ggn-touch-controls button .ggn-caption-key { font-size: .88em; }');
assertSourceContains('#ggn-touch-controls button.ggn-big .ggn-caption-action { font-size: .9em; }');
assertSourceContains('#ggn-touch-controls button.ggn-big .ggn-caption-key { font-size: 1.02em; }');
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
  buildScript.includes('name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover"'),
  'mobile viewport sizing should be present in the initial HTML response',
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
assert.ok(buildScript.includes('id="ggn-visual-viewport"'), 'the web page should install a visual viewport synchronizer');
assert.ok(
  buildScript.includes('viewport.addEventListener("resize",schedule,false)')
    && buildScript.includes('viewport.addEventListener("scroll",schedule,false)'),
  'the visual viewport synchronizer should follow browser chrome and viewport movement',
);
assert.ok(
  buildScript.includes('window.innerHeight||root.clientHeight'),
  'browsers without VisualViewport should fall back to the window dimensions',
);
assert.ok(
  buildScript.includes('bottom:calc(var(--ggn-visual-viewport-bottom,0px) + max(12px,env(safe-area-inset-bottom)))'),
  'portrait controls should be raised above hidden layout-viewport space and the safe area',
);
assert.ok(
  buildScript.includes('transform:scale(var(--ggn-portrait-pad-scale,1))')
    && buildScript.includes('transform-origin:bottom left')
    && buildScript.includes('transform-origin:bottom right'),
  'portrait should uniformly scale whole authored pads around their safe-edge anchors',
);
assert.ok(
  buildScript.includes('html.ggn-controls-hidden body.ggn-mobile-ready canvas.emscripten{width:min(var(--ggn-visual-viewport-width,100vw),calc(var(--ggn-visual-viewport-height,100dvh) * 1.333333))'),
  'hidden mode should expand the game canvas into the actual visible viewport',
);
assert.ok(
  buildScript.includes('#ggn-page-links{top:calc(var(--ggn-visual-viewport-top,0px) + max(4px,env(safe-area-inset-top)))'),
  'portrait page controls should follow the visible viewport top',
);
assert.ok(
  buildScript.includes('top:calc(var(--ggn-visual-viewport-top,0px) + var(--ggn-portrait-page-strip,40px))'),
  'portrait canvas should start below the safe-area-aware page-control strip',
);
assert.ok(
  buildScript.includes('pageStrip=Math.max(40,safeTop+36)')
    && buildScript.includes('minimumCanvas=clamp(height*.2,48,96)'),
  'adaptive control sizing should reserve a compact game canvas',
);
assert.ok(
  buildScript.includes('$pageControlStyle + $portraitCanvasStyle + $visualViewportStyle + $hiddenCanvasStyle + $visualViewportPositionStyle'),
  'the real patcher should concatenate every generated style in contract order',
);
assert.ok(
  buildScript.includes('"<body>" + $audioLifecycleScript + $visualViewportScript + $pageControls'),
  'the real patcher should concatenate every generated body control in contract order',
);
assert.ok(
  buildScript.includes('$content.Replace("</head>", $mobileAppMeta + "</head>")'),
  'the real patcher should inject the complete mobile metadata block',
);
assert.ok(
  buildScript.includes('--ggn-controls-height:calc(var(--ggn-portrait-panel-height,var(--ggn-panel-height)) + max(12px,env(safe-area-inset-bottom)))'),
  'canvas reservation must include both scaled panel height and the bottom safe edge',
);
['left', 'right', 'top', 'bottom'].forEach((side) => {
  assert.ok(buildScript.includes(`--ggn-safe-area-${side}:env(safe-area-inset-${side},0px)`), `CSS must provide the ${side} safe-area input`);
  assert.ok(buildScript.includes(`getPropertyValue("--ggn-safe-area-${side}")`), `synchronizer must sample the ${side} safe-area input`);
});
assert.ok(
  buildScript.includes('canvas.emscripten{box-sizing:border-box;width:min(100%,calc((100vh - var(--ggn-controls-height) - 12px) * 1.333333))'),
  'portrait canvas should shrink into the space left above taller controls',
);
function assignedPowerShellString(name) {
  const match = buildScript.match(new RegExp(`^\\s*\\$${name}\\s*=\\s*'([^\\r\\n]*)'\\s*$`, 'm'));
  assert.ok(match, `tools\\build-web.ps1 should assign $${name} on one line`);
  return match[1].replace(/''/g, "'");
}

const expectedInjectedStyle = [
  'pageControlStyle',
  'portraitCanvasStyle',
  'visualViewportStyle',
  'hiddenCanvasStyle',
  'visualViewportPositionStyle',
].map(assignedPowerShellString).join('');
function assertNoAuthoredControlOverrides(styles) {
  assert.doesNotMatch(styles, /#ggn-btn-/, 'generator must not override any authored button placement');
  assert.doesNotMatch(styles, /#ggn-touch-controls\s+button/, 'generator must not override authored button presentation');
  assert.doesNotMatch(styles, /grid-template|grid-column|grid-row|--ggn-cell:/, 'generator must not redefine the authored grid or its cell');
}
assertNoAuthoredControlOverrides(expectedInjectedStyle);
assert.throws(() => assertNoAuthoredControlOverrides(expectedInjectedStyle + '#ggn-btn-turn{grid-column:1}'), /authored button placement/);
assert.throws(() => assertNoAuthoredControlOverrides(expectedInjectedStyle + '#ggn-touch-controls button.ggn-big{font-size:26px}'), /authored button presentation/);
assert.throws(() => assertNoAuthoredControlOverrides(expectedInjectedStyle + '.ggn-pad{grid-template-columns:repeat(4,1fr)}'), /authored grid/);
const expectedInjectedBody = [
  'audioLifecycleScript',
  'visualViewportScript',
  'pageControls',
].map(assignedPowerShellString).join('');
const expectedMobileAppMeta = assignedPowerShellString('mobileAppMeta');

const generatedHtml = ['ggn.html', 'index.html'].map((name) => ({
  name,
  content: fs.readFileSync(path.join(root, 'docs', 'play', name), 'utf8'),
}));
generatedHtml.forEach(({ name, content }) => {
  assert.match(content, /id=(?:"ggn-fullscreen-button"|ggn-fullscreen-button)/, `${name} should contain the fullscreen toggle`);
  assert.ok(content.includes('requestFullscreen||root.webkitRequestFullscreen'), `${name} should contain fullscreen entry logic`);
  assert.ok(content.includes('navigationUI:"hide"'), `${name} should request hidden browser navigation`);
  assert.ok(content.includes('apple-mobile-web-app-capable'), `${name} should contain the iOS app-mode metadata`);
  const viewportTags = content.match(/<meta\s+name=["']?viewport["']?\s+content=["'][^"']+["']>/g) || [];
  assert.equal(viewportTags.length, 1, `${name} should contain exactly one static viewport tag`);
  assert.ok(content.indexOf(viewportTags[0]) < content.indexOf('<body>'), `${name} viewport metadata should precede body parsing`);
  assert.ok(content.includes('このブラウザでは全画面表示を利用できません'), `${name} should contain the unsupported-browser guidance`);
  assert.match(content, /id=(?:"ggn-controls-toggle"|ggn-controls-toggle)/, `${name} should contain the touch-control toggle`);
  assert.ok(content.includes('ggn-controls-hidden #ggn-touch-controls'), `${name} should contain hidden-mode styling`);
  assert.ok(content.includes('align-items:center;padding-left:0;padding-right:0'), `${name} should remove hidden landscape gutters`);
  assert.ok(content.includes('ggn-controls-hidden #ggn-fullscreen-button'), `${name} should hide utility controls except restore`);
  assert.ok(content.includes('ggn-touch-controls-hidden'), `${name} should persist hidden mode`);
  assert.ok(content.includes('id="ggn-visual-viewport"'), `${name} should contain the visual viewport synchronizer`);
  assert.ok(content.includes('--ggn-visual-viewport-bottom'), `${name} should contain visual viewport bottom compensation`);
  assert.ok(content.includes('--ggn-portrait-pad-scale'), `${name} should contain whole-pad portrait scaling`);
  assert.ok(content.includes('html.ggn-controls-hidden body.ggn-mobile-ready canvas.emscripten'), `${name} should expand the hidden-mode canvas`);
  assert.equal(content.split(expectedInjectedStyle).length - 1, 1, `${name} should contain exactly the generated style block`);
  assert.equal(content.split(expectedInjectedBody).length - 1, 1, `${name} should contain exactly the generated body controls`);
  assert.equal(content.split(expectedMobileAppMeta).length - 1, 1, `${name} should contain exactly the generated mobile metadata`);
  assert.ok(content.includes('canvas.emscripten{box-sizing:border-box;width:min(100%,calc((100vh - var(--ggn-controls-height) - 12px) * 1.333333))'), `${name} should keep the portrait canvas visible above taller controls`);
  assert.ok(!content.includes('&#25147;&#12377;'), `${name} script labels should not contain undecoded HTML entities`);
});
assert.equal(generatedHtml[0].content, generatedHtml[1].content, 'ggn.html and index.html should stay identical');

const visualViewportScriptMatch = generatedHtml[0].content.match(
  /<script id="ggn-visual-viewport">([\s\S]*?)<\/script>/,
);
assert.ok(visualViewportScriptMatch, 'the generated visual viewport synchronizer should be executable in isolation');
function runViewportProjection({ innerWidth = 360, innerHeight = 640, clientWidth = innerWidth, clientHeight = innerHeight, visualViewport, safeTop = 0, safeBottom = 0, safeLeft = 0, safeRight = 0 } = {}) {
  const values = new Map();
  const frames = [];
  const windowHandlers = {};
  const viewportHandlers = {};
  const safeAreas = { top: safeTop, bottom: safeBottom, left: safeLeft, right: safeRight };
  const root = { clientWidth, clientHeight, style: { setProperty(name, value) { values.set(name, value); } } };
  const viewport = visualViewport && { ...visualViewport, addEventListener(type, handler) { viewportHandlers[type] = handler; } };
  const win = {
    innerWidth, innerHeight, visualViewport: viewport,
    requestAnimationFrame(callback) { frames.push(callback); return frames.length; },
    addEventListener(type, handler) { windowHandlers[type] = handler; },
  };
  new Function('window', 'document', 'getComputedStyle', 'setTimeout', visualViewportScriptMatch[1])(
    win, { documentElement: root },
    () => ({ getPropertyValue: (name) => `${safeAreas[name.replace('--ggn-safe-area-', '')] || 0}px` }),
    (callback) => { frames.push(callback); return frames.length; },
  );
  return { values, frames, windowHandlers, viewportHandlers, viewport, safeAreas, win };
}
const viewportCase = runViewportProjection({ visualViewport: { width: 360, height: 520, offsetLeft: 0, offsetTop: 0 } });
assert.equal(viewportCase.values.get('--ggn-visual-viewport-height'), '520px', 'visible height should come from VisualViewport');
assert.equal(viewportCase.values.get('--ggn-visual-viewport-bottom'), '120px', 'hidden layout space should become bottom inset');
assert.equal(viewportCase.values.get('--ggn-portrait-pad-scale'), '1', 'normal portrait should preserve authored dimensions');
assert.equal(viewportCase.values.get('--ggn-portrait-panel-height'), '281px', '360px portrait should retain its original 281px panel');
assert.equal(viewportCase.win.ggnVisualViewport.isWithinPortraitFitGuarantee, true, 'normal portrait should be inside guarantee');
Object.assign(viewportCase.viewport, { width: 336, offsetLeft: 12, offsetTop: 32 });
viewportCase.viewportHandlers.scroll();
assert.equal(viewportCase.frames.length, 1, 'viewport movement should schedule one update');
viewportCase.frames.shift()();
assert.equal(viewportCase.values.get('--ggn-visual-viewport-left'), '12px');
assert.equal(viewportCase.values.get('--ggn-visual-viewport-top'), '32px');
assert.equal(viewportCase.values.get('--ggn-visual-viewport-right'), '12px');
assert.equal(viewportCase.values.get('--ggn-visual-viewport-bottom'), '88px');
assert.equal(Number(viewportCase.values.get('--ggn-portrait-pad-scale')), 310 / 334, 'narrow visual viewport should scale the original pads, not reflow them');
Object.assign(viewportCase.safeAreas, { top: 47, bottom: 34, left: 47, right: 47 });
viewportCase.viewportHandlers.resize();
viewportCase.viewportHandlers.scroll();
assert.equal(viewportCase.frames.length, 1, 'viewport bursts should coalesce');
viewportCase.frames.shift()();
assert.equal(viewportCase.values.get('--ggn-portrait-page-strip'), '83px');
assert.equal(Number(viewportCase.values.get('--ggn-portrait-pad-scale')), 232 / 334, 'both side safe areas must constrain whole-pad width');
assert.ok(viewportCase.windowHandlers.resize && viewportCase.windowHandlers.orientationchange, 'window fallback listeners should remain installed');

[320, 360, 390, 412].forEach((width) => {
  const compact = runViewportProjection({ innerWidth: width, innerHeight: 915,
    visualViewport: { width, height: 400, offsetLeft: 0, offsetTop: 0 }, safeTop: 47, safeBottom: 34 });
  const originalCell = Math.min(28, (width - 66) / 12);
  assert.equal(compact.values.get('--ggn-portrait-panel-height'), '191px', '400px safe-area boundary reserves 191px for the original pad');
  assert.equal(Number(compact.values.get('--ggn-portrait-pad-scale')), 191 / (10 * originalCell + 36), 'all original content should use the same scale');
  assert.equal(compact.win.ggnVisualViewport.isWithinPortraitFitGuarantee, true);
});
[320, 200, 160].forEach((height) => {
  const unsupported = runViewportProjection({ visualViewport: { width: 360, height, offsetLeft: 0, offsetTop: 0 }, safeTop: 47, safeBottom: 34 });
  assert.equal(unsupported.win.ggnVisualViewport.isWithinPortraitFitGuarantee, false, 'below-floor geometry must not be reported as supported');
});
const unsafeInset = runViewportProjection({ safeTop: 48 });
assert.equal(unsafeInset.win.ggnVisualViewport.isWithinPortraitFitGuarantee, false, 'outside tested safe-area bounds is explicitly best effort');
const innerFallback = runViewportProjection({ innerWidth: 390, innerHeight: 700, clientWidth: 375, clientHeight: 667 });
assert.equal(innerFallback.values.get('--ggn-visual-viewport-width'), '390px');
assert.equal(innerFallback.values.get('--ggn-visual-viewport-height'), '700px');
['left', 'top', 'right', 'bottom'].forEach((side) => assert.equal(innerFallback.values.get(`--ggn-visual-viewport-${side}`), '0px'));
assert.equal(innerFallback.values.get('--ggn-portrait-pad-scale'), '1');
assert.equal(innerFallback.values.get('--ggn-portrait-panel-height'), '306px');
assert.equal(innerFallback.win.ggnVisualViewport.isWithinPortraitFitGuarantee, true);
const clientFallback = runViewportProjection({ innerWidth: 0, innerHeight: 0, clientWidth: 320, clientHeight: 568 });
assert.equal(clientFallback.values.get('--ggn-visual-viewport-width'), '320px');
assert.equal(clientFallback.values.get('--ggn-visual-viewport-height'), '568px');
assert.equal(clientFallback.values.get('--ggn-visual-viewport-center-x'), '160px');
assert.equal(clientFallback.values.get('--ggn-portrait-pad-scale'), '1');
assert.equal(clientFallback.values.get('--ggn-portrait-panel-height'), '247.67px');

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

const GAP = PRODUCTION_TOUCH_GAP;
const EDGE = 8;
const SIDE_GAP = 8;
const MIN_LANDSCAPE_CELL = 18;
const MAX_CELL = 28;
const MIN_PORTRAIT_PAGE_STRIP = 40;
const PORTRAIT_PAGE_CONTROLS_HEIGHT = 36;
const MIN_SUPPORTED_VISUAL_HEIGHT = 400;
const MANUAL_LINK_WIDTH = 96;
const MANUAL_LINK_HEIGHT = 32;

const pageControlCss = assignedPowerShellString('pageControlStyle');
const baseToolbarRule = pageControlCss.match(/#ggn-page-links\{([^}]*)\}/);
const mobileToolbarRule = pageControlCss.match(/@media \(pointer:coarse\),\(max-width:900px\)\{#ggn-page-links\{([^}]*)\}#ggn-page-links a,#ggn-page-links button\{([^}]*)\}/);
assert.ok(baseToolbarRule && mobileToolbarRule, 'generated desktop and mobile toolbar rules should be extractable');
const baseToolbarFont = baseToolbarRule[1].match(/font:[^;]*?(\d+(?:\.\d+)?)px\/(\d+(?:\.\d+)?)/);
const mobileToolbarFontSize = mobileToolbarRule[1].match(/font-size:(\d+(?:\.\d+)?)px/);
const mobileToolbarPadding = mobileToolbarRule[2].match(/padding:(\d+(?:\.\d+)?)px\s+(\d+(?:\.\d+)?)px/);
const toolbarButtonRule = pageControlCss.match(/#ggn-page-links a,#ggn-page-links button\{([^}]*)\}/);
const toolbarButtonBorder = toolbarButtonRule && toolbarButtonRule[1].match(/border:(\d+(?:\.\d+)?)px/);
assert.ok(baseToolbarFont && mobileToolbarFontSize && mobileToolbarPadding && toolbarButtonBorder, 'generated toolbar dimensions should be extractable from production CSS');
const GENERATED_MOBILE_TOOLBAR_HEIGHT = Number(mobileToolbarFontSize[1]) * Number(baseToolbarFont[2])
  + 2 * Number(mobileToolbarPadding[1]) + 2 * Number(toolbarButtonBorder[1]);
assert.ok(
  source.includes('body.ggn-mobile-ready #ggn-page-links { position: fixed; top: max(4px, env(safe-area-inset-top)); right: max(8px, env(safe-area-inset-right)); width: auto; margin: 0; z-index: 30; }'),
  'runtime mobile CSS should remove generated toolbar margins and fix it to the visual viewport',
);
assert.ok(Math.abs(GENERATED_MOBILE_TOOLBAR_HEIGHT - 33.6) < 0.001, 'production mobile toolbar should render at 33.6px high');
assert.ok(GENERATED_MOBILE_TOOLBAR_HEIGHT <= PORTRAIT_PAGE_CONTROLS_HEIGHT, 'rendered mobile toolbar should fit the reserved 36px portrait strip');

function clamp(value, min, max) {
  return Math.max(min, Math.min(max, value));
}

function padWidth(cell) {
  return 6 * cell + 5 * GAP;
}

function panelHeight(cell) {
  return 10 * cell + 9 * GAP;
}

function originalPortraitCell(width) {
  return Math.max(0, Math.min(MAX_CELL, (width - 66) / 12));
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

function originalButtonRects(layout) {
  const scale = layout.scale ?? 1;
  return Object.entries(ORIGINAL_BUTTON_LAYOUT).map(([name, p]) => {
    const pad = p.parent === 'left' ? layout.leftPad : layout.rightPad;
    return rect(name,
      pad.x + (p.col - 1) * (layout.cell + GAP) * scale,
      pad.y + (p.row - 1) * (layout.cell + GAP) * scale,
      (p.colSpan * layout.cell + (p.colSpan - 1) * GAP) * scale,
      (p.rowSpan * layout.cell + (p.rowSpan - 1) * GAP) * scale);
  });
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

function portraitLayout({ width, height, safeTop = 0, safeBottom = 0, safeLeft = 0, safeRight = 0, visualViewport = null }) {
  const visible = visualViewport || { x: 0, y: 0, width, height };
  const cell = originalPortraitCell(Math.max(width, visible.x + visible.width));
  const pageStrip = Math.max(MIN_PORTRAIT_PAGE_STRIP, safeTop + PORTRAIT_PAGE_CONTROLS_HEIGHT);
  const minimumCanvas = clamp(visible.height * 0.2, 48, 96);
  const leftEdge = Math.max(EDGE, safeLeft);
  const rightEdge = Math.max(EDGE, safeRight);
  const bottomEdge = Math.max(12, safeBottom);
  const scale = Math.min(1,
    Math.max(0, (visible.width - leftEdge - rightEdge - 10) / (2 * padWidth(cell))),
    Math.max(0, (visible.height - pageStrip - minimumCanvas - 12 - bottomEdge) / panelHeight(cell)));
  const panel = panelHeight(cell) * scale;
  const padW = padWidth(cell) * scale;
  const gameHeight = visible.height - pageStrip - panel - bottomEdge - 12;
  const canvasWidth = Math.max(0, Math.min(visible.width, gameHeight * 4 / 3));
  const canvasHeight = canvasWidth * 0.75;
  const padY = visible.y + visible.height - bottomEdge - panel;
  const leftPad = rect('leftPad', visible.x + leftEdge, padY, padW, panel);
  const rightPad = rect('rightPad', visible.x + visible.width - rightEdge - padW, padY, padW, panel);
  return {
    cell, scale, pageStrip,
    isWithinPortraitFitGuarantee: visible.width >= 320 && visible.height >= 400 && safeTop <= 47 && safeBottom <= 34 && safeLeft <= 47 && safeRight <= 47 && scale > 0,
    projection: runViewportProjection({ innerWidth: width, innerHeight: height,
      visualViewport: { width: visible.width, height: visible.height, offsetLeft: visible.x, offsetTop: visible.y }, safeTop, safeBottom, safeLeft, safeRight }),
    viewport: rect('viewport', 0, 0, width, height),
    visualViewport: rect('visualViewport', visible.x, visible.y, visible.width, visible.height),
    canvas: rect('canvas', visible.x + (visible.width - canvasWidth) / 2, visible.y + pageStrip + gameHeight - canvasHeight, canvasWidth, canvasHeight),
    manualLink: rect('manualLink', visible.x + visible.width - EDGE - MANUAL_LINK_WIDTH, visible.y + Math.max(4, safeTop), MANUAL_LINK_WIDTH, PORTRAIT_PAGE_CONTROLS_HEIGHT),
    pageControls: rect('pageControls', visible.x + EDGE, visible.y + Math.max(4, safeTop), visible.width - 2 * EDGE, PORTRAIT_PAGE_CONTROLS_HEIGHT),
    controlPanel: rect('controlPanel', leftPad.x, padY, rightPad.right - leftPad.x, panel),
    leftPad, rightPad, contentGap: 0,
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
  const buttons = originalButtonRects(layout);
  assert.equal(buttons.length, 17, 'all 17 authored controls must be measured');
  assertNoOverlap([layout.leftPad, layout.rightPad]);

  buttons.forEach((button) => {
    assertInside(layout.viewport, button);
    assert.ok(button.width >= 40 || orientation === 'portrait', `${button.name} is too narrow for landscape touch`);
    assert.ok(button.height >= 40 || orientation === 'portrait' || button.name === 'step', `${button.name} is too short for landscape touch`);
    if (orientation === 'portrait') {
      assertInside(layout.controlPanel, button);
      assertInside(ORIGINAL_BUTTON_LAYOUT[button.name].parent === 'left' ? layout.leftPad : layout.rightPad, button);
      assert.ok(button.width >= 32, `${button.name} is too narrow in the supported original layout`);
      assert.ok(button.height >= 32 || button.name === 'step', `${button.name} is too short in the supported original layout`);
    }
  });
  assertNoOverlap(buttons);

  if (orientation === 'landscape') {
    assertNoOverlap([layout.leftPad, layout.rightPad]);
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
    assert.equal(layout.isWithinPortraitFitGuarantee, true, 'supported-layout assertions must not accept outside-range geometry');
    assert.equal(layout.projection.win.ggnVisualViewport.isWithinPortraitFitGuarantee, true);
    assert.ok(Math.abs(Number(layout.projection.values.get('--ggn-portrait-pad-scale')) - layout.scale) < 1e-10, 'production whole-pad scale must match geometry');
    assert.ok(Math.abs(parseFloat(layout.projection.values.get('--ggn-portrait-panel-height')) - layout.leftPad.height) < 0.011, 'production panel height must match geometry');
    assert.ok(layout.rightPad.x - layout.leftPad.right >= 10 - 0.001, 'original pads must remain separated by at least 10px');
    assertInside(layout.visualViewport, layout.canvas);
    assertInside(layout.visualViewport, layout.controlPanel);
    assertInside(layout.visualViewport, layout.pageControls);
    buttons.forEach((button) => assertInside(layout.visualViewport, button));
    assert.ok(!overlaps(layout.pageControls, layout.canvas), 'portrait page controls overlap canvas');
    assert.ok(layout.canvas.bottom <= layout.leftPad.y - 12 + 0.001, 'portrait controls overlap canvas');
    assert.ok(Math.abs(layout.canvas.width / layout.canvas.height - 4 / 3) < 0.001, 'portrait canvas lost its 4:3 aspect ratio');
  }
}

[
  { name: 'compact phone portrait', width: 320, height: 568 },
  { name: 'phone portrait', width: 390, height: 844 },
  { name: 'small phone portrait', width: 360, height: 640 },
  { name: 'large phone portrait', width: 412, height: 915, safeBottom: 24 },
  { name: 'ipad portrait', width: 768, height: 1024, safeBottom: 20 },
].forEach((viewport) => {
  assertLayout(portraitLayout(viewport), 'portrait');
});

function assertOriginalControlZones(layout) {
  const buttons = originalButtonRects(layout);
  const left = buttons.filter((b) => ORIGINAL_BUTTON_LAYOUT[b.name].parent === 'left');
  const right = buttons.filter((b) => ORIGINAL_BUTTON_LAYOUT[b.name].parent === 'right');
  assert.equal(left.length, 10);
  assert.equal(right.length, 7);
  assert.ok(Math.max(...left.map((b) => b.right)) < Math.min(...right.map((b) => b.x)), 'all seven original right-pad controls, including turn and diag, must remain right of all left controls');
  const byId = Object.fromEntries(buttons.map((b) => [b.name, b]));
  assert.equal(byId.turn.y, byId.diag.y);
  assert.equal(byId.diag.y, byId.shot.y);
  assert.equal(byId.attack.y, byId.dash.y);
  assert.ok(byId.attack.width > byId.turn.width, 'authored big-action hierarchy must remain');
  assert.equal(byId.smartdash.width, layout.rightPad.width, 'smartdash must retain its full pad width');
  assert.ok(byId.step.y > byId.down.y, 'step must remain below the d-pad, not in its empty center');
}
[320, 360].forEach((width) => {
  const layout = portraitLayout({ width, height: width === 320 ? 568 : 640 });
  assertOriginalControlZones(layout);
  assert.equal(layout.scale, 1, 'normal portrait should exactly preserve original dimensions');
  const buttons = Object.fromEntries(originalButtonRects(layout).map((b) => [b.name, b]));
  assert.ok(Math.abs(buttons.up.width - (width === 320 ? 46.33333333333333 : 53)) < 0.001);
  assert.ok(Math.abs(buttons.attack.width - (width === 320 ? 71.5 : 81.5)) < 0.001);
});
[
  { width: 360, height: 640, visualViewport: { x: 0, y: 0, width: 360, height: 520 } },
  { width: 360, height: 640, visualViewport: { x: 12, y: 32, width: 336, height: 520 } },
  { width: 390, height: 640, safeTop: 47, safeBottom: 34, visualViewport: { x: 0, y: 0, width: 390, height: 520 } },
  { width: 320, height: 640, safeLeft: 47, safeRight: 47, safeTop: 47, safeBottom: 34, visualViewport: { x: 0, y: 0, width: 320, height: 400 } },
].forEach((viewport) => {
  const layout = portraitLayout(viewport);
  assertLayout(layout, 'portrait');
  assertOriginalControlZones(layout);
});
[320, 360, 390, 412].forEach((width) => {
  const layout = portraitLayout({ width, height: 915, safeTop: 47, safeBottom: 34,
    visualViewport: { x: 0, y: 0, width, height: MIN_SUPPORTED_VISUAL_HEIGHT } });
  assertLayout(layout, 'portrait');
  assertOriginalControlZones(layout);
  assert.ok(Math.abs(layout.leftPad.height - 191) < 0.001);
  assert.ok(layout.canvas.height >= 80 - 0.001, '400px boundary must retain the original 80px canvas floor');
  assert.ok(layout.scale < 1, 'compact fitting must scale the whole pad');
  originalButtonRects(layout).forEach((button) => {
    const p = ORIGINAL_BUTTON_LAYOUT[button.name];
    assert.ok(Math.abs(button.width / layout.scale - (p.colSpan * layout.cell + (p.colSpan - 1) * GAP)) < 0.001, 'uniform scale must preserve historical geometry');
  });
});
[320, 200, 160].forEach((height) => {
  const layout = portraitLayout({ width: 360, height: 640, safeTop: 47, safeBottom: 34,
    visualViewport: { x: 0, y: 0, width: 360, height } });
  assert.equal(layout.isWithinPortraitFitGuarantee, false);
  assert.equal(layout.projection.win.ggnVisualViewport.isWithinPortraitFitGuarantee, false);
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

console.log('web touch layout tests passed (17 authored controls, both pad layouts)');
