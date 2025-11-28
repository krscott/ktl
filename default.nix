{
  cmake,
  lib,
  stdenv,
  kcli,
  doCheck ? false,
}:
stdenv.mkDerivation {
  name = "ktl";
  src = lib.cleanSource ./.;
  inherit doCheck;

  nativeBuildInputs = [ cmake ];

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
    if [[ "$CC" == *"mingw32"* ]]; then
      # Workaround broken pkgCross cmake install
      mkdir -p "$out/bin"
      cp build/app/*.exe "$out/bin"
      if compgen -G "build/src/*.a" > /dev/null; then
        mkdir -p "$out/lib"
        cp build/src/*.a "$out/lib"
      fi
    else
      cmake --install build --prefix $out
    fi

    mkdir -p "$out/include"
    cp include/*.h "$out/include"
  '';

  checkPhase = ''
    (
      cd build/test
      ctest --output-on-failure
    )
  '';
}
