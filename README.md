# H1-Mod

This is a client modification for H1!  
Developed by [Aurora](https://auroramod.dev/).  
This is a fork of [H1-Mod](https://github.com/auroramod/h1-mod) with added CB patches.  
Big thanks to all the contributors.              
Join us on [Discord](https://cbservers.xyz/discord) for support.  
Follow the original project on [GitHub](https://github.com/auroramod).  

NOTE: This fork is not affiliated or endorsed by Aurora. Please do not bug original client maintainers with support requests in regards to this fork.

<p align="center">
  <img src="assets/github/banner.png?raw=true" />
</p>

## Compile from source

- Clone the Git repo. Do NOT download it as ZIP, that won't work.
- Update the submodules and run `premake5 vs2022` or simply use the delivered `generate.bat`.
- Build via solution file in `build\h1-mod.sln`.

### Premake arguments

| Argument                    | Description                                    |
|:----------------------------|:-----------------------------------------------|
| `--copy-to=PATH`            | Optional, copy the EXE to a custom folder after build, define the path here if wanted. |
| `--dev-build`               | Enable development builds of the client. |

## Credits

- [s1x-client](https://github.com/HeartbeatingForCenturies/s1x-client) - codebase and research (predecessor of MWR)
- [h2-mod](https://github.com/fedddddd/h2-mod) - research (successor of MWR)
- [momo5502](https://github.com/momo5502) - Arxan/Steam research, former lead developer of [XLabsProject](https://github.com/XLabsProject)

## Disclaimer

This software has been created purely for the purposes of academic research. It is not intended to be used to attack other systems. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.
