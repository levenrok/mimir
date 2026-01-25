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
      packages.${system}.default = pkgs.callPackage ./default.nix { };

      devShells.${system}.default = pkgs.mkShell {
        name = "c";

        inputsFrom = [ self.packages.${system}.default ];

        nativeBuildInputs = with pkgs;[
          clang-tools
          clang
          gcc

          gnumake
          valgrind
          gdb

          (python3.withPackages (pypkgs: with pypkgs; [
            pip
          ]))
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
