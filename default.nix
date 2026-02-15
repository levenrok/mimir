{ stdenv
, cmake
, pkg-config
, zlib
, sqlite
}:
stdenv.mkDerivation {
  pname = "mimir";
  version = "0.7.0";

  src = ./.;

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  buildInputs = [
    sqlite
    zlib
  ];
}
