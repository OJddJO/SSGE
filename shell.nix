let
  pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
  packages = with pkgs; [
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
  ];
}