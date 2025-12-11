{
  cmake,
  lib,
  stdenv,
  kcli,
  ktest,
  doCheck ? false,
}:
stdenv.mkDerivation {
  name = "ktl";
  src = lib.cleanSource ./.;
  inherit doCheck;

  nativeBuildInputs = [
    cmake
    (ktest.override { inherit stdenv; })
  ];

  buildInputs = [
    (kcli.override { inherit stdenv; })
  ];

  configurePhase = ''
    cmake -B build
  '';

  buildPhase = ''
    cmake --build build
  '';

  installPhase = ''
    mkdir -p "$out/include"
    cp -r include "$out"
  '';

  checkPhase = ''
    (
      cd build/test
      ctest --output-on-failure
    )
  '';
}
