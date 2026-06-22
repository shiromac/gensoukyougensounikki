const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');

const root = path.resolve(__dirname, '..');
const source = fs.readFileSync(path.join(root, 'source', 'gameMainSystem', 'cInput.cpp'), 'utf8');

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

const GAP = 4;
const EDGE = 8;
const SIDE_GAP = 8;
const MIN_LANDSCAPE_CELL = 18;
const MAX_CELL = 28;

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
  return {
    cell,
    viewport: rect('viewport', 0, 0, width, height),
    canvas: rect('canvas', contentLeft + (contentWidth - canvasWidth) / 2, (height - canvasHeight) / 2, canvasWidth, canvasHeight),
    leftPad: rect('leftPad', leftX, padY, padW, panel),
    rightPad: rect('rightPad', rightX, padY, padW, panel),
    contentGap: SIDE_GAP,
  };
}

function assertLayout(layout, orientation) {
  assert.ok(layout.cell >= 0, 'cell size should be non-negative');
  assertInside(layout.viewport, layout.canvas);
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
    assert.ok(button.height >= 40 || orientation === 'portrait', `${button.name} is too short for landscape touch`);
  });

  if (orientation === 'landscape') {
    assert.ok(layout.leftPad.right + layout.contentGap <= layout.canvas.x + 0.001, 'left pad overlaps canvas');
    assert.ok(layout.canvas.right + layout.contentGap <= layout.rightPad.x + 0.001, 'right pad overlaps canvas');
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
