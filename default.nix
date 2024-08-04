{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
  name = "lute";
  version = "0.1.0";
  src = ./.;

  outputs = [ "out" "lib" "dev" ];

  nativeBuildInputs = [
    pkgs.clang-tools
    pkgs.clang
    pkgs.gnumake
    pkgs.makeWrapper
    pkgs.pkg-config
  ];

  depsBuildInputs = [
    pkgs.pkg-config
  ];

  buildPhase = ''
    make clean all
  '';

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin
    cp -r out/lute $out/bin

    mkdir -p $out/include
    cp -r lib/include/* $out/include/

    mkdir -p $lib
    cp -r out/lib/liblute.so $lib

    mkdir -p $dev/lib/pkgconfig
    cat > $dev/lib/pkgconfig/lute.pc <<EOF
    prefix=$out
    exec_prefix=$out
    libdir=$lib
    includedir=$out/include

    Name: lute
    Description: A build system for C and C++ projects
    Version: 0.1.0
    Libs: -L$lib -llute
    Cflags: -I$out/include
    EOF

    runHook postInstall
  '';

  meta = {
    pkgConfigModules = [ "lute" ];
  };
}
