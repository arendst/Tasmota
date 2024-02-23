# Shell expression for the Nix package manager
#
# This nix expression creates an environment with necessary packages installed:
#
#  * `tockloader`
#  * arm-none-eabi toolchain
#  * elf2tab
#  * (optionally) riscv32-embedded toolchain
#
# To use:
#
#  $ nix-shell
#
# The RISC-V toolchain can be disabled optionally. This will further
# prevent RISC-V specific environment variables from being set in the
# Nix shell environment:
#
#  $ nix-shell shell.nix --arg disableRiscvToolchain true

{ pkgs ? import <nixpkgs> {}, disableRiscvToolchain ? false }:

with builtins;
let
  inherit (pkgs) stdenv lib;
  pythonPackages = lib.fix' (self: with self; pkgs.python3Packages //
  {

    tockloader = buildPythonPackage rec {
      pname = "tockloader";
      version = "1.9.0";
      name = "${pname}-${version}";

      propagatedBuildInputs = [
        argcomplete
        colorama
        crcmod
        pyserial
        toml
        tqdm
        questionary
      ];

      src = fetchPypi {
        inherit pname version;
        sha256 = "sha256-7W55jugVtamFUL8N3dD1LFLJP2UDQb74V6o96rd/tEg=";
      };
    };
  });
  elf2tab = pkgs.rustPlatform.buildRustPackage rec {
    name = "elf2tab-${version}";
    version = "0.10.2";

    src = pkgs.fetchFromGitHub {
      owner = "tock";
      repo = "elf2tab";
      rev = "v${version}";
      sha256 = "sha256-mlb94K3mTSGpkP+bFAQd6/AN2cssR+48nreTOym21jU=";
    };

    cargoSha256 = "sha256-Dt6iPb7xXD6bvf1GS17xdFhRSm5qd3FfZaJfW0eRBf8=";
  };
in
  pkgs.mkShell {
    name = "tock-dev";

    buildInputs = with pkgs; [
      elf2tab
      gcc-arm-embedded
      python3Full
      pythonPackages.tockloader
    ] ++ (lib.optional (!disableRiscvToolchain) (
      pkgsCross.riscv32-embedded.buildPackages.gcc.override (oldCc: {
        cc = (pkgsCross.riscv32-embedded.buildPackages.gcc.cc.override (oldCcArgs: {
          libcCross = oldCcArgs.libcCross.overrideAttrs (oldNewlibAttrs: {
            configureFlags = oldNewlibAttrs.configureFlags ++ [
              "--enable-libssp"
            ];
          });
        })).overrideAttrs (oldCcAttrs: {
          configureFlags = oldCcAttrs.configureFlags ++ [
            "--without-headers"
            "--disable-shared"
            "--disable-libssp"
            "--disable-multilib"
            "--with-newlib"
          ];
          gcc_cv_libc_provides_ssp = "yes";
        });
      })
    ));

    shellHook = ''
      ${if (!disableRiscvToolchain) then ''
        export RISCV=1
      '' else ""}
    '';
  }
