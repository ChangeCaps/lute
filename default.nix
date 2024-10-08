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
  ];

  depsBuildBuild = [
    pkgs.pkg-config
  ];

  buildPhase = ''
    make clean all
  '';

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin
    cp -r out/lute $out/bin 
    cp -r lib/include $out/bin
    cp -r out/lib/lutebuild.o $out/bin

    mkdir -p $lib
    cp -r out/lib $lib

    mkdir -p $dev
    cp -r lib/include $dev/include

    mkdir -p $dev/lib/pkgconfig
    cat > $dev/lib/pkgconfig/lute.pc <<EOF
    prefix=$out
    exec_prefix=$out
    libdir=$lib
    includedir=$dev/include

    Name: lute
    Description: A build system for C and C++ projects
    Version: 0.1.0
    Libs: -L$lib -llute
    Cflags: -I$out/include
    EOF

    runHook postInstall
  '';

  postFixup = ''
    wrapProgram $out/bin/lute \
      --prefix PATH : "${pkgs.lib.makeBinPath [ pkgs.clang pkgs.git ]}" \
      --prefix LUTE_CFLAGS : "-I$out/bin/include" \
      --prefix LUTE_LIBS : "$out/bin/lutebuild.o" \
  '';

  meta = {
    pkgConfigModules = [ "lute" ];
  };
}
