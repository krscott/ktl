{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    kcli = {
      url = "github:krscott/kcli";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      kcli,
    }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
    in
    flake-utils.lib.eachSystem supportedSystems (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        # Final derivation including any overrides made to output package
        inherit (self.packages.${system}) ktl ktl-gcc;

        devPkgs =
          with pkgs;
          [
            shfmt
            cmake-format
            clang-tools # NOTE: clang-tools must come before clang
            clang
          ]
          ++ ktl.buildInputs;

        mkApp = text: {
          type = "app";
          program = pkgs.lib.getExe (
            pkgs.writeShellApplication {
              name = "app";
              runtimeInputs = devPkgs;
              inherit text;
            }
          );
        };
      in
      {
        packages = {
          ktl = pkgs.callPackage ./. {
            inherit (kcli.packages.${system}) kcli;
            stdenv = pkgs.clangStdenv;
          };

          ktl-gcc = ktl.override {
            inherit (pkgs) stdenv;
          };

          ktl-win = ktl.override {
            inherit (pkgs.pkgsCross.mingwW64) stdenv;
          };

          default = ktl;

          ktl-test = ktl.override {
            doCheck = true;
          };
          ktl-gcc-test = ktl-gcc.override {
            doCheck = true;
          };
        };

        devShells = {
          default = pkgs.mkShell {
            inputsFrom = [ ktl ];
            nativeBuildInputs = devPkgs;
            shellHook = ''
              source dev_shell.sh
            '';
          };
        };

        apps = {
          format = mkApp ''
            ./format.sh
          '';
        };

        formatter = pkgs.nixfmt-rfc-style;
      }
    );
}
