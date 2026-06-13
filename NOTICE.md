# Notice

This file records copyright, attribution, and license notices currently known for the public branch.
It is intentionally conservative: items with unclear redistribution permission should be removed from the public branch instead of treated as licensed.

## Project notice

- Repository/project name: `laug_th` / GGN public archive.
- This is an unofficial Touhou Project fan work. It is not official, not affiliated with Team Shanghai Alice, and not endorsed by Team Shanghai Alice or ZUN.
- Touhou Project characters, settings, and related intellectual property belong to Team Shanghai Alice / ZUN.
- The public branch is intended to exclude extracted official assets and private/build artifacts. If any such material is found, remove it before publishing.
- Historical documents in the original project identify `@N-Factory`, `白mac`, and contributors as project creators/contributors.

Relevant Touhou Project references:

- Official Touhou Project fan content guidelines: https://touhou-project.news/guidelines_en/
- Team Shanghai Alice / ZUN site referenced by the original manual: http://www16.big.or.jp/~zun/

## Credits recovered from the original manual

The original bundled manual credited the following creators and sites. These credits are preserved here for attribution and audit purposes. They do not grant additional reuse rights.

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

- KGM 様
- Music with Myuu, http://www.ne.jp/asahi/music/myuu/

Large audio files are not included in this public branch. The credit is retained because the original manual listed it and older/private builds may have used those materials.

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
