# Notice

This file records copyright, attribution, and license notices currently known for the public branch.
It is intentionally conservative: items with unclear redistribution permission should be removed from the public branch instead of treated as licensed.

## Project notice

- Repository/project name: `gensoukyougensounikki` / GGN public archive. Released product name: `幻想郷幻想日記`.
- Current public distribution version: `1.0.5.2` (`2026-06-14`).
- The original source code and build/package scripts are licensed under the MIT License. Game assets, BGM/music, sound effects, Touhou Project content, third-party materials, external libraries, and packed runtime data are not covered by that MIT grant.
- This is an unofficial Touhou Project fan work. It is not official, not affiliated with Team Shanghai Alice, and not endorsed by Team Shanghai Alice or ZUN.
- Touhou Project characters, settings, and related intellectual property belong to Team Shanghai Alice / ZUN.
- The public branch is intended to exclude extracted official assets and private/build artifacts. If any such material is found, remove it before publishing.
- Historical documents and the released readme identify `@N-Factory`, `白mac`, and contributors as project creators/contributors.

Relevant Touhou Project references:

- Official Touhou Project fan content guidelines: https://touhou-project.news/guideline/
- Official Touhou Project fan content guidelines (English): https://touhou-project.news/guidelines_en/
- Team Shanghai Alice / ZUN site referenced by the original manual: http://www16.big.or.jp/~zun/

## Released package reference

The released binary used as an audit reference identifies as `幻想郷幻想日記` / `ggn.exe` with FileVersion/ProductVersion `1.0.4.0`. Its release notes list `1ST 1.0.4.0` dated `2014-12-21`.

The current public distribution version cut from this branch is `1.0.5.2`, dated `2026-06-14`. It is rebuilt from the `1.0.4.0` release baseline, with public-source licensing/build/package fixes applied and later in-development gameplay/data changes excluded.

The released runtime used packed data and audio files such as `data/Packed_*.id.cdat`, `graphic/graphicpack`, `sound/data1`, and `sound/data2`, plus the `Language` directory. This public branch intentionally excludes generated binaries, save data, logs, and large audio packs. The released readme also states that redistributing data without permission from rights holders is prohibited.

The original online manual is archived in `docs/manual/` from `http://atn-factory.jp/ggn/manual/`. It is included as project documentation. Its screenshots, images, HTML, and related files are not standalone reusable material assets and are not covered by the MIT license grant for source code.

## Credits recovered from the original manual and released readme

The original bundled manual and the released readme credited the following creators and sites. These credits are preserved here for attribution and audit purposes. They do not grant additional reuse rights.

### Project staff credited by the released readme

- @N-Factory, http://ato-nfactory.jp/
- Planning: 白mac
- Programming: 白mac
- Programming cooperation: spk, 霊屋 倭
- Graphics: 白mac, overx
- Balance adjustment: 霊屋 倭, まやん

### Graphics and artwork contributors

- みらんこ 様, Pixiv ID: 80904, http://www.pixiv.net/member.php?id=80904
- きいろからー 様
- ツキシロ 様, Pixiv ID: 681148, http://www.pixiv.net/member.php?id=681148
- しらじす 様 / Dream upper, http://www.geocities.jp/capt_syu/
- 槍騎ランナイ 様 / 馬上の一本槍, http://d.hatena.ne.jp/Lankni/
- はるやん 様
- ししゃも 様
- 風音みこと 様

### Music and sound contributors

- KGM 様 (music)
- 山可祢 様 (sound effects; credited by the released readme)
- 白mac 様 (sound effects; credited by the online manual)
- 霊屋 倭 様 (sound effects; credited by the online manual)
- Music with Myuu, http://www.ne.jp/asahi/music/myuu/

Maintainer note (`2026-06-14`): BGM redistribution permission has been confirmed by the maintainer as granted by the BGM creator for distribution as part of this playable game package. This note is limited to BGM; if any packed audio file also contains sound effects or other third-party material, keep the corresponding attribution and permission records with the release. BGM/music is not MIT-licensed and must not be extracted, redistributed, reused, or repurposed as standalone material for any other project, soundtrack, asset pack, or other purpose.

Gameplay streaming, recorded gameplay videos, screenshots, and monetized videos/streams of this game are permitted, provided that the Touhou Project fan content guidelines are followed. This permission covers gameplay capture of this game; it does not permit extracting, redistributing, or reusing packed BGM/music, sound effects, graphics, textures, scripts, or data as standalone materials.

Maintainer note (`2026-06-14`): this project has a prior commercial release history, and the maintainer states that rights-problematic audio files were not used in the released project. This statement supports distributing the audio packs only as part of the playable game package; it does not grant permission to extract or reuse audio files as standalone sound materials.

Large audio files are not included in this public branch. The credit is retained because the original manual listed it and older/private builds may have used those materials.

### Audio redistribution audit notes

The released runtime's `sound/data1` pack contains 122 sound-effect entries. Header inspection confirmed that `sound/data1` is separate from the BGM pack `sound/data2`.

Confirmed public-source exact matches (`2026-06-14`): 11 packed sound-effect files are byte-identical to files from `ザ・マッチメイカァズ2nd` old sound archive `se_old_pack00.zip`.

- `kaminari.wav` <- `crash17.wav`
- `mob29.wav` <- `gun24.wav`
- `hitlight.wav` <- `hit33.wav`
- `swing1031.wav` <- `hit_p07.wav`
- `trap.wav` <- `kachi17.wav`
- `mob32.wav` <- `metal25_a.wav`
- `powerup.wav` <- `power32.wav`
- `mob31.wav` <- `puu05.wav`
- `powerdown.wav` <- `puu23.wav`
- `jump.wav` <- `pyoro35_c.wav`
- `swing27.wav` <- `swing27.wav`

The current `ザ・マッチメイカァズ2nd` material page describes the materials as usable for paid or unpaid works, with no permission-contact or copyright-display requirement, while prohibiting redistribution of the material files themselves as standalone material.

Additional metadata note: `swing1034.wav`, `swing1038.wav`, and `swing1058.wav` contain embedded metadata referencing `Crypton Future Media, Inc.`, `Sonicwire Sound Archiver`, and `http://sonicwire.com`. The maintainer recalls editing/pitch-shifting sound-effect material and states that the commercial release did not use rights-problematic audio. Keep this note for audit traceability; do not distribute extracted WAV files or treat the packed audio as a reusable material library.

### Material sites credited by the original manual

- ザ・マッチメイカァズ2nd, http://osabisi.sakura.ne.jp/m2/
- cgtextures.com / Textures.com, http://www.cgtextures.com/

Material site terms may have changed since the original project was created. Re-check the current terms before restoring, redistributing, or reusing any material from these sources.

## Third-party source notices included in this repository

### Mersenne Twister random number generator

Files:

- `source/utility/cRandomGen.cpp`
- `source/utility/mt19937ar.cpp`

The source files contain the following license notice:

Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. The names of its contributors may not be used to endorse or promote
   products derived from this software without specific prior written
   permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

### SpriteText helper

Files:

- `source/utility/SpriteText.cpp`
- `source/utility/SpriteText.h`

The file header credits the original source as TWINTAIL LOVERS and notes modification by すき焼き:

- TWINTAIL LOVERS, http://www.twin-tail.jp/
- すき焼き, http://www.c3.club.kyutech.ac.jp/~sukiyaki/

The header comment indicates that source modification is permitted, but no complete standalone redistribution license was found. Keep this attribution with any distribution and re-check rights before reusing the helper outside this project.

## Removed from the public branch because license permission was unclear

- `source/sound/mpegDecode/mpegdecode.h` contained an old Xing Technology MP3 decoder copyright notice but no redistribution license grant. It has been removed from the public branch and should not be restored unless a valid redistribution license is confirmed.

## External build dependencies not vendored here

The project references several external SDKs/libraries that are not licensed by this repository. Obtain them separately and follow their own licenses/terms:

- Boost
- Lua 5.1
- luabind 0.9.1
- zlib
- Microsoft DirectX SDK / DirectSound
- Windows HID APIs / platform SDK headers

## Publication checklist

Before pushing this branch publicly:

- Push the `public` branch only. Do not push the `temp` backup branch.
- Re-run a scan for e-mail addresses, private paths, credentials, archives, save data, generated binaries, and large assets.
- Re-check any restored media, textures, fonts, or generated/editable source assets against their current license terms.
