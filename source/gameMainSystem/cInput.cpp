
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

			function clearInputKeys() {
				Module['ggnKeys'] = {};
				Module['ggnTouchKeyCounts'] = {};
				var activeButtons = document.querySelectorAll('#ggn-touch-controls button.ggn-active');
				for (var i = 0; i < activeButtons.length; ++i) activeButtons[i].classList.remove('ggn-active');
			}

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
					'body.ggn-mobile-ready { overflow: hidden; touch-action: none; }',
					'#emscripten_logo, #spinner, #status, #progress, #controls, #output { display: none !important; }',
					'div.emscripten_border { border: 0 !important; width: 100vw; height: 100vh; height: 100dvh; display: flex; align-items: center; justify-content: center; background: #000; }',
					'canvas.emscripten { width: min(100vw, calc(100vh * 1.333333)); width: min(100vw, calc(100dvh * 1.333333)); height: min(100vh, calc(100vw * 0.75)); height: min(100dvh, calc(100vw * 0.75)); image-rendering: pixelated; image-rendering: crisp-edges; }',
					'#ggn-touch-controls { --ggn-cell: 22px; --ggn-gap: 5px; display: none; position: fixed; inset: auto 0 0 0; height: min(46vh, 300px); z-index: 20; pointer-events: none; user-select: none; -webkit-user-select: none; touch-action: none; }',
					'#ggn-touch-controls .ggn-pad { position: absolute; bottom: max(12px, env(safe-area-inset-bottom)); display: grid; grid-template-columns: repeat(6, var(--ggn-cell)); grid-template-rows: repeat(10, var(--ggn-cell)); gap: var(--ggn-gap); pointer-events: none; }',
					'#ggn-touch-controls .ggn-left { left: max(12px, env(safe-area-inset-left)); }',
					'#ggn-touch-controls .ggn-right { right: max(12px, env(safe-area-inset-right)); }',
					'#ggn-touch-controls button { pointer-events: auto; border: 1px solid rgba(255,255,255,.65); border-radius: 8px; background: rgba(10,10,10,.62); color: #fff; font: 700 12px/1 Arial, sans-serif; padding: 0; min-width: 0; min-height: 0; touch-action: none; -webkit-tap-highlight-color: transparent; }',
					'#ggn-touch-controls button.ggn-wide { font-size: 12px; }',
					'#ggn-touch-controls button.ggn-big { font-size: 20px; }',
					'#ggn-touch-controls button.ggn-active { background: rgba(255,255,255,.86); color: #000; }',
					'#ggn-btn-map { grid-column: 1 / span 6; grid-row: 1 / span 2; }',
					'#ggn-btn-up-left { grid-column: 1 / span 2; grid-row: 3 / span 2; } #ggn-btn-up { grid-column: 3 / span 2; grid-row: 3 / span 2; } #ggn-btn-up-right { grid-column: 5 / span 2; grid-row: 3 / span 2; }',
					'#ggn-btn-left { grid-column: 1 / span 2; grid-row: 5 / span 2; } #ggn-btn-right { grid-column: 5 / span 2; grid-row: 5 / span 2; }',
					'#ggn-btn-down-left { grid-column: 1 / span 2; grid-row: 7 / span 2; } #ggn-btn-down { grid-column: 3 / span 2; grid-row: 7 / span 2; } #ggn-btn-down-right { grid-column: 5 / span 2; grid-row: 7 / span 2; }',
					'#ggn-btn-menu { grid-column: 1 / span 6; grid-row: 1 / span 2; } #ggn-btn-smartdash { grid-column: 1 / span 6; grid-row: 8 / span 2; }',
					'#ggn-btn-turn { grid-column: 1 / span 2; grid-row: 3 / span 2; } #ggn-btn-diag { grid-column: 3 / span 2; grid-row: 3 / span 2; } #ggn-btn-shot { grid-column: 5 / span 2; grid-row: 3 / span 2; }',
					'#ggn-btn-attack { grid-column: 1 / span 3; grid-row: 5 / span 3; } #ggn-btn-dash { grid-column: 4 / span 3; grid-row: 5 / span 3; }',
					'@media (pointer: coarse), (max-width: 900px) { #ggn-touch-controls { display: block; } }',
					'@media (max-width: 560px) { #ggn-touch-controls { --ggn-cell: 19px; --ggn-gap: 5px; height: min(47vh, 286px); } #ggn-touch-controls button { font-size: 11px; } #ggn-touch-controls button.ggn-wide { font-size: 11px; } #ggn-touch-controls button.ggn-big { font-size: 18px; } }'
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
					{ parent: left, id: 'ggn-btn-map', key: 32, label: 'マップ', name: 'マップ表示', wide: true },
					{ parent: left, id: 'ggn-btn-up-left', keys: [38, 37], label: 'UL', name: 'Up left' },
					{ parent: left, id: 'ggn-btn-up', key: 38, label: '^', name: 'Up' },
					{ parent: left, id: 'ggn-btn-up-right', keys: [38, 39], label: 'UR', name: 'Up right' },
					{ parent: left, id: 'ggn-btn-left', key: 37, label: '<', name: 'Left' },
					{ parent: left, id: 'ggn-btn-right', key: 39, label: '>', name: 'Right' },
					{ parent: left, id: 'ggn-btn-down-left', keys: [40, 37], label: 'DL', name: 'Down left' },
					{ parent: left, id: 'ggn-btn-down', key: 40, label: 'v', name: 'Down' },
					{ parent: left, id: 'ggn-btn-down-right', keys: [40, 39], label: 'DR', name: 'Down right' },
					{ parent: right, id: 'ggn-btn-menu', key: 86, label: 'メニュー', name: 'メニュー', wide: true },
					{ parent: right, id: 'ggn-btn-turn', key: 67, label: '方向', name: '方向転換', wide: true },
					{ parent: right, id: 'ggn-btn-diag', key: 16, label: '斜め', name: '斜め固定', wide: true },
					{ parent: right, id: 'ggn-btn-shot', key: 83, label: '弾幕', name: '装備弾幕を撃つ', wide: true },
					{ parent: right, id: 'ggn-btn-attack', key: 90, label: 'Z', name: '攻撃・素振り', big: true },
					{ parent: right, id: 'ggn-btn-dash', key: 88, label: 'X', name: 'ダッシュ', big: true },
					{ parent: right, id: 'ggn-btn-smartdash', key: 68, label: 'スマート', name: 'スマートダッシュ', wide: true }
				];
				function specKeys(spec) {
					return spec.keys || [spec.key];
				}

				function pressButton(button, spec) {
					specKeys(spec).forEach(function(key) {
						setTouchKey(key, true);
					});
					button.classList.add('ggn-active');
				}

				function releaseButton(button, spec) {
					if (!button.classList.contains('ggn-active')) return;
					specKeys(spec).forEach(function(key) {
						setTouchKey(key, false);
					});
					button.classList.remove('ggn-active');
				}

				specs.forEach(function(spec) {
					var button = document.createElement('button');
					button.type = 'button';
					button.id = spec.id;
					button.textContent = spec.label;
					button.setAttribute('aria-label', spec.name);
					var classNames = [];
					if (spec.wide) classNames.push('ggn-wide');
					if (spec.big) classNames.push('ggn-big');
					if (classNames.length) button.className = classNames.join(' ');
					button.addEventListener('pointerdown', function(e) {
						e.preventDefault();
						try { button.setPointerCapture(e.pointerId); } catch (ignore) {}
						pressButton(button, spec);
					}, { passive: false });
					button.addEventListener('pointerup', function(e) {
						e.preventDefault();
						releaseButton(button, spec);
					}, { passive: false });
					button.addEventListener('pointercancel', function(e) {
						e.preventDefault();
						releaseButton(button, spec);
					}, { passive: false });
					button.addEventListener('lostpointercapture', function() {
						releaseButton(button, spec);
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

	void BrowserReadInputState(cInputState& state)
	{
		state = cInputState();
		state.attack = BrowserKeyDown(90);
		state.dash = BrowserKeyDown(88);
		state.turn = BrowserKeyDown(67);
		state.menu = BrowserKeyDown(86) | BrowserKeyDown(65) | BrowserKeyDown(13);
		state.diagon = BrowserKeyDown(16);
		state.shot = BrowserKeyDown(83);
		state.miniMap = BrowserKeyDown(32);
		state.smartdash = BrowserKeyDown(68);
		state.ue = BrowserKeyDown(38) | BrowserKeyDown(73);
		state.shita = BrowserKeyDown(40) | BrowserKeyDown(75);
		state.hidari = BrowserKeyDown(37) | BrowserKeyDown(74);
		state.migi = BrowserKeyDown(39) | BrowserKeyDown(76);
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
	BrowserReadInputState(platformInputState_);
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
	return 0;
#else
	if(MAX_PLAYERS <= player || MAX_BUTTONS <= buttom) return 0;
	return patInputManager->getrawPadInput(player,buttom);
#endif
}
