{
  description = "Flake for C";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages."${system}";
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        name = "c";

        nativeBuildInputs = with pkgs;[
          clang-tools
          clang

          gnumake
          cmake
          valgrind
          gdb

          pkg-config
        ];

        buildInputs = with pkgs; [
          zlib

          sqlite
        ];

        env.LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
          pkgs.stdenv.cc.cc.lib
          pkgs.zlib
        ];

        shellHook = ''
          echo -e "\033[0;32mDone!\033[0m"
        '';
      };
    };
}
