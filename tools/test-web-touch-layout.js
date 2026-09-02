const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');

const root = path.resolve(__dirname, '..');
const source = fs.readFileSync(path.join(root, 'source', 'gameMainSystem', 'cInput.cpp'), 'utf8');
const saveSource = fs.readFileSync(path.join(root, 'source', 'cSaveStore.cpp'), 'latin1');
const buildScript = fs.readFileSync(path.join(root, 'tools', 'build-web.ps1'), 'utf8');

const PORTRAIT_BUTTON_PLACEMENTS = Object.freeze({
  map: { col: 1, row: 1, colSpan: 2 },
  menu: { col: 3, row: 1, colSpan: 2 },
  'up-left': { col: 1, row: 2 },
  up: { col: 2, row: 2 },
  'up-right': { col: 3, row: 2 },
  attack: { col: 4, row: 2 },
  left: { col: 1, row: 3 },
  step: { col: 2, row: 3 },
  right: { col: 3, row: 3 },
  dash: { col: 4, row: 3 },
  'down-left': { col: 1, row: 4 },
  down: { col: 2, row: 4 },
  'down-right': { col: 3, row: 4 },
  smartdash: { col: 4, row: 4 },
  turn: { col: 1, row: 5 },
  diag: { col: 2, row: 5 },
  shot: { col: 4, row: 5 },
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
    };
  });
assert.equal(productionTouchSpecs.length, 17, 'production should define exactly 17 touch controls');
assert.equal(new Set(productionTouchSpecs.map((spec) => spec.id)).size, 17, 'production touch-control ids should be unique');
assert.deepEqual(
  new Set(productionTouchSpecs.map((spec) => spec.id)),
  new Set(Object.keys(PORTRAIT_BUTTON_PLACEMENTS)),
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
});

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
  buildScript.includes('width:min(calc(var(--ggn-visual-viewport-width,100vw) - 16px),520px)'),
  'portrait controls should fit the actually visible viewport width',
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
  buildScript.includes('$pageControlStyle + $portraitControlStyle + $portraitCanvasStyle + $visualViewportStyle + $hiddenCanvasStyle + $visualViewportPositionStyle'),
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
  buildScript.includes('grid-template-columns:repeat(4,minmax(0,1fr))'),
  'portrait controls should use the wider four-column layout',
);
assert.ok(
  buildScript.includes('--ggn-portrait-main-row:clamp(66px,11.25dvh,76px)'),
  'portrait gameplay controls should be visibly taller outside fullscreen',
);
assert.ok(
  buildScript.includes('--ggn-portrait-utility-row:clamp(56px,8dvh,64px)'),
  'portrait utility controls should remain comfortably touchable',
);
assert.ok(
  buildScript.includes('#ggn-touch-controls button.ggn-big{font-size:clamp(15px,min(6vw,calc((var(--ggn-portrait-action-row) - 4px)/2.1)),26px)}'),
  'portrait action labels should scale with the action row without overflowing compact mode',
);
assert.ok(
  buildScript.includes('canvas.emscripten{box-sizing:border-box;width:min(100%,calc((100vh - var(--ggn-controls-height) - 12px) * 1.333333))'),
  'portrait canvas should shrink into the space left above taller controls',
);
assert.ok(
  buildScript.includes('#ggn-btn-step{grid-column:2;grid-row:3}'),
  'portrait step control should occupy a full-size center cell',
);

function assignedPowerShellString(name) {
  const match = buildScript.match(new RegExp(`^\\s*\\$${name}\\s*=\\s*'([^\\r\\n]*)'\\s*$`, 'm'));
  assert.ok(match, `tools\\build-web.ps1 should assign $${name} on one line`);
  return match[1].replace(/''/g, "'");
}

const expectedInjectedStyle = [
  'pageControlStyle',
  'portraitControlStyle',
  'portraitCanvasStyle',
  'visualViewportStyle',
  'hiddenCanvasStyle',
  'visualViewportPositionStyle',
].map(assignedPowerShellString).join('');
const portraitControlCss = assignedPowerShellString('portraitControlStyle');
assert.ok(
  portraitControlCss.includes('column-gap:var(--ggn-gap);row-gap:var(--ggn-gap)'),
  'portrait CSS should use the production touch gap for both grid axes',
);
const portraitSelectorIds = [...portraitControlCss.matchAll(/#ggn-btn-([a-z-]+)\{/g)].map((match) => match[1]);
assert.equal(portraitSelectorIds.length, 17, 'portrait CSS should place exactly 17 button selectors');
assert.equal(new Set(portraitSelectorIds).size, 17, 'portrait CSS button selectors should be unique');
assert.deepEqual(
  new Set(portraitSelectorIds),
  new Set(Object.keys(PORTRAIT_BUTTON_PLACEMENTS)),
  'portrait CSS and the geometry contract should cover the same button ids',
);
Object.entries(PORTRAIT_BUTTON_PLACEMENTS).forEach(([id, placement]) => {
  const declaration = portraitControlCss.match(new RegExp(`#ggn-btn-${id}\\{([^}]*)\\}`));
  assert.ok(declaration, `portrait CSS should contain #ggn-btn-${id}`);
  const column = `${placement.col}${placement.colSpan ? `/span ${placement.colSpan}` : ''}`;
  assert.ok(declaration[1].includes(`grid-column:${column}`), `${id} should use portrait grid column ${column}`);
  assert.ok(declaration[1].includes(`grid-row:${placement.row}`), `${id} should use portrait grid row ${placement.row}`);
});
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
  assert.ok(content.includes('--ggn-synced-portrait-main-row'), `${name} should contain adaptive portrait row sizing`);
  assert.ok(content.includes('html.ggn-controls-hidden body.ggn-mobile-ready canvas.emscripten'), `${name} should expand the hidden-mode canvas`);
  assert.equal(content.split(expectedInjectedStyle).length - 1, 1, `${name} should contain exactly the generated style block`);
  assert.equal(content.split(expectedInjectedBody).length - 1, 1, `${name} should contain exactly the generated body controls`);
  assert.equal(content.split(expectedMobileAppMeta).length - 1, 1, `${name} should contain exactly the generated mobile metadata`);
  assert.ok(content.includes('grid-template-columns:repeat(4,minmax(0,1fr))'), `${name} should contain the wider portrait control grid`);
  assert.ok(content.includes('--ggn-portrait-main-row:clamp(66px,11.25dvh,76px)'), `${name} should enforce the main portrait button height`);
  assert.ok(content.includes('--ggn-portrait-utility-row:clamp(56px,8dvh,64px)'), `${name} should enforce the utility portrait button height`);
  assert.ok(content.includes('canvas.emscripten{box-sizing:border-box;width:min(100%,calc((100vh - var(--ggn-controls-height) - 12px) * 1.333333))'), `${name} should keep the portrait canvas visible above taller controls`);
  assert.ok(!content.includes('&#25147;&#12377;'), `${name} script labels should not contain undecoded HTML entities`);
});
assert.equal(generatedHtml[0].content, generatedHtml[1].content, 'ggn.html and index.html should stay identical');

const visualViewportScriptMatch = generatedHtml[0].content.match(
  /<script id="ggn-visual-viewport">([\s\S]*?)<\/script>/,
);
assert.ok(visualViewportScriptMatch, 'the generated visual viewport synchronizer should be executable in isolation');
const viewportValues = new Map();
const visualViewportHandlers = {};
const viewportWindowHandlers = {};
const animationFrames = [];
const viewportRoot = {
  clientWidth: 360,
  clientHeight: 640,
  style: {
    setProperty(name, value) { viewportValues.set(name, value); },
  },
};
const mockVisualViewport = {
  width: 360,
  height: 520,
  offsetLeft: 0,
  offsetTop: 0,
  addEventListener(type, handler) { visualViewportHandlers[type] = handler; },
};
const viewportWindow = {
  innerWidth: 360,
  innerHeight: 640,
  visualViewport: mockVisualViewport,
  requestAnimationFrame(callback) { animationFrames.push(callback); return animationFrames.length; },
  addEventListener(type, handler) { viewportWindowHandlers[type] = handler; },
};
const viewportSafeAreas = {
  '--ggn-safe-area-top': '0px',
  '--ggn-safe-area-bottom': '0px',
};
new Function('window', 'document', 'getComputedStyle', 'setTimeout', visualViewportScriptMatch[1])(
  viewportWindow,
  { documentElement: viewportRoot },
  () => ({ getPropertyValue: (name) => viewportSafeAreas[name] || '0px' }),
  (callback) => { animationFrames.push(callback); return animationFrames.length; },
);
assert.equal(viewportValues.get('--ggn-visual-viewport-height'), '520px', 'the visible height should come from VisualViewport');
assert.equal(viewportValues.get('--ggn-visual-viewport-bottom'), '120px', 'hidden layout-viewport space should become a bottom inset');
assert.equal(viewportValues.get('--ggn-visual-viewport-center-x'), '180px', 'portrait controls should center in the visible viewport');
assert.equal(viewportValues.get('--ggn-synced-portrait-utility-row'), '56px', 'toolbar reduction should retain useful utility button height');
assert.equal(viewportValues.get('--ggn-synced-portrait-main-row'), '66px', 'toolbar reduction should retain useful gameplay button height');
assert.equal(viewportValues.get('--ggn-synced-portrait-action-extra'), '8px', 'normal portrait height should retain the enlarged action-row extra');
mockVisualViewport.width = 336;
mockVisualViewport.offsetLeft = 12;
mockVisualViewport.offsetTop = 32;
visualViewportHandlers.scroll();
assert.equal(animationFrames.length, 1, 'visual viewport movement should schedule a layout update');
animationFrames.shift()();
assert.equal(viewportValues.get('--ggn-visual-viewport-left'), '12px', 'horizontal visual offset should be published');
assert.equal(viewportValues.get('--ggn-visual-viewport-top'), '32px', 'vertical visual offset should be published');
assert.equal(viewportValues.get('--ggn-visual-viewport-right'), '12px', 'the opposite horizontal inset should be published');
assert.equal(viewportValues.get('--ggn-visual-viewport-bottom'), '88px', 'bottom compensation should account for visual offsetTop');
viewportSafeAreas['--ggn-safe-area-top'] = '47px';
viewportSafeAreas['--ggn-safe-area-bottom'] = '34px';
visualViewportHandlers.resize();
animationFrames.shift()();
assert.equal(viewportValues.get('--ggn-portrait-page-strip'), '83px', 'notched devices should reserve safe top plus the page-control row');
viewportSafeAreas['--ggn-safe-area-top'] = '0px';
viewportSafeAreas['--ggn-safe-area-bottom'] = '0px';
mockVisualViewport.width = 360;
mockVisualViewport.height = 640;
mockVisualViewport.offsetLeft = 0;
mockVisualViewport.offsetTop = 0;
visualViewportHandlers.resize();
visualViewportHandlers.scroll();
assert.equal(animationFrames.length, 1, 'viewport bursts should be coalesced into one animation frame');
animationFrames.shift()();
assert.equal(viewportValues.get('--ggn-visual-viewport-bottom'), '0px', 'expanded browser chrome should remove the bottom compensation');
assert.equal(viewportValues.get('--ggn-synced-portrait-main-row'), '72px', 'full-height portrait should retain the enlarged gameplay buttons');
assert.equal(viewportValues.get('--ggn-synced-portrait-action-extra'), '8px', 'full-height portrait should retain the full action-row extra');
assert.ok(viewportWindowHandlers.resize && viewportWindowHandlers.orientationchange, 'window resize and rotation should also resync the viewport');

viewportSafeAreas['--ggn-safe-area-top'] = '47px';
viewportSafeAreas['--ggn-safe-area-bottom'] = '34px';
mockVisualViewport.height = 400;
visualViewportHandlers.resize();
animationFrames.shift()();
assert.equal(viewportValues.get('--ggn-synced-portrait-utility-row'), '32px', 'supported compact boundary should use the utility-row floor');
assert.equal(viewportValues.get('--ggn-synced-portrait-main-row'), '32px', 'supported compact boundary should use the main-row floor');
assert.equal(viewportValues.get('--ggn-synced-portrait-action-extra'), '5px', 'supported compact boundary should distribute the remaining 15px over three action rows');

function runVisualViewportFallback({ innerWidth, innerHeight, clientWidth, clientHeight }) {
  const values = new Map();
  const handlers = {};
  const fallbackRoot = {
    clientWidth,
    clientHeight,
    style: { setProperty(name, value) { values.set(name, value); } },
  };
  const fallbackWindow = {
    innerWidth,
    innerHeight,
    addEventListener(type, handler) { handlers[type] = handler; },
  };
  new Function('window', 'document', 'getComputedStyle', 'setTimeout', visualViewportScriptMatch[1])(
    fallbackWindow,
    { documentElement: fallbackRoot },
    () => ({ getPropertyValue: () => '0px' }),
    (callback) => { callback(); return 1; },
  );
  return { values, handlers };
}

const innerViewportFallback = runVisualViewportFallback({
  innerWidth: 390,
  innerHeight: 700,
  clientWidth: 375,
  clientHeight: 667,
});
assert.equal(innerViewportFallback.values.get('--ggn-visual-viewport-width'), '390px', 'fallback should use window.innerWidth without VisualViewport');
assert.equal(innerViewportFallback.values.get('--ggn-visual-viewport-height'), '700px', 'fallback should use window.innerHeight without VisualViewport');
assert.equal(innerViewportFallback.values.get('--ggn-visual-viewport-left'), '0px', 'fallback viewport should start at the layout origin');
assert.equal(innerViewportFallback.values.get('--ggn-visual-viewport-top'), '0px', 'fallback viewport should start at the layout origin');
assert.equal(innerViewportFallback.values.get('--ggn-visual-viewport-right'), '0px', 'fallback should not invent a right inset');
assert.equal(innerViewportFallback.values.get('--ggn-visual-viewport-bottom'), '0px', 'fallback should not invent a bottom inset');
assert.equal(innerViewportFallback.values.get('--ggn-synced-portrait-main-row'), '76px', 'fallback should still publish portrait row sizing');
assert.ok(innerViewportFallback.handlers.resize && innerViewportFallback.handlers.orientationchange, 'fallback should retain window resize/orientation listeners');

const clientViewportFallback = runVisualViewportFallback({
  innerWidth: 0,
  innerHeight: 0,
  clientWidth: 320,
  clientHeight: 568,
});
assert.equal(clientViewportFallback.values.get('--ggn-visual-viewport-width'), '320px', 'fallback should use root.clientWidth when innerWidth is unavailable');
assert.equal(clientViewportFallback.values.get('--ggn-visual-viewport-height'), '568px', 'fallback should use root.clientHeight when innerHeight is unavailable');
assert.equal(clientViewportFallback.values.get('--ggn-visual-viewport-center-x'), '160px', 'root-client fallback should center portrait controls');
assert.equal(clientViewportFallback.values.get('--ggn-synced-portrait-utility-row'), '56px', 'root-client fallback should publish utility-row sizing');
assert.equal(clientViewportFallback.values.get('--ggn-synced-portrait-main-row'), '66px', 'root-client fallback should publish main-row sizing');
assert.equal(clientViewportFallback.values.get('--ggn-synced-portrait-action-extra'), '8px', 'root-client fallback should publish action-row sizing');

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
const MIN_PORTRAIT_UTILITY_ROW = 56;
const MAX_PORTRAIT_UTILITY_ROW = 64;
const MIN_PORTRAIT_MAIN_ROW = 66;
const MAX_PORTRAIT_MAIN_ROW = 76;
const PORTRAIT_ACTION_EXTRA = 8;
const MIN_PORTRAIT_PAGE_STRIP = 40;
const PORTRAIT_PAGE_CONTROLS_HEIGHT = 36;
const MIN_SUPPORTED_VISUAL_HEIGHT = 400;
const MAX_PORTRAIT_PANEL_WIDTH = 520;
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

function portraitUtilityRow(height) {
  return clamp(height * 0.08, MIN_PORTRAIT_UTILITY_ROW, MAX_PORTRAIT_UTILITY_ROW);
}

function portraitMainRow(height) {
  return clamp(height * 0.1125, MIN_PORTRAIT_MAIN_ROW, MAX_PORTRAIT_MAIN_ROW);
}

function portraitRows(visibleHeight, safeBottom = 0, safeTop = 0) {
  let utilityRow = portraitUtilityRow(visibleHeight);
  let mainRow = portraitMainRow(visibleHeight);
  const pageStrip = Math.max(MIN_PORTRAIT_PAGE_STRIP, safeTop + PORTRAIT_PAGE_CONTROLS_HEIGHT);
  const minimumCanvas = clamp(visibleHeight * 0.2, 48, 96);
  const availableTracks = Math.max(
    0,
    visibleHeight - pageStrip - minimumCanvas
      - 4 * GAP - 12 - Math.max(12, safeBottom),
  );
  const rowBudget = Math.max(0, availableTracks - 3 * PORTRAIT_ACTION_EXTRA);
  const rowTotal = utilityRow + 4 * mainRow;
  if (rowTotal > rowBudget && rowBudget > 0) {
    const scale = rowBudget / rowTotal;
    utilityRow = Math.max(32, utilityRow * scale);
    mainRow = Math.max(32, mainRow * scale);
    if (utilityRow + 4 * mainRow > rowBudget) {
      utilityRow = Math.max(32, Math.min(utilityRow, rowBudget / 5));
      mainRow = Math.max(32, (rowBudget - utilityRow) / 4);
    }
  }
  const actionExtra = clamp((availableTracks - utilityRow - 4 * mainRow) / 3, 0, PORTRAIT_ACTION_EXTRA);
  return { utilityRow, mainRow, actionExtra, pageStrip };
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

function landscapeButtonRects(layout) {
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

function portraitButtonGrid(layout, name, col, row, colSpan = 1) {
  const rowHeights = [layout.utilityRow, layout.actionRow, layout.actionRow, layout.actionRow, layout.mainRow];
  const rowHeight = rowHeights[row - 1];
  const y = layout.controlPanel.y + rowHeights.slice(0, row - 1).reduce((total, value) => total + value, 0) + (row - 1) * GAP;
  return rect(
    name,
    layout.controlPanel.x + (col - 1) * (layout.column + GAP),
    y,
    colSpan * layout.column + (colSpan - 1) * GAP,
    rowHeight,
  );
}

function portraitButtonRects(layout) {
  return Object.entries(PORTRAIT_BUTTON_PLACEMENTS).map(([name, placement]) => (
    portraitButtonGrid(layout, name, placement.col, placement.row, placement.colSpan)
  ));
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

function portraitLayout({ width, height, safeTop = 0, safeBottom = 0, visualViewport = null }) {
  const visible = visualViewport || { x: 0, y: 0, width, height };
  const { utilityRow, mainRow, actionExtra, pageStrip } = portraitRows(visible.height, safeBottom, safeTop);
  const actionRow = mainRow + actionExtra;
  const panel = utilityRow + 4 * mainRow + 3 * actionExtra + 4 * GAP;
  const controlsHeight = panel + Math.max(12, safeBottom);
  const gameHeight = visible.height - pageStrip - controlsHeight - 12;
  const canvasWidth = Math.min(visible.width, gameHeight * 4 / 3);
  const canvasHeight = Math.min(gameHeight, visible.width * 0.75);
  const panelWidth = Math.min(visible.width - 2 * EDGE, MAX_PORTRAIT_PANEL_WIDTH);
  const panelX = visible.x + (visible.width - panelWidth) / 2;
  const controlPanel = rect(
    'controlPanel',
    panelX,
    visible.y + visible.height - Math.max(12, safeBottom) - panel,
    panelWidth,
    panel,
  );
  return {
    cell: mainRow,
    utilityRow,
    mainRow,
    actionExtra,
    actionRow,
    column: (panelWidth - 3 * GAP) / 4,
    viewport: rect('viewport', 0, 0, width, height),
    visualViewport: rect('visualViewport', visible.x, visible.y, visible.width, visible.height),
    canvas: rect(
      'canvas',
      visible.x + (visible.width - canvasWidth) / 2,
      visible.y + pageStrip + gameHeight - canvasHeight,
      canvasWidth,
      canvasHeight,
    ),
    manualLink: rect('manualLink', visible.x + visible.width - EDGE - MANUAL_LINK_WIDTH, visible.y + Math.max(4, safeTop), MANUAL_LINK_WIDTH, PORTRAIT_PAGE_CONTROLS_HEIGHT),
    pageControls: rect('pageControls', visible.x + EDGE, visible.y + Math.max(4, safeTop), visible.width - 2 * EDGE, PORTRAIT_PAGE_CONTROLS_HEIGHT),
    controlPanel,
    leftPad: controlPanel,
    rightPad: controlPanel,
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

function assertLayout(layout, orientation, { allowCompactPortrait = false } = {}) {
  assert.ok(layout.cell >= 0, 'cell size should be non-negative');
  assertInside(layout.viewport, layout.canvas);
  assertInside(layout.viewport, layout.manualLink);
  assertInside(layout.viewport, layout.leftPad);
  assertInside(layout.viewport, layout.rightPad);
  const buttons = orientation === 'portrait' ? portraitButtonRects(layout) : landscapeButtonRects(layout);

  buttons.forEach((button) => {
    assertInside(layout.viewport, button);
    assert.ok(button.width >= 40 || orientation === 'portrait', `${button.name} is too narrow for landscape touch`);
    assert.ok(button.height >= 40 || orientation === 'portrait' || button.name === 'step', `${button.name} is too short for landscape touch`);
    if (orientation === 'portrait') {
      assertInside(layout.controlPanel, button);
      assert.ok(button.width >= 63, `${button.name} is too narrow for portrait touch`);
      assert.ok(button.height >= (allowCompactPortrait ? 32 : 56), `${button.name} is too short for portrait touch`);
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

const narrowPortraitButtons = portraitButtonRects(portraitLayout({ width: 360, height: 640 }));
assert.ok(
  narrowPortraitButtons.every((button) => button.width >= 83 && button.height >= 56),
  '360px portrait controls should remain substantially larger than the old grid',
);
assert.ok(narrowPortraitButtons.find((button) => button.name === 'up').height >= 72, '360px direction buttons should be visibly taller');
assert.ok(narrowPortraitButtons.find((button) => button.name === 'attack').height >= 80, '360px action buttons should be visibly taller');

function assertPortraitControlZones(layout) {
  const buttons = Object.fromEntries(portraitButtonRects(layout).map((button) => [button.name, button]));
  const actionRailX = layout.controlPanel.x + 3 * (layout.column + GAP);
  ['up-left', 'up', 'up-right', 'left', 'step', 'right', 'down-left', 'down', 'down-right'].forEach((id) => {
    assert.ok(buttons[id].right <= actionRailX - GAP + 0.001, `${id} should stay left of the portrait action rail`);
  });
  ['attack', 'dash', 'smartdash', 'shot'].forEach((id) => {
    assert.equal(buttons[id].x, actionRailX, `${id} should stay in the right-side portrait action rail`);
    assert.equal(buttons[id].width, layout.column, `${id} should occupy exactly one full-size action column`);
  });
  assert.equal(buttons.attack.y, portraitButtonGrid(layout, 'expected', 4, 2).y, 'attack should align with the upper direction row');
  assert.equal(buttons.dash.y, portraitButtonGrid(layout, 'expected', 4, 3).y, 'dash should align with the middle direction row');
  assert.equal(buttons.smartdash.y, portraitButtonGrid(layout, 'expected', 4, 4).y, 'smartdash should align with the lower direction row');
}

assertPortraitControlZones(portraitLayout({ width: 320, height: 568 }));
assertPortraitControlZones(portraitLayout({ width: 360, height: 640 }));

const toolbarReducedLayout = portraitLayout({
  width: 360,
  height: 640,
  visualViewport: { x: 0, y: 0, width: 360, height: 520 },
});
assertLayout(toolbarReducedLayout, 'portrait');
assert.equal(portraitButtonRects(toolbarReducedLayout).length, 17, 'all portrait controls should remain visible with browser chrome');

const offsetVisualViewportLayout = portraitLayout({
  width: 360,
  height: 640,
  visualViewport: { x: 12, y: 32, width: 336, height: 520 },
});
assertLayout(offsetVisualViewportLayout, 'portrait');

const notchedReducedLayout = portraitLayout({
  width: 390,
  height: 640,
  safeTop: 47,
  safeBottom: 34,
  visualViewport: { x: 0, y: 0, width: 390, height: 520 },
});
assertLayout(notchedReducedLayout, 'portrait', { allowCompactPortrait: true });
assert.ok(!overlaps(notchedReducedLayout.pageControls, notchedReducedLayout.canvas), 'notched page controls should stay above the canvas');

const veryShortVisibleLayout = portraitLayout({
  width: 360,
  height: 640,
  visualViewport: { x: 0, y: 0, width: 360, height: 320 },
});
const veryShortButtons = portraitButtonRects(veryShortVisibleLayout);
assertLayout(veryShortVisibleLayout, 'portrait', { allowCompactPortrait: true });
assert.ok(veryShortVisibleLayout.canvas.height >= 64, 'very short portrait should retain a useful game canvas');
assert.ok(veryShortVisibleLayout.mainRow < MIN_PORTRAIT_MAIN_ROW, 'rows should shrink only when the visible viewport cannot fit the targets');

const minimumSupportedLayout = portraitLayout({
  width: 320,
  height: 640,
  safeTop: 47,
  safeBottom: 34,
  visualViewport: { x: 0, y: 0, width: 320, height: MIN_SUPPORTED_VISUAL_HEIGHT },
});
assertLayout(minimumSupportedLayout, 'portrait', { allowCompactPortrait: true });
assert.ok(minimumSupportedLayout.canvas.height >= 80, 'minimum supported visual height should retain the compact canvas floor');
assert.equal(minimumSupportedLayout.utilityRow, 32, 'minimum supported visual height should use the 32px utility floor');
assert.equal(minimumSupportedLayout.mainRow, 32, 'minimum supported visual height should use the 32px main floor');
assert.equal(minimumSupportedLayout.actionExtra, 5, 'minimum supported visual height should allocate a 5px action extra');
assert.equal(minimumSupportedLayout.actionRow, 37, 'minimum supported visual height should produce a 37px action row');
const compactBigButtonContentHeight = 2 * 15 * 1.05 + 2 + 2;
assert.ok(
  compactBigButtonContentHeight <= minimumSupportedLayout.actionRow,
  'compact two-line action labels should fit inside the 37px action row',
);

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
