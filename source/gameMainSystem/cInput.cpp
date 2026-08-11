
#include "../stdafx.h"
#ifndef __EMSCRIPTEN__
#include "Wiicon/CWiimoteManager.h"
#include "Wiicon/CWiimote.h"
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>

namespace
{
	void BrowserInstallInputHandlers()
	{
		EM_ASM((function() {
			if (typeof window === 'undefined') return;
			if (Module['ggnInputInstalled']) return;
			Module['ggnInputInstalled'] = true;
			Module['ggnKeys'] = Module['ggnKeys'] || {};
			Module['ggnPadButtons'] = Module['ggnPadButtons'] || {};
			Module['ggnPadConfig'] = Module['ggnPadConfig'] || [0, 1, 2, 3, 4, 5, 6, 7];
			Module['ggnGamepadDeadzone'] = 0.45;
			Module['ggnGetGamepads'] = function() {
				if (typeof Module['ggnGamepadProvider'] === 'function') {
					return Module['ggnGamepadProvider']() || [];
				}
				if (typeof navigator === 'undefined' || typeof navigator.getGamepads !== 'function') return [];
				try { return navigator.getGamepads() || []; } catch (ignore) { return []; }
			};
			Module['ggnGamepadButtonDown'] = function(buttonIndex) {
				var index = buttonIndex | 0;
				if (index < 0) return 0;
				var pads = Module['ggnGetGamepads']();
				for (var i = 0; i < pads.length; ++i) {
					var pad = pads[i];
					if (!pad || pad.connected === false || !pad.buttons || index >= pad.buttons.length) continue;
					var button = pad.buttons[index];
					if (typeof button === 'number' ? button > 0.5 : button && (button.pressed || button.value > 0.5)) return 1;
				}
				return 0;
			};
			Module['ggnGamepadDirectionDown'] = function(buttonIndex, axisIndex, axisSign) {
				if (Module['ggnGamepadButtonDown'](buttonIndex)) return 1;
				var pads = Module['ggnGetGamepads']();
				var deadzone = Module['ggnGamepadDeadzone'];
				for (var i = 0; i < pads.length; ++i) {
					var pad = pads[i];
					if (!pad || pad.connected === false || !pad.axes || axisIndex < 0 || axisIndex >= pad.axes.length) continue;
					var value = Number(pad.axes[axisIndex]);
					if (!isFinite(value)) continue;
					if (axisSign < 0 ? value < -deadzone : value > deadzone) return 1;
				}
				return 0;
			};
			Module['ggnShouldBlockKey'] = function(keyCode) {
				switch (keyCode | 0) {
				case 13: case 16: case 32:
				case 37: case 38: case 39: case 40:
				case 65: case 67: case 68: case 73:
				case 74: case 75: case 76: case 83:
				case 86: case 88: case 90:
					return true;
				default:
					return false;
				}
			};

			function setKey(keyCode, pressed) {
				Module['ggnKeys'][keyCode | 0] = pressed ? 1 : 0;
			}

			function setTouchKey(keyCode, pressed) {
				var code = keyCode | 0;
				Module['ggnTouchKeyCounts'] = Module['ggnTouchKeyCounts'] || {};
				var count = Module['ggnTouchKeyCounts'][code] | 0;
				count += pressed ? 1 : -1;
				if (count < 0) count = 0;
				Module['ggnTouchKeyCounts'][code] = count;
				setKey(code, count > 0);
			}

			function setTouchButton(buttonIndex, pressed) {
				var button = buttonIndex | 0;
				Module['ggnTouchButtonCounts'] = Module['ggnTouchButtonCounts'] || {};
				var count = Module['ggnTouchButtonCounts'][button] | 0;
				count += pressed ? 1 : -1;
				if (count < 0) count = 0;
				Module['ggnTouchButtonCounts'][button] = count;
				Module['ggnPadButtons'][button] = count > 0 ? 1 : 0;
			}

			function configuredActionButton(actionIndex) {
				var action = actionIndex | 0;
				var config = Module['ggnPadConfig'] || [];
				var button = config[action] | 0;
				return button >= 0 && button < 32 ? button : action;
			}
			function clearInputKeys() {
				Module['ggnKeys'] = {};
				Module['ggnTouchKeyCounts'] = {};
				Module['ggnPadButtons'] = {};
				Module['ggnTouchButtonCounts'] = {};
				var activeButtons = document.querySelectorAll('#ggn-touch-controls button.ggn-active');
				for (var i = 0; i < activeButtons.length; ++i) { activeButtons[i].classList.remove('ggn-active'); activeButtons[i].setAttribute('aria-pressed', 'false'); }
			}


			Module['ggnSetPadConfig'] = function(config) {
				var next = [];
				for (var i = 0; i < 8; ++i) {
					var button = config && config.length > i ? config[i] | 0 : i;
					next[i] = button >= 0 && button < 32 ? button : i;
				}
				var current = Module['ggnPadConfig'] || [];
				if (current.join(',') === next.join(',')) return;
				clearInputKeys();
				Module['ggnPadConfig'] = next;
			};
			window.addEventListener('keydown', function(e) {
				setKey(e.keyCode | 0, true);
				if (Module['ggnShouldBlockKey'](e.keyCode | 0)) e.preventDefault();
			}, false);
			window.addEventListener('keyup', function(e) {
				setKey(e.keyCode | 0, false);
				if (Module['ggnShouldBlockKey'](e.keyCode | 0)) e.preventDefault();
			}, false);
			window.addEventListener('blur', function() {
				clearInputKeys();
			}, false);
			window.addEventListener('contextmenu', function(e) {
				e.preventDefault();
			}, false);

			function installMobileViewport() {
				if (!document.querySelector('meta[name="viewport"]')) {
					var viewport = document.createElement('meta');
					viewport.name = 'viewport';
					viewport.content = 'width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no, viewport-fit=cover';
					document.head.appendChild(viewport);
				}
			}

			function installMobileStyles() {
				if (document.getElementById('ggn-mobile-style')) return;
				var style = document.createElement('style');
				style.id = 'ggn-mobile-style';
				style.textContent = [
					'html, body { margin: 0; background: #050505; overscroll-behavior: none; }',
					'body.ggn-mobile-ready { overflow: hidden; touch-action: none; --ggn-edge: 8px; --ggn-gap: 4px; --ggn-cell: min(28px, calc((100vw - 66px) / 12)); --ggn-pad-width: calc(var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap)); --ggn-panel-height: calc(var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-cell) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap) + var(--ggn-gap)); --ggn-controls-height: calc(var(--ggn-panel-height) + max(12px, env(safe-area-inset-bottom))); }',
					'#emscripten_logo, #spinner, #status, #progress, #controls, #output { display: none !important; }',
					'body.ggn-mobile-ready #ggn-page-links { position: fixed; top: max(4px, env(safe-area-inset-top)); right: max(8px, env(safe-area-inset-right)); width: auto; margin: 0; z-index: 30; }',
					'div.emscripten_border { border: 0 !important; width: 100vw; height: 100vh; height: 100dvh; display: flex; align-items: center; justify-content: center; background: #000; }',
					'canvas.emscripten { width: min(100vw, calc(100vh * 1.333333)); width: min(100vw, calc(100dvh * 1.333333)); height: min(100vh, calc(100vw * 0.75)); height: min(100dvh, calc(100vw * 0.75)); image-rendering: pixelated; image-rendering: crisp-edges; }',
					'#ggn-touch-controls { display: none; position: fixed; inset: auto 0 0 0; height: var(--ggn-controls-height); z-index: 20; pointer-events: none; user-select: none; -webkit-user-select: none; touch-action: none; }',
					'#ggn-touch-controls .ggn-pad { position: absolute; bottom: max(12px, env(safe-area-inset-bottom)); display: grid; grid-template-columns: repeat(6, var(--ggn-cell)); grid-template-rows: repeat(10, var(--ggn-cell)); gap: var(--ggn-gap); pointer-events: none; }',
					'#ggn-touch-controls .ggn-left { left: max(var(--ggn-edge), env(safe-area-inset-left)); }',
					'#ggn-touch-controls .ggn-right { right: max(var(--ggn-edge), env(safe-area-inset-right)); }',
					'#ggn-touch-controls button { pointer-events: auto; border: 1px solid rgba(255,255,255,.65); border-radius: 8px; background: rgba(10,10,10,.62); color: #fff; font: 700 clamp(12px, 3.4vw, 14px)/1.05 Arial, sans-serif; padding: 0; min-width: 0; min-height: 0; display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 2px; touch-action: none; -webkit-tap-highlight-color: transparent; }',
					'#ggn-touch-controls button.ggn-wide { font-size: clamp(13px, 3.7vw, 15px); }',
					'#ggn-touch-controls button.ggn-big { font-size: clamp(19px, 5.6vw, 23px); }',
					'#ggn-touch-controls button .ggn-caption-key { font-size: .88em; }',
					'#ggn-touch-controls button.ggn-big .ggn-caption-action { font-size: .9em; }',
					'#ggn-touch-controls button.ggn-big .ggn-caption-key { font-size: 1.02em; }',
					'#ggn-touch-controls button.ggn-active { background: rgba(255,255,255,.86); color: #000; }',
					'#ggn-btn-map { grid-column: 1 / span 6; grid-row: 1 / span 2; }',
					'#ggn-btn-up-left { grid-column: 1 / span 2; grid-row: 3 / span 2; } #ggn-btn-up { grid-column: 3 / span 2; grid-row: 3 / span 2; } #ggn-btn-up-right { grid-column: 5 / span 2; grid-row: 3 / span 2; }',
					'#ggn-btn-left { grid-column: 1 / span 2; grid-row: 5 / span 2; } #ggn-btn-right { grid-column: 5 / span 2; grid-row: 5 / span 2; }',
					'#ggn-btn-down-left { grid-column: 1 / span 2; grid-row: 7 / span 2; } #ggn-btn-down { grid-column: 3 / span 2; grid-row: 7 / span 2; } #ggn-btn-down-right { grid-column: 5 / span 2; grid-row: 7 / span 2; }',
					'#ggn-btn-step { grid-column: 1 / span 2; grid-row: 10 / span 1; }',
					'#ggn-btn-menu { grid-column: 1 / span 6; grid-row: 1 / span 2; } #ggn-btn-smartdash { grid-column: 1 / span 6; grid-row: 8 / span 2; }',
					'#ggn-btn-turn { grid-column: 1 / span 2; grid-row: 3 / span 2; } #ggn-btn-diag { grid-column: 3 / span 2; grid-row: 3 / span 2; } #ggn-btn-shot { grid-column: 5 / span 2; grid-row: 3 / span 2; }',
					'#ggn-btn-attack { grid-column: 1 / span 3; grid-row: 5 / span 3; } #ggn-btn-dash { grid-column: 4 / span 3; grid-row: 5 / span 3; }',
					'@media (pointer: coarse), (max-width: 900px) { body.ggn-mobile-ready div.emscripten_border { height: calc(100dvh - var(--ggn-controls-height) - 12px); align-items: flex-end; } #ggn-touch-controls { display: block; } }',
					'@media (pointer: coarse) and (orientation: landscape), (max-width: 900px) and (orientation: landscape) { body.ggn-mobile-ready { --ggn-cell: max(18px, min(28px, calc((100dvh - 52px) / 10), calc((100vw - 420px) / 12))); --ggn-controls-height: 0px; } body.ggn-mobile-ready #ggn-page-links { top: calc(50% + (var(--ggn-panel-height) / 2) - var(--ggn-cell)); right: max(var(--ggn-edge), env(safe-area-inset-right)); left: auto; width: var(--ggn-pad-width); padding: 0; text-align: center; transform: none; font-size: 10px; line-height: 1.1; } body.ggn-mobile-ready #ggn-page-links a { padding: 1px 6px; } body.ggn-mobile-ready div.emscripten_border { width: 100vw; height: 100dvh; box-sizing: border-box; padding-left: calc(var(--ggn-pad-width) + max(var(--ggn-edge), env(safe-area-inset-left)) + 8px); padding-right: calc(var(--ggn-pad-width) + max(var(--ggn-edge), env(safe-area-inset-right)) + 8px); align-items: center; justify-content: center; } body.ggn-mobile-ready canvas.emscripten { width: min(100%, calc(100dvh * 1.333333)); height: auto; max-height: 100dvh; aspect-ratio: 4 / 3; } #ggn-touch-controls { inset: 0; height: auto; } #ggn-touch-controls .ggn-pad { bottom: auto; top: 50%; transform: translateY(-50%); } }',
					'@media (max-width: 340px) { #ggn-touch-controls button.ggn-wide { font-size: 12px; } }'
				].join(String.fromCharCode(10));
				document.head.appendChild(style);
				document.body.classList.add('ggn-mobile-ready');
			}

			function installTouchControls() {
				if (document.getElementById('ggn-touch-controls')) return;
				var controls = document.createElement('div');
				controls.id = 'ggn-touch-controls';
				var left = document.createElement('div');
				left.className = 'ggn-pad ggn-left';
				var right = document.createElement('div');
				right.className = 'ggn-pad ggn-right';
				controls.appendChild(left);
				controls.appendChild(right);
				document.body.appendChild(controls);
				var specs = [
					{ parent: left, id: 'ggn-btn-map', action: 6, label: 'マップ', name: 'マップ表示', wide: true },
					{ parent: left, id: 'ggn-btn-up-left', keys: [38, 37], label: 'UL', name: 'Up left' },
					{ parent: left, id: 'ggn-btn-up', key: 38, label: '^', name: 'Up' },
					{ parent: left, id: 'ggn-btn-up-right', keys: [38, 39], label: 'UR', name: 'Up right' },
					{ parent: left, id: 'ggn-btn-left', key: 37, label: '<', name: 'Left' },
					{ parent: left, id: 'ggn-btn-right', key: 39, label: '>', name: 'Right' },
					{ parent: left, id: 'ggn-btn-down-left', keys: [40, 37], label: 'DL', name: 'Down left' },
					{ parent: left, id: 'ggn-btn-down', key: 40, label: 'v', name: 'Down' },
					{ parent: left, id: 'ggn-btn-down-right', keys: [40, 39], label: 'DR', name: 'Down right' },
					{ parent: left, id: 'ggn-btn-step', actions: [0, 1], label: '\u8db3\u8e0f', name: '\u8db3\u8e0f\u307f' },
					{ parent: right, id: 'ggn-btn-menu', action: 3, label: 'メニュー', name: 'メニュー', wide: true },
					{ parent: right, id: 'ggn-btn-turn', action: 2, label: '振向き C', caption: ['振向き', 'C'], name: '振向き・方向転換', wide: true },
					{ parent: right, id: 'ggn-btn-diag', action: 4, label: '\u659c\u3081', caption: ['\u659c\u3081', '\u4fbf\u5229'], name: '斜め固定', wide: true },
					{ parent: right, id: 'ggn-btn-shot', action: 5, label: '弾幕', name: '装備弾幕を撃つ', wide: true },
					{ parent: right, id: 'ggn-btn-attack', action: 0, label: '攻撃 Z', caption: ['攻撃', 'Z'], name: '攻撃・素振り', big: true },
					{ parent: right, id: 'ggn-btn-dash', action: 1, label: 'ダッシュ X', caption: ['ダッシュ', 'X'], name: 'ダッシュ', big: true },
					{ parent: right, id: 'ggn-btn-smartdash', action: 7, label: 'スマート', name: 'スマートダッシュ', wide: true, toggle: true }
				];
				function specKeys(spec) {
					return spec.keys || (typeof spec.key === 'number' ? [spec.key] : []);
				}

				function specActions(spec) {
					return spec.actions || (typeof spec.action === 'number' ? [spec.action] : []);
				}

				function setButtonActive(button, active) {
					button.classList.toggle('ggn-active', !!active);
					button.setAttribute('aria-pressed', active ? 'true' : 'false');
				}

				function pressButton(button, spec) {
					if (spec.toggle && button.classList.contains('ggn-active')) {
						releaseButton(button, spec);
						return;
					}
					if (button.classList.contains('ggn-active')) return;
					button._ggnPressedKeys = specKeys(spec);
					button._ggnPressedButtons = specActions(spec).map(configuredActionButton);
					button._ggnPressedKeys.forEach(function(key) {
						setTouchKey(key, true);
					});
					button._ggnPressedButtons.forEach(function(rawButton) {
						setTouchButton(rawButton, true);
					});
					setButtonActive(button, true);
				}

				function releaseButton(button, spec) {
					if (!button.classList.contains('ggn-active')) return;
					(button._ggnPressedKeys || []).forEach(function(key) {
						setTouchKey(key, false);
					});
					(button._ggnPressedButtons || []).forEach(function(rawButton) {
						setTouchButton(rawButton, false);
					});
					button._ggnPressedKeys = [];
					button._ggnPressedButtons = [];
					setButtonActive(button, false);
				}
				specs.forEach(function(spec) {
					var button = document.createElement('button');
					button.type = 'button';
					button.id = spec.id;
					if (spec.caption) {
						spec.caption.forEach(function(line, index) {
							var span = document.createElement('span');
							span.textContent = line;
							span.className = index ? 'ggn-caption-key' : 'ggn-caption-action';
							button.appendChild(span);
						});
					} else {
						button.textContent = spec.label;
					}
					button.setAttribute('aria-label', spec.name);
					button.setAttribute('aria-pressed', 'false');
					var classNames = [];
					if (spec.wide) classNames.push('ggn-wide');
					if (spec.big) classNames.push('ggn-big');
					if (spec.toggle) classNames.push('ggn-toggle');
					if (classNames.length) button.className = classNames.join(' ');
					button.addEventListener('pointerdown', function(e) {
						e.preventDefault();
						try { button.setPointerCapture(e.pointerId); } catch (ignore) {}
						pressButton(button, spec);
					}, { passive: false });
					button.addEventListener('pointerup', function(e) {
						e.preventDefault();
						if (!spec.toggle) releaseButton(button, spec);
					}, { passive: false });
					button.addEventListener('pointercancel', function(e) {
						e.preventDefault();
						if (!spec.toggle) releaseButton(button, spec);
					}, { passive: false });
					button.addEventListener('lostpointercapture', function() {
						if (!spec.toggle) releaseButton(button, spec);
					}, false);
					spec.parent.appendChild(button);
				});

				window.addEventListener('visibilitychange', function() {
					if (document.hidden) clearInputKeys();
				}, false);
			}

			installMobileViewport();
			installMobileStyles();
			installTouchControls();
		})());
	}
	int BrowserKeyDown(int keyCode)
	{
		return EM_ASM_INT({
			var keys = Module['ggnKeys'];
			return keys && keys[$0 | 0] ? 1 : 0;
		}, keyCode);
	}

	int BrowserPadButtonDown(int buttonIndex)
	{
		return EM_ASM_INT({
			var index = $0 | 0;
			var buttons = Module['ggnPadButtons'];
			if (buttons && buttons[index]) return 1;
			return Module['ggnGamepadButtonDown'] ? Module['ggnGamepadButtonDown'](index) : 0;
		}, buttonIndex);
	}
	int BrowserGamepadDirectionDown(int buttonIndex, int axisIndex, int axisSign)
	{
		return EM_ASM_INT({
			return Module['ggnGamepadDirectionDown'] ? Module['ggnGamepadDirectionDown']($0 | 0, $1 | 0, $2 | 0) : 0;
		}, buttonIndex, axisIndex, axisSign);
	}

	int BrowserConfiguredButtonDown(const std::vector<int>& padConfig, int actionIndex)
	{
		if(actionIndex < 0 || actionIndex >= (int)padConfig.size()) return 0;
		const int buttonIndex = padConfig[actionIndex];
		if(buttonIndex < 0 || buttonIndex >= MAX_BUTTONS) return 0;
		return BrowserPadButtonDown(buttonIndex);
	}

	void BrowserSyncPadConfig(const std::vector<int>& padConfig)
	{
		int config[8] = {0, 1, 2, 3, 4, 5, 6, 7};
		for(int i = 0; i < 8 && i < (int)padConfig.size(); ++i)
		{
			if(padConfig[i] >= 0 && padConfig[i] < MAX_BUTTONS) config[i] = padConfig[i];
		}
		EM_ASM({
			var config = [];
			config[0] = $0 | 0;
			config[1] = $1 | 0;
			config[2] = $2 | 0;
			config[3] = $3 | 0;
			config[4] = $4 | 0;
			config[5] = $5 | 0;
			config[6] = $6 | 0;
			config[7] = $7 | 0;
			if (Module['ggnSetPadConfig']) Module['ggnSetPadConfig'](config);
		}, config[0], config[1], config[2], config[3], config[4], config[5], config[6], config[7]);
	}
	void BrowserReadInputState(cInputState& state, const std::vector<int>& padConfig)
	{
		state = cInputState();
		state.attack = BrowserKeyDown(90) | BrowserConfiguredButtonDown(padConfig, 0);
		state.dash = BrowserKeyDown(88) | BrowserConfiguredButtonDown(padConfig, 1);
		state.turn = BrowserKeyDown(67) | BrowserConfiguredButtonDown(padConfig, 2);
		state.menu = BrowserKeyDown(86) | BrowserKeyDown(65) | BrowserKeyDown(13) | BrowserConfiguredButtonDown(padConfig, 3);
		state.diagon = BrowserKeyDown(16) | BrowserConfiguredButtonDown(padConfig, 4);
		state.shot = BrowserKeyDown(83) | BrowserConfiguredButtonDown(padConfig, 5);
		state.miniMap = BrowserKeyDown(32) | BrowserConfiguredButtonDown(padConfig, 6);
		state.smartdash = BrowserKeyDown(68) | BrowserConfiguredButtonDown(padConfig, 7);
		state.ue = BrowserKeyDown(38) | BrowserKeyDown(73) | BrowserGamepadDirectionDown(12, 1, -1);
		state.shita = BrowserKeyDown(40) | BrowserKeyDown(75) | BrowserGamepadDirectionDown(13, 1, 1);
		state.hidari = BrowserKeyDown(37) | BrowserKeyDown(74) | BrowserGamepadDirectionDown(14, 0, -1);
		state.migi = BrowserKeyDown(39) | BrowserKeyDown(76) | BrowserGamepadDirectionDown(15, 0, 1);
	}
}
#endif


cInput::cInput(void)
{
#ifdef __EMSCRIPTEN__
	patInputManager = new CPatInput();
#else
	patInputManager = NULL;
	WiiconInputManager = NULL;
#endif
}

cInput::~cInput(void)
{
#ifdef __EMSCRIPTEN__
	delete patInputManager;
	patInputManager = NULL;
#else
	RELEASE(WiiconInputManager);
	RELEASE(patInputManager);
#endif
}

//毎回呼ばれて情報更新
int cInput::UpdateInput()
{
#ifndef __EMSCRIPTEN__
	patInputManager->UpdateState();
	WiiconInputManager->UpdateInput();
#endif

	setPlayerInput();
	return 0;
}


//リフレッシュ
void cInput::ClearInput()
{
#ifndef __EMSCRIPTEN__
	WiiconInputManager->ClearInput();

	patInputManager->ClearState();
#else
	platformInputState_ = cInputState();
	applyPlayerInput(platformInputState_);
#endif
}

//初期化。始めに一度だけ呼ばれる。
int cInput::InitInput(HWND WindowHandle)
{
#ifdef __EMSCRIPTEN__
	NumOfWiicon = 0;
	EnableOfPat = 0;
	platformInputState_ = cInputState();
	BrowserInstallInputHandlers();
	return true;
#else
	NumOfWiicon = InitInput_WiiconDevice(WindowHandle);

	EnableOfPat = InitInput_PatAndKey(WindowHandle);




	return true;
#endif
}

#ifndef __EMSCRIPTEN__
int cInput::InitInput_PatAndKey(HWND WindowHandle)
{
	_TCHAR Temp[64] = _T("");

	patInputManager = new CPatInput(WindowHandle);

	patInputManager->UpdateState();

	wsprintf( Temp, _T("パッドを%dつ発見。\n"), patInputManager->JoyNum());
	OutputDebugString( Temp);

	if(patInputManager->JoyNum() == 0) return false;
	return true;
}

int cInput::InitInput_WiiconDevice(HWND WindowHandle)
{

	WiiconInputManager = new CWiiconInput(WindowHandle);

	
	//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH
	TCHAR Temp[64] = _T("");

	_stprintf( Temp, _T("Wiiコンを%dつ発見。\n"), WiiconInputManager->WiiconNum());
	OutputDebugString( Temp);

	return WiiconInputManager->WiiconNum();
#else
	return 0;
#endif
}

#endif

void cInput::readPlatformInput(cInputState& state)
{
#ifdef __EMSCRIPTEN__
	BrowserSyncPadConfig(patInputManager->padconfigI2B);
	BrowserReadInputState(platformInputState_, patInputManager->padconfigI2B);
	state = platformInputState_;
#else
	state.attack = patInputManager->GetState(0)->Button[0]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Two.on : 0);

	state.turn = patInputManager->GetState(0)->Button[2]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->One.on : 0);

	state.menu = patInputManager->GetState(0)->Button[3]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->A.on : 0);

	state.miniMap = patInputManager->GetState(0)->Button[6]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Minus.on : 0);

	state.diagon = patInputManager->GetState(0)->Button[4]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->B.on : 0);

	state.shot = patInputManager->GetState(0)->Button[5]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Plus.on : 0);

	state.ue = patInputManager->GetState(0)->Up
		| patInputManager->GetState(0)->Up2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Right.on : 0);

	state.shita = patInputManager->GetState(0)->Down
		| patInputManager->GetState(0)->Down2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Left.on : 0);

	state.hidari = patInputManager->GetState(0)->Left
		| patInputManager->GetState(0)->Left2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Up.on : 0);

	state.migi = patInputManager->GetState(0)->Right
		| patInputManager->GetState(0)->Right2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Down.on : 0);

	state.dash = patInputManager->GetState(0)->Button[1];
	state.smartdash = patInputManager->GetState(0)->Button[7];
#endif
}

int cInput::applyPlayerInput(const cInputState& state)
{
	PlayerInput.setattack().process(state.attack);
	PlayerInput.setturn().process(state.turn);
	PlayerInput.setmenu().process(state.menu);
	PlayerInput.setminiMap().process(state.miniMap);
	PlayerInput.setdiagon().process(state.diagon);
	PlayerInput.setshot().process(state.shot);

	PlayerInput.setue().process(state.ue);
	PlayerInput.setshita().process(state.shita);
	PlayerInput.sethidari().process(state.hidari);
	PlayerInput.setmigi().process(state.migi);

	PlayerInput.X = PlayerInput.migi().on - PlayerInput.hidari().on;
	PlayerInput.Y = PlayerInput.shita().on - PlayerInput.ue().on;

	if(PlayerInput.diagon().on)
	{
		if(abs(PlayerInput.X)+abs(PlayerInput.Y) != 2)
		{
			PlayerInput.X = 0;
			PlayerInput.Y = 0;
		}
	}

	PlayerInput.setdash().process(state.dash);
	PlayerInput.setsmartdash().process(state.smartdash);

	return true;
}

int cInput::setPlayerInput()
{
	cInputState state;
	readPlatformInput(state);
	return applyPlayerInput(state);
}

int cInput::setInputState(const cInputState& state)
{
#ifdef __EMSCRIPTEN__
	platformInputState_ = state;
	return applyPlayerInput(platformInputState_);
#else
	return applyPlayerInput(state);
#endif
}

int cInput::getrawPadInput(int player, int buttom)
{
#ifdef __EMSCRIPTEN__
	if(player != 0 || buttom < 0 || buttom >= MAX_BUTTONS) return 0;
	return BrowserPadButtonDown(buttom);
#else
	if(MAX_PLAYERS <= player || MAX_BUTTONS <= buttom) return 0;
	return patInputManager->getrawPadInput(player,buttom);
#endif
}
