const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');

const root = path.resolve(__dirname, '..');
const buildScript = fs.readFileSync(path.join(root, 'tools', 'build-web.ps1'), 'utf8');
const lifecycleMatch = buildScript.match(/\$audioLifecycleScript = '(<script id="ggn-audio-lifecycle">[\s\S]*?<\/script>)'/);
assert.ok(lifecycleMatch, 'the generated page should install the audio lifecycle controller');
const lifecycleCode = lifecycleMatch[1].replace(/^<script[^>]*>/, '').replace(/<\/script>$/, '');

function eventTarget() {
  const handlers = new Map();
  return {
    addEventListener(type, handler) {
      if (!handlers.has(type)) handlers.set(type, []);
      handlers.get(type).push(handler);
    },
    dispatch(type) {
      (handlers.get(type) || []).forEach((handler) => handler({ type }));
    },
  };
}

class MockAudioContext {
  constructor() {
    this.state = 'running';
    this.suspendCalls = 0;
    this.resumeCalls = 0;
  }

  suspend() {
    this.suspendCalls += 1;
    this.state = 'suspended';
    return Promise.resolve();
  }

  resume() {
    this.resumeCalls += 1;
    this.state = 'running';
    return Promise.resolve();
  }
}

async function settle(window) {
  await window.ggnAudioLifecycle.transition;
  await Promise.resolve();
}

async function main() {
  const windowTarget = eventTarget();
  const documentTarget = eventTarget();
  const mockWindow = { ...windowTarget };
  const mockDocument = { ...documentTarget, hidden: false };
  const context = new MockAudioContext();
  const module = {
    ggnAudio: {
      context,
      masterGain: { gain: { value: 1 } },
      bgmSources: [{}],
    },
  };

  new Function('window', 'document', 'Module', 'console', lifecycleCode)(
    mockWindow,
    mockDocument,
    module,
    console,
  );

  mockDocument.hidden = true;
  mockDocument.dispatch('visibilitychange');
  assert.equal(module.ggnAudio.masterGain.gain.value, 0, 'backgrounding should mute immediately');
  await settle(mockWindow);
  assert.equal(context.state, 'suspended', 'backgrounding should suspend Web Audio');
  assert.equal(context.suspendCalls, 1);

  mockWindow.dispatch('pagehide');
  await settle(mockWindow);
  assert.equal(context.suspendCalls, 1, 'duplicate background events should not suspend twice');

  mockDocument.hidden = false;
  mockDocument.dispatch('visibilitychange');
  await settle(mockWindow);
  assert.equal(context.state, 'running', 'returning to the page should resume active BGM');
  assert.equal(module.ggnAudio.masterGain.gain.value, 1, 'returning should restore output');

  mockWindow.dispatch('pagehide');
  await settle(mockWindow);
  assert.equal(context.state, 'suspended', 'pagehide should suspend before document.hidden changes');
  mockWindow.dispatch('pageshow');
  await settle(mockWindow);
  assert.equal(context.state, 'running', 'pageshow should resume after pagehide');

  mockDocument.dispatch('freeze');
  await settle(mockWindow);
  assert.equal(context.state, 'suspended', 'page freeze should suspend Web Audio');
  mockWindow.dispatch('pageshow');
  await settle(mockWindow);
  assert.equal(context.state, 'running', 'pageshow should resume after a freeze');

  context.state = 'suspended';
  module.ggnAudio.bgmSources = [];
  mockWindow.ggnAudioLifecycle.resumeOnShow = false;
  const resumeCallsBefore = context.resumeCalls;
  mockDocument.hidden = true;
  mockDocument.dispatch('visibilitychange');
  await settle(mockWindow);
  mockDocument.hidden = false;
  mockDocument.dispatch('visibilitychange');
  await settle(mockWindow);
  assert.equal(context.state, 'suspended', 'an autoplay-suspended context without BGM should remain suspended');
  assert.equal(context.resumeCalls, resumeCallsBefore, 'lifecycle resume should not bypass autoplay restrictions');

  module.ggnAudio.bgmSources = [{}];
  mockDocument.hidden = true;
  mockDocument.dispatch('visibilitychange');
  await settle(mockWindow);
  mockDocument.hidden = false;
  mockDocument.dispatch('visibilitychange');
  await settle(mockWindow);
  assert.equal(context.state, 'running', 'active BGM should resume even if the browser suspended first');

  const generatedHtml = ['ggn.html', 'index.html'].map((name) => ({
    name,
    content: fs.readFileSync(path.join(root, 'docs', 'play', name), 'utf8'),
  }));
  generatedHtml.forEach(({ name, content }) => {
    assert.equal((content.match(/id="?ggn-audio-lifecycle"?/g) || []).length, 1, `${name} should install one audio lifecycle controller`);
    assert.ok(content.indexOf('ggn-audio-lifecycle') < content.indexOf('ggn.js'), `${name} should install lifecycle handlers before the game runtime`);
    assert.ok(content.includes('addEventListener("visibilitychange"'), `${name} should handle background visibility`);
    assert.ok(content.includes('addEventListener("pagehide"'), `${name} should handle mobile page suspension`);
    assert.ok(content.includes('addEventListener("freeze"'), `${name} should handle page freezing`);
  });

  console.log('web audio lifecycle tests passed');
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
