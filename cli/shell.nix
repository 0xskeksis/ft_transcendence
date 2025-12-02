{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "gtk-dev-env";

  packages = with pkgs; [
    clang_17
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
	glibcLocales
	libjwt
	jp2a
  ];

  shellHook = ''
	export LOCALE_ARCHIVE="${pkgs.glibcLocales}/lib/locale/locale-archive"
	export LANG="en_US.UTF-8"
	export LC_ALL="en_US.UTF-8"
	export GSK_RENDERER=cairo
	clear
	zsh
  '';
}
