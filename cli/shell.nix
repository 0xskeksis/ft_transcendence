{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "gtk-dev-env";

  packages = with pkgs; [
    clang
    pkg-config
    gtk4
    glib
    gobject-introspection
    gettext
    libxml2
    gtksourceview3
    meson
    ninja
	curl
	cjson
	valgrind
  ];

  shellHook = ''
	clear
	zsh
  '';
}

