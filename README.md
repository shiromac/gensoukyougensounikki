# gensoukyougensounikki / GGN public archive

This branch is a public archival/reference branch for `幻想郷幻想日記` (`gensoukyougensounikki`).

Current public distribution version: `1.0.5.2` (`2026-06-14`).

Known historical released executable used as the recovery reference: `幻想郷幻想日記` / `ggn.exe` version `1.0.4.0` (`2014-12-21`).

Version `1.0.5.2` is rebuilt from the `1.0.4.0` release baseline, with public-source licensing/build/package fixes applied and later in-development gameplay/data changes excluded.

It has been prepared for publication by removing private e-mail addresses, build outputs, save data, large archives, and known unclear-license artifacts. The `temp` branch is a local backup of the original working tree and should not be pushed publicly.

This repository is an unofficial Touhou Project fan work. It is not affiliated with or endorsed by Team Shanghai Alice or ZUN.

The original source code and build/package scripts are licensed under the MIT License. Game assets, BGM/music, sound effects, Touhou Project content, third-party materials, external libraries, and packed runtime data are not covered by that MIT grant.

Before publishing or reusing anything from this repository, read:

- `LICENSE.md`
- `NOTICE.md`

The original online manual is archived in `docs/manual/`. Open
`docs/manual/index.html` to read it locally.
When GitHub Pages is enabled for this repository from the `public` branch
`/docs` directory, it is available at
`https://shiromac.github.io/gensoukyougensounikki/manual/`.

Binary release packages may be unpacked to run the game, but the packed runtime assets must not be extracted, redistributed, or reused as standalone BGM/music, sound, image, texture, script, or data materials. Included BGM/music may only be used as part of playing this game.

The project also depends on external SDKs/libraries that are not vendored here, including Boost, Lua 5.1, luabind 0.9.1, zlib, DirectX/DirectSound, and Windows platform SDK headers.

## Build

For the maintained Debug build and smoke-test workflow, see `BUILDING.md`.
