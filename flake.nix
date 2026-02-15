{
  description = "Flake for C Development";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        packages.default = pkgs.callPackage ./default.nix { };

        devShells.default = pkgs.mkShell {
          name = "c";

          inputsFrom = [ self.packages.${system}.default ];

          nativeBuildInputs = with pkgs; [
            clang-tools
            clang
            gcc

            gnumake
            valgrind
            gdb

            (python3.withPackages (pkpkgs: with pkpkgs; [
                pip
            ]))
          ];

          buildInputs = with pkgs; [
            zlib
          ];

          env.LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
            pkgs.stdenv.cc.cc.lib
            pkgs.zlib
          ];

          shellHook = ''
            echo -e "\033[0;32mDone!\033[0m"
          '';
        };
      }
    );
}
