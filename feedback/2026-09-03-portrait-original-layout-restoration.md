# Portrait original-layout restoration regression

Status: RESOLVED

## Challenge

Restore the author's original left/right touch-pad design. The attempted restoration in `92bbbe4` moved turn (振り向き) and diagonal-lock (斜め固定) to the left and retained a redesigned four-column portrait layout.

## Evidence and authority

- User: 「元に戻っていません。斜めと振り向きも右側にないといけません」 and 「元の洗練された配置に戻してください。私がデザインしたものです」.
- Original refinement: `abe54d2`; last pre-reflow baseline: `0cfa159`; original geometry remains in `cInput.cpp`.
- The `92bbbe4` design narrowed restoration to four combat controls. Its tests validated that narrower design, not the author's original placement.

## Notes

- Challenge counter: one failed restoration attempt (`92bbbe4`), classified as a regression.
- Previous verdict: Layer 2 design; escalated verdict: Layer 3 specification narrowing.
- Next escalation level if this restoration fails: requirements/authority review; no further ad-hoc placement patch.
- Selected response: remove portrait reflow; reuse original runtime parent/row/column/span/presentation authority, with whole-pad viewport scaling only. Design review rejected cell-only shrinking because it changes text-to-button ratios.

## Verification (2026-09-03)

- Original restoration reproduction: the generator contained per-ID turn/diag overrides before the fix; the same assertion passes after removing portrait reflow.
- `node tools/test-web-touch-layout.js`: passed for all 17 authored controls and both orientations, including projection/fallback/safe-area cases and override mutation checks.
- `node tools/test-web-audio-lifecycle.js`: passed. PowerShell parser and `git diff --check`: passed.
- Real browser, production HTML: 320x568 and 360x640 preserve original placement, all 17 controls fit, and no label overflow was observed. At 360px, turn/diag start at x=185/242; attack/dash are 81.5px squares.
- Reduced visible-height fixture: widths 320/360/390/412, layout height >=640, visual height 400, safe top/bottom 47/34. Native DOM Range measurements found zero escaped labels or overlapping controls; every pad ends at y=366, canvas spans y=83..163, and the fit-guarantee flag is true. The fixture substitutes CSS safe-area values and VisualViewport geometry; it is not physical-device evidence.
- Native PointerEvents through the original handlers: left direction plus turn, diagonal-lock, and attack all remained held together, released independently, and cleared completely at all four compact widths.
- Production landscape 844x390: original separated pads remain; hiding controls expands the canvas from 436x327 to approximately 520x390. Fullscreen UI entered/exited and the normal portrait layout returned; automated fullscreen/hidden-control checks passed.
- Temporary browser diagnostic HTML was removed before publication. `cInput.cpp`, compiled JS, and WASM remain identical to the pre-reflow baseline.
- Fresh team implementation review: layout/spec/naming, architecture/security/runtime, and test/documentation reviewers each returned zero findings; no unresolved implementation item remains.
